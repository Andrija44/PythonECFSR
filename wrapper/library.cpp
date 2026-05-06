#include <iostream>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/detail/wrap_python.hpp>

#include <ECF/ECF.h>
#include "Model.hpp"

namespace py = boost::python;
namespace np = boost::python::numpy;

class ECFWrap {
private:
    StateP state;
    ModelP model;
    int argc;
    char **argv;
public:
    ECFWrap() {
        argc = 2;

        argv = new char*[2];
        argv[0] = new char[1];
        argv[1] = new char[256];
    }

    ~ECFWrap() {
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
        model = std::make_shared<TreeModel>(nSamples, nFeatures, domain, codomain, linearScaling);

        state->setEvalOp(model);

        state->initialize(argc, argv);
        state->run();

        model->setGenotype(state->getHoF()->getBest().front()->getGenotype());
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

BOOST_PYTHON_MODULE(ecf_wrap) {
    np::initialize();
    py::class_<ECFWrap, std::shared_ptr<ECFWrap> >("ECFWrap", py::init<>())
            .def("getString", &ECFWrap::getString)
            .def("getSize", &ECFWrap::getSize)
            .def("fit", &ECFWrap::fit)
            .def("predict", &ECFWrap::predict)
            .def("setPath", &ECFWrap::setPath)
            .def("getPath", &ECFWrap::getPath);
}
