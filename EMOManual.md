# Evolutionary Multiobjective Optimization Manual #

**TODO: An important update is require here**

Since version 2.1.0, Open BEAGLE includes support for Evolutionary Multiobjective Optimization (EMO). For this purpose, two fitness measure types have been defined in the framework, that is a maximization one named `FitnessMultiObj`, and its minimization equivalent `FitnessMultiObjMin`. In both these two fitness measures, a Pareto domination test has been added in method `isDominated`. If the user want to use a different multiobjective measure, for example a fitness measure comprising a mix of minimizing and maximizing objectives, the method `isDominated` of the new fitness measure class must be over-defined accordingly.

The other element of EMO support in Open BEAGLE is the multiobjective selection operator. There is currently two of these implemented, that is [NSGA-II](http://www.iitk.ac.in/kangal/Deb_NSGA-II.pdf), implemented as a replacement strategy of a breeder model, and the [NPGA 2](http://dl.acm.org/citation.cfm?id=739267), implemented as a classical selection operator. There is also a multiobjective-specific hall-of-fame class called `ParetoFrontHOF`, that contains the population's Pareto front.

To make use of it in your application, you must do the three following things:
  1. Return a multiobjective fitness measure (class `FitnessMultiObj` or derived) in your evaluation operator;
  1. Set the population allocators for the used multiobjective fitness measure allocators in the `main` routine;
  1. Set-up a configuration file with the appropriate evolver structure, that is a multiobjective selection operator and a statistics calculation operator compatible with the fitness measure used.

The basic EMO fitness measure is implemented in class `FitnessMultiObj`, which maximize all the objectives. A derived fitness measure is also defined in class `FitnessMultiObjMin` and consists to minimize the objectives. You should either use one of the two preceding standard EMO fitness measure, or use a custom one derived from these as fitness measure. The class `FitnessMultiObj` is derived from a `std::vector<double>` and thus the user should use the STL vector interface to set the different objectives value.

Once the type of fitness measure used is stated in the evaluation operator, the population allocators must be set up to use it. This can be done by following the explanations given in Section [Building a System for an Evolution](UserGuide#Building_a_System_for_an_Evolution.md), at the item population. This is necessary to clone/copy fitness value between individuals and to read a milestone from uninitialized population.

Finally, the evolutionary algorithm must be set-up for the use of EMO. This first imply that a multiobjective selection operation must be used. There is actually two of these in Open BEAGLE, NSGA-II implemented as replacement strategy operator `NSGA2Op`, and NPGA 2 implemented as a more classical selection operator `NPGA2Op`. The following presents how the NSGA-II configuration file of the multiobjective bit string GA example `knapsack`.
```
<?xml version="1.0" encoding="ISO-8859-1"?>
<Beagle version="2.1.0">
  <Evolver>
    <BootStrapSet>
      <IfThenElseOp parameter="ms.restart.file" value="">
        <PositiveOpSet>
          <GA-InitBitStrOp/>
          <KnapsackEvalOp/>
          <StatsCalcFitnessMultiObjOp/>
        </PositiveOpSet>
        <NegativeOpSet>
          <MilestoneReadOp/>
        </NegativeOpSet>
      </IfThenElseOp>
      <TermMaxGenOp/>
      <MilestoneWriteOp/>
    </BootStrapSet>
    <MainLoopSet>
      <NSGA2Op>
        <KnapsackEvalOp>
          <GA-CrossoverOnePointBitStrOp>
            <SelectRandomOp/>
            <SelectRandomOp/>
          </GA-CrossoverOnePointBitStrOp>
        </KnapsackEvalOp>
        <KnapsackEvalOp>
          <GA-MutationFlipBitStrOp>
            <SelectRandomOp/>
          </GA-MutationFlipBitStrOp>
        </KnapsackEvalOp>
      </NSGA2Op>
      <MigrationRandomRingOp/>
      <StatsCalcFitnessMultiObjOp/>
      <TermMaxGenOp/>
      <ParetoFrontCalculateOp/>
      <MilestoneWriteOp/>
    </MainLoopSet>
  </Evolver>
</Beagle>
```
The NSGA-II algorithm proceed first by generating a population of _n_ children from a population of _n_ parents. This is actually done here by calling the breeder tree of the `NSGA2Op` replacement strategy _n_ times. The breeder tree generates the children one at the time by either applying one point crossover or bit mutation on randomly chosen individuals from the parent individuals, and then evaluating fitness of the newly generated individuals. When the _n_ children are generated, the parents and children populations are merged and a non-dominated sort is done to keep the _n_ best, which constitutes the new population. Note in the previous configuration file, the use of operator `StatsCalcFitnessMultiObjOp`, which is the appropriate statistics calculation operator for fitness measures `FitnessMultiObj` and `FitnessMultiObjMin`.

The NPGA 2 EMO selection operator can be more simply used as a replacement of a usual mono-objective selection operators. The following configuration file shows the use of NPGA 2 in the `knapsack` EMO example.
```
<?xml version="1.0" encoding="ISO-8859-1"?>
<Beagle version="2.0.0">
  <Evolver>
    <BootStrapSet>
      <IfThenElseOp parameter="ms.restart.file" value="">
        <PositiveOpSet>
          <GA-InitBitStrOp/>
          <KnapsackEvalOp/>
          <StatsCalcFitnessMultiObjOp/>
        </PositiveOpSet>
        <NegativeOpSet>
          <MilestoneReadOp/>
        </NegativeOpSet>
      </IfThenElseOp>
      <TermMaxGenOp/>
      <MilestoneWriteOp/>
    </BootStrapSet>
    <MainLoopSet>
      <NPGA2Op/>
      <GA-CrossoverOnePointBitStrOp/>
      <GA-MutationFlipBitStrOp/>
      <KnapsackEvalOp/>
      <MigrationRandomRingOp/>
      <StatsCalcFitnessMultiObjOp/>
      <TermMaxGenOp/>
      <ParetoFrontCalculateOp/>
      <MilestoneWriteOp/>
    </MainLoopSet>
  </Evolver>
</Beagle>
```
This time the formulation of the evolver structure is more straight-forward, without the use of a breeder tree. Note that once again the multiobjective fitness measure statistics calculation operator `StatsCalcFitnessMultiObjOp` is used.

Attentive reader may note the use of an operator named `ParetoFrontCalculateOp` in the evolver's main-loop operator set. This operator is used to compute the Pareto front of the population into the hall-of-fame, just before exiting. This operator must be put in the main-loop operator set after the termination operator (in this case operator `TermMaxGenOp`), to intercept the moment that the evolution must stop, but before the milestone writing operator to get the Pareto front written into the milestone. It is not compulsory to use this operator in your evolver, although it may be very useful for result analysis.