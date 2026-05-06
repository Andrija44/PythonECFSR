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

bool TreeModel::initialize(StateP state) {
    TreeP tree = TreeP(new Tree::Tree);

    for (uint i = 0; i < nFeatures; ++i) {
        Tree::PrimitiveP term = Tree::PrimitiveP(new Tree::Primitives::Terminal);
        term->setName("x" + to_string(i));
        this->terminal_names_.push_back("x" + to_string(i));
        tree->addTerminal(term);
    }

    state->addGenotype(tree);

    return true;
}

FitnessP TreeModel::evaluate(IndividualP individual) {
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

// implementacija TreeModel klase
void TreeModel::execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) {
	Tree::Tree* tree = (Tree::Tree*) genotype.get();

    for(uint i = 0; i < domain.size(); i++) {
        for(uint j = 0; j < nFeatures; j++)
            tree->setTerminalValue(terminal_names_[j], &domain[i][j]);
        tree->execute(&results[i]);
    }
}

// FitnessP CGPModel::evaluate(IndividualP individual)
// {
//     FitnessP fitness(new FitnessMin);

//     if (Parameters::RUN_BEST) {
//         fitness->setValue(0);
//         return fitness;
//     }

//     this->setGenotype(individual->getGenotype());

//     PriorityHeuristic heuristic(shared_from_this());

//     Simulator sim;

//     double score = run_simulation(sim, heuristic, Parameters::SIM_STEPS);
//     fitness->setValue(score);

//     return fitness;
// }

// // implementacija CGPModel klase (TODO)
// void CGPModel::execute(std::vector<double> &results, std::vector<double> features) {
// 	Cartesian::Cartesian* cartesian = (Cartesian::Cartesian*)this->genotype_.get();
// 	//std::cout << tree->toString() << std::endl; // za testiranje
	
// 	std::vector<double> results;
// 	cartesian->evaluate(features, results);

// 	result = results[0];
// }