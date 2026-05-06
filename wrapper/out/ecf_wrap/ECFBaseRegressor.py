import tempfile

from sklearn.base import BaseEstimator, RegressorMixin
from sklearn.utils.validation import check_X_y, check_array, check_is_fitted

from ecf_wrap import ECFWrap

class ECFBaseRegressor(BaseEstimator, RegressorMixin):
    def __init__(self, parameters: str,
                 functionset="+ - *", maxdepth=4, terminalset="1 [-1 1]",
                 mutation_indprob=0.3, population_size=500, term_stagnation=20, term_fitnessval=0,
                 linear_scaling=False, random_state=None):
        self.parameters = parameters
        self.functionset = functionset
        self.maxdepth = maxdepth
        self.terminalset = terminalset
        self.mutation_indprob = mutation_indprob
        self.population_size = population_size
        self.term_stagnation = term_stagnation
        self.term_fitnessval = term_fitnessval
        self.linear_scaling = linear_scaling
        self.random_state = random_state

    def fit(self, X, y):
        self._impl_ = ECFWrap()

        X, y = check_X_y(X, y)
        self.n_features_in_ = X.shape[1]

        l = [self.terminalset]
        for i in range(X.shape[1]):
            l.append(f'x{i}')
        real_terminalset = ' '.join(l)

        if self.random_state is None:
            real_random_state = 0
        elif self.random_state <= 0:
            real_random_state = self.random_state - 1
        else:
            real_random_state = self.random_state

        with tempfile.NamedTemporaryFile(mode="w") as f:
            f.write(self.parameters.format(**self.get_params(),
                                           real_terminalset=real_terminalset, real_random_state=real_random_state))
            f.flush()
            self._impl_.setPath(f.name)
            self._impl_.fit(X, y, self.linear_scaling)
        return self

    def predict(self, X):
        check_is_fitted(self)
        X = check_array(X)
        return self._impl_.predict(X)

    def _more_tags(self):
        return {"_xfail_checks": {"check_estimators_pickle": "Not pickleable",
                                  "check_regressor_data_not_an_array": "Weird stuff"}}

    def get_model(self):
        return self._impl_.getString()

    def get_n_nodes(self):
        return self._impl_.getSize()
