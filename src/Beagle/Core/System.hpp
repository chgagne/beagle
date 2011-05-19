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
 *  \file   Beagle/Core/System.hpp
 *  \brief  Definition of the class System.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.27 $
 *  $Date: 2007/09/21 17:45:46 $
 */

/*!
 *  \defgroup Sys Evolutionary System
 *  \ingroup Core
 *  \brief Evolutionary system, part of the Core library.
 *
 *  The internal system is the structure that holds and gives access to the state
 *  of the genetic engine. These structures are fundamental, because they are used as entry points to
 *  the data of the evolution.
 *
 *  During the evolutionary processes, a context gives the current state of the evolution. A basic,
 *  general context is implemented in class Beagle::Context. It provides some essential contextual
 *  informations such as the current deme, individual, genotype and generation. For a specific EC
 *  algorithm, a specific context could be used. For example, a GP specific context is defined,
 *  Beagle::GP::Context, which contains the call stack with some other GP specific informations. An
 *  Open BEAGLE context is similar to the execution context of a computer that contains the different
 *  registers, counters and pointers to the actual state of the machine.
 *
 *  Given that the parameters of Open BEAGLE are distributed in the appropriate objects, an agent is
 *  implemented to take into account these parameter: the register. All the variables that are
 *  considered as parameters should be registered by giving the reference (object handle) of the
 *  parameter with the associated namespace and tag. The class Beagle::Register can be seen as a
 *  centralized database from which any entity could dynamically add, delete, access or modify
 *  parameters. The register is also responsible of the interpretation of the configuration file and
 *  set the parameters in consequence. The register also contains reference to every evolution
 *  operators available.
 *
 *  All the output messages given to the user pass by the logger. It consists of an interface with
 *  the user, that receives all messages, associated with a type, the class name associated to the
 *  message and an output level, and output them in a given device if the used log level allow it.
 *  This is very interesting if a user want, for example, to use Open BEAGLE into a broader system
 *  using a graphical user interface. In such case, the user only need to define his own specialized
 *  logger that will intercept the messages and log them into the desired device, for example and
 *  specific graphical windows. There is  actually one specialized logger, Beagle::LoggerXML, that
 *  log messages in a XML format into a file and the console (STDOUT). The other very interesting
 *  aspect of the logger is the possibility to choose the log level desired. The messages outputed
 *  are classified into eight categories:
 *  - Nothing (0): Log nothing
 *  - Basic (1): Log essential informations
 *  - Stats (2): Log evolution statistics
 *  - Info (3): Log general informations (default)
 *  - Detailed (4): Log details on operations
 *  - Trace (5): Log trace of the algorithms
 *  - Verbose (6): Log details on everything (disabled in optimization mode)
 *  - Debug (7): Debug (enabled only in full debug mode)
 *
 *  The registered parameters \c lg.console.level and \c lg.level.level allows the user to select
 *  the desired log level for the console and the log file, respectively. For example, if the user
 *  choose the log level \e info (3), all messages classified in categories \e basic (1),
 *  \e stats (2), and \e info (3) will be outputed the given device. Log levels
 *  \e basic (1) to \e detailed (4) are appropriate for monitoring evolutions, log levels
 *  \e detailed (4) and \e trace (5) are good to get familiar with the internal functioning of
 *  Open BEAGLE, while log levels \e trace (5) to \e debug (7) may be useful to debug an user
 *  application, or the framework.
 *
 *  Class Beagle::Randomizer provides a common pseudo-random numbers generator. The internal state
 *  of the randomizer of logged into milestone files, allowing restarting an evolution from it
 *  without changing the sequence of random numbers generated. There is also a seed number,
 *  registered as parameter \c ec.rand.seed , that is used to generate the initial state of the
 *  randomizer. By default, the seed is initiated to the timer value. The seed can be set by the
 *  user to reproduce an evolution.
 *
 *  The factory, implemented in class Beagle::Factory, is used to obtain dynamically references to a
 *  given object allocator from its name (given into a string). This thus allow dynamic allocation,
 *  copy, and cloning of any arbitrary type present in the factory. The factory also holds a map of
 *  concept-type association, allowing setting dynamically, from the code or a configuration file,
 *  the concrete type to use for a given concept of the framework. This can be useful, for example,
 *  to state a new fitness object type by simply modifying the configuration, without recompiling
 *  anything.
 *
 *  The entry point to these resources is given by an extensible central repository: the system. This
 *  is simply an entry point that possessed a set of varied component, with as the four vital
 *  components the factory, the logger, the randomizer, and the register. This general system is
 *  implemented in the class Beagle::System, and other specific components can be added into it.
 *  The system is accessible by any context of a given evolution.
 *
 */


#ifndef Beagle_Core_System_hpp
#define Beagle_Core_System_hpp

#include <set>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/Genotype.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/HallOfFame.hpp"
#include "Beagle/Core/Deme.hpp"
#include "Beagle/Core/Randomizer.hpp"
#include "Beagle/Core/Register.hpp"
#include "Beagle/Core/Factory.hpp"
#include "Beagle/Core/Logger.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/ComponentMap.hpp"
#include "Beagle/Core/Package.hpp"

#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDSET)
#include <tr1/unordered_set>
#elif defined(BEAGLE_HAVE_STD_UNORDEREDSET)
#include <unordered_set>
#endif //BEAGLE_HAVE_STDTR1_UNORDEREDSET

#ifdef BEAGLE_HAVE_OPENMP
#include "Beagle/Core/OpenMP.hpp"
#include "Beagle/Core/RandomizerMulti.hpp"
#include <omp.h>
#endif

namespace Beagle
{

/*!
 *  \class System Beagle/Core/System.hpp "Beagle/Core/System.hpp"
 *  \brief Evolutionary system.
 *  \ingroup Core
 *  \ingroup Sys
 *
 *  A system is a container for the following basic components:
 *   - Factory
 *   - Logger
 *   - Randomizer
 *   - Register
 */
class System : public ComponentMap
{

public:

	//! System allocator type.
	typedef AllocatorT<System,ComponentMap::Alloc> Alloc;
	//! System handle type.
	typedef PointerT<System,ComponentMap::Handle> Handle;
	//! System bag type.
	typedef ContainerT<System,ComponentMap::Bag> Bag;

	System(void);
	explicit System(Factory::Handle inFactory,
	                Logger::Handle inLogger,
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	                RandomizerMulti::Handle inRandomizer,
	                OpenMP::Handle inOpenMP,
#else
	                Randomizer::Handle inRandomizer,
#endif
	                Register::Handle inRegister);
	virtual ~System()
	{ }

	virtual void                    addComponent(Component::Handle inComponent);
	virtual void                    addPackage(PackageAbstract::Handle inPackage);
	virtual const Component::Handle getComponent(const std::string& inName) const;
	virtual Component::Handle       getComponent(const std::string& inName);
	virtual const Component::Handle haveComponent(const std::string& inName) const;
	virtual Component::Handle       haveComponent(const std::string& inName);
	virtual void                    replaceComponentsByConcepts(void);
	virtual void                    initComponents(void);
	virtual void                    initialize(int inArgc, char** inArgv);
	virtual void                    initialize(const std::string& inFileName);
	virtual void                    read(PACC::XML::ConstIterator inIter);
	virtual void                    readFromFile(const std::string& inFilename);
	virtual void                    registerComponentParams(void);
	virtual void                    setEvaluationOp(const std::string& inName, Allocator::Handle inEvalOpAlloc);
	virtual void                    write(PACC::XML::Streamer& outStreamer, bool inIndent=true) const;

	//! Return a const reference to the factory.
	inline const Factory& getFactory(void) const {
		Beagle_StackTraceBeginM();
		return *mFactory;
		Beagle_StackTraceEndM();
	}

	//! Return a reference to the factory.
	inline Factory& getFactory(void) {
		Beagle_StackTraceBeginM();
		return *mFactory;
		Beagle_StackTraceEndM();
	}

	//! Return a handle to the factory.
	inline Factory::Handle getFactoryHandle(void) {
		Beagle_StackTraceBeginM();
		return mFactory;
		Beagle_StackTraceEndM();
	}

	//! Return a const reference to the logger.
	inline const Logger& getLogger(void) const {
		Beagle_StackTraceBeginM();
		return *mLogger;
		Beagle_StackTraceEndM();
	}

	//! Return a reference to the logger.
	inline Logger& getLogger(void) {
		Beagle_StackTraceBeginM();
		return *mLogger;
		Beagle_StackTraceEndM();
	}

	//! Return a handle to the logger.
	inline Logger::Handle getLoggerHandle(void) {
		Beagle_StackTraceBeginM();
		return mLogger;
		Beagle_StackTraceEndM();
	}

	//! Return a const reference to the randomizer.
	inline const Randomizer& getRandomizer(void) const {
		Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
		return *((*mRandomizer)[mOpenMP->getThreadNum()]);
#else
		return *mRandomizer;
#endif
		Beagle_StackTraceEndM();
	}

	//! Return a reference to the randomizer.
	inline Randomizer& getRandomizer(void) {
		Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
		return *((*mRandomizer)[mOpenMP->getThreadNum()]);
#else
		return *mRandomizer;
#endif
		Beagle_StackTraceEndM();
	}

	//! Return a handle to the randomizer.
	inline Randomizer::Handle getRandomizerHandle(void) {
		Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
		return (*mRandomizer)[mOpenMP->getThreadNum()];
#else
		return mRandomizer;
#endif
		Beagle_StackTraceEndM();
	}

	//! Return a const reference to the register.
	inline const Register& getRegister(void) const {
		Beagle_StackTraceBeginM();
		return *mRegister;
		Beagle_StackTraceEndM();
	}

	//! Return a reference to the register.
	inline Register& getRegister(void) {
		Beagle_StackTraceBeginM();
		return *mRegister;
		Beagle_StackTraceEndM();
	}

	//! Return a handle to the register.
	inline Register::Handle getRegisterHandle(void) {
		Beagle_StackTraceBeginM();
		return mRegister;
		Beagle_StackTraceEndM();
	}

protected:

	friend class Evolver;

	void parseCommandLine(int inArgc, char** inArgv);
	void showHelp(const std::string& inProgram, std::ostream& outStream=std::cerr) const;
	void showUsage(const std::string& inProgram, std::ostream& outStream=std::cerr) const;


#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDSET)
	typedef std::tr1::unordered_set<std::string,HashString> StringSet;
#elif defined(BEAGLE_HAVE_STD_UNORDEREDSET)
	typedef std::unordered_set<std::string,HashString> StringSet;
#else // No unordered_set found
	typedef std::set<std::string> StringSet;
#endif

	StringSet           mPackages;    //!< Packages that have been added in.
	Factory::Handle     mFactory;     //!< Object factory system component.
	Logger::Handle      mLogger;      //!< Logger system component.

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	RandomizerMulti::Handle  mRandomizer;  //!< Randomizer system component.
	OpenMP::Handle			 mOpenMP;
#else
	Randomizer::Handle  mRandomizer;  //!< Randomizer system component.
#endif

	Register::Handle    mRegister;    //!< Register system component.

};

}

#endif // Beagle_Core_System_hpp
