

# Introduction #

This page is a tutorial on Open BEAGLE as a genetic programming framework. It first summarizes GP basics, and then proceeds with a simple example in order to illustrate both the power and ease of use of the framework.

# Genetic Programming #

This sub-section is not a tutorial about tree-based GP. It is rather a summary of what needs to be defined to resolve a problem with tree-based GP. If you are not familiar with GP, references (Banzhaf _et al._, 1998) and (Koza, 1992) are good introduction books on the topic.

To genetically program a computer for an application, three issues of the problem domain must be addressed:
  * What type of data elements are handled by the genetic programs?
  * What is the set of computation primitives?
  * How does one measures the fitness of individual programs?

To address the first issue, one needs to define the data type that will be manipulated by the genetic programs. In Open BEAGLE, these data types are called _datums_. The datums are application-specific. For example, if the goal of an evolution is to discover the mathematical expression of a hidden physical phenomenon, the datum could simply be floating-point numbers.

Once the datum type is defined, the primitives (more usually called functions and terminals in GP) that will compose the GP individuals must be specified. This step is important because these primitives are the building blocks of the genetic programs. A primitive is an application-specific operation to which the nodes of the GP trees are associated.

All primitives must process and return data of the specified datum type. A primitive is defined by three properties: its (unique) name, its number of arguments, and its characteristic function. These properties must be carefully established for all Open BEAGLE primitives.

Each primitives used are inserted into the set of usable primitives. This is a kind of instruction set for the genetic programs. A valid set of primitives must be made of at least one function primitive and one terminal primitive.

Finally, once the datum type and the set of primitives are established, the fitness function must be specified, that is the objective criteria that allow quality discrimination between solutions.

# Simple GP Example #

This section provides an implementation of a well known symbolic regression problem genetically programmed with Open BEAGLE. The discussion follows a top-down approach, starting with the `main` routine, followed by a description of the different implemented modules. Note that in this document, all C++ code is intentionally written in a compact format without separation between interface (class definition) and implementation (method definition). This is not a recommended practice for good C++ programming, nor for Open BEAGLE. Note also that header file `beagle/GP.hpp` must be included in the application code to get the declarations of the framework classes for GP. All generic Open BEAGLE classes are defined in the C++ namespace `Beagle`, while the GP specific classes are in namespace `GP`, which is in the global namespace `Beagle`.

## Symbolic Regression ##

A classical GP problem is the symbolic regression of a set of data points where the computer searches for an analytical function that fits these data points. The problem is fully described in (Koza, 1992), pages 162 to 169. Following table summarizes this problem.

| Objective | Find a function f(x) = x<sup>4</sup> + x<sup>3</sup> + x<sup>2</sup> + x, in its symbolic form, from a set of sampled points (x<sub>i</sub>,f(x<sub>i</sub>)).|
|:----------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Datum     | Floating-point numbers.                                                                                                                                       |
| Terminals set | x (the independent variable).                                                                                                                                 |
| Functions set | `+`, `-`, `*`, and `/`. `/` is a protected division returning 1.0 when the denominator is near to 0 (0 ± 0.001).                                             |
| Fitness cases | A random sample of 20 points (x<sub>i</sub>,f(x<sub>i</sub>)), with x<sub>i</sub> in [-1,1].                                                                  |
| Raw fitness | Sum of the square error for all the fitness cases: F<sub>raw</sub> =  ∑<sub>i=0..19</sub> (f<sub>obtained</sub>(x<sub>i</sub>) - f<sub>desired</sub>(x<sub>i</sub>))<sup>2</sup>. |
| Standardized fitness | Mean square error (MSE) for all the fitness cases: F<sub>standard</sub> = F<sub>raw</sub> / 20.                                                               |
| Adjusted fitness | Root mean square error (RMSE) for all the fitness cases: F<sub>adjusted</sub> = sqrt(F<sub>standard</sub>).                                                   |
| Normalized fitness | F<sub>normalized</sub> = 1.0 / (1.0 + F<sub>adjusted</sub>).                                                                                                  |
| Hits      | Number of fitness cases for which the difference between the desired and obtained value is less than 0.01.                                                    |
| Stop criterions | Whether the evolution reached the maximum number of generations allowed, or that an individual with 20 hits is found.                                         |

## Function main ##

To define the `main` routine of a Open BEAGLE GP application, the following five steps are needed:
  1. Build primitives (`GP::Primitive`) and insert them into one or more primitive sets (`GP::PrimitiveSet`);
  1. Build an evolutionary system (`GP::System`) using primitive sets;
  1. Build evaluation (`GP::EvaluationOp`);
  1. Build an evolver (`GP::Evolver`);
  1. Build a vivarium (`GP::Vivarium`).

Afterward, the genetic system is ready to evolve. Here is the C++ code for the `main` function of the symbolic regression.
```
01  #include <beagle/GP.hpp>
02  #include "SymbRegEvalOp.hpp"
03  using namespace Beagle;
04  int main(int argc, char *argv[]) {
05    try {
06      // 1: Build primitive set
07      GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
08      lSet->insert(new GP::Add);
09      lSet->insert(new GP::Subtract);
10      lSet->insert(new GP::Multiply);
11      lSet->insert(new GP::Divide);
12      lSet->insert(new GP::TokenT<Double>("x"));
13      // 2: Build a system
14      GP::System::Handle lSystem = new GP::System(lSet);
15      // 3: Build evaluation operator
16      SymbRegEvalOp::Handle lEvalOp = new SymbRegEvalOp;
17      // 4: Build evolver
18      GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
19      // 5: Build vivarium
20      GP::Vivarium::Handle lVivarium = new GP::Vivarium;
21      // 6: Initialize and evolve the vivarium
22      lEvolver->initialize(lSystem,argc,argv);
23      lEvolver->evolve(lVivarium);
24    }
25    catch(Exception& inException) {
26      inException.terminate();
27    }
28    return 0;
29  }
```

Step 1 (lines 7 to 12) defines the set of usable primitives. The `GP::PrimitiveSet` object is a container for our primitives. The GP package of Open BEAGLE predefines several common primitives (see Section [GP Primitives Library](Architecture#GP_Primitives_Library.md) of the Architecture page). Here we use only basic arithmetic operators: add, subtract, multiply, and divide. The generic primitive `GP::TokenT<Double>` is used to contain a floating point number that represents the independent variable of the regression. It is of type  `Double` which is a simple Open BEAGLE wrapper for atomic type `double`. All GP primitives must be allocated on the heap (with a call to operator `new`), as shown in the example. Henceforth, they will be memory-managed by the primitives set (see Section [Pointers](Architecture#Pointers.md) of the Architecture page for more details). The primitives themselves are defined through inheritance from abstract class `GP::Primitive` (see Section [GP Primitives and Sets](Architecture#GP_Primitives_and_Sets.md) of the Architecture page).

Step 2 (line 14) builds the system, that is an object that centralize references to the important entities of the Open BEAGLE framework: the context, the register, the logger, the randomizer and, for GP, the set of usable primitives. These entities are described in more details in the [Architecture](Architecture.md) page. In most cases, the Open BEAGLE user does not need to interact with these entities. They need to be used explicitly only for advanced applications, or for developing new evolutionary algorithms.

Step 3 (lines 16) concerns the creation of evaluation operator. The evaluation operator is an object that implements the fitness evaluation method. The user needs to subclass abstract class `GP::EvaluationOp` and define an `evaluate` method. In this case, we use class `SymbRegEvalOp` to evaluate the fitness of an individual (i.e. a regressed function). This class is detailed in Section [Fitness Evaluation](GPTutorial#Fitness_Evaluation.md).

Step 4 (line 18) builds an evolver that encapsulates the operators of the evolutionary algorithm. These operators refer to all processes that evolve populations in a given paradigm. In the current example, we use the default GP evolver, class `GP::Evolver`. This includes the _ramped half-and-half_ initialization, a tournament selection mechanism, standard crossover and three different mutation operators, and miscellaneous operations like saving populations milestone and computing generational statistics. The user can change the operations of predefined evolver from either a configuration file or some programming. This generic approach, which is one of the most powerful features of Open BEAGLE, when combined with generic population allocators, enables the construction of any evolutionary paradigm.

Step 5 (line 20) builds the vivarium that contains the population. A vivarium is composed of a set of demes, which are composed of individuals that are themselves composed of genotypes. A deme, Open BEAGLE class `Deme`, is simply a set of individuals that evolve together in a closed circuit. An individual is composed by one or some genotypes and a fitness value. It can in fact contain more than one genotype. For instance, Automatically Defined Functions (ADF) (Koza, 1994) are coded using a set of genotypes (i.e. forest of trees). The genotype is the basic data structure used for coding individuals. For GP, this data structure is a rooted tree, defined by class `GP::Tree`. Using the `GP::Vivarium`, the vivarium is initialized using the default allocators of demes/individuals/genotypes for GP. However it is possible for the user to override the default setting of the vivarium and used allocators to desired population elements. More details about allocators can be found in Section [Allocators](Architecture#Allocators.md) of the Architecture page.

At line 22, the evolver is initialized using both command-line arguments and the Open BEAGLE system. Through the command-line, it is possible to change the value any of the evolution parameters (how to do this is explained in Section [Experimenting with the Example](GPTutorial#Experimenting_with_the_Example.md)). A XML configuration file is also parsed by the initialize method (see Section [Using the Register](UserGuide#Using_the_Register.md) of the User Guide for more details). Line 23 then makes the vivarium evolving until the stopping criterion is reached.

Notice the fact that all instructions in function `main` are within a try-catch block. Open BEAGLE defines many standard exceptions for dealing with abnormal situations. See Section [Exceptions](Architecture#Exceptions.md) of the Architecture page for more details. The users are encouraged to proceed this way. Note also that every Open BEAGLE object is dynamically allocated (by a call to the C++ `new` operator) and its pointer is wrapped using a `Handle` object. The `Handle` type is a smart pointer that manages objects allocated on the heap. The use of smart pointers prevents some common memory leaks and facilitate the work of Open BEAGLE users. These handlers are fully described in Section [Fitness Evaluation](GPTutorial#Fitness_Evaluation.md).

## Datum and Primitives ##

In the Open BEAGLE framework, every class inherits from an abstract base class named `Object` (there are some exceptions to this rule). This class includes many virtual methods that makes the framework very powerful, based on strong abstractions and sophisticated OO mechanisms. But because of this, the GP primitives cannot directly use atomic C++ types like `int` or `double`. They must use an object wrapper that adapts the desired type to the `Object` class. Thus, GP primitives must be designed to manipulate object derived from type `GP::Datum` which is a simple typedef:
```
namespace GP {
  typedef Object Datum;
}
```

To simplify the wrapping of external types, Open BEAGLE defines a class template, named `WrapperT`, that implements the virtual methods of class `Object`. The methods of `WrapperT` are implemented as calls to the usual C++ operators (i.e. constructor, destructor, copy-constructor, assignment operator, operators `==` and `<`, etc). Moreover, casting operators are also defined in order to enable conversion between wrapper and wrapped types.

For the example of the symbolic regression, standard Open BEAGLE wrapper type `Double` (note the capital letter) is used to encapsulate floating point numbers:
```
typedef WrapperT<double> Double;
```

If unsigned integers were needed, a corresponding wrapper type would be used. For instance, Open BEAGLE type `UInt` is defined as:
```
typedef WrapperT<unsigned int> UInt;
```

Other wrapper types such as `Float` and `Bool` are also defined in Open BEAGLE (see Section [Object Wrappers](Architecture#Object_Wrappers.md) of the Architecture page for more details).

After defining a common datum type for an application, the primitives themselves need to be defined. For the symbolic regression problem, we have chosen to use four of the predefined arithmetic operators: `+`, `-`, `*`, and `/`. Next listing gives the C++ code for class `GP::Add` that implements the `+` primitive between two `Double` data.
```
01  #include "beagle/GP.hpp"
02  using namespace Beagle;
03  class Add : public GP::Primitive {
04  public:
05    Add() : GP::Primitive(2, "+") { }
06    virtual void execute(GP::Datum& outResult,GP::Context& ioContext) {
07      Double lArg1;
08      get1stArgument(lArg1,ioContext);
09      Double lArg2;
10      get2ndArgument(lArg2,ioContext);
11      Double& lResult = castObjectT<Double&>(outResult);
12      lResult = lArg1 + lArg2;
13    }
14  };
```

The definitions of the other three primitives are almost identical. Any primitive must inherit from abstract class `GP::Primitive` (line 3) and define method `execute` (line 6). It is this method that defines the characteristic function of the primitive. A primitive is usually constructed with a call to the constructor of its superclass, as in line 5. The first argument of this call specifies the number of arguments for the corresponding primitive, while the second argument assigns a string label (a name) to it.

Method `execute` (line 6) receives two arguments: a reference to a `GP::Datum` for returning its result, and a reference to the current context that holds the state of the whole evolutionary process, class `GP::Context`. The later is needed for evaluating the operands of the primitive. Although the Open BEAGLE context is a relatively complex object, the end user need not know anything about its internal working. He is only required to pass on this reference to method `get1stArgument` and `get2ndArgument` implemented in `GP::Primitive`, in order to retrieve the primitive's operands. This is done at lines 8 and 10 for the two operands of our primitive. The two arguments of methods `get1stArgument` and `get2ndArgument` are respectively: the datum for returning the value, and the current context. In order to return its own result, method `execute` also needs to cast the `GP::Datum` reference that it received at line 6, into an adequate reference for assigning the result of it computation, in this case a `Double`. This is done at line 11 using the Open BEAGLE `castObjectT` method that implements a dynamic cast.

In the listing of the `main` function, at line 12, a special primitive of type `GP::TokenT<Double>` was also inserted into the primitive set. This standard primitive is for encapsulating an input variable of the problem at hand. In this case, the datum is of type `Double`, and the variable is named `x` (for more details, the reader is referred to Section [GP Primitives and Sets](Architecture#GP_Primitives_and_Sets.md) of the Architecture page).


## Fitness Evaluation ##

**NOTE: The code of the symbolic regression example is now slighlty different from this one. An update is required here.**

In this section, we examine the C++ code of class `SymbRegEvalOp` which implements the fitness evaluation function for our symbolic regression application.
```
01  #include <cmath>
02  #include <vector>
03  #include "beagle/GP.hpp"
04  using namespace Beagle;
05  class SymbRegEvalOp : public GP::EvaluationOp {
06  private:
07    std::vector<Double> mX; // Sampled x-axis values
08    std::vector<Double> mY; // Sampled y-axis values
09  public:
10    SymbRegEvalOp() { }
11    virtual Fitness::Handle evaluate(GP::Individual& inIndividual,
12                                     GP::Context& ioContext) {
13      double lQErr = 0.; // square error
14      for(unsigned int i=0; i<mX.size(); i++) {
15        setValue("x",mX[i],ioContext);
16        Double lResult;
17        inIndividual.run(lResult,ioContext);
18        double lError = mY[i]-lResult;
19        lQErr += (lError*lError);
20      }
21      double lMSE  = lSquareError / mX.size();
22      double lRMSE = std::sqrt(lMSE);
23      double lFitness = (1.0 / (lRMSE + 1.0));
24      return new FitnessSimple(lFitness);
25    }
26    virtual void postInit(System& ioSystem) {
27      GP::EvaluationOp::postInit(ioSystem);
28      for(unsigned int i=0; i<20; i++) {
29        mX.push_back(ioSystem.getRandomizer().rollUniform(-1.,1.));
30        mY.push_back(mX[i]*(mX[i]*(mX[i]*(mX[i]+1.)+1.)+1.));
31      }
32    }
33  };
```

This class is not a standard Open BEAGLE class, it is derived from abstract class `GP::EvaluationOp`, and especially designed for our application. The method `evaluate` is declared as pure virtual in the class `GP::EvaluationOp`. This mean that the method must be overridden in a concrete subclass. The method receives as argument a reference to an individual to evaluate, and a reference to the context to use. In the evaluation method, an individual is parsed by a call to the method `run`  of `GP::Individual`. The result of the interpretation is put into the datum given as argument. The value of the input value of the individuals, can be modified for each evaluation case with a call to the method `setValue` of `GP::EvaluationOp`. The first argument of the call to the method is the name of the GP primitive to modify. The second argument is a reference to the datum that will contain the result of the evaluation. And finally, the third argument is the context to be used. The method `evaluate` terminates by returning a dynamically allocated fitness value object. The default fitness object type to use is `FitnessSimple`, which define a single fitness measure to maximize.

The method `postInit` (line 26 to 32) is a hook called one time by Open BEAGLE, just after the system has been initialized, but before fitness evaluation are done. In the actual case, the method is implemented to sample the equation to regress. This could not have been done before, as the random number generator is not valid until the system is completely initialized. But this also cannot be done after, as the equation to regress must be sampled before any fitness evaluation. This method `postInit` is originally defined in the class `Operator`, which is a parent of `EvaluationOp`.

Three nested data types are defined for all object-derived Open BEAGLE classes: `Handle`, `Alloc`, and `Bag`. The handle type (`Handle`) defines a smart pointer of the Open BEAGLE object to which it belong. It allows the emulation of a garbage collection mechanism similar to the one of the _Java_ language. The use of a smart pointer (or handle if you prefer) is equivalent to the use of a standard C/C++ pointer. The difference is that a smart pointers increments/decrements the reference counter when a Open BEAGLE object is referred/unreferred. An handled object is _self-destructed_ when its reference counter is equal to 0.

There is some rules to follow when using smart pointers in conjunction of Open BEAGLE objects:
  1. An handled object must always be allocated on the heap, with a call to the `new` operator;
  1. Programmers must _never_ delete a smart pointed object. The deallocation of the instance is the responsibility of the handles;
  1. The access to a pointed object, using a referring handle, is made with the same operators than the standard C/C++ pointers, the operators `*` and `->`.

The second nested data type is the allocator (the type `Alloc`). It defines a Open BEAGLE type able to allocate, clone and copy object of the associated type. For example an instance of `Double::Alloc` is an object that allocate, clone (on the heap) and copy `Double`. This data type is a central aspect of Open BEAGLE design and allows some sophisticated mechanism with little effort for the application developer. It is explained in more details in Section [Allocators](Architecture#Allocators.md) of the Architecture page.

Finally, the last nested data type is the container (the type `Bag`). It correspond to the Open BEAGLE basic structure containing data of a given type. A container of a given type is simply an array of smart pointers to the given type. It can contain instances of the associated type, and/or instances of types derived from the associated type. More details are given in Section [Data Structures](Architecture#Data_Structures.md) of the Architecture page.

## Experimenting with the Example ##

The code of the symbolic regression example can be found in the folder `examples/GP/symbreg` of the Open BEAGLE distribution. Once the Open BEAGLE libraries built, you can compile the example `symbreg` by following the instructions given in the `INSTALL` file. Then, it can be very interesting to experiment with the example.

To do so, open a shell (or a DOS terminal under Windows) and `cd` to the `symbreg` directory of the example directory. If your built is successful, you should find here the binary `symbreg`. Execute the program with the following command.
```
./symbreg
```
At the console output, you get XML logs on the running evolution. Here is the typical vivarium statistics given by the logs.
```
<Log>
  <Stats id="vivarium" generation="21" popsize="100">
    <Item key="processed">96</Item>
    <Item key="total-processed">2031</Item>
    <Measure id="fitness">
      <Avg>0.583894</Avg>
      <Std>0.104593</Std>
      <Max>0.867907</Max>
      <Min>0.287821</Min>
    </Measure>
    <Measure id="treedepth">
      <Avg>4.74</Avg>
      <Std>2.39368</Std>
      <Max>11</Max>
      <Min>1</Min>
    </Measure>
    <Measure id="treesize">
      <Avg>10.4</Avg>
      <Std>6.98339</Std>
      <Max>33</Max>
      <Min>1</Min>
    </Measure>
  </Stats>
</Log>
```
The values in the XML tags `Log` indicate a log entry. The values in tags `Stats` are statistics of a population (a deme or a vivarium). The value of the statistics item named `processed` tags is the number of individuals processed (evaluated) during the actual generation, while the value associated to the item `total-processed` is the cumulative number of individuals processed since the beginning of the evolution. The values between each `Measure` tags are statistics of the given measure: the first is the simple fitness value and the two last are tree grow measure (i.e. maximum tree depth and total tree size). The values in the tags are respectively: `Avg` for the average, `Std` for the standard deviation, `Max` for the maximum value, and `Min` for the minimum value.

Once the evolution finished, a milestone is written on disk. By default, the milestone is written as the file `beagle.obm` in the current directory. If an installation of zlib is detected on your system at the compilation time, which is usually the case with most modern Unix/Linux, the milestone might be written as compressed file `beagle.obm.gz`. If you want to take a look to it, you only need to uncompress it with the command `gunzip` (i.e. `gunzip beagle.obm.gz`). A milestone file contains a bunch of useful informations on the evolution: the evolver structure, the value of the parameters, some statistics, the best individuals of the evolution, and the population at the last generation. It is also useful to restart evolutions that were interrupted prematurely. Take a look on the milestone file and try to find the best-of-run individuals (it is between two `HallOfFame` XML tags).

Now, it can be interesting to change some evolution parameters. This can be done in two ways: on the command-line, or using a configuration file. To get the command-line usage of the example, type the following command.
```
./symbreg -OBusage
```
All the Open BEAGLE specific arguments start with the prefix `-OB`, followed by the tag, an equal sign and the values of the parameter (except for special arguments `usage` and `help`). For example, if you want to set the crossover probability to 0.75, the command-line would be the following.
```
./symbreg -OBgp.cx.indpb=0.75
```
You can specify several parameters on the command-line by separating them by a comma.
```
./symbreg -OBgp.cx.indpb=0.75,ec.term.maxgen=100
```
To get detailed about the different parameters available, use the argument `-OBhelp`.
```
./symbreg -OBhelp
```

The second way to set parameters is to use a parameter file. You can generate a default configuration file by using the command-line argument `-OBec.conf.dump` with the filename of your configuration file.
```
./symbreg -OBec.conf.dump=mysymbreg.conf
```
The generated configuration file, named here `mysymbreg.conf`, comprises all the available parameters set to their default values, and the evolver algorithm used. You can edit the file if you want to change the value of the parameters. Once you have a customized configuration file, you can start an evolution using this file by setting the `ec.conf.file` parameter on the command-line.
```
./symbreg -OBec.conf.file=mysymbreg.conf
```
The configuration file can also describe the form of the evolutionary algorithm used. For some examples distributed with the Open BEAGLE, there is two configuration files containing pre-configured evolvers: _name_`-generational.conf`, and _name_`-steadystate.conf`, where _name_ is the name of the example. You can change the evolver structure in the configuration file, which mean changing the operation used in the evolutionary algorithm. This is explained later in the document, at Section [Building a Custom Evolver](UserGuide#Building_a_Custom_Evolver.md) of the User Guide.

# References #

  * W. Banzhaf, P. Nordin, R. E. Keller, and F. D. Francone, [Genetic Programming, an Introduction: On the Automatic Evolution of Computer Programs and Its Applications](http://www.citeulike.org/group/1314/article/608198), Morgan Kaufmann Publishers, 1998.
  * J. R. Koza, [Genetic Programming: On the Programming of Computers by Means of Natural Selection](http://mitpress.mit.edu/catalog/item/default.asp?ttype=2&tid=5888), MIT Press, 1992.
  * J. R. Koza, [Genetic Programming II: Automatic Discovery of Reusable Programs](http://mitpress.mit.edu/catalog/item/default.asp?ttype=2&tid=8307), MIT Press, 1994.