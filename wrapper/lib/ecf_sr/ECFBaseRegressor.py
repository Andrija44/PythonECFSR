import tempfile
import os

from sklearn.base import BaseEstimator, RegressorMixin
from sklearn.utils.validation import check_X_y, check_array, check_is_fitted

from ecf_sr import ECF, ModelType

class ECFBaseRegressor(BaseEstimator, RegressorMixin):
    def __init__(self, parameters: str, model_type=ModelType.TREE_MODEL):
        self.parameters = parameters
        self.model_type = model_type
        self.impl = ECF(self.model_type)

    def fit(self, X, y):
        X, y = check_X_y(X, y)
        self.n_features_in_ = X.shape[1]

        f = tempfile.NamedTemporaryFile(mode="w", delete=False)
        try:
            f.write(self.parameters)
            f.flush()
            f.close()
            self.impl.setPath(f.name)
            self.impl.fit(X, y, self.linear_scaling)
            self.fitted_ = True
        finally:
            os.unlink(f.name)
        return self

    def predict(self, X):
        check_is_fitted(self)
        X = check_array(X)
        return self.impl.predict(X)

    def _more_tags(self):
        return {"_xfail_checks": {"check_estimators_pickle": "Not pickleable",
                                  "check_regressor_data_not_an_array": "Weird stuff"}}

    def get_model(self):
        check_is_fitted(self)
        return self.impl.getString()
    
    def get_model_type(self):
        return self.impl.getModelType()

    def get_n_nodes(self):
        check_is_fitted(self)
        return self.impl.getSize()
