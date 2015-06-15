# Open BEAGLE C++ Naming Convention #

All the Open BEAGLE framework has been implemented following a simple, but precise, naming convention. This naming convention is stated by the following rules.

### Rule 1 ###

An identifier is typically constituted of several concatenated words which all begins with a capital letter:
```
MyIdentifier
```

### Rule 2 ###

As well are formed _namespace_, _class_, _struct_, and _typedef_ identifiers:
```
namespace   MyNamespace;
class       MyClass;
struct      MyStruct;
typedef int MyInt;
```

### Rule 3 ###

As well are formed _template_ identifiers, with an additional capital T at the end of the identifiers:
```
template<class T> MyTemplateT;
```

### Rule 4 ###

The enumerate types follow the basic rule, but the identifiers of the enum values start with a small _e_:
```
enum MyEnum {eValue1, eValue2};
```

### Rule 5 ###

The methods identifiers are exceptions to the first rule by the fact that the first word of their name is entirely in small letters, including the first letter. Generally, the first word of a method is an action word.
```
void MyClass::setValue(...);
```

### Rule 6 ###

The variable identifiers, class attributes or local variables are also exceptions to the first rule by the fact that they must start with a key word all in small letters. These key words specify the functionality of the variable.

  * _Class_, _struct_ or _templates_ attributes starts with a small _m_.
```
class MyClass {
  int mAttribute;
};	  
```

  * If the attribute is also a _static_ member, the identifier starts with a small _sm_.
```
class MyClass {
  static int smStaticAttribute;
};
```

  * A local variable in a list of formal parameters of a method starts with the prefix _in_ if it's an input parameter, _out_ if it's an output parameter or _io_ if it's a bidirectional parameter.
```
void simpleMethod(int inParam1, double &ioParam2, char *outParam3);
```

  * A local variable that is not a member of a list of formal parameters starts with the letter _l_.
```
void MyClass::simpleMethod2(...) {
  ...
  int lLocalVariable;
  ...
}
```

  * A constant (that is typed with the _const_ qualifier) starts with the _c_ letter. A constant _class_ attribute starts with a _cm_ attribute.

### Rule 7 ###

A macros identifier follows the first rule of the identifier and ends with a capital _M_:
```
#define MyMacroM(X) (...)
```
A macro specific to a namespace start with the namespace identifier, followed by an underscore (\):
```
namespace MyNamespace {
#define MyNamespace_MyMacroM(X) (...)
}
```