/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/Core/Operator.hpp
 *  \brief  Definition of the class Operator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:13 $
 */

/*!
 *  \defgroup Op Operators and Evolvers
 *  \ingroup Core
 *  \brief Generic evolutionary operators and evolvers, part of the Core library.
 *
 *  The operator is a central concept of Open BEAGLE as an EC framework. In Open BEAGLE, the
 *  main-loop of operations executed on populations is dynamically defined. The operators are
 *  specified at run-time and the user is free to define them for his evolution. This gives the
 *  opportunity to easily and rapidly experiment numerous variants of EC. The operator and evolver
 *  model is based on the \e Strategy design pattern, which is applied to the
 *  evolutionary algorithms. The operator interface is declared in the abstract class
 *  Beagle::Operator.
 *  \code
 *  class Operator : public Object {
 *  public:
 *    Operator() { }
 *    virtual void registerParams(System& ioSystem) { }
 *    virtual void operate(Deme& ioDeme,Context& ioContext) = 0;
 *  };
 *  \endcode
 *
 *  Before the characteristic method is applied to demes, method Beagle::Operator::registerParams is
 *  invocated. In this method, the operator usually registers it's own parameters, probabilities or
 *  anything else, used by the characteristic operation. The characteristic operation is defined in
 *  the virtual method Beagle::Operator::operate. There is a bunch of predefined operators in Open
 *  BEAGLE. To name a few of them, the tournament selection operator (Beagle::TournamentSelectionOp),
 *  the GP tree crossover operator  (Beagle::GP::CrossoverOp) and and the statistics
 *  computation operator (Beagle::StatsCalculateOp).
 *
 *  The user that define his own operators must be aware that the system is not fully
 *  set-up when the Beagle::Operator::registerParams method is called. For example, the random number
 *  generator must not be used, as the seed can be modified thereafter on the command-line or in a
 *  configuration file. The rule is that the Beagle::Operator::registerParams method must be used only to
 *  add elements to the evolution system.
 *
 *  The operators of a specific evolution are inserted into the evolver that supervises the evolution
 *  process. This object, implemented in class Beagle::Evolver, comprises two major attributes: the
 *  bootstrap operator set and the main-loop operator set. The bootstrap operators set contains an
 *  ordered list of operators to apply on each deme, for the initial generation. The main loop
 *  operators set is an ordered list of operators to apply iteratively, at each generation, on each
 *  deme. The user could launch an evolution, by calling the method Beagle::Evolver::evolve with the
 *  vivarium to evolve as argument.
 *  \code
 *  class Evolver : public Object {
 *  public:
 *    virtual void initialize(System::Handle ioSystem, int inArgc, char** inArgv);
 *    virtual void evolve(Vivarium::Handle ioVivarium);
 *  protected:
 *    Operator::Bag mBootStrapSet;
 *    Operator::Bag mMainLoopSet;
 *  };
 *  \endcode
 *  For common EC algorithms, the user usually needs not to create custom sequences of operators. In
 *  fact, some classes inheriting of Beagle::Evolver can be used to create evolvers with predefined
 *  operator sets. If a special EC algorithm is needed, a custom building method can be invocated and
 *  the evolver should be configured properly.
 *
 */


#ifndef Beagle_Core_Operator_hpp
#define Beagle_Core_Operator_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/AbstractAllocT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/NamedObject.hpp"

namespace Beagle
{

// Forward declaration.
class Deme;
class Context;
class System;
class OperatorMap;


/*!
 *  \class Operator Beagle/Core/Operator.hpp "Beagle/Core/Operator.hpp"
 *  \brief Evolutionary operator abstract class.
 *  \ingroup Core
 *  \ingroup Op
 */
class Operator : public NamedObject
{

public:

	//! Operator allocator type.
	typedef AbstractAllocT<Operator,NamedObject::Alloc> Alloc;
	//! Operator handle type.
	typedef PointerT<Operator,NamedObject::Handle> Handle;
	//! Operator bag type.
	typedef ContainerT<Operator,NamedObject::Bag> Bag;

	explicit Operator(std::string inName);
	virtual ~Operator()
	{ }

	/*!
	 *  \brief Apply operation to a deme in a given context.
	 *  \param ioDeme Reference to the deme on which the operation takes place.
	 *  \param ioContext Evolutionary context of the operation.
	 *
	 *  This method needs to be specialized in a sub-class.
	 */
	virtual void operate(Deme& ioDeme, Context& ioContext) = 0;

	virtual void init(System& ioSystem);
	virtual void read(PACC::XML::ConstIterator);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	//! Return whether the parameters of this operator have been registered.
	bool hasRegisteredParams(void) {
		return mRegistered;
	}

	//! Return whether this operator has been initialized.
	bool isInitialized(void) {
		return mInitialized;
	}

	//! Set the state of the registered flag to value \c inValue.
	void setRegisteredFlag(bool inValue=true) {
		mRegistered = inValue;
	}

	//! Set the state of the initialized flag to value \c inValue.
	void setInitializedFlag(bool inValue=true) {
		mInitialized = inValue;
	}

protected:

	bool mRegistered; //!< Flag that indicates whether operator parameters have been registered.
	bool mInitialized; //!< Flag that indicates whether this operator has been intialized.

};

}

#endif // Beagle_Core_Operator_hpp
