from ecf_sr import ECFBaseRegressor, ModelType

with open("parameters.txt", "r") as f:
    parameters = f.read()

a = ECFBaseRegressor(parameters, ModelType.TREE_MODEL, True)
a.fit([[1], [2]], [1,2])
print(a.get_model())