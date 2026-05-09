#include "Model.hpp"

void Model::computeLinearScaling(std::vector<double> &results) {
    if (!linearScaling) {
        a = 1;
        b = 0;
        return;
    }

    double meanResult = 0;
    double meanCodomain = 0;
    for (int i = 0; i < nSamples; ++i) {
        meanResult += results[i];
        meanCodomain += codomain[i];
    }
    meanResult /= nSamples;
    meanCodomain /= nSamples;

    double covar = 0, var = 0;
    for (int i = 0; i < nSamples; ++i) {
        covar += (results[i] - meanResult) * (codomain[i] - meanCodomain);
        var += (results[i] - meanResult) * (results[i] - meanResult);
    }

    if (var == 0) {
        a = 0;
        b = meanCodomain;
    } else {
        a = covar / var;
        b = meanCodomain - a * meanResult;
    }
}

FitnessP Model::evaluate(IndividualP individual) {
    FitnessP fitness(new FitnessMin);

    vector<double> results(nSamples);

    execute(individual->getGenotype(), results, domain);
    computeLinearScaling(results);

    double score = 0;
    for(uint i = 0; i < nSamples; i++) {
        double result = a * results[i] + b;
        score += fabs(codomain[i] - result);
    }
    fitness->setValue(score);

    return fitness;
}

bool TreeModel::initialize(StateP state) {
    if (!state->getGenotypes()[0]->isParameterDefined(state, "terminalset"))
        return false;

    voidP sptr = state->getGenotypes()[0]->getParameterValue(state, "terminalset");

    std::string terminals = *((std::string*)sptr.get());

    std::stringstream ss(terminals);
    std::string token;
    this->terminal_names_.clear();

    while (ss >> token) {
        this->terminal_names_.push_back(token);
    }

    return true;
}

void TreeModel::execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) {
	Tree::Tree* tree = (Tree::Tree*) genotype.get();

    for(uint i = 0; i < domain.size(); i++) {
        for(uint j = 0; j < nFeatures; j++)
            tree->setTerminalValue(this->terminal_names_[j], &domain[i][j]);
        tree->execute(&results[i]);
    }
}

std::string TreeModel::getString() {
    TreeP tree = std::dynamic_pointer_cast<Tree::Tree>(this->genotype_);
    std::string output;
    getString(tree, 0, output);
    if (linearScaling)
        output = to_string(b) + "+" + to_string(a) + "*(" + output + ")";

    return output;
}

int TreeModel::getString(TreeP tree, int index, std::string& result) {
    Tree::Node* node = (*tree)[index].get();
    int arity = node->primitive_->getNumberOfArguments();

    if (arity == 0) {
        result += node->primitive_->getName();
        return index + 1;
    } else if (arity == 1) {
        result += node->primitive_->getName() + "(";
        index = getString(tree, index + 1, result);
        result += ")";
        return index;
    } else {
        std::string funcName = node->primitive_->getName();
        if (funcName == "+" || funcName == "-" || funcName == "*" || funcName == "/") {
            result += "(";
            index = getString(tree, index + 1, result);
            result += funcName;
            index = getString(tree, index, result);
            result += ")";
        }
        else {
            result += funcName;
            result += "(";
            index = getString(tree, index + 1, result);
            result += ",";
            index = getString(tree, index, result);
            result += ")";
        }
        return index;
    }
}

void CGPModel::execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) {
	Cartesian::Cartesian* cartesian = (Cartesian::Cartesian*) genotype.get();

    for(uint i = 0; i < domain.size(); i++) {
        std::vector<double> result;
        cartesian->evaluate(domain[i], result);
        results[i] = result[0];
    }
}

std::string CGPModel::getString(){
    Cartesian::Cartesian* cartesian = (Cartesian::Cartesian*) this->genotype_.get();
    std::vector<uint>& genome = *cartesian;
    uint i = 0;
    
    std::vector<std::string> nodeFormulas;
    
    for (uint inp = 0; inp < cartesian->nInputs; inp++)
        nodeFormulas.push_back("x" + std::to_string(inp));
    
    do {
        uint iFunction = genome[i++];
        std::string funcName = cartesian->functionSet->vFunctions[iFunction]->getName();
        int arity = cartesian->functionSet->vFunctions[iFunction]->getNumberOfArguments();
        
        std::vector<uint> args(cartesian->maxArity);
        for (uint arg = 0; arg < cartesian->maxArity; arg++)
            args[arg] = genome[i++];
        
        std::string nodeStr;
        if (arity == 1) {
            nodeStr = funcName + "(" + nodeFormulas[args[0]] + ")";
        } else if (arity == 2) {
            if (funcName == "+" || funcName == "-" || funcName == "*" || funcName == "/")
                nodeStr = "(" + nodeFormulas[args[0]] + funcName + nodeFormulas[args[1]] + ")";
            else
                nodeStr = funcName + "(" + nodeFormulas[args[0]] + "," + nodeFormulas[args[1]] + ")";
        } else {
            nodeStr = funcName + "(";
            for (int a = 0; a < arity; a++) {
                if (a > 0) nodeStr += ",";
                nodeStr += nodeFormulas[args[a]];
            }
            nodeStr += ")";
        }
        
        nodeFormulas.push_back(nodeStr);
        
    } while (i < (genome.size() - cartesian->nOutputs));
    
    std::string result = "";
    for (uint out = 0; out < cartesian->nOutputs; out++) {
        uint outputIdx = genome[i++];
        if (out > 0) result += ",";
        result += nodeFormulas[outputIdx];
    }

    if (linearScaling)
        result = to_string(b) + "+" + to_string(a) + "*(" + result + ")";
    
    return result;
}