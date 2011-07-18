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
 *  \file   Beagle/Core/System.cpp
 *  \brief  Source code of class System.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.44 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include <cstring>

#include "Beagle/Core.hpp"

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#else // BEAGLE_HAVE_LIBZ
#include <fstream>
#endif // BEAGLE_HAVE_LIBZ

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a new system with the default basic components.
 */
System::System(void) :
	mFactory(new Factory),
	mLogger(new LoggerXML),
#if defined(BEAGLE_USE_OMP_NR) ||  defined(BEAGLE_USE_OMP_R)
	mRandomizer(new RandomizerMulti),
	mOpenMP(new OpenMP),
#else
	mRandomizer(new Randomizer),
#endif
	mRegister(new Register)
{
	Beagle_StackTraceBeginM();
	// Install basic components
	addComponent(mLogger);
	addComponent(mFactory);
	addComponent(mRandomizer);
	addComponent(mRegister);

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	addComponent(mOpenMP);
#endif

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Construct a new system with a given factory, logger, randomizer, and register.
 *  \param inFactory Handle to the object factory component used.
 *  \param inLogger Handle to the logger component used.
 *  \param inRandomizer Handle to the randomizer component used.
 *  \param inRegister Handle to the register component used.
 */
System::System(Factory::Handle    inFactory,
               Logger::Handle     inLogger,
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
               RandomizerMulti::Handle inRandomizer,
               OpenMP::Handle inOpenMP,
#else
               Randomizer::Handle inRandomizer,
#endif
               Register::Handle   inRegister) :
	mFactory(inFactory),
	mLogger(inLogger),
	mRandomizer(inRandomizer),
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	mOpenMP(inOpenMP),
#endif
	mRegister(inRegister)
{
	Beagle_StackTraceBeginM();
	// Install basic components
	addComponent(mLogger);
	addComponent(mFactory);
	addComponent(mRandomizer);
	addComponent(mRegister);

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	addComponent(mOpenMP);
#endif

	Beagle_StackTraceEndM();
}

void System::replaceComponentsByConcepts(void)
{
	Beagle_StackTraceBeginM();
	const std::string& lLoggerName = mFactory->getConceptTypeName("Logger");
	if(lLoggerName != "LoggerXML") {
		Logger::Alloc::Handle lLoggerAlloc = castHandleT<Logger::Alloc>(mFactory->getAllocator(lLoggerName));
		Logger::Handle lNewLogger = castHandleT<Logger>(lLoggerAlloc->allocate());
		lNewLogger->copyBuffer(*mLogger);
		mLogger = lNewLogger;
		(*this)["Logger"] = mLogger;
	}
#if !defined(BEAGLE_USE_OMP_NR) &&  !defined(BEAGLE_USE_OMP_R)
	const std::string& lRandomizerName = mFactory->getConceptTypeName("Randomizer");
	if(lRandomizerName != "Randomizer") {
		Randomizer::Alloc::Handle lRandomizerAlloc = castHandleT<Randomizer::Alloc>(mFactory->getAllocator(lRandomizerName));
		Randomizer::Handle lNewRandomizer = castHandleT<Randomizer>(lRandomizerAlloc->allocate());
		mRandomizer = lNewRandomizer;
		(*this)["Randomizer"] = mRandomizer;
	}
#endif
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Add a new component to the system.
 *  \param inComponent The component to add.
 *
 *  To be initialized, components need to be add to the system.
 */
void System::addComponent(Component::Handle inComponent)
{
	Beagle_StackTraceBeginM();

	// check that component is valid and not yet added
	Beagle_NonNullPointerAssertM(inComponent);
	if(find(inComponent->getName()) != end())
		throw Beagle_ObjectExceptionM(string("A component named '")+inComponent->getName()+
		                              string("' is already installed in the system!"));

	// add component
	Beagle_LogDetailedM(
	    getLogger(),
	    string("Adding component '")+inComponent->getName()+string("' to the system")
	);
	(*this)[inComponent->getName()] = inComponent;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a new package in the system.
 *  \param inPackage Package to add.
 */
void System::addPackage(PackageAbstract::Handle inPackage)
{
	Beagle_StackTraceBeginM();

	// Look if package is installed
	StringSet::const_iterator lIterPack = mPackages.find(inPackage->getName());
	if(lIterPack != mPackages.end()) {
		Beagle_LogDetailedM(
		    getLogger(),
		    std::string("Not installing package '")+inPackage->getName()+
		    std::string("' as it has already been added")
		);
		return;
	}
	mPackages.insert(inPackage->getName());

	// Look for dependencies of the package
	PackageAbstract::Bag lDependencies = inPackage->listDependencies();
	for(unsigned int i=0; i<lDependencies.size(); ++i) {
		Beagle_LogDetailedM(
		    getLogger(),
		    std::string("Package '")+inPackage->getName()+
		    std::string("' depends on package '")+lDependencies[i]->getName()+
		    std::string("'")
		);
		addPackage(lDependencies[i]);
	}

	// Install package itself
	Beagle_LogDetailedM(
	    getLogger(),
	    std::string("Installing package '")+inPackage->getName()+std::string("'")
	);
	inPackage->configure(*this);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get a component by name.
 *  \param inName Component name.
 *  \return Component handle.
 */
const Beagle::Component::Handle System::getComponent(const std::string& inName) const
{
	Beagle_StackTraceBeginM();

	const ComponentMap::const_iterator lItr = find(inName);
	if (lItr == end()) {
		throw Beagle_RunTimeExceptionM(string("Could not retrieve component named '")+inName+
		                               string("'. Either the component is not installed, or the name is invalid!"));
	}
	return castHandleT<Component>(lItr->second);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get a component by name.
 *  \param inName Component name.
 *  \return Component handle.
 */
Beagle::Component::Handle System::getComponent(const std::string& inName)
{
	Beagle_StackTraceBeginM();

	ComponentMap::iterator lItr = find(inName);
	if (lItr == end()) {
		throw Beagle_RunTimeExceptionM(string("Could not retrieve component named '")+inName+
		                               string("'. Either the component is not installed, or the name is invalid!"));
	}
	return castHandleT<Component>(lItr->second);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check for the existence of a component by name.
 *  \param inName Component name.
 *  \return A valid component handle if it exists, a null handle otherwise.
 */
const Beagle::Component::Handle System::haveComponent(const std::string& inName) const
{
	Beagle_StackTraceBeginM();

	const ComponentMap::const_iterator lItr = find(inName);
	if (lItr == end()) return Component::Handle(NULL);
	return castHandleT<Component>(lItr->second);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check for the existence of a component by name.
 *  \param inName Component name.
 *  \return A valid component handle if it exists, a null handle otherwise.
 */
Beagle::Component::Handle System::haveComponent(const std::string& inName)
{
	Beagle_StackTraceBeginM();

	const ComponentMap::const_iterator lItr = find(inName);
	if (lItr == end()) return Component::Handle(NULL);
	return castHandleT<Component>(lItr->second);

	Beagle_StackTraceEndM();
}


/*!
*  \brief Initialize the components of this system.
 */
void System::initComponents(void)
{
	Beagle_StackTraceBeginM();

	Beagle_LogDetailedM(
	    getLogger(),
	    "Initializing system components"
	);

	// Make sure that the logger is initialized before anything else
	mLogger->init(*this);
	mLogger->setInitializedFlag(true);

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	// OpenMP component has to be initialised before the randomizer
	mOpenMP->init(*this);
	mOpenMP->setInitializedFlag(true);
#endif

	// Randomizer should be initialized second
	mRandomizer->init(*this);
	mRandomizer->setInitializedFlag(true);

	// Initialize remaining components
	for(iterator lItr = begin(); lItr != end(); ++lItr) {
		Component::Handle lComponent = castHandleT<Component>(lItr->second);
		if(lComponent->isInitialized() == false) {
			Beagle_LogDetailedM(
			    getLogger(),
			    string("Initializing component '")+lComponent->getName()+string("'")
			);
			lComponent->init(*this);
			lComponent->setInitializedFlag(true);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this system from the command-line.
 *  \param inArgc Number of command-line arguments.
 *  \param inArgv Array of command-line string arguments.
 */
void System::initialize(int inArgc, char** inArgv)
{
	Beagle_StackTraceBeginM();

	// replace component by their equivalent factory concept
	replaceComponentsByConcepts();

	// register component parameters.
	registerComponentParams();

	// read default configuration file
	try {
		readFromFile("beagle.conf");
	} catch(const Exception&) {}

	// parse the command-line
	parseCommandLine(inArgc, inArgv);

	// initialize components.
	initComponents();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this system from a configuration file.
 *  \param inFileName Configuration file name.
 */
void System::initialize(const std::string& inFileName)
{
	Beagle_StackTraceBeginM();

	// replace component by their equivalent factory concept
	replaceComponentsByConcepts();

	// register component parameters
	registerComponentParams();

	// read specified configuration file
	readFromFile(inFileName);

	// initialize components
	initComponents();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Parse the command-line options.
 *  \param inArgc Number of arguments on the command-line.
 *  \param inArgv Arguments of the command-line.
 *  \throw Exception If something bad occured.
 *
 *  Options have either of the following two formats:
 *  - [-OBparameter=value]
 *  - [-OBparam1=value1,param2=value2, ... ,paramN=valueN]
 *  .
 */
void System::parseCommandLine(int inArgc, char** inArgv)
{
	Beagle_StackTraceBeginM();

	// process all command-line arguments
	Beagle_LogDetailedM(
	    getLogger(),
	    "Parsing the command-line"
	);
	for(int i = 1; i < inArgc; i++) {
		if(memcmp(inArgv[i], "-OB", 3) == 0) {
			// process every -OB argument
			std::string lParsedString(inArgv[i]+3);
			std::istringstream lStream(lParsedString);
			PACC::Tokenizer lTokenizer(lStream);
			lTokenizer.setDelimiters("", "=,");
			std::string lName, lToken, lValue;
			while(lTokenizer.getNextToken(lName)) {
				// process every option
				if(getRegister().isRegistered(lName) == false) {
					std::cerr << "Keyword '" << lName << "' is not a valid command-line option!" << std::endl;
					showUsage(inArgv[0], std::cerr);
					exit(-1);
				}
				if(lTokenizer.getNextToken(lToken)) {
					if(lToken != "=") {
						std::cerr << "Invalid argument format: '" << inArgv[i] << "'" << std::endl;
						showUsage(inArgv[0], std::cerr);
						exit(-1);
					}
					if(lTokenizer.getNextToken(lValue)) {
						if(lValue == "=" || lValue == ",") {
							std::cerr << "Invalid argument format: '" << inArgv[i] << "'" << std::endl;
							showUsage(inArgv[0], std::cerr);
							exit(-1);
						}
					}
					if(lTokenizer.getNextToken(lToken)) {
						if(lToken != ",") {
							std::cerr << "Invalid argument format: '" << inArgv[i] << "'" << std::endl;
							showUsage(inArgv[0], std::cerr);
							exit(-1);
						}
					}
				}
				if(lName == "dump") {
					// dump a configuration
					std::string lFilenameBak = lValue + "~";
					std::remove(lFilenameBak.c_str());
					std::rename(lValue.c_str(), lFilenameBak.c_str());
					std::cerr << "Dumping a configuration file named '" << lValue << "'" << std::endl << std::flush;
					std::ofstream lStream(lValue.c_str());
					PACC::XML::Streamer lStreamer(lStream);
					lStreamer.insertHeader();
					lStreamer.openTag("Beagle");
					lStreamer.insertAttribute("version", BEAGLE_VERSION);
					lStreamer.insertComment("This file is the result of a configuration dump");
					write(lStreamer);
					lStreamer.closeTag();
					lStream.close();
					exit(0);
				} else if(lName == "help") {
					// show detailed usage
					showHelp(inArgv[0], std::cerr);
					exit(0);
				} else if(lName == "register") {
					// read register configuration
					getRegister().readFromFile(lValue, *this);
				} else if(lName == "system" || lName == "ec.conf.file") {
					if (lName=="ec.conf.file") {
						Beagle_LogBasicM(getLogger(), "The register variable 'ec.conf.file' is DEPRECATED, please use 'system' instead.");
					}
					// read system configuration
					readFromFile(lValue);
					// register the parameters of any new component
					registerComponentParams();
				} else if(lName == "usage") {
					// show short usage
					showUsage(inArgv[0], std::cerr);
					exit(0);
				} else {
					// update register value
					PACC::XML::Node lNode(lValue, PACC::XML::eString);
					getRegister()[lName]->read(PACC::XML::ConstIterator(&lNode));
				}
			}
		} else if(memcmp(inArgv[i], "-EC", 3) == 0) {
			// deprecated -EC option
			std::cerr << "The -EC command-line option prefix is DEPRECATED, please use -OB instead." << std::endl;
			showUsage(inArgv[0], std::cerr);
			exit(-1);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read this system from a parse tree node.
 *  \param inIter System node.
 */
void System::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();

	Beagle_LogDebugM(
	    getLogger(),
	    "Reading system components"
	);

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="System"))
		throw Beagle_IOExceptionNodeM(*inIter, "invalid node type or tag name!");

	// read all components.
	for(PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			if(find(lChild->getValue()) == end())
				throw Beagle_ObjectExceptionM(string("WARNING: unknown component named '")+
				                              string(lChild->getValue())+
				                              string("'; not read!"));
			(*this)[lChild->getValue()]->readWithSystem(lChild, *this);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read system from an XML file.
 *  \param inFileName Name of the file to read the system from.
 */
void System::readFromFile(const std::string& inFileName)
{
#ifdef BEAGLE_HAVE_LIBZ
	igzstream lStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	if(lStream.good() == false) {
		throw Beagle_RunTimeExceptionM(std::string("Could not open file '")+inFileName+"'");
	}
	Beagle_LogBasicM(
	    getLogger(),
	    std::string("Reading file '")+inFileName+"' for system configuration"
	);
	PACC::XML::Document lDocument(lStream, inFileName);
	lStream.close();
	PACC::XML::ConstFinder lFinder(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPos = lFinder.find("/Beagle/System");
	if(!lPos) {
		Beagle_LogBasicM(
		    getLogger(),
		    "WARNING: file does not contain any valid system"
		);
	} else {
		// read all system instances
		read(lPos);
		if(lPos = lFinder.findNext()) {
			Beagle_LogBasicM(
			    getLogger(),
			    "WARNING: file contains multiple systems"
			);
			do {
				read(lPos);
			} while(lPos = lFinder.findNext());
		}
	}
}


/*!
 *  \brief Register the component parameters of this system.
 */
void System::registerComponentParams(void)
{
	Beagle_StackTraceBeginM();

	Beagle_LogDetailedM(
	    getLogger(),
	    "Registering system component parameters"
	);

	for(iterator lItr = begin(); lItr != end(); ++lItr) {
		Component::Handle lComponent = castHandleT<Component>(lItr->second);
		if(lComponent->hasRegisteredParams() == false) {
			Beagle_LogTraceM(
			    getLogger(),
			    std::string("Registering '") + lComponent->getName() + "' parameters"
			);
			lComponent->registerParams(*this);
			lComponent->setRegisteredFlag(true);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the evaluation operator to use.
 *  \param inName Name of the evaluation operator.
 *  \param inEvalOpAlloc Allocator of the evaluation operator to use.
 */
void System::setEvaluationOp(const std::string& inName, Allocator::Handle inEvalOpAlloc)
{
	Beagle_StackTraceBeginM();
	mFactory->insertAllocator(inName, inEvalOpAlloc);
	mFactory->setConcept("EvaluationOp", inName);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the long program usage.
 *  \param inProgram Program name.
 *  \param outStream Output stream.
 */
void System::showHelp(const std::string& inProgram, std::ostream& outStream) const
{
	Beagle_StackTraceBeginM();

	outStream << "Open BEAGLE detailed help  (package: ";
	outStream << BEAGLE_PACKAGE << ", version: " << BEAGLE_VERSION << ")" << std::endl << std::endl;
	outStream << "usage: " << inProgram << " [-OBparameter=value] ..." << std::endl;
	outStream << "       " << inProgram << " [-OBparam1=value1,param2=value2, ... ,paramN=valueN] ..." << std::endl;
	outStream << std::endl;
	outStream << "Common options:" << std::endl;
	outStream << "--------------" << std::endl;
	outStream << std::endl;
	outStream << "-OBdump=String         Dump configuration file" << std::endl;
	outStream << "-OBhelp                Show detailed help"      << std::endl;
	outStream << "-OBregister=String     Load register from file" << std::endl;
	outStream << "-OBsystem=String       Load system from file"   << std::endl;
	outStream << "-OBusage               Show program usage"      << std::endl;
	outStream << std::endl;
	outStream << "Specific options for this program:" << std::endl;
	outStream << "---------------------------------"  << std::endl;
	outStream << std::endl;

	mRegister->showHelp(outStream);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the short program usage.
 *  \param inProgram Program name.
 *  \param outStream Output stream.
 */
void System::showUsage(const std::string& inProgram, std::ostream& outStream) const
{
	Beagle_StackTraceBeginM();

	outStream << "usage: " << inProgram << " [-OBparameter=value] ..." << std::endl;
	outStream << "       " << inProgram << " [-OBparam1=value1,param2=value2, ... ,paramN=valueN] ..." << std::endl;
	outStream << std::endl;
	outStream << "Common options:" << std::endl;
	outStream << "--------------" << std::endl;
	outStream << std::endl;
	outStream << "-OBdump=String         Dump configuration file" << std::endl;
	outStream << "-OBhelp                Show detailed help"      << std::endl;
	outStream << "-OBregister=String     Load register from file" << std::endl;
	outStream << "-OBsystem=String       Load system from file"   << std::endl;
	outStream << "-OBusage               Show program usage"      << std::endl;
	outStream << std::endl;
	outStream << "Specific options for this program:" << std::endl;
	outStream << "---------------------------------"  << std::endl;
	outStream << std::endl;

	mRegister->showUsage(outStream);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write system to stream.
 *  \param outStreamer Output streamer.
 *  \param inIndent Whether markup should be indented.
 */
void System::write(PACC::XML::Streamer& outStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	outStreamer.openTag("System", inIndent);
	std::map<string, Component::Handle> lOrderedMap;
	for(ComponentMap::const_iterator lItr = begin(); lItr != end(); ++lItr) {
		lOrderedMap[lItr->first] = castHandleT<const Component>(lItr->second);
	}
	for(std::map<string, Component::Handle>::const_iterator lItr = lOrderedMap.begin();
	        lItr != lOrderedMap.end(); ++lItr) {
		const Component::Handle lComponent = castHandleT<const Component>(lItr->second);
		lComponent->write(outStreamer, inIndent);
	}
	outStreamer.closeTag();

	Beagle_StackTraceEndM();
}

