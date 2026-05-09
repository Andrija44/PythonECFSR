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
    FitnessP evaluate(IndividualP individual);
    GenotypeP getGenotype() { return genotype_; }
    void setGenotype(GenotypeP genotype) { genotype_ = genotype; }
    double getA() { return a; }
    double getB() { return b; }
    void computeLinearScaling(std::vector<double> &results);
    void computeLinearScaling() {
        std::vector<double> temp_results(nSamples);
        execute(this->genotype_, temp_results, this->domain);
        computeLinearScaling(temp_results);
    }
    void predict(std::vector<double> &results, std::vector<std::vector<double>> &new_domain) { execute(this->genotype_, results, new_domain); }
    
    // abstraktne metode
    virtual void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) = 0;
    virtual std::string getString() = 0;
    virtual int getSize() = 0;
};
typedef std::shared_ptr<Model> ModelP;

class TreeModel : public Model {
protected:
    std::vector<std::string> terminal_names_;
    int getString(TreeP tree, int index, std::string& result);
public:
    // konstruktor
    TreeModel(uint nSamples, uint nFeatures, const std::vector<std::vector<double>>& domain,
            const std::vector<double>& codomain, bool linearScaling) : Model(nSamples, nFeatures, domain, codomain, linearScaling) {}

    // metode
    bool initialize(StateP state);
	void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) override;
    std::string getString() override;
    int getSize() override {
        Tree::Tree* tree = (Tree::Tree*) this->genotype_.get();
        return tree->size();
    }
};
typedef std::shared_ptr<TreeModel> TreeModelP;

class CGPModel : public Model {
public:
    // konstruktor
    CGPModel(uint nSamples, uint nFeatures, const std::vector<std::vector<double>>& domain,
            const std::vector<double>& codomain, bool linearScaling) : Model(nSamples, nFeatures, domain, codomain, linearScaling) {}

    // metode
    void execute(GenotypeP genotype, std::vector<double> &results, std::vector<std::vector<double>> &domain) override;
    std::string getString() override;
    int getSize() override {
        Cartesian::Cartesian* cartesian = (Cartesian::Cartesian*) this->genotype_.get();
        return cartesian->size();
    }
};
typedef std::shared_ptr<CGPModel> CGPModelP;

// primjer podklase za odreden model
// class TemplateModel : public Model {
// public:
//     // konstruktor
//     TemplateModel(uint nSamples, uint nFeatures, const std::vector<std::vector<double>>& domain,
//             const std::vector<double>& codomain, bool linearScaling) :
//             Model(nSamples, nFeatures, domain, codomain, linearScaling) {} // osnovni konstruktor, po potrebi moguce napraviti svoj
//
//     // metode
//     bool initialize(StateP state); // opcionalna funckija koja se odvija prije pokretanja treniranja
//     FitnessP evaluate(IndividualP individual); // opcionalna funkcija, osnovna od Model klase bi trebala biti dovoljno dobra za simbolicku regresiju
//     void execute(GenotypeP genotype, std::vector<double> &results,
//         std::vector<std::vector<double>> &domain) override; // glavna funkcija u kojoj se ulazni podaci postavljaju na ulaz genotipa i provlace kroz njega
//     std::string getString() override; // funkcija koja vraca string kompatabilan s sympy paketom za this->genotype_
//     int getSize() override; // funkcija koja vraca velicinu modela
// };

#endif // Model_hpp