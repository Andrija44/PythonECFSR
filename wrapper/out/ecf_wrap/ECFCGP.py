from .ECFBaseRegressor import ECFBaseRegressor, ModelType
from sklearn.utils.validation import check_X_y

parameters = """
<ECF>
    <Genotype>
        <Cartesian>
            <Entry key="numvariables">{real_numvariables}</Entry>
			<Entry key="numoutputs">{numoutputs}</Entry>
			<Entry key="numrows">{numrows}</Entry>
			<Entry key="numcols">{numcols}</Entry>
			<Entry key="levelsback">{levelsback}</Entry>
            <Entry key="functionset">{functionset}</Entry>
        </Cartesian>
    </Genotype>
    <Registry>
        <Entry key="batch.repeats">0</Entry>
        <Entry key="log.level">0</Entry>
        <Entry key="mutation.indprob">{mutation_indprob}</Entry>
        <Entry key="population.size">{population_size}</Entry>
        <Entry key="term.maxgen">{term_maxgen}</Entry>
        <Entry key="term.fitnessval">{term_fitnessval}</Entry>
    </Registry>
</ECF>
"""

class ECFCGP(ECFBaseRegressor):
    def __init__(self, numvariables=3, numoutputs=1, numrows=2, numcols=5, levelsback=3, functionset="+ * - / pos min max",
                mutation_indprob=0.7, population_size=1000, term_maxgen=50, term_fitnessval=0, linear_scaling=True):
        super().__init__(parameters, ModelType.CGP_MODEL)
        self.numvariables = numvariables
        self.numoutputs = numoutputs
        self.numrows = numrows
        self.numcols = numcols
        self.levelsback = levelsback
        self.functionset = functionset
        self.mutation_indprob = mutation_indprob
        self.population_size = population_size
        self.term_maxgen = term_maxgen
        self.term_fitnessval = term_fitnessval
        self.linear_scaling = linear_scaling
    
    def fit(self, X, y):
        X, y = check_X_y(X, y)
        real_numvariables = X.shape[1]

        self.parameters = self.parameters.format(**self.get_params(), real_numvariables=real_numvariables)
        
        super().fit(X, y)
