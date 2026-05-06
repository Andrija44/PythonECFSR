from .ECFBaseRegressor import ECFBaseRegressor

parameters = """<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<ECF>
    <Algorithm>
        <Elimination>
            <Entry key="gengap">{gengap}</Entry>
            <Entry key="selpressure">{selpressure}</Entry>
        </Elimination>
    </Algorithm>
    <Genotype>
        <Tree>
            <Entry desc="" key="functionset">{functionset}</Entry>
            <Entry desc="" key="maxdepth">{maxdepth}</Entry>
            <Entry desc="" key="terminalset">{real_terminalset}</Entry>
        </Tree>
    </Genotype>
    <Registry>
        <Entry desc="" key="randomizer.seed">{real_random_state}</Entry>
        <Entry desc="" key="batch.repeats">0</Entry>
        <Entry desc="" key="log.level">0</Entry>
        <Entry desc="" key="mutation.indprob">{mutation_indprob}</Entry>
        <Entry desc="" key="population.size">{population_size}</Entry>
        <Entry desc="" key="term.stagnation">{term_stagnation}</Entry>
        <Entry desc="" key="term.fitnessval">{term_fitnessval}</Entry>
    </Registry>
</ECF>
"""

class ECFElimination(ECFBaseRegressor):
    def __init__(self, gengap=0.6, selpressure=10,
                 functionset="+ - *", maxdepth=4, terminalset="1 [-1 1]",
                 mutation_indprob=0.3, population_size=500, term_stagnation=20, term_fitnessval=0,
                 linear_scaling=True, random_state=None):
        super().__init__(parameters, functionset, maxdepth, terminalset,
                         mutation_indprob, population_size, term_stagnation, term_fitnessval,
                         linear_scaling, random_state)
        self.gengap = gengap
        self.selpressure = selpressure
