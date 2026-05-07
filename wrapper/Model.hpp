#ifndef Model_hpp
#define Model_hpp
#include <ECF/ECF.h>

class Model : public EvaluateOp, public std::enable_shared_from_this<Model> {
protected:
    // clanske var
    GenotypeP genotype_;
    uint nSamples, nFeatures;
    std::vector<std::vector<double>> domain;
	std::vector<double> codomain;
    bool linearScaling;
    double a, b;

public:
    // konstruktor
    Model(uint nSamples, uint nFeatures, const std::vector<std::vector<double>> domain,
            const std::vector<double> codomain, bool linearScaling):
            nSamples(nSamples), nFeatures(nFeatures), domain(domain), codomain(codomain), linearScaling(linearScaling), a(1), b(0) {}
    virtual ~Model() = default;

    // metode
    virtual int getSize() = 0;
    FitnessP evaluate(IndividualP individual);
    virtual void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) = 0; // abstraktna, hence = 0
    void setGenotype(GenotypeP genotype) { genotype_ = genotype; }
    GenotypeP getGenotype() { return genotype_; }
    double getA() { return a; }
    double getB() { return b; }
    void computeLinearScaling(std::vector<double> &results);

    std::string getString() {
        std::string output = genotype_->toString();
        if (linearScaling)
            output = to_string(b) + "+" + to_string(a) + "*(" + output + ")";

        return output;
    }
    void predict(std::vector<double> &results, std::vector<std::vector<double>> &new_domain) {
        std::vector<double> temp_results(nSamples);
        execute(this->genotype_, temp_results, this->domain);
        computeLinearScaling(temp_results);

        execute(this->genotype_, results, new_domain);
    }
};
typedef std::shared_ptr<Model> ModelP;

// GP
class TreeModel : public Model {
protected:
    std::vector<std::string> terminal_names_;
public:
    // konstruktor
    TreeModel(uint nSamples, uint nFeatures, const std::vector<std::vector<double>>& domain,
            const std::vector<double>& codomain, bool linearScaling) : Model(nSamples, nFeatures, domain, codomain, linearScaling) {}

    // metode
    bool initialize(StateP state);
	void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain);
    int getSize() override {
        Tree::Tree* tree = (Tree::Tree*) this->genotype_.get();
        return tree->size();
    }
};
typedef std::shared_ptr<TreeModel> TreeModelP;

// AP
class APModel : public TreeModel {
public:
    // konstruktor
    APModel(uint nSamples, uint nFeatures, const std::vector<std::vector<double>>& domain,
            const std::vector<double>& codomain, bool linearScaling) : TreeModel(nSamples, nFeatures, domain, codomain, linearScaling) {}

    // metode
    void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain);
    int getSize() override {
        Tree::Tree* tree = (Tree::Tree*) ((Tree::APGenotype*) this->genotype_.get())->convertToPhenotype();
        return tree->size();
    }
};
typedef std::shared_ptr<APModel> APModelP;

// CGP
// class CGPModel : public Model {
// public:
//     // konstruktor
//     CGPModel(uint nSamples, uint nFeatures, const std::vector<std::vector<double>>& domain,
//             const std::vector<double>& codomain, bool linearScaling) : Model(nSamples, nFeatures, domain, codomain, linearScaling) {}

//     // metode
//     FitnessP evaluate(IndividualP individual);
//     void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain);
//     int getSize() override {
//         Cartesian::Cartesian* cartesian = (Cartesian::Cartesian*) this->genotype_.get();
//         return cartesian->size();
//     }
// };
// typedef std::shared_ptr<CGPModel> CGPModelP;


#endif // Model_hpp