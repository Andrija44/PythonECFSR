from ecf_sr import ECFTree, ECFCGP

est = ECFTree()
est.fit([[1],[2],[3]],[1,2,1])
print(est.get_model())
print(est.get_n_nodes())
print(est.predict([[4]]))

est = ECFCGP()
est.fit([[1],[2],[3]],[1,2,1])
print(est.get_model())
print(est.get_n_nodes())
print(est.predict([[4]]))