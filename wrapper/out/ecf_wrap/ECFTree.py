from .ECFBaseRegressor import ECFBaseRegressor, ModelType
from sklearn.utils.validation import check_X_y

parameters = """
<ECF>
    <Genotype>
        <Tree>
            <Entry key="functionset">{functionset}</Entry>
            <Entry key="maxdepth">{maxdepth}</Entry>
            <Entry key="terminalset">{real_terminalset}</Entry>
        </Tree>
    </Genotype>
    <Registry>
        <Entry key="batch.repeats">0</Entry>
        <Entry key="log.level">0</Entry>
        <Entry key="mutation.indprob">{mutation_indprob}</Entry>
        <Entry key="population.size">{population_size}</Entry>
        <Entry key="term.stagnation">{term_stagnation}</Entry>
        <Entry key="term.fitnessval">{term_fitnessval}</Entry>
    </Registry>
</ECF>
"""

class ECFTree(ECFBaseRegressor):
    def __init__(self, functionset="+ - * / min max", maxdepth=6, terminalset="1 [-1 1]",
                mutation_indprob=0.3, population_size=500, term_stagnation=20, term_fitnessval=0,
                linear_scaling=True):
        super().__init__(parameters, ModelType.TREE_MODEL)
        self.functionset = functionset
        self.maxdepth = maxdepth
        self.terminalset = terminalset
        self.mutation_indprob = mutation_indprob
        self.population_size = population_size
        self.term_stagnation = term_stagnation
        self.term_fitnessval = term_fitnessval
        self.linear_scaling = linear_scaling
    
    def fit(self, X, y):
        X, y = check_X_y(X, y)
        l = []
        for i in range(X.shape[1]):
            l.append(f'x{i}')
        l.append(self.terminalset)
        real_terminalset = ' '.join(l)

        self.parameters = self.parameters.format(**self.get_params(), real_terminalset=real_terminalset)
        
        super().fit(X, y)

    def get_model(self):
        return super().get_model().replace("D_", "")
