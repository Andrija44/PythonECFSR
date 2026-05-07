from .ECFBaseRegressor import ECFBaseRegressor, ModelType
from sklearn.utils.validation import check_X_y

parameters = """
<ECF>
    <Genotype>
		<APGenotype>
			<Entry key="lbound">{lbound}</Entry>
			<Entry key="ubound">{ubound}</Entry>
			<Entry key="dimension">{dimension}</Entry>
			<Entry key="functionset">{functionset}</Entry>
			<Entry key="terminalset">{real_terminalset}</Entry>
		</APGenotype>
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

class ECFAP(ECFBaseRegressor):
    def __init__(self, lbound=-1, ubound=1, dimension=9,
                functionset="+ - / * sin cos", terminalset="1 [-1 1]",
                mutation_indprob=0.3, population_size=50, term_stagnation=100, term_fitnessval=0,
                linear_scaling=True):
        super().__init__(parameters, ModelType.AP_MODEL)
        self.lbound = lbound
        self.ubound = ubound
        self.dimension = dimension
        self.functionset = functionset
        self.terminalset = terminalset
        self.mutation_indprob = mutation_indprob
        self.population_size = population_size
        self.term_stagnation = term_stagnation
        self.term_fitnessval = term_fitnessval
        self.linear_scaling = linear_scaling
    
    def fit(self, X, y):
        X, y = check_X_y(X, y)
        l = [self.terminalset]
        for i in range(X.shape[1]):
            l.append(f'x{i}')
        real_terminalset = ' '.join(l)

        self.parameters = self.parameters.format(**self.get_params(), real_terminalset=real_terminalset)
        
        super().fit(X, y)
