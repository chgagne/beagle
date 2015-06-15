

# General Architecture #

This page presents the global class architecture of Open BEAGLE. The framework has a three levels architecture, as illustrated below.

![http://wiki.beagle.googlecode.com/hg/images/architecture.png](http://wiki.beagle.googlecode.com/hg/images/architecture.png)

The foundations are located at the bottom, as an object oriented extension of the C++ language and STL. The EC generic framework is built on these foundations and is composed of features that are common to all Evolutionary Algorithms (EA). Finally, different independent modules specialize this generic framework, each module implementing a specific EA.

**Some general explanations on PACC and its introduction in Open BEAGLE is necessary here.**


## Namespaces and Libraries ##

**Present here an overview of the namepaces and libraries making the framework in version 4.1.0.**


## Portable Agile C++ Classes ##

**A detailed section on PACC is required here.**


## Core Components ##

This section presents the generic Object Oriented (OO) mechanisms used in Open BEAGLE. Although these mechanisms were designed to be the foundation of Open BEAGLE, they could have been implemented in most OO frameworks. Some of our design choices are inspired from [design patterns](http://en.wikipedia.org/wiki/Software_design_pattern). Some concepts are also took from well-known OO environments such the [C++ Standard Template Library (STL)](http://www.sgi.com/tech/stl/), the [Java Class Library](http://en.wikipedia.org/wiki/Java_Class_Library), and [CORBA](http://en.wikipedia.org/wiki/Common_Object_Request_Broker_Architecture).

### Objects ###

In Open BEAGLE, most classes are derived from an abstract class called `Object`. An Open BEAGLE object comprises some general methods that are characteristic of a complete C++ object.
```
namespace Beagle {
class Object
{
public:
  virtual const std::string& getName() const;
          unsigned int       getRefCounter() const; 
  virtual bool               isEqual(const Object&) const;
  virtual bool               isLess(const Object&) const;
  virtual void               read(PACC::XML::ConstIterator);
          Object*            refer();
          void               unrefer();
  virtual void               write(XMLStreamer&) const;
private:
  unsigned int mRefCounter;
};
}
```
The usual C++ objects comparison operators are defined, receiving arguments of the `Object` type and calling the appropriate comparison method (`isEqual` or `isLess`). The method `read` and `write` are used to read and write objects in the XML format. Section [XML Input/Output](Architecture#XML_Input/Output.md) gives more details about the Open BEAGLE I/O in XML. Method `getName` gives the name of the object.

An Open BEAGLE object also implements a reference counter that remembers the number of references that point to it. The C++ language defines two ways of allocating an object instance. First, the instance can be allocated on the stack, it will then be destructed at the end of the scope. Second, the instance can be allocated on the heap with a call to the `new` operator, which means that it is necessary to apply a symmetrical `delete` at the end of the instance lifetime. With Open BEAGLE, there is a variation on the second way to allocate objects: an Open BEAGLE object can be allocated on the heap and affected to Open BEAGLE smart pointers that interact with the object reference counter. The destruction of the object is then the responsibility of the smart pointers, as explained next.

### Smart Pointers ###

An Open BEAGLE smart pointer acts like a standard C++ pointer that manage the memory used by the pointed objects. It is implemented in class `Pointer`, which is tightly linked to the reference counter of class `Object`. The implementation of the `Pointer` class guarantees that the pointed object exists since there is a smart pointer that refer to it. It also guarantees that the object will be destroyed when its last reference will vanish. This emulates the appreciated garbage collection mechanism of some programming languages. In fact, the Open BEAGLE smart pointers are coherent with the creation and the garbage collecting of objects of the _Java_ language. The two things that the user should remember are to allocate objects on the heap (with a `new`) and never interfere with object destruction after assigning them to smart pointers. Once an object is referred by smart pointer, the memory management responsibility is held by the smart pointer.

Exceptionally, the `Pointer` class and its subclasses do not inherit from superclass `Object`. Like a C++ pointer, an Open BEAGLE pointer can be assigned to a null pointer. The class also provides the two usual dereferencing operators, `operator*` and `operator->`, which return a reference to the pointed object. There is also two comparison operators (`operator==` and `operator!=`) between two `Pointer`, between a `Pointer` and an `Object*` and the null pointer testing operator (`operator!`). The relation between the objects and the smart pointers is presented in figure below.

![http://wiki.beagle.googlecode.com/hg/images/pointer.png](http://wiki.beagle.googlecode.com/hg/images/pointer.png)

Since any instance are concrete objects and smart pointers give references to the abstract `Object` type, each access to methods or attributes that are not declared in the object interface needs a cast of the reference given by the smart pointer to the desired derived type. This could lead to inelegant code, or even type inconsistencies if old C-style casts were used. To avoid these problems, there is a templated class, `PointerT`, that defines the pointer unreferencing operators to the desired type.
```
template <class T, class BaseType>
class PointerT : public BaseType {
public:
  inline T& operator*();
  inline T* operator->();
};
```

The `PointerT` template also emulates the mechanism of automatic pointer type binding for inheritance-related classes. This allows a compile-time binding of `T`-type smart pointers that inherit from the `BaseType`, when a `BaseType` object is needed. For example, suppose a method taking an argument of the type `Base::Handle`, which is a smart pointer to an object instance of the class `Base`. With the automatic type binding of smart pointer, the method can get as argument a smart pointer to the type `Derived::Handle`, supposing that the class `Derived` inherits from the class `Base`. This can be done without any explicit cast of the smart pointers. `PointerT` has two templated parameters: the type of object handled, the `T`-type, and the parent type of the smart pointer, the `BaseType`. The `BaseType` is useful for the automatic type binding emulation by inheritance.

For each class of Open BEAGLE, the nested type `Handle` is declared. This is the type of handle to the class, that is, a smart pointer that gives exact reference type. Usually, this type is simply declared as a synonym of a parametrized `PointerT`.
```
class AnyClass : public SuperClass {
public:
  typedef PointerT<AnyClass,SuperClass::Handle> Handle;
  ...
};
```
Doing so, a smart pointer can be used to return reference to the right type, the type `AnyClass`, simply by working with the right handle type, `AnyClass::Handle`. It is a good practice to define the nested `Handle` type for every class that inherits directly or indirectly from an `Object`.

Once we have a reference to an object of a given general type, we might be interested in making a type casting of that pointer into a more specific type, to be able to call methods only available in that type. For that purpose, two casting operators have been defined, `castHandleT<T>` and `castObjectT<T>`, which will make necessary run time verification on the validity of the type casting requested, return an exception if the object or smart pointer cannot be casted in the type requested.

### Allocators ###

Open BEAGLE is characterized by great versatility. This is achieved by different features and design choices, the first being an abstract object superclass that gives some essential generic mechanisms and from which almost every other Open BEAGLE class is derived. Another important design choice that gives flexibility to Open BEAGLE is the allocators, a kind of object factory that generates objects in an abstract fashion. An abstract allocator class named `Allocator` is implemented to define the methods to create objects on the heap.
```
class Allocator : public Object {
public:
  virtual Object* allocate() const =0;
  virtual Object* clone(const Object&) const =0;
  virtual void    copy(Object&, const Object&) const =0;
};
```
The purpose of the allocators is to provide factory methods to create and clone new types of objects derived from Open BEAGLE constituents. With such mechanism, any user could create new type of objects that redefine the default one and use associated allocators that return pointers to this new type of objects. This mechanism is similar and coherent with the [Factory Method](http://en.wikipedia.org/wiki/Factory_method_pattern) design pattern.

Usually, an allocator is used to allocate objects through their default constructor and clones the objects by calling their copy constructor. An allocator can also copy an existing object of a given type into an other. To simplify the task, a simple standard parametrized allocator named `AllocatorT` is defined to override the virtual method of the abstract class `Allocator`. Like the smart pointers `Handle` type, each component of Open BEAGLE has a nested allocator type called `Alloc`. The users are encouraged to define it in their classes.
```
class AnyClass : public SuperClass {
public:
  typedef PointerT<AnyClass,SuperClass::Handle>  Handle;
  typedef AllocatorT<AnyClass,SuperClass::Alloc> Alloc;
};
```
However, the template `AllocatorT` cannot be used to define an `Alloc` type for an abstract type. The reason is that the method `allocate` and `clone` cannot be implemented to instantiate object of the abstract allocated type. To solve this problem, another templated allocator type is implemented, `AbstractAllocT`. This allocator does not define the methods `allocate` and `clone`, but is usable in the same way than `AllocatorT` as an allocator type for abstract types.

### Data Structures ###

It is natural to design data structures that take advantage of all the mechanisms exposed in the current section. Working with the new features of the C++, it is essential to exploit the [C++ Standard Template Library (STL)](http://www.sgi.com/tech/stl/) which provides common, well known and portable data structures and algorithms. An Open BEAGLE specific, but STL compliant, data structures is extensively used: the container.

The Open BEAGLE container is a random access table of heterogeneous objects. It is implemented as a dynamic array of specified smart pointers. Any objects that is a specialization of the contained type could be put in a container. The constraints are the same as those of the smart pointers: the objects must be allocated on the heap (via the `new` operator) and these objects must not be unallocated explicitly (i.e. `delete` of the objects are forbidden). Our container is implemented as a Beagle object that is also a STL vector of object handles.
```
class Container : public Object, public std::vector<Object::Handle> { ... };
```
Built this way, our container could be manipulated by the STL generic algorithms. It is also possible to make container of containers, since a container is an object that can be smart pointed.

As with the smart pointers and allocators, every Open BEAGLE type has a nested type that define a container of the specified type, the nested type named `Bag`. For example, the type that is a container of floating-point values, `Float`, is the type `Float::Bag`. The nested type is declared in classes as usual for the nested types `Handle` and `Alloc`, using a typedef to a template, in this case to template class `ContainerT`.
```
class MyClass : public SuperClass {
public:
  typedef AllocatorT<MyClass,SuperClass::Alloc> Alloc;
  typedef PointerT<MyClass,SuperClass::Handle>  Handle;
  typedef ContainerT<MyClass,SuperClass::Bag>   Bag;
};
```

The use of dynamically typed containers raises a problem when it is necessary to allocate containers of objects not from the basic usual associated type. Supposing a container of genotype, namely a `Genotype::Bag`. It may be possible for the container to contain a different type of genotype, for example genotypes of the type `SpecializedGenotype`, which is derived from the type `Genotype`.

### Object Factory ###

**Add explanations on the object factory here.**

### XML Input/Output ###

**XML facilities are now provided by PACC. The way we use it is now fundamentally different from what is explained here, although the whole subsection should be completly revised.**

Reading and writing files containing evolutionary data, such the population, the parameters, the algorithm, the results of the evolution, is an important aspect of an EC framework. The C++ already provides an interesting streaming mechanism to read and write objects into a file, the memory or the standard input/output. Although, there is also a very interesting technology that is more and more used as a file format, the [eXtensible Markup Language (XML)](http://en.wikipedia.org/wiki/XML). Mixing these two concepts together lead us to an important mechanism of this framework: the Open BEAGLE input XML parser and output object streamer.

The XML format was first intended to be an improved generic replacement to HTML, but it appears to be so generic, and so well accepted by the industry, that it's now a central technology for domains such data modeling (as in the modern word processors), lightweight distributed computing (SOAP and XML-RPC), databases interactions and Web integration in general. Although the XML is a human understandable format, it is also a flexible and powerful file format, ideal for a to be used in conjunction of a framework such Open BEAGLE. A XML document can also be interpreted to screen by most modern Web browsers such _Mozilla_ and _Microsoft Internet Explorer_.

The new Open BEAGLE XML input / output classes (since version 2.0.0) consists into a DOM-like XML parser and a XML streamer. The XML parser read file by constructing a tree with the XML files it reads, using object of the class `XMLNode` to build the tree. The XML parser uses a standard C++ input stream as input devices. The XML output streamer is a simple API class that can generate XML output into a standard C++ output stream.

For every Open BEAGLE objects that can read and written, the methods `read` and `write` must be over-defined consequently. For the `read`, this is done by a exploring the tree build by the XML parser. For the `write` method, the object must be serialized using the XML streamer API.

To illustrate this, the following example presents the listing of the method `read` and `write` of the hypothetical class `DataValue`.
```
class DataValue : public Beagle::Object {
private:
  std::string mName;
  double      mValue;
public:
  virtual void read(Beagle::XMLNode::Handle& inNode) {
    if(inNode->getTagName() != "Data")
      throw Beagle_IOExceptionNodeM(*inNode, "tag <Data> expected");
    mName = inNode()->getTagAttribute("name");
    if(inNode->getFirstChild() == NULL)
      throw Beagle_IOExceptionNodeM(*inNode, "no value for tag <Data> found");
    mValue = uint2str(inNode->getFirstChild()->getValue());
  }
  virtual void write(Beagle::XMLStreamer& ioStreamer) const {
    ioStreamer.openTag("Data");
    if(mName != "") ioStreamer.insertAttribute("name", mName);
    ioStreamer.insertFloat(mValue);
    ioStreamer.closeTag();
  }
};
```
The XML output produced of an instance of a `DataValue` object that have as name `mydata` and as value 1.5 would be the following.
```
<Data name="mydata">1.5</Data> 
```

In conjunction of the XML file format used in Open BEAGLE, it is planed to develop an infrastructure to analyze the files containing the populations or evolution statistics, into a standard Web browser, using modern Web technologies. This will eventually allows distant evolution management with simple Web interfaces and utility scripts to do some sophisticated analysis of the evolutions.

**NOTE: A short overview (1-2 sentences) of Open BEAGLE file format is necessary here.**


### Object Wrappers ###

Given all this functionalities, the predominance of the object abstraction in Open BEAGLE's design is evident. But, if a user has some elaborated classes, or if a user wants to use other libraries in conjunction of Open BEAGLE, it might be restrictive that everything must inherit from the superclass `Object`. The user might not want to redefine everything or to use multiple inheritance to created hybrid classes. To enhance the user experience, a class template that would keep the use of Open BEAGLE simple is defined: the object wrapper. The wrapper is a simple adapter of any type to the Open BEAGLE `Object` interface. The concept of object wrapper is based upon the [Adapter](http://en.wikipedia.org/wiki/Adapter_pattern) design pattern.

The wrapper is defined in the C++ template `WrapperT`. The template maps the `Object` interface to the usual methods of the wrapped type. The template got some casting operators to indifferently use the wrapper as the wrapped type. Furthermore, some types that are wrappers of the C++ fundamental types are defined as standard types of Open BEAGLE. The following table presents all the standard wrapped type defined in Open BEAGLE.
| **C++ name** | **Wrapper name** |
|:-------------|:-----------------|
| `bool`       | `Beagle::Bool`   |
| `char`       | `Beagle::Char`   |
| `double`     | `Beagle::Double` |
| `float`      | `Beagle::Float`  |
| `int`        | `Beagle::Int`    |
| `long`       | `Beagle::Long`  |
| `short`      | `Beagle::Short` |
| `std::string` | `Beagle::String` |
| `unsigned char` | `Beagle::UChar` |
| `unsigned int` | `Beagle::UInt`  |
| `unsigned long` | `Beagle::ULong` |
| `unsigned short` | `Beagle::UShort` |

Another template named `ArrayT` has been define, as wrapper of a std::vector of some objects. For example, a `ArrayT<double>` would correspond to a wrapper of `std::vector<double>`, with the complete interface of Open BEAGLE class `Object` defined for that type. For more standard C++ types, an array type has been defined. The following table list them.
| **C++ name** | **Array name** |
|:-------------|:---------------|
| `std::vector<bool>` | `Beagle::BoolArray` |
| `std::vector<char>` | `Beagle::CharArray` |
| `std::vector<double>` | `Beagle::DoubleArray` |
| `std::vector<float>` | `Beagle::FloatArray` |
| `std::vector<int>` | `Beagle::IntArray` |
| `std::vector<long>` | `Beagle::LongArray` |
| `std::vector<short>` | `Beagle::ShortArray` |
| `std::vector<unsigned char>` | `Beagle::UCharArray` |
| `std::vector<unsigned int>` | `Beagle::UIntArray` |
| `std::vector<unsigned long>` | `Beagle::ULongArray` |
| `std::vector<unsigned short>` | `Beagle::UShortArray` |



### Exceptions ###

Open BEAGLE integrates the C++ exceptions mechanism. Keeping it OO, an hierarchy of exceptions are defined. At the top there is the abstract exception superclass, `Exception`, from which every other Open BEAGLE exception classes inherit. This class inherit from the standard `std::exception` class, which allow catch of Open BEAGLE exceptions in a simple `catch(std::exception&)` expression.
```
class Exception : public Object, public std::exception {
public:
  explicit Exception(std::string inMessage="");
  void terminate(std::ostream& ioES=std::cerr) throw();
  virtual const char* what() const throw();
protected:
  std::string  mMessage;
};
```

Some other exceptions classes are defined into Open BEAGLE for different type of problem encountered. For most concrete standard Open BEAGLE exceptions, there is an associated macro that simplify the throwing of an exception. The following list presents exceptions defined in Open BEAGLE, with a short description of the context they are used.
  * `Exception`: Base exception class.
  * `TargetedException`: Base class of exceptions relative to a specific piece of code. A targeted exception includes the file name and line number where the exception is thrown.
  * `ValidationException`: Generally related to the detection of invalid parameter values.
  * `AssertException`: Threw when an Open BEAGLE assertion test fail. Several macros such as `Beagle_AssertM` are also defined to do C-like assertion tests.
  * `BadCastException`: Threw when a `castObjectT` or `castHandleT` operation fail.
  * `InternalException`: Threw when unexpected situation denoting internal problems are detected.
  * `IOException`: Threw when problems with the XML parser or streamer, or in the `read` or `write` methods, are detected.
  * `ObjectException`: Exception associated to a specific object. The error message usually display the serial representation of the associated object.
  * `RunTimeException`: Generic exception denoting a problem detected at the run-time that doesn't fall into the previous categories.
  * `ExecutionException`: Base class of exceptions threw when the interpretation of a GP tree failed.
  * `ResourcesExceededExecutionException`:  Base class of exception related to resources exhaustion in the interpretation of a GP tree.
  * `MaxTimeExecutionException`: Threw when the maximum execution time allowed for intepreting a GP tree has passed.
  * `MaxNodeExecutionException`: Threw when the maximum number of nodes in a GP tree allowed for execution has passed.

### Summary ###

Figure below summarizes the Open BEAGLE OO foundation in [UML (Unified Modeling Language)](http://en.wikipedia.org/wiki/Unified_Modeling_Language). It presents the different entities that compose the generic part of the framework. The most important of them is the class `Object`, the basic class from which most others Open BEAGLE class inherits. Any object subtype could be smart pointed by a Open BEAGLE pointer, which managed the deallocation of the object when it is appropriate. The allocators are objects that allocate, clone, and copy Open BEAGLE objects. The container is a flexible data structures tightly related with the objects, smart pointers and allocators. The Open BEAGLE XML parser and output streamer are objects to insert and extract objects into/from C++ streams. They allow a transparent reading and writing of Open BEAGLE object the powerful XML file format. There is some templates related to these entities that exploit great concepts of generic programming and make the programmer's life easier. Finally, a short discussion of the error handling mechanism deployed in Open BEAGLE terminates the section.

![http://wiki.beagle.googlecode.com/hg/images/foundations.png](http://wiki.beagle.googlecode.com/hg/images/foundations.png)
**TODO: The image above should be updated, especially on the XML I/O and ContainerAlloc classes.**

## Generic EC Framework ##

The generic EC framework is the extension of OO foundations. It offers a solid basis for implementing Evolutionary Algorithms (EA). It is composed of a generic structure for _populations_, an _evolution system_ and a set of _operators_ packed in an _evolver_. All components of the generic EC framework are integrated together as modules that can be replaced or specialized independently. This modular design gives much versatility to the framework and simplifies the implementation of any EA.

![http://wiki.beagle.googlecode.com/hg/images/relations.png](http://wiki.beagle.googlecode.com/hg/images/relations.png)

The following discussion explains this generic framework and is divided into three sections: the populations and statistics, the internal system, and the operators and evolvers.


### Fitness and Statistics ###

The statistics of the fitness measurements of a population is held in the following structure.
```
struct Measure {
  std::string mId;
  float       mAvg;
  float       mStd;
  float       mMax;
  float       mMin;
};

class Stats : public Object, public std::vector<Measure> { 
public:
  void          addItem(std::string inTag, double inValue);
  void          clearItems();
  double        deleteItem(std::string inTag);
  double&       getItem(std::string inTag);
  const double& getItem(std::string inTag) const;
...
protected:
  std::map<std::string,double>  mItemMap; 
  std::string                   mId;
  unsigned int                  mGeneration;
  unsigned int                  mPopSize;
};
```
For the usual EC algorithm, statistics for a population are calculated every generation. Two important elements compose the statistics: the _measures_ and the _items_. A measure provides the statistical analysis of the distribution (average, standard deviation, maximum, and minimum) of a given value of interest taken on the population. On the other hand, an item is a single value represented by a floating-point number associated to a tag name. Items and measures are dynamically added to the statistics, and are generally computed by a statistics computation operators. These operators are specific to the fitness object and inherit from the abstract class `StatsCalculationOp`.
```
class StatsCalculateOp : public Operator {
public:
  virtual void calculateStatsDeme(Stats& outStats, Deme& ioDeme, 
                                  Context& ioContext) const =0;
};
```
When a non-standard fitness object is used, a companion statistics computation operator must defined by inheriting from `StatsCalculationOp` class and over-defining the pure virtual method `calculateStatsDeme`.

The fitness holder of an individual is represented in an object derived of the type `Fitness`. When the user implements a concrete evaluation class, the method `evaluate` must returns a fitness object appropriate to the application. There is some of the predefined standard fitness measures, such `FitnessSimple`, `FitnessSimpleMin`, `FitnessMultiObj`, `FitnessMultiObjMin`, or `GP::FitnessKoza`, which defined respectively a fitness made of a single float, the minimization equivalent of single float, multiobjective maximization fitness measures, multiobjective minimization fitness measures or a more elaborated fitness proposed by Koza for GP. A specialized fitness type could also be defined for more sophisticated evolution.

### Population ###

In Open BEAGLE, the population is structured into four layers: a vivarium, the demes, the individuals, and the genotypes. The first layer, the vivarium, comprises the whole population of a specific evolution, that is, an aggregate of one or more demes. A deme is a population that evolves in an independent environment. Generally, at each generation, there are some individuals that migrate between the demes that compose a vivarium. A deme is implemented in class `Deme`. The class provides and implements an interface of standard methods necessary to evolve its own population. The class `Vivarium` describes a bag of demes, which is itself a bag of individuals. Every single vivarium and deme also has an hall-of-fame (a `HallOfFame` object), where the best-of-run individuals are conserved, and its statistics (a `Stats` object).

The next underlying layer, the individual, is defined in class `Individual`. It is a bag of genotypes, the parts that compose a genome. The genotypes are tightly related to the representation of the individuals for a given EC algorithm. The genotype interface is declared in the abstract class `Genotype`. This hierarchical organization of the population is illustrated below.

![http://wiki.beagle.googlecode.com/hg/images/popstruct.png](http://wiki.beagle.googlecode.com/hg/images/popstruct.png)
**TODO: update the previous figure by adding the Factory, Algorithm, and Package classes.**

### Internal System ###

This section presents the internal system of Open BEAGLE, which is the structure that holds and gives access to the state of the genetic engine. These structures are fundamental, because they are used as entry points to the data of the evolution.

During the evolutionary processes, a context gives the current state of the evolution. A basic, general context is implemented in class `Context`. It provides some essential contextual informations such as the current deme, individual, genotype and generation. For a specific EC algorithm, a specific context could be used. For example, a GP specific context is defined, `GP::Context`, which contains the call stack with some other GP specific informations. An Open BEAGLE context is similar to the execution context of a computer that contains the different registers, counters and pointers to the actual state of the machine.

Given that the parameters of Open BEAGLE are distributed in the appropriate objects, an agent is implemented to take into account these parameter: the register. All the variables that are considered as parameters should be registered by giving the reference (object handle) of the parameter with the associated namespace and tag. The class `Register` can be seen as a centralized database from which any entity could dynamically add, delete, access or modify parameters. The register is also responsible of interpreting the parameters part of the configuration file (see Section [Using the Register](UserGuide#Using_the_Register.md) of the User Guide for further details).

All the output messages given to the user pass by the logger. It consists of an interface with the user, that receives all messages, associated with a type, a class name, and an output level, and output them in a given device if the used log level allow it. This is very interesting if a user want, for example, to use Open BEAGLE into a broader system using a graphical user interface. In such case, the user only need to define his own specialized logger that will intercept the messages and log them into the desired device, for example a specific graphical windows. There is actually one specialized logger, `LoggerXML`, that log messages to a file and/or the console (the STDOUT device) in a XML format. The other very interesting aspect of the logger is the possibility to choose the log level desired. The messages outputted are classified into eight categories, as depicted in the following.

| **Log Level** | **Value** | **Description** |
|:--------------|:----------|:----------------|
| Nothing       | 0         | Log nothing     |
| Basic         | 1         | Log essential informations |
| Stats         | 2         | Log evolution statistics (default for console logging) |
| Info          | 3         | Log general informations (default for file logging) |
| Detailed      | 4         | Log details on operations |
| Trace        | 5         | Log trace of the algorithms |
| Verbose      | 6         | Log details on everything (disabled in optimization mode) |
| Debug         | 7         | Debug (enabled only in full debug mode) |

The registered parameters `lg.console.level` and `lg.file.level` allows the user to select the desired log level. For example, if the user choose the log level _info (3)_, all messages classified in categories _basic (1)_, _stats (2)_, and _info (3)_ will be output. Log levels _basic (1)_ to _detailed (4)_ are appropriate for monitoring evolutions, log levels _detailed (4)_ and _trace (5)_ are good to get familiar with the internal functioning of Open BEAGLE, while log levels _trace (5)_ to _debug (7)_ may be useful to debug an user application, or the framework.

Class `Factory` implements the [abstract factory](http://en.wikipedia.org/wiki/Abstract_factory_pattern) design pattern, for allocating at the run-time instances of objects. These instances are generated simply by obtaining `Allocator` instances from type names given as `std::string`, through method `getAllocator`. Therefore, the factory has been populated at the initialization time to contain allocators of all Open BEAGLE classes available in the current context, which is done through the addition of packages in the system -- see Section [Architecture#Package](Architecture#Package.md) for more details.

The factory also implement a mapping of concepts. A concept is a general name describing a conceptual type, which is internally mapped to a specific specific type depending on the situation. For example, we can have a `CrossoverOp` concept that defines a generic crossover operator. This concept can be assigned to the specific `Beagle::BitStr::CrossoverUniformOp` type, that is uniform crossover for bit string genotypes. In the definition of the algorithm used, only the concept `CrossoverOp` can be use to define the crossover operator type. At the runtime, the specific type for uniform crossover will be used in the algorithm. If one want to use a different crossover operator, he just needs to replace the specific type of the `CrossoverOp` concept by something else, and the algorithm will be modified accordingly, in a transparent manner.

Class `Randomizer` provides a common pseudo-random numbers generator. The randomizer comprises two parameters that are registered (in the register): the internal state and the seed. The internal state is a set of integers that give the actual state of the randomizer. This state changes at every generation of a random number. This value is useful mainly to restart an evolution from a milestone. The seed is the value to set the first state of the randomizer. By default, the seed is initiated a random value given by the OS on Linux and OS X (through device `/dev/urandom`), or to the timer value on Windows. The seed can be set by the user to reproduce an evolution. The default random number generator used in the framework is based on the [Mersenne Twister](http://en.wikipedia.org/wiki/Mersenne_twister) of Matsumoto and Nishimura.

The entry point to these resources is given by an extensible central repository: the system. This is simply an entry point that possessed some vital resources of the EC engine: a context allocator, a reference to the register, a reference to the logger, a reference to the factory, and a reference to the randomizer. This general system is implemented in the class `System`. The system is accessible by any context of a given evolution. The relations between the different constituents of the internal system is presented in figure below.

![http://wiki.beagle.googlecode.com/hg/images/system.png](http://wiki.beagle.googlecode.com/hg/images/system.png)

**TODO: Previous figure should be updated by adding the Factory.**

**TODO: We should present the concept of _system components_ here.**


### Operators and Evolver ###

The operator is a central concept of Open BEAGLE as an EC framework. The main-loop of operations executed on populations is dynamically defined. The operators are specified at runtime and the user is free to define them for his evolution. This give the opportunity to easily and rapidly experiment numerous variants of EC. The operator and evolver model is based on the [Strategy](http://en.wikipedia.org/wiki/Strategy_pattern) design pattern, which is applied to the evolutionary algorithms. The operator interface is declared in the abstract class `Operator`.
```
class Operator : public Object {
public:
  Operator() { }
  virtual void init(System& ioSystem) { }
  virtual void operate(Deme& ioDeme,Context& ioContext) =0;
  virtual void registerParams(System& ioSystem) { }
};
```
Before the characteristic method is applied to demes, method `registerParam` is invocated. In this method, the operator registers its own parameters, probabilities or anything else, used by the characteristic operation. The characteristic operation is defined in the virtual method `operate`. There is a bunch of predefined operators in Open BEAGLE. To name a few of them, the tournament selection operator (`SelectTournamentOp`), the GP tree crossover operator (`GP::CrossoverOp`) and the milestone writing operator (`MilestoneWriteOp`).

The user that define his own operators must be aware that the system is not fully set-up when the `registerParams` method is called. For example, the random number generator must not be used, as the seed can be modified thereafter on the command-line or in the configuration file. The rule is that the `registerParams` method must be used only to add parameters to the register. There is also a second hook called `init`, which is also called once, just after the whole system is set-up. The `init` method can be use, for example, to configure some data using the random number generator, which is now fully configured.

The operators of a specific evolution are inserted into the evolver that supervises the evolution process. This object, implemented in class `Evolver`, comprises two major attributes: the bootstrap operator set and the main-loop operator set. The bootstrap operators set contains an ordered list of operators to apply on each deme, for the initial generation. The main-loop operators set is an ordered list of operators to apply iteratively, at each generation, on each deme. The user could launch an evolution, by calling the method `evolve` with the vivarium to evolve as argument.
```
class Evolver : public Object {
public:
  virtual void initialize(System::Handle ioSystem,int& ioArgc,char** ioArgv);
  virtual void evolve(Vivarium::Handle ioVivarium);
protected:
  Operator::Bag mBootStrapSet;
  Operator::Bag mMainLoopSet;
};
```
For common EC algorithms, the user usually needs not to create custom sequences of operators. In fact, some classes inheriting of `Evolver` can be used to create evolvers with predefined operator sets. If a special EC algorithm is needed, a custom building method can be invocated and the evolver should be configured properly.


### Breeder Model ###

**TODO: The explanations on the breeder model are somewhat fuzzy. It is not easy to understand this thing, although this is an central mechanism of Open BEAGLE. More explanations are required in order to make it clear.**

A conceptual layer is added over the operator/evolver model: the breeder model. This allow the manipulation of the population at the individual level, in comparison to manipulations at the deme level by the standard operators. Note that the use of the breeder model over the standard operator/evolver model is optional; users don't have to use it if they don't need it.

The breeder model includes a set of operators structured into a tree: the breeder tree. At the root of the breeder tree, there is a replacement strategy, which act as an interface between the operator/evolver mechanism and the breeder model. The function of a replacement strategy is to generate, by applying the breeder tree on a deme, the next generation deme. The direct children in the breeder tree to the replacement strategy are called alternatively following their respective breeding probability. The operators that compose the nodes of the breeding tree process the bred individual, while the leaves of the breeder tree are generally used to select individuals. The following snippet presents the XML configuration of a breeder tree for the `maxfct` example, using a steady-state replacement strategy.
```
...
<SteadyStateOp>
  <MaxFctEvalOp>
    <GA-CrossoverOp>
      <SelectTournamentOp/>
      <SelectTournamentOp/>
    </GA-CrossoverOp>
  </MaxFctEvalOp>
  <MaxFctEvalOp>
    <GA-MutationOp>
      <SelectTournamentOp/>
    </GA-MutationOp>
  </MaxFctEvalOp>
  <SelectTournamentOp/>
</SteadyStateOp>
...
```

Three classes are supporting the breeder model: `BreederOp`, `BreederNode`, and `ReplacementStrategyOp`. Class `BreederOp` designates operators that can, in addition to the facilities of the operators, process a pool of individuals to return one bred individual. The interface of the class `BreederOp` is the following.
```
class BreederOp : public Operator {
public:
  virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
                                   BreederNode::Handle inChild,
                                   Context& ioContext) = 0;
  virtual float getBreedingProba(BreederNode::Handle inChild) = 0;
};
```
The pure virtual method `breed` is used to apply the characteristic breeder operation. It takes as first argument a pool of individual from which the breeding operation is done, as second argument a breeder node giving the position of the first child to the actual operator in the breeder tree, and as last argument the evolutionary context. The method `getBreedingProba` returns the breeding probability, which is used by the replacement strategy to decide which of its subtrees is called to generate a bred individual. The `BreederNode` class defines the breeder tree structure. It is made of three smart pointers, one to its first child, another to its next sibling, and one last to the associated breeder operator. Finally, the class `ReplacementStrategyOp` defines an item of the evolver's operator set that can be used as the root of the breeding tree, to generate a new generation of individuals. Five replacement strategies operator are actually implemented in Open BEAGLE: `GenerationalOp`, which defines a generational replacement operator using a breeder tree, `SteadyStateOp`, which defines a steady-state replacement operator, `NSGA2Op`, which defines the [NSGA-II](http://www.iitk.ac.in/kangal/Deb_NSGA-II.pdf) multiobjective evolutionary algorithm, and finally the `MuCommaLambdaOp` and `MuPlusLambdaOp` replacement operators, which define the (mu,lambda) and (mu+lambda) approaches commonly used with the evolution strategy EC paradigm.

### Algorithm ###

**Add explanations on algorithms here.**

### Package ###

**Add explanations on packages here.**


### Summary ###

This section had presented three important components of the EC architecture of Open BEAGLE. The first component is the structure of the population of the framework, that includes elements such the vivarium, the demes, the individuals, and the genotypes. The following is the internal system, where the parameters and some other entities are held. The third component is a central aspect of Open BEAGLE as EC framework, the operators and the evolver. An operator is simple operation, such writing a population to the disk or GP tree crossover operation, which is applied sequentially and iteratively in conjunction of others operators to evolve a population. An evolver is an agglomerate of operators to apply on the population. An applications developers can use an evolver pre-packed with usual operators or instantiate his own custom evolver composed of more exotic operators. And finally, the breeder model represents a set of specific operators that allow the manipulation of population with a finer granularity.


## Miscellaneous Architecture Elements ##

This section presents of different miscellaneous architecture elements more specific certain EC flavors or applications. It presents of some common application-specific elements such the evaluation and termination operators, and some other more EC flavor-specific elements such the GP primitives. It also discuss of a more general elements such the different standard operators defined in the Open BEAGLE framework.


### Evaluation Operator ###

In most EC algorithms, modified individuals are evaluated at each generation. To do so, there is a common operator to evaluate individuals, the evaluation operator. This operator is implemented in class `EvaluationOp`, which is a standard abstract interface for the evaluation of an individual.
```
class EvaluationOp : public Operator {
public:
  virtual Fitness::Handle evaluate(Individual& inIndividual,
                                   Context& ioContext) =0;
  virtual void operate(Deme& ioDeme,Context& ioContext);
};
```
Usually, for a specific problem, the user must implements a concrete class that specialize `EvaluationOp`. In this concrete class, he must declare a definition of the method `evaluate`. This method provides the way an individual is evaluated.


### Termination Operator ###

As explained in previous sections, the basic Open BEAGLE algorithm is an infinite loop over the main operator set. But, there is a standard Open BEAGLE operator, the termination criterion, that determine whether to stop or not the evolution process. In Open BEAGLE, the termination criterion is implemented in operator `TerminationOp`.
```
class TerminationOp : public Operator {
public:
  virtual bool terminate(const Deme& inDeme,const Context& inContext) = 0;
  virtual void operate(Deme& ioDeme,Context& ioContext);
};
```

The default termination criteria is the number of generations the algorithm iterates for an evolution. This criterion is defined in the operator class `TermMaxGenOp`. There is also several other termination operators such `TermMaxFitnessOp` (maximum fitness value), `TermMinFitnessOp` (minimum fitness value), ` TermMaxEvalsOp` (maximum number of fitness evaluations), and `GP::TermMaxHitsOp` (maximum number of hits for GP Koza's fitness measure). The user could change or add termination criteria for his evolutionary application by redefining the method `terminate`.