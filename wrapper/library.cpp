#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/detail/wrap_python.hpp>

#include <ECF/ECF.h>
#include "Model.hpp"

namespace py = boost::python;
namespace np = boost::python::numpy;

enum class ModelType {
    TREE_MODEL,
    CGP_MODEL
    // TEMPLATE_MODEL
};

class ECFSR {
private:
    StateP state;
    ModelP model;
    int argc;
    char **argv;
    ModelType model_type_;
public:
    ECFSR(ModelType model_type = ModelType::TREE_MODEL) : model_type_(model_type) {
        argc = 2;

        argv = new char*[2];
        argv[0] = new char[1];
        argv[1] = new char[256];
    }

    ~ECFSR() {
        delete argv[1];
        delete argv[0];
        delete argv;
    }

    void setPath(const std::string& path) const {
        strcpy(argv[1], path.c_str());
    }

    const char* getPath() const {
        return argv[1];
    }
    
    py::str getString() const {
        return model->getString().c_str();
    }

    int getSize() const {
        return model->getSize();
    }

    ModelType getModelType() const {
        return model_type_;
    }

    void fit(np::ndarray X, np::ndarray y, bool linearScaling) {
        uint nSamples = X.shape(0), nFeatures = X.shape(1);
        std::vector<std::vector<double>> domain;
        std::vector<double> codomain;

        X = X.astype(np::dtype::get_builtin<double>());
        y = y.astype(np::dtype::get_builtin<double>());

        double* X_data = reinterpret_cast<double*>(X.get_data());
        double* y_data = reinterpret_cast<double*>(y.get_data());

        for(uint i = 0; i < nSamples; i++) {
            domain.emplace_back();
            for (uint j = 0; j < nFeatures; j++)
                domain[i].push_back(X_data[i * nFeatures + j]);
            codomain.push_back(y_data[i]);
        }

        state = StateP(new State);

        switch (model_type_) {
            case ModelType::TREE_MODEL:
                model = std::make_shared<TreeModel>(nSamples, nFeatures, domain, codomain, linearScaling);
                break;
            case ModelType::CGP_MODEL:
                model = std::make_shared<CGPModel>(nSamples, nFeatures, domain, codomain, linearScaling);
                break;
            // case ModelType::TEMPLATE_MODEL:
            //     model = std::make_shared<TemplateModel>(nSamples, nFeatures, domain, codomain, linearScaling);
            //     break;
            default:
                throw std::invalid_argument("Invalid model type");
        }
        
        state->setEvalOp(model);

        state->initialize(argc, argv);
        state->run();

        model->setGenotype(state->getHoF()->getBest().front()->getGenotype());
        model->computeLinearScaling();
    }

    np::ndarray predict(np::ndarray X) const {
        uint nSamples = X.shape(0), nFeatures = X.shape(1);
        std::vector<std::vector<double>> domain;

        X = X.astype(np::dtype::get_builtin<double>());
        double* X_data = reinterpret_cast<double*>(X.get_data());

        for(uint i = 0; i < nSamples; i++) {
            domain.emplace_back();
            for (uint j = 0; j < nFeatures; j++)
                domain[i].push_back(X_data[i * nFeatures + j]);
        }

        std::vector<double> exeResults(nSamples);
        model->predict(exeResults, domain);

        np::ndarray results = np::zeros(py::make_tuple(nSamples), np::dtype::get_builtin<double>());
        for(uint i = 0; i < nSamples; i++) {
            results[i] = model->getA() * exeResults[i] + model->getB();
        }

        return results;
    }
};

BOOST_PYTHON_MODULE(ecf_sr) {
    np::initialize();
    
    py::enum_<ModelType>("ModelType")
        .value("TREE_MODEL", ModelType::TREE_MODEL)
        .value("CGP_MODEL", ModelType::CGP_MODEL);
        // .value("TEMPLATE_MODEL", ModelType::TEMPLATE_MODEL);
    
    py::class_<ECFSR, std::shared_ptr<ECFSR> >("ECF", py::init<ModelType>(py::arg("model_type") = ModelType::TREE_MODEL))
        .def("getString", &ECFSR::getString)
        .def("getSize", &ECFSR::getSize)
        .def("getModelType", &ECFSR::getModelType)
        .def("fit", &ECFSR::fit)
        .def("predict", &ECFSR::predict)
        .def("setPath", &ECFSR::setPath)
        .def("getPath", &ECFSR::getPath);
}
