# Bit String Representation Manual #

**TODO: This page must be revised completely.**

The Genetic Algorithm (GA) framework has been re-engineered in version 2.1.0 to extend its support over the basic binary representation to real-valued GA and simple Evolution Strategy (ES) representations. The three standard representations of the GA framework share the namespace `GA` and a similar organization. Each representation has a genotype class that implement the linear structure which are derived from a `std::vector`. There is several crossover operators generic (1-point, 2-points, uniform) for vector-based representation implemented as class template, where the templated parameter must respect the STL _sequence_ conceptual interface (the `std::vector` template respects this interface). A representation-specific type for each generic crossover operator is defined for each standard GA representation. There is also a representation-specific mutation operator defined for each standard representation, along with a per representation evolver.

To implement an application using a GA representation, we should follow the steps explained in Section [Building a System for an Evolution](UserGuide#Building_a_System_for_an_Evolution.md) by taking attention to specify the genotype used and the appropriate evolver. The following code snippet is extracted for the `onemax` example and illustrates this for a bit-string GA representation.
```
// 1. Build the system.
System::Handle lSystem = new System;
// 2. Build evaluation operator.
OneMaxEvalOp::Handle lEvalOp = new OneMaxEvalOp;
// 3. Instanciate the evolver.
const unsigned int lNumberOfBitsPerGenotype = 50;
IntegerVector lEncoding(1, lNumberOfBitsPerGenotype);
GA::EvolverBitString::Handle lEvolver = new GA::EvolverBitString(lEvalOp, lEncoding);
// 4. Initialize the vivarium for bit string GA population.
GA::BitString::Alloc::Handle lBSAlloc = new GA::BitString::Alloc;
Vivarium::Handle lVivarium = new Vivarium(lBSAlloc);
// 5. Initialize the evolver and evolve the vivarium.
lEvolver->initialize(lSystem, argc, argv);
lEvolver->evolve(lVivarium);
```
Each of three GA standard evolvers have a constructor that take as argument an integer vector. The size of this vector states the number of genotypes in each individual, while the integer value of it specify the number of elements of each linear genotype. So in the previous example, the integer vector passed to the evolver at step 3 configures the evolve to initialize the individuals with one bit-string of 50 random bits. The vivarium is initialized at step 4 with a bit string allocator to generate the population. For the two other representation, we would have used a `GA::FloatVector::Alloc` for real-valued GA or a `GA::ESVector::Alloc` for ES.

In the bit-string GA representation, the bits can be used as is (for example in the _OneMax_ problem), or transformed into floating-point value in a given interval and a given precision, depending the number of bits used (for example in the function maximization problem). When the bits are converted to floating-point values, these are used as parameters to solve the problem, the fitness value is deduced from these parameters. The representation of a binary GA genotype is implemented in the class `GA::BitString`, which both inherits from the class `Genotype` and `std::vector<bool>`.
```
namespace GA {
class BitString : public Beagle::Genotype, public std::vector<bool> {
public:
  struct DecodingKey {
    double       mLowerBound;
    double       mUpperBound;
    unsigned int mEncoding;
  };
  void decode(const std::vector<DecodingKey>& inKeys, 
              std::vector<double>& outVector) const;
  void decodeGray(const std::vector<DecodingKey>& inKeys,
                  std::vector<double>& outVector) const;
  ...
};
}
```
The method `decode` is used to decode binary-encoded bit strings, to translate it into a real-valued vector using a decoding key. The method `decodeGray` is similar to the `decode` method, except that it supposes Gray-encoded bit strings. If the application needs to work directly on the bits, the interface of the class `std::vector<bool>` can be used.