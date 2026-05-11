from ecf_sr import ECFTree

parameters = """
<ECF>
    <Algorithm>
        <GeneticAnnealing>
            <Entry key="energybank">{energybank}</Entry>
            <Entry key="coolingfactor">{coolingfactor}</Entry>
            <Entry key="elitism">{elitism}</Entry>
        </GeneticAnnealing>
    </Algorithm>
    <Genotype>
        <Tree>
            <Entry key="functionset">{functionset}</Entry>
            <Entry key="maxdepth">{maxdepth}</Entry>
            <Entry key="terminalset">{real_terminalset}</Entry>
        </Tree>
    </Genotype>
    <Registry>
        <Entry key="randomizer.seed">{random_state}</Entry>
        <Entry key="batch.repeats">0</Entry>
        <Entry key="log.level">0</Entry>
        <Entry key="mutation.indprob">{mutation_indprob}</Entry>
        <Entry key="population.size">{population_size}</Entry>
        <Entry key="term.stagnation">{term_stagnation}</Entry>
        <Entry key="term.fitnessval">{term_fitnessval}</Entry>
    </Registry>
</ECF>
"""


class ECFGeneticAnnealing(ECFTree):
    def __init__(self, energybank=100, coolingfactor=0.7, elitism=0,
                functionset="+ - * / min max", maxdepth=6, terminalset="1 [-1 1]",
                mutation_indprob=0.3, population_size=500, term_stagnation=20, term_fitnessval=0,
                linear_scaling=True, random_state=0):
        super().__init__(functionset, maxdepth, terminalset,
                        mutation_indprob, population_size, term_stagnation, term_fitnessval,
                        linear_scaling, random_state)
        self.parameters = parameters
        self.energybank = energybank
        self.coolingfactor = coolingfactor
        self.elitism = elitism
