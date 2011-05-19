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
 *  \file   Beagle/Core/Evolver.hpp
 *  \brief  Definition of the class Evolver.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_Core_Evolver_hpp
#define Beagle_Core_Evolver_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/AbstractAllocT.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/System.hpp"
#include "Beagle/Core/Operator.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/OperatorMap.hpp"
#include "Beagle/Core/Vivarium.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/Package.hpp"


namespace Beagle
{

/*!
 *  \class Evolver Beagle/Core/Evolver.hpp "Beagle/Core/Evolver.hpp"
 *  \brief Basic Beagle evolver class.
 *  \ingroup Core
 *  \ingroup Op
 */
class Evolver : public Object
{

public:

	//! Evolver allocator type.
	typedef AllocatorT<Evolver,Object::Alloc> Alloc;
	//! Evolver handle type.
	typedef PointerT<Evolver,Object::Handle> Handle;
	//! Evolver bag type.
	typedef ContainerT<Evolver,Object::Bag> Bag;

	Evolver(void);
	virtual ~Evolver()
	{ }

	virtual void  configureDefaultAlgorithm(System& ioSystem);
	virtual void  evolve(Vivarium::Handle ioVivarium, System::Handle ioSystem);
	virtual void  initialize(System::Handle ioSystem, int inArgc, char** inArgv);
	virtual void  initialize(System::Handle ioSystem, const std::string& inConfigFilename);
	virtual void  initOperators(System& ioSystem);
	virtual void  read(PACC::XML::ConstIterator inIter);
	virtual void  readFromFile(const std::string& inFileName, System& ioSystem);
	virtual void  readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void  registerParams(System& ioSystem);
	virtual void  registerOperatorParams(System& ioSystem);
	virtual void  write(PACC::XML::Streamer& outStreamer, bool inIndent=true) const;

	//! Return a const reference to the bootstrap operator set.
	inline const Operator::Bag& getBootStrapSet(void) const {
		Beagle_StackTraceBeginM();
		return mBootStrapSet;
		Beagle_StackTraceEndM();
	}

	//! Return a reference to the bootstrap operator set.
	inline Operator::Bag& getBootStrapSet(void) {
		Beagle_StackTraceBeginM();
		return mBootStrapSet;
		Beagle_StackTraceEndM();
	}

	//! Return a const reference to the main-loop operator set.
	inline const Operator::Bag& getMainLoopSet(void) const {
		Beagle_StackTraceBeginM();
		return mMainLoopSet;
		Beagle_StackTraceEndM();
	}

	//! Return a reference to the main-loop operator set.
	inline Operator::Bag& getMainLoopSet(void) {
		Beagle_StackTraceBeginM();
		return mMainLoopSet;
		Beagle_StackTraceEndM();
	}

protected:

	void logWelcomeMessages(System& ioSystem);
	void parseCommandLine(System& ioSystem, int inArgc, char** inArgv);
	void readMilestone(const std::string& inFileName, System& ioSystem);
	void readVivarium(const std::string& inFileName, Vivarium& outVivarium, Context& ioContext);
	void showHelp(System& ioSystem,
	              const std::string& inProgram,
	              std::ostream& outStream=std::cerr) const;
	void showUsage(System& ioSystem,
	               const std::string& inProgram,
	               std::ostream& outStream=std::cerr) const;
	void testIndividual(Vivarium::Handle ioVivarium, Context& ioContext);

	Operator::Bag      mBootStrapSet;      //!< Bootstrap operator set.
	Operator::Bag      mMainLoopSet;       //!< Main-loop operator set.
	UIntArray::Handle  mPopSize;           //!< Population size for the evolution.
	String::Handle     mTestIndi;          //!< File name of indi to test.
	std::string        mVivariumFileName;  //!< File from which vivarium can be read.
	unsigned int       mGeneration;        //!< Generation number of evolution
	unsigned int       mDemeIndex;         //!< Deme index of evolution
	bool               mConfigChanged;     //!< Flag if evolver config changed.

};

}

#endif // Beagle_Core_Evolver_hpp
