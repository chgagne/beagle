# Strongly Typed Genetic Programming Manual #

**TODO: Typing verification in STGP has been changed to compare references to `type_info` object instead of strings. The whole subsection on STGP should be updated accordingly.**

Strongly typed GP (STGP) is a standard Open BEAGLE GP feature. In every standard operators that modify the structure of the GP individuals, a dual operator supporting constraints is defined. These operator apply their operation while checking whether the typing of the nodes is respected when a GP genome is altered. Following table presents the list standard constrained GP operators actually implemented in the framework.
| **GP Operator** | **Constrained Dual Operator** |
|:----------------|:------------------------------|
| `GP::InitFullOp` | `GP::InitFullConstrainedOp`   |
| `GP::InitGrowOp` | `GP::InitGrowConstrainedOp`   |
| `GP::InitHalfOp` | `GP::InitHalfConstrainedOp`   |
| `GP::CrossoverOp` | `GP::CrossoverConstrainedOp`  |
| `GP::MutationSwapOp` | `GP::MutationSwapConstrainedOp` |
| `GP::MutationShrinkOp` | `GP::MutationShrinkConstrainedOp` |
| `GP::MutationStandardOp` | `GP::MutationStandardConstrainedOp` |
| `GP::MutationSwapSubtreeOp` | `GP::MutationSwapSubtreeConstrainedOp` |


When an user want to take advantage of STGP, he first needs to use an evolver made of the constrained operators (instead of the simple unconstrained ones). The example `spambase` included in the distribution contains configuration files where evolver composed of operators that support constraints. Section [Building a Custom Evolver](UserGuide#Building_a_Custom_Evolver.md) also explain how to change the set-up of evolvers.

The second point needed to do STGP is to set the typing in the primitives used. The typing is made by associating a char string to a type, generally the RTTI name of the datum type. To implement the typing of the individuals, two methods must be over-defined in the concrete primitives classes, `getArgType` and `getReturnType`. The method `getReturnType` gives the type of the datum that is returned by the primitives. The method `getArgType` give the string type of the ith argument of the nodes associated to the primitive.

To illustrate this, suppose an application where two different datum types are used, `Double` and `Bool`. STGP is usually implemented by using the RTTI naming of the types used. Supposing a _if-then-else_ primitive implemented in the class `IfThenElse`. The primitive received a Boolean as first argument and return a floating-point number, that is the second argument of the primitive if the Boolean is true and the third if not.
```
using namespace Beagle;
class IfThenElse : public GP::Primitive {
public:
  IfThenElse() : GP::Primitive(3,"if-then-else") { }
  virtual void execute(GP::Datum& outResult, 
                       GP::Context& ioContext) {
    Bool lTest;
    get1stArguments(lTest,ioContext);
    if(lTest == true) get2ndtArgument(outResult,ioContext);
    else get3rdArgument(outResult,ioContext);
  }
  virtual std::string getArgType(unsigned int inN) const {
    if(inN == 0) return typeid(Bool).name();
    return typeid(Double).name();
  }
  virtual std::string getReturnType() const {
    return typeid(Double).name();
  }
};
```
The type of the root of a given tree is stated in the primitive set associated to the given GP tree. The constructor of the class `GP::PrimitiveSet` is defined in such way that you only need to pass the name of the type returned from the root of the tree when constructing the set.

This implementation of STGP is usually sophisticated enough to allow quite constrained tree structures. But, the use of STGP does not allow all kind of structural constraints, and the user may want to apply to the GP individuals more elaborated constraints. The method `validate` of `GP::Primitive` is called to do the typing validation. As defined in `GP::Primitive`, this method implements STGP by checking the types returned by the methods `getArgType` and `getReturnType`. The user can over-define the method to do his custom structural checking. The method returns true if the structure imposed is respected, and false if not.

**TODO: More explanations should be given on general constraints that can be applied on GP trees. The use of constrained operator for dynamic ADFs should also be detailed.**