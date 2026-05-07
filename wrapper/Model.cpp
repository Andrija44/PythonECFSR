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

void APModel::execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) {
    Tree::APGenotype* apg = (Tree::APGenotype*) genotype.get();
    Tree::Tree* tree = (Tree::Tree*) ((Tree::APGenotype*) genotype.get())->convertToPhenotype();

    for(uint i = 0; i < domain.size(); i++) {
        for(uint j = 0; j < nFeatures; j++)
            apg->setTerminalValue(tree, this->terminal_names_[j], &domain[i][j]);
        tree->execute(&results[i]);
    }
}

// FitnessP CGPModel::evaluate(IndividualP individual) {
//     FitnessP fitness(new FitnessMin);

//     vector<double> results(nSamples);

//     execute(individual->getGenotype(), results, domain);
//     computeLinearScaling(results);

//     double score = 0;
//     for(uint i = 0; i < nSamples; i++) {
//         double result = a * results[i] + b;
//         score += fabs(codomain[i] - result);
//     }
//     fitness->setValue(score);

//     return fitness;
// }

// // implementacija CGPModel klase
// void CGPModel::execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) {
// 	Cartesian::Cartesian* cartesian = (Cartesian::Cartesian*) genotype.get();

//     for(uint i = 0; i < domain.size(); i++) {
//         std::vector<double> result;
//         cartesian->evaluate(domain[i], result);
//         results[i] = result[0];
//     }
// }