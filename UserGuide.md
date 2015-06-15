This page is a extensive presentation of what an application programmer, which is called the user from now, needs to know to develop EC process with Open BEAGLE. Some important aspects of C++ programming using Open BEAGLE are presented here. The discussion starts with what the user should and should not do (the guidelines). Following this is presented a simple pattern of what is needed to build a system for an evolution. Thereafter the different ways to modify the parameters of the system are exposed. Finally is presented the different approaches to customize the evolutionary process with Open BEAGLE.



# Guidelines #

Open BEAGLE is a powerful framework for EC. It is based on different elaborated mechanisms that make it both a flexible and programmer-friendly environment. But, these mechanisms require that to the user follows some general guidelines. A little adaptation effort is necessary to make the experience with Open BEAGLE as pleasant as possible. There is these general guidelines, accompanied by a deeper discussion on each of them.

**Guideline 1: Any instance (i.e. _C++ objects_) used in combination of Open BEAGLE must inherit, directly or indirectly, from the abstract `Object` class.**

Every Open BEAGLE classes are derived from the `Object` abstract class (with as exception the class `Pointer` and derived). The Open BEAGLE class architecture can be see as a tree with the `Object` class as root. The internal mechanisms of Open BEAGLE usually manipulate data types by using the common `Object` interface. It is then necessary for the user to integrate his application classes to the basic architecture. This is often done implicitly by sub-classing some Open BEAGLE entities. But, in some circumstances, a user may need to define a new class that do not have any relation with other existing Open BEAGLE classes. To do so, it is advised to use the `WrapperT` template to encapsulate these custom types.

**Guideline 2: Any Open BEAGLE objects that are smart pointed must be allocated on the heap and memory-managed by object handles.**

The smart pointer, also called handle, is a very useful mechanism that both simplify the task of the user and add global functionality and flexibility to the Open BEAGLE environment. But, users have to be cautious when they use smart pointers. They must be sure to allocate smart pointed objects on the heap, by a call to the `new` operator. This is fundamental because the smart pointed objects are automatically destructed, with a call to the `delete` operator, when their reference counter reach 0. This is incompatible with stack-allocated instances.

**Guideline 3: The nested types `Handle`, `Alloc`, and `Bag` must be defined in any object classes.**

In every classes of the Open BEAGLE framework, the types `Handle`, `Alloc`, `Bag` are defined. The handle type is a smart pointer to the type associated (i.e. a `Double::Handle` is a smart pointer of `Double`). An allocator is a type that can allocate, clone, and copy the associated type. A bag is an Open BEAGLE container of the given type. Both these types are used extensively by the Open BEAGLE mechanisms. These three types can be easily defined by a synonym declaration to the associated template classes, as in the following listing.
```
class MyClass : public SuperClass {
public: 
  typedef PointerT<MyClass,SuperClass::Handle>  Handle;
  typedef AllocatorT<MyClass,SuperClass::Alloc> Alloc;
  typedef ContainerT<MyClass,SuperClass::Bag>   Bag;
  ...
};
```
The templates take as first parametrized value the type of the associated object. In this case, the associated object is the type `MyClass`. The second parametrized value is the equivalent nested type of the direct superclass. In this case, the class `MyClass` inherits from the class `Object` so the equivalent nested type to a `MyClass::Handle` is a `Object::Handle`.

**Guideline 4: Any upcast of Open BEAGLE objects must be done by a call to the Open BEAGLE `castObjectT` operator for references and C-pointers, and by call to the Open BEAGLE `castHandleT` operator for smart pointers.**

With the ANSI/ISO C++ standard, it is highly recommended to use the new style cast operators instead of the old C-style one. This new style cast operators, such the operators `static_cast` and `const_cast`, are more specialized cast operations. The use of different cast operators prevent some unwanted casting operations that could lead to nasty hidden bugs. In the `Beagle` namespace, two new cast operators are defined, `castObjectT` and `castHandleT`. These operators must be used to cast any Open BEAGLE object type into another object type. The usage of this casting operator is identical to the usage of new style cast operators.

**Guideline 5: Any call to Open BEAGLE methods must be into a try-catch block to handle properly any threw Open BEAGLE exceptions.**

Open BEAGLE defines his own exceptions in spirit of the modern C++ exception handling mechanisms. In fact, an hierarchy of classes are defined to cover different cases of error, with at the top the class `Beagle::Exception`. When programming an application, the user should be aware to catch any Open BEAGLE exceptions and treat them accordingly. At least, the user should catch them and call the exception aborting method. The following code is an example of an internal structure of Open BEAGLE that detects an irregular situation and throws an appropriate exception.
```
if(n > size()) throw Beagle_RunTimeExceptionM("Out of bound!");
```
The macro `Beagle_RunTimeExceptionM` is a wrapper to create an exception object of the type `Beagle::RunTimeException`, with the message given and the good file name and line number. For almost all the exception classes of Open BEAGLE there is a wrapper macro associated to construct exception with the appropriate file name and line number. A `Beagle_AssertM` macro is also provided to check some condition as with the useful standard C `assert` macro. A simple way to achieve the current guideline is to try-catch all the `main` routine of the user application, to intercept any Open BEAGLE exceptions. Using the following code, the exceptions not previously caught is displayed at the standard error output, just before the program exit.
```
int main(int argc, char** argv) {
  try {
    ...
  } 
  catch(Beagle::Exception& inException) {
    inException.terminate();
  }
  return 0;
}
```


# Building a System for an Evolution #

To build an EC application with Open BEAGLE, some components need to be configured. Depending the type of evolutionary algorithm and the application, the configuration step is more or less demanding. Conceptually, the components to be configured are divided in three groups, the system, the population, and the operator and evolver.

## The system ##

The Open BEAGLE system of EC contains handles to some central entities used all over the evolution, such the register, the randomizer, the logger, and the context. Depending of the evolutionary algorithm used, the system can handles other specialized entities.

## The population ##

In Open BEAGLE, the representation of the individual is not restricted. The application developer can use different standard population representation or implement his own. However the way to instantiate a population for an evolution follows the same bottom-up pattern:
  1. Instantiate an allocator of the fitness type used;
  1. Instantiate an allocator of the genotype type used;
  1. Instantiate an allocator of the individual type used with the genotype and fitness allocators;
  1. Instantiate an allocator of statistics type used;
  1. Instantiate an allocator of the deme type used with the individual and statistics allocators;
  1. Instantiate the vivarium with the deme and statistics allocators.
As example, for multiobjective real-valued GA, the above pattern is translated into the following code.
```
FitnessMultiObj::Alloc::Handle lFitsAlloc  = new FitnessMultiObj::Alloc;
GA::FloatVector::Alloc::Handle lGenoAlloc  = new GA::FloatVector::Alloc;
Individual::Alloc::Handle      lIndivAlloc = 
  new Individual::Alloc(lGenoAlloc,lFitsAlloc);
Stats::Alloc::Handle           lStatsAlloc = new Stats::Alloc;
Deme::Alloc::Handle            lDemeAlloc  = new Deme::Alloc(lIndivAlloc,
                                                             lStatsAlloc);
Vivarium::Handle               lVivarium   = new Vivarium(lDemeAlloc,
                                                          lStatsAlloc);
```
For the other EC flavors the pattern is the same, only the types used must be changed. There is several other vivarium constructors, which can take directly, for example, allocators to genotypes and fitness values. You can use these instead of the previously exposed long pattern, if you only wanted to use a specific genotype of fitness type.

## The operators and the evolver ##

As explained in Section [Operators and Evolver](Architecture#Operators_and_Evolver.md) of the Architecture page, the evolver applies iteratively, at each generation, the operators on each deme. The operators are packed in the evolver into the bootstrap and the main-loop sets. For usual application, the user can instantiate the default evolvers. If the user want to customize the evolutionary algorithm, he can build his exotic evolver, using the configuration file. This is the topic of Section [Customizing the Evolutionary Algorithm](UserGuide#Customizing_the_Evolutionary_Algorithm.md). In all case, once the operator sets is built, the whole EC system is initialized with a call to the evolver `initialize` method. The method takes as arguments an handle to the system and the command-line arguments. Once the EC system initialized, the evolution can then be launch with a call to the method `evolve` of the evolver, taking the vivarium to evolve as argument.

All the EC applications are built following the previously presented scheme. To get more details on how to develop an application for a specific EC flavor, take a look in the [GP Tutorial](GPTutoral.md) and [GP Manual](GPManual.md) section for GP and [Bit String Representation Manual](BitStrManual.md) for bit string GA. Section [Customizing the Evolutionary Algorithm](UserGuide#Customizing_the_Evolutionary_Algorithm.md) gives also good insight on configuring a custom evolution.

# Using the Register #

The register is a central repository of all Open BEAGLE modifiable parameters. All these parameters must be registered when the EC system is initialized. The parameters are held in the register as a pair of tag and smart pointer. The tag is the unique identifier of a parameter associated to a smart pointer which is an indirection to the instance of the parameter.

## Registering Parameters ##

When the EC system is initialized, the different parameters must be registered by the appropriate holding objects. As example, a probability of crossover is registered at the initialization time by a crossover operator. Usually, the parameters are registered when the `registerParams` methods are called. The register is referred in the system object. The parameters are added in the register with a call to the method `insertEntry`. The method take as arguments the tag of the parameter, the smart pointer the value of the parameter, and the description of the parameter. It returns the inserted parameter, which should be assigned to a local variable. The following example add the parameters `ec.cx.indpb` with a reference to the floating point attribute `mMatingProba` of the crossover operator.
```
class CrossoverOp : public BreederOp {
public:
  CrossoverOp() { }
  virtual void registerParams(Beagle::System& ioSystem) {
	BreederOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Individual crossover probability",
	    "Double",
	    "0.5",
	    "Single individual crossover probability for a generation."
	);
	mMatingProba = castHandleT<Double>(
	                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.5), lDescription));
  }
}
  ...
protected:
  Double::Handle mMatingProba;
};
```
For registering a parameter, a description should be given. This description is used by the usage of an application and to explain the meaning of the parameter. Note that the parameter referenced by the smart pointer attribute `mMatingProba` is a `Double` Open BEAGLE object instead of a C++ `double`. The type `Double` is a wrapper of `double`. All the parameters registered must be smart pointed, which mean that they must be Open BEAGLE objects.

## Interaction with the Register ##

The application programmer can fetch parameters from the register with a call to the method `getEntry` (or the operator `operator[]`) of the class `Register`. This method takes as argument a tag and returns an handle to the parameter associated. The handle returned is a smart pointer of `Object`. The developer must upcast the handle referred into a exact derived type, if needed. If the named parameter is not registered, the method `getEntry` (or the operator `operator[]`) returns a null handle.
```
void MyClass::MyMethod(System::Handle ioSystem) {
  Object::Handle lParameter = ioSystem->getRegister()["ec.cx.indpb"];
  if(lParameter == NULL) {
    std::cout << "No crossover probability registered" << std::endl;
  }
  else {
    try {
      Double::Handle lCastedParameter = castHandleT<Double>(lParameter);
      std::cout << "The crossover probability is " 
                << *lCastedParameter << std::endl;
    } 
    catch(Beagle::BadCastException&) {
      std::cout << "Object registered is not of the Double type" << std::endl;
    }
  }
}
```
The exception handling of the object casting is to intercept some abnormal cases, that is when the parameter fetched is not of the desired `Double` type.


# Modifying Parameters #

The parameters recorded in the register can be modified in three different ways: by directly modifying the value in the program, by passing specials arguments on the command-line and by using configuration files. The initialization sequence is the following:
  1. Initialize the parameters to their default value (at the registration time);
  1. Read the default configuration file, if present in the current directory;
  1. Parse the command-line, the parameters being modified in the order they are expressed (left to right);
  1. Read the configuration files given on the command-line, if any. The files are read at the time the configuration file argument is parsed.

## Modifying Parameters on the Command-Line ##

It is possible to modify the parameters registered with arguments on the command-line. When an evolver is initialized in the method `initialize`, it takes the command-line value (i.e the variables `argc` and `argv`) as method argument. This is done to parse the command-line and extract Open BEAGLE specific arguments. All the Open BEAGLE specific arguments start with the prefix `-OB`, followed by the tag, an equal sign and the values of the parameter. As example, if our executable is named `maxfct` and the GA crossover probability must be changed to 0.75, the command-line would be:
```
./maxfct -OBga.cx.indpb=0.75
```
If more than a parameter must be changed on the command-line, the user can specify numerous arguments starting with a `-OB`, or enumerate all of them following a single `-OB`, separated by comas:
```
./maxfct -OBga.cx.indpb=0.75,ec.term.maxgen=100
```
Note that all the command-line arguments starting with the `-OB` prefix are erased from the structure `argc/argv` once they are parsed.

There are four special parameters that can be given on the command-line. First, there is the special argument `usage`, that made the application to display the tag with a short description of the available parameters and exit normally without evolving.
```
./maxfct -OBusage
```
The second special argument, `help`, is very similar to the first one except that it display the tags with everything you want to know on the parameters and exit normally without evolving.
```
./maxfct -OBhelp
```
The third special argument, `system`, can be assigned to the configuration file to use to set the parameters of the evolution. This argument take as value a string that contains the name of the configuration file.
```
./maxfct -OBsystem=maxfct.conf
```
And finally, the fourth special command-line argument, `dump`, can be used to generate configuration file with the default parameters value for a given application. This argument takes the name of the configuration file to generate as value.
```
./maxfct -OBdump=mymaxfct.conf
```

## Modifying Parameters Using a Configuration File ##

Another interesting way to modify parameters is to use a configuration file. This file, formatted in XML, contains the new value of some or all of the parameters registered. The file format is quite straightforward in the XML style, the logical sequence being preserved. The following is a presentation of a short, but valid, Open BEAGLE XML configuration file.
```
<?xml version="1.0" encoding="ISO-8859-1"?>
<Beagle version="2.1.1">
  <Register> 
    <Entry key="ec.hof.demesize">0</Entry>
    <Entry key="ec.hof.vivasize">1</Entry>
    <Entry key="ec.mig.interval">1</Entry>
    <Entry key="ec.mig.size">5</Entry>
    <Entry key="ec.pop.size">100</Entry>
    <Entry key="ec.sel.tournsize">2</Entry>
    <Entry key="ec.term.maxgen">50</Entry>
    <Entry key="ga.cx1p.prob">0.3</Entry>
    <Entry key="ga.init.bitpb">0.5</Entry>
    <Entry key="ga.init.bitstrsize">125</Entry>
    <Entry key="ga.mutflip.bitpb">0.01</Entry>
    <Entry key="ga.mutflip.indpb">1</Entry>
    <Entry key="lg.console.enabled">1</Entry>
    <Entry key="lg.file.name">beagle.log</Entry>
    <Entry key="lg.log.level">3</Entry>
    <Entry key="lg.show.class">0</Entry>
    <Entry key="lg.show.level">0</Entry>
    <Entry key="lg.show.type">0</Entry>
    <Entry key="ms.write.interval">0</Entry>
    <Entry key="ms.write.over">1</Entry>
    <Entry key="ms.write.perdeme">0</Entry>
    <Entry key="ms.write.prefix">beagle</Entry>
  </Register>
</Beagle>
```

# Customizing the Evolutionary Algorithm #

The present section thread of important aspect of Open BEAGLE: how to customize an existing evolutionary algorithm (EA). This can be as simple as to modify a genetic operator of a standard EA to the more complex task of implementing a brand new EA. In any cases, this is an advanced topics that a newcomer can skip in the first time. This section presents the important concepts that is related to the customizing of the EA. It is advised to consult the reference manual for the implementation details.

## Building a Custom Evolver ##

The evolver is a high level entity that apply the iterative evolution process on a population. An evolver is composed of two operator sets, the bootstrap and the main-loop. The bootstrap operators are applied once on the population, at the beginning of an evolution (at generation 0). The main-loop operators are then applied iteratively, until the termination state is reached. These iterations can be seen as generations in a generational evolution model or as certain amount of individual processing in the steady-state evolution model. In all case the basic algorithm is independent of the underlying evolution operations.

Evolvers can be dynamically configured directly by the configuration file. The set-up of the evolver is stated between two `Evolver` tag, with the list of the operators to used listed in the `BootStrapSet` and the `MainLoopSet` tags. The following XML snippet presents the default evolver of bit string GA, for the `maxfct` example.
```
<?xml version="1.0" encoding="ISO-8859-1"?>
<Beagle version="2.1.0">
  <Evolver>
    <BootStrapSet>
      <IfThenElseOp parameter="ms.restart.file" value="">
        <PositiveOpSet>
          <GA-InitBitStrOp/>
          <MaxFctEvalOp/>
          <StatsCalcFitnessSimpleOp/>
        </PositiveOpSet>
        <NegativeOpSet>
          <MilestoneReadOp/>
        </NegativeOpSet>
      </IfThenElseOp>
      <TermMaxGenOp/>
      <MilestoneWriteOp/>
    </BootStrapSet>
    <MainLoopSet>
      <SelectTournamentOp/>
      <GA-CrossoverOnePointBitStrOp/>
      <GA-MutationFlipBitStrOp/>
      <MaxFctEvalOp/>
      <MigrationRandomRingOp/>
      <StatsCalcFitnessSimpleOp/>
      <TermMaxGenOp/>
      <MilestoneWriteOp/>
    </MainLoopSet>
  </Evolver>
</Beagle>
```
When an user want to change the evolutionary algorithm by changing the order of operations, or using other standard operators, he only needs to modify the configuration file accordingly. If the user wants to use custom operators, he must first implement them, as explained at the following section, and then add an instance of them to the evolver, using the method `addOperator`, before initializing the evolver. Several typical configuration files are given with the examples to illustrate how these can be used to modify the EA used.

## Custom Evolver with a Breeder Tree ##

The breeder model (see Section [Breeder Model](Architecture#Breeder_Model.md) of the Architecture page} allows the implementation of sophisticated EA. For example, the steady-state formulation of GA, you can use a specialized configuration file using the steady-state replacement strategy operator, with crossover, mutation, and selection operators structured under it as a breeder tree. The configuration file of a steady-state version of `maxfct` example, using a breeder tree would be the following.
```
<?xml version="1.0" encoding="ISO-8859-1"?>
<Beagle version="2.1.0">
  <Evolver>
    <BootStrapSet>
      <IfThenElseOp parameter="ms.restart.file" value="">
        <PositiveOpSet>
          <GA-InitBitStrOp/>
          <MaxFctEvalOp/>
          <StatsCalcFitnessSimpleOp/>
        </PositiveOpSet>
        <NegativeOpSet>
          <MilestoneReadOp/>
        </NegativeOpSet>
      </IfThenElseOp>
      <TermMaxGenOp/>
      <MilestoneWriteOp/>
    </BootStrapSet>
    <MainLoopSet>
      <SteadyStateOp>
        <MaxFctEvalOp>
          <GA-CrossoverOnePointBitStrOp>
            <SelectTournamentOp/>
            <SelectTournamentOp/>
          </GA-CrossoverOnePointBitStrOp>
        </MaxFctEvalOp>
        <MaxFctEvalOp>
          <GA-MutationFlipBitStrOp>
            <SelectTournamentOp/>
          </GA-MutationFlipBitStrOp>
        </MaxFctEvalOp>
        <SelectTournamentOp/>
      </SteadyStateOp>
      <MigrationRandomRingOp/>
      <StatsCalcFitnessSimpleOp/>
      <TermMaxGenOp/>
      <MilestoneWriteOp/>
    </MainLoopSet>
  </Evolver>
</Beagle>
```
The important thing to understand is that the steady-state operator (`SteadyStateOp`) is a replacement strategy and that the XML tree under it (between the `<SteadyStateOp>` and `</SteadyStateOp>` tags)) is its breeding tree. The three sub-trees under it are called randomly following a uniform probability density function parametrized by their respective breeding probability. For the first breeding sub-tree to the steady-state replacement strategy, the breeding probability is given by parameter `ga.cx1p.indpb`, while for the second sub-tree the breeding probability is given by parameter `ga.mutflip.indpb`, and the last sub-tree breeding probability is given by parameter `ec.repro.prob`. The steady-state replacement strategy generates _n_ individuals with its breeder tree at each generation, where _n_ is the size of the population. Each newly generated individual replaces a randomly chosen individual existing in the actual population in a steady-state fashion. Once the configuration file set-up correctly, an evolution using the given EA structure can be launched by given the good configuration file to use on the command-line. See previous Section [Modifying Parameters on the Command-Line](UserGuide#Modifying_Parameters_on_the_Command-Line.md) for more details.

## Defining New Operators ##

An operator is a specialized agent that apply an operation to an EC system. It is generally a genetic or utility operations such applying the crossover operation on a deme or calculating the fitness statistics for the actual generation. The operators are generally applied on one deme at the time for each generation, or sometime once a generation on all the population.

The definition of a new operator is not that complicated, usually the developer only needs to subclass the `Operator` class and over-define the method `operate`. The method received as argument a `Context` object. All the important contextual data are in the context object: smart pointers to the actual population, deme and system, the actual generation, etc. Furthermore, some EA use an extended context that haves some more specific informations in it. Several general pre-defined operators are already defined for most common operations such crossover, mutation, and selection. For these operators, the `operate` is usually already defined, and another operator-specific method need to be coded. This operator-specific method is usually the piece of coded very specific to the operation and representation used. Take a look into the code of existing operator to figure out the specificities of each case.

An usual operator also holds some evolution parameters, probabilities, and such. These parameters must be registered in the register of the evolutions system. This is done by redefining the `registerParams` method by calling the appropriation registration method of the class `Register`. The method `registerParams` is called once an evolution, before any the operator is applied to any population.

The user that define his own operators must be aware that the system is not fully set-up when the `registerParams` method is called. For example, the random number generator must not be used, as the seed can be modified thereafter on the command-line or a configuration file. The rule is that the `registerParams` method must be used only to add elements to the evolution system. If the user defines a new operator derived from an existing one, he must also call the `registerParams` method from the super-class operator, as illustrated here.
```
class DerivedClassOp: public SuperClassOp {
  virtual void registerParams(System& ioSystem) {
    SuperClassOp::registerParams(ioSystem);
    // DerivedClassOp initialization code follow ...
  }
};
```
This is necessary as the super-class operator might do some specific operation such as registering its parameter.

The `init` operator method is the a companion method of `registerParams`. The `initialize` method is called once for each operator, just after the parameters are registered and the whole system is initialized. This method is best to do some parameters value checking and to setup operators internal structures. As with method `registerParams`, if the `init` method is over-defined in a given operator, the parent's `init` must be called in the implementation before executing the actual operator post-initialization code.

There is a important hierarchy of standard defined operators in Open BEAGLE. The user is advised to interface his operators classes to already defined operator, using inheritance. This is in the spirit of OO programming and code reusing.