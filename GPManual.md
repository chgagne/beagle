# Genetic Programming Manual #

## Getting the Most of the Primitives ##

**TODO: The `Primitive` interface has slightly evolved since the writting of this section. New methods and changes to modified methods should be given here.**

The declaration of the primitives are a central aspect of the implementation of GP in Open BEAGLE. An abstract class `Primitive` is declared with the following standardized interface.
```
namespace GP {
class Primitive : public Object {

public:
  typedef AllocatorT<Primitive,Object::Alloc> Alloc;
  typedef PointerT<Primitive,Object::Handle>  Handle;
  typedef ContainerT<Primitive,Object::Bag>   Bag;

  explicit Primitive(unsigned int inNumberArguments=0,std::string inName="");
  virtual ~Primitive();

  virtual void execute(GP::Datum& outDatum,GP::Context& ioContext) =0;

  virtual std::string  getArgType(unsigned int inN) const;
  virtual std::string  getAttribute() const;
          unsigned int getChildrenNodeIndex(unsigned int,GP::Context&) const;
          std::string  getName() const;
          unsigned int getNumberArguments() const;
  virtual std::string  getReturnType() const;
  virtual void         getValue(Object& outValue);
  virtual Handle       giveReference(GP::Context& ioContext);
  virtual bool         isEqual(const Object& inRightObj) const;
  virtual void         initialize(GP::System& ioSystem);
  virtual void         read(InputStream& ioIS);
  virtual void         setAttribute(std::string inAttribute);
  virtual void         setValue(const Object& inValue);
  virtual bool         validate(GP::Context& ioContext) const;
  virtual void         write(OutputStream& ioOS) const;

protected:
   void setName(std::string inName);
   void setNumberArguments(unsigned int inNumberArguments);
   void getArgument(unsigned int inN,GP::Datum& outResult,GP::Context& ioContext);
   void getArguments(GP::Datum outResults[],size_t inSizeTDatum, 
                     GP::Context& ioContext);
   void get1stArgument(GP::Datum& outResult,GP::Context& ioContext);
   void get2ndArgument(GP::Datum& outResult,GP::Context& ioContext);
   void get3rdArgument(GP::Datum& outResult,GP::Context& ioContext);

private:
  std::string  mName;             //!< Name of the primitive.
  unsigned int mNumberArguments;  //!< Number of arguments of the primitive.
};
}
```

As explained in previous sections, the primitive represent the operation to be done by nodes of the GP trees. This operation is implemented the method `execute`, in classes derived from `GP::Primitive`. In this method, the user can get the value of the child subtrees to the node and apply the characteristic operation. The result must be returned in the datum given as argument. A primitive has a fixed number of arguments, the child subtrees, and a unique name. This attribute are generally given by the constructor of the derived subclass to the constructor of `GP::Primitive`, as in the following declaration.
```
class Add : public Beagle::GP::Primitive {
public:
  Add() : Beagle::GP::Primitive(2,"+") { }
};
```
The number of arguments and the name of the primitive can also be stated by a call to the methods `setNumberArguments` and `setName`.

The primitives are put into sets of usable primitives before creating any GP trees. In the process of trees creation, for every node, an associated primitive is chosen. Usually, the association is made by a call to the method `giveReference`, affecting the node's primitive smart pointer to the chosen primitives. But, for some special case, such when using ephemeral constants, the operation done must be different. In this case the method `giveReference` can be over-defined to desired behavior. By default, the method return an handle to the primitive instance, which is then put into the primitive smart pointer of the node. By over-defining the method, the user can return any other kind of primitive instance that will be put into the node's primitive smart pointer.

**TODO: Give explanations here on primitives with variable number of arguments and primitive with variable selection weight in primitive set.**

**TODO: GP trees are no more written as Lisp-like string but rather as complete XML trees. The following paragraph should be completely rewritten.**

