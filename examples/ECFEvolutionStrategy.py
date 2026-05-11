from ecf_sr import ECFTree

parameters = """
<ECF>
    <Algorithm>
        <EvolutionStrategy>
            <Entry key="lambda">{lmbda}</Entry>
            <Entry key="mu">{mu}</Entry>
            <Entry key="rho">{rho}</Entry>
            <Entry key="selection">{selection}</Entry>
        </EvolutionStrategy>
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


class ECFEvolutionStrategy(ECFTree):
    def __init__(self, lmbda=4, mu=1, rho=1, selection="plus",
                functionset="+ - * / min max", maxdepth=6, terminalset="1 [-1 1]",
                mutation_indprob=0.3, population_size=500, term_stagnation=20, term_fitnessval=0,
                linear_scaling=True, random_state=0):
        super().__init__(functionset, maxdepth, terminalset,
                        mutation_indprob, population_size, term_stagnation, term_fitnessval,
                        linear_scaling, random_state)
        self.parameters = parameters
        self.lmbda = lmbda
        self.mu = mu
        self.rho = rho
        self.selection = selection
