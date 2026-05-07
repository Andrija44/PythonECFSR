from .ECFBaseRegressor import ECFBaseRegressor, ModelType
from sklearn.utils.validation import check_X_y

parameters = """
<ECF>
    <Algorithm>
		<GEP>
			<Entry key="crxprob">{crxprob}</Entry>
			<Entry key="selpressure">{selpressure}</Entry>
		</GEP>
	</Algorithm>
    <Genotype>
		<GEPChromosome>
            <Entry key="headlength">{headlength}</Entry>
			<Entry key="genes">{genes}</Entry>
			<Entry key="functionset">{functionset}</Entry>
			<Entry key="terminalset">{real_terminalset}</Entry>
			<Entry key="linkingfunctions">{linkingfunctions}</Entry>
			<Entry key="linklength">{linklength}</Entry>
		</GEPChromosome>
    </Genotype>
    <Registry>
        <Entry key="batch.repeats">0</Entry>
        <Entry key="log.level">0</Entry>
        <Entry key="mutation.indprob">{mutation_indprob}</Entry>
        <Entry key="population.size">{population_size}</Entry>
		<Entry key="term.maxgen">{term_maxgen}</Entry>
		<Entry key="term.eval">{term_eval}</Entry>
        <Entry key="term.fitnessval">{term_fitnessval}</Entry>
    </Registry>
</ECF>
"""

class ECFGEP(ECFBaseRegressor):
    def __init__(self, crxprob=0.7, selpressure=10, headlength=3, genes=2,
                functionset="avg sin + * - /", terminalset="1", linkingfunctions="+ * / - sin", linklength=1,
                mutation_indprob=0.5, population_size=100, term_maxgen=50, term_eval=5000, term_fitnessval=0.000001,
                linear_scaling=True):
        super().__init__(parameters, ModelType.GEP_MODEL)
        self.crxprob = crxprob
        self.selpressure = selpressure
        self.headlength = headlength
        self.genes = genes
        self.functionset = functionset
        self.terminalset = terminalset
        self.linkingfunctions = linkingfunctions
        self.linklength = linklength
        self.mutation_indprob = mutation_indprob
        self.population_size = population_size
        self.term_maxgen = term_maxgen
        self.term_eval = term_eval
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