The way the GP trees are inserted and extracted from a Open BEAGLE XML stream is standard. For each trees, the composing nodes are written into a Lisp-like string. By default, for each node, only the name of the primitive associated is written in the Lisp-like string. If appropriate when the primitive is written, some information can be added to the name of the primitive. This is called the attribute of the primitive. The attribute of the primitive is given by a call to the method `getAttribute` and the attribute can be changed by a call to the method `setAttribute`. When an attribute different from the null string is used, the value is put between braces just after the name of the primitive, in the Lisp-like string.

A primitive can have a value that is modified at the runtime by a call to the method `setValue`. A similar method exist in class `GP::Primitive`, taking a second argument that is the name of the primitive to modify the value. Actually, the method of `GP::EvaluationOp` only call the equivalent `setValue` of the variable primitive, with the `GP::Datum` given as argument. For the abstract class `GP::Primitive`, the method `setValue` does nothing. This method can be over-defined in the more specific subclasses to do what is needed to set the value of the primitive.

Three of the remaining primitive methods `getArgType`, `getReturnType`, and `validate` are related to the usage of strongly typed GP. This is the topic of the next section.

## GP Genotypes ##

For the GP specific individual implementation, the abstract interface `Individual` is sub-classed in the concrete class `GP::Individual`. An standard GP genotype, `GP::Tree`, is also defined. This class is declared as genotype and a vector of nodes. The nodes are declared by the struct `GP::Node`, that comprises a smart pointer to a GP primitive and the size of the sub-tree.
```
namespace GP {
struct Node {
  Primitive::Handle mPrimitive;
  unsigned int      mSubTreeSize;
};
class Tree : public Genotype, public std::vector<Node> { ... };
}
```

One of the most notable points with Open BEAGLE is the possibility to redefine and modify almost everything. This is easily done by giving the appropriate allocator of a subclass to the upper layer class constructor. As example, when a `GP::Vivarium` is created, it creates by default `GP::Individual` by passing to its constructor an allocator of `GP::FitnessKoza` for the fitness value and an allocator of `GP::Tree` to allocate its genotypes.
```
namespace GP {
class Individual : public Beagle::Individual {
public:
  Individual(Fitness::Alloc::Handle inFitnessAlloc,
             GP::Tree::Alloc::Handle inTreeAlloc);
  ...
};
}
```
But, if the user want to use a different genotype, suppose a user defined custom genotype implemented in the class `MyGenotype`, he only needs to pass an allocator of his custom genotype to the allocator of `GP::Individual`:
```
Fitness::Alloc::Handle        lFitsAlloc     = new GP::FitnessKoza::Alloc;
MyGenotype::Alloc::Handle     lGenotypeAlloc = new MyGenotype::Alloc;
GP::Individual::Alloc::Handle lIndivAlloc    = 
  new GP::Individual::Alloc(lFitsAlloc,lGenotypeAlloc);
```
The allocator of individual will then create individuals that are bags of `MyGenotype`. Furthermore, if the `GP::Individual` is copied into another `GP::Individual`, the reference to the allocator of custom genotype is copied and the genotypes of the bag are cloned.

## GP Primitives and Sets ##

Open BEAGLE uses a real OO approach to implement the primitives that compose GP genotypes. The genotypes are composed of nodes that have one attribute, that is a smart pointer to an abstract primitive. Using this abstract interface, it is easy to implement primitives that have specific behavior without loosing any generality.

To make a concrete primitive that is usable to compose GP trees, the user needs to declare a concrete class derived from the abstract superclass `GP::Primitive`. Given that, existing primitives can be reused or specialized. These are some fundamental principles of OO programming. These also give us powerful mechanisms to defined atypical primitives without tweaking the internal structure.

**NOTE: The following explanations on association between the number of primitive sets in the super set and the number of trees in the GP individuals is no more true. This implicit link is broken and it is now possible to have GP individuals with several trees while having one primitive set. An index to the associated primitive set has been added to the GP trees. The following paragraph should be rewritten.**

Once the primitives used for a genetic evolution of programs are determined, they must be packed into sets that are given to the GP system. In Open BEAGLE, the process is straightforward: the user directly creates the set of primitives by inserting references to primitives. The primitives set is implemented in the class `GP::PrimitiveSet`, a bag of `GP::Primitive`. When the primitives sets are made, they are put into the super set of primitives, implemented in the class `GP::PrimitiveSuperSet`. A `GP::PrimitiveSuperSet` is a bag of `GP::PrimitiveSet`. Finally, this super set of primitives is given to the system of GP. When only one set of primitives is used, which is usual when there is no use of automatically defined structures, the user could directly pass the simple primitive set as reference to the system. Then, the system build a super set of primitives that wraps the simple set. Each set is associated with corresponding trees of each individual, as presented in figure below.

![http://wiki.beagle.googlecode.com/hg/images/primit-set.png](http://wiki.beagle.googlecode.com/hg/images/primit-set.png)

**NOTE: Lot of improvements have been made to the GP framework concerning the individuals. For example, primitives with variable number of arguments, variable selection weight of primitives in primitive set, dynamic ADFs, and evolutionary module acquisition should be detailed.**

## GP Primitives Library ##

**NOTE: The ADFs mechanisms has been completely changed.**

Some common primitives are defined from the abstract interface `GP::Primitive`. As with the operators, there is some common GP primitives into Open BEAGLE. At this moment, there is three different series of standard primitives: the generic, the arithmetics and the Boolean functions. The generic series comprise a simple terminal containing a token, `TokenT`, a primitives to program automatically subroutines, `AdfT`, and a primitive for randomly generated constants, `EphemeralT`. The second bunch comprises usual arithmetics operators. The third bunch comprises some common logical operators that work on the Open BEAGLE `Bool` type. Following table presents and describe of all these Primitives.

| **Primitive** | **Datum** | **Description** |
|:--------------|:----------|:----------------|
| `TokenT`      | Any       | Terminal that contain a value, the token. |
| `AdfT`        | Any       | Call to an associated ADF. |
| `EphemeralT`  | Any       | Generate and represent randomly generated constants. |
| `EphemeralDouble` | `Double`  | Generate and represent randomly generated floating-point constants in _[-1,1]_. |
| `AddT`        | Any (must have `operator+` defined) | Add two floating-point numbers (_x<sub>1</sub> + x<sub>2</sub>_). |
| `Add`         | `Double`  | Synonym of `AddT<Double>`. |
| `SubtractT`   | Any (must have `operator-` defined) | Subtract two floating-point numbers (_x<sub>1</sub> - x<sub>2</sub>_). |
| `Subtract`    | `Double`  | Synonym of `SubtractT<Double>`. |
| `MultiplyT`   | Any (must have `operator*` defined) | Multiply two floating-point numbers (**x<sub>1</sub> _x<sub>2</sub>_).**|
| `Multiply`    | `Double`  | Synonym of `MultiplyT<Double>`. |
| `DivideT`     | Any (must have `operator/` defined be built from a `float`) | Protected division of two floating-point numbers, return (_x<sub>1</sub> / x<sub>2</sub>_)  when _|x<sub>2</sub>| ≥ 0.001_, return _1.0_ otherwise. |
| `Divide`      | `Double`  | Synonym of `DivideT<Double>`. |
| `Sin`         | `Double`  | Sinus of a floating-point number in radians (_sin(x<sub>1</sub>)_). |
| `Cos`         | `Double`  | Cosine of a floating-point number in radians (_cos(x<sub>1</sub>)_). |
| `Log`         | `Double`  | Natural logarithm of a floating-point number (_ln(x<sub>1</sub>)_). |
| `Exp`         | `Double`  | Exponentiation of a floating-point number (_exp(x<sub>1</sub>)_). |
| `And`        | `Bool`    | And logical operator. |
| `Or`          | `Bool`    | Or logical operator. |
| `Not`         | `Bool`    | Not logical operator. |
| `Nand`        | `Bool`    | Nand logical operator. |
| `Nor`         | `Bool`    | Nor logical operator. |
| `Xor`         | `Bool`    | Xor logical operator. |