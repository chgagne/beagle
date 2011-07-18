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
 *  \file   Beagle/Core/Evolver.cpp
 *  \brief  Source code of class Evolver.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.74 $
 *  $Date: 2007/09/12 12:40:43 $
 */

#include "Beagle/Core.hpp"

#include <set>
#include <cstring>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Construct an evolver.
 */
Evolver::Evolver(void) :
	mGeneration(0),
	mDemeIndex(0),
	mConfigChanged(false)
{ }

/*!
 *  \brief Configure evolver with the default evolutionary algorithm used.
 *  \param ioSystem Evolutionary system.
 */
void Evolver::configureDefaultAlgorithm(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Algorithm::Alloc::Handle lAlgoAlloc =
	    castHandleT<Algorithm::Alloc>(ioSystem.getFactory().getConceptAllocator("Algorithm"));
	if(lAlgoAlloc == NULL) {
		Beagle_LogDetailedM(
		    ioSystem.getLogger(),
		    "No default algorithm defined, evolver stay empty"
		);
		return;
	}
	Algorithm::Handle lAlgo = castHandleT<Algorithm>(lAlgoAlloc->allocate());
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    std::string("Configuring evolver with default algorithm '")+
	    ioSystem.getFactory().getConceptTypeName("Algorithm")+"'"
	);
	lAlgo->configure(*this, ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Evolve a given vivarium.
 *  \param ioVivarium Handle to the vivarium to evolve.
 *  \param ioSystem Evolutionary system.
 */
void Evolver::evolve(Vivarium::Handle ioVivarium, System::Handle ioSystem)
{
	Beagle_StackTraceBeginM();

	// Initialize the evolution context.
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioSystem->getFactory().getConceptAllocator("Context"));
	Context::Handle lEvolContext = castHandleT<Context>(lContextAlloc->allocate());
	lEvolContext->setSystemHandle(ioSystem);
	lEvolContext->setEvolverHandle(this);
	lEvolContext->setVivariumHandle(ioVivarium);
	lEvolContext->setDemeIndex(mDemeIndex);
	lEvolContext->setGeneration(mGeneration);
	lEvolContext->setContinueFlag(true);
	lEvolContext->setTerminationSuccessful(false);

	// Check if an individual as been provided for evaluation
	if(!mTestIndi->getWrappedValue().empty()) {
		testIndividual(ioVivarium, *lEvolContext);
		return;
	}

	logWelcomeMessages(*ioSystem);

	if(mVivariumFileName.empty()) {
		Beagle_LogTraceM(
		    ioSystem->getLogger(),
		    std::string("Vivarium resized to ")+uint2str(mPopSize->size())+" demes"
		);
		const Factory& lFactory = ioSystem->getFactory();
		Deme::Alloc::Handle lDemeAlloc =
		    castHandleT<Deme::Alloc>(lFactory.getConceptAllocator("Deme"));
		ioVivarium->clear();
		for(unsigned int i=0; i<mPopSize->size(); ++i) {
			ioVivarium->push_back(castHandleT<Deme>(lDemeAlloc->allocate()));
		}
	} else {
		Beagle_LogTraceM(
		    ioSystem->getLogger(),
		    std::string("Reading vivarium from file '")+mVivariumFileName+"'"
		);
		readVivarium(mVivariumFileName, *ioVivarium, *lEvolContext);
		unsigned int lDemeIndex = mDemeIndex+1;
		unsigned int lGeneration = mGeneration;
		if(lDemeIndex == ioVivarium->size()) {
			++lGeneration;
			lDemeIndex = 0;
		}
		lEvolContext->setDemeIndex(lDemeIndex);
		lEvolContext->setGeneration(lGeneration);
	}

	while( lEvolContext->getContinueFlag() ) {
		unsigned int lGeneration = lEvolContext->getGeneration();
		lEvolContext->setSelectionIndex(0);

		Beagle_LogBasicM(
		    ioSystem->getLogger(),
		    std::string("Evolving generation ")+uint2str(lGeneration)
		);
		if(lGeneration == 0) {
			for(unsigned int i=lEvolContext->getDemeIndex(); i<ioVivarium->size(); i++) {
				lEvolContext->setDemeIndex(i);
				lEvolContext->setDemeHandle((*ioVivarium)[i]);

				Beagle_LogInfoM(
				    ioSystem->getLogger(),
				    std::string("Applying bootstrap operators to the ")+uint2ordinal(i+1)+
				    std::string(" deme")
				);
				for(unsigned int j=0; j<mBootStrapSet.size(); j++) {
					Beagle_LogDetailedM(
					    ioSystem->getLogger(),
					    std::string("Applying '")+mBootStrapSet[j]->getName()+std::string("'")
					);
					mBootStrapSet[j]->operate(*(*ioVivarium)[i], *lEvolContext);
					// Check if config changed, log Evolver if it has.
					if(mConfigChanged) {
						Beagle_LogBasicM(
						    ioSystem->getLogger(),
						    "Evolver configuration changed after applying '" <<
						    mBootStrapSet[j]->getName() << "' operator from bootstrap set"
						);
						Beagle_LogDetailedM(ioSystem->getLogger(), (*this));
						mConfigChanged = false;
					}
				}
				if(lEvolContext->getContinueFlag() == false) break;
				if(i != lEvolContext->getDemeIndex()) break;
				if(lGeneration != lEvolContext->getGeneration()) break;
				if(i == (ioVivarium->size()-1)) {
					lEvolContext->setGeneration(lGeneration+1);
					lEvolContext->setDemeIndex(0);
				}
			}
		} else {
			for(unsigned int i=lEvolContext->getDemeIndex(); i<ioVivarium->size(); i++) {
				lEvolContext->setDemeIndex(i);
				lEvolContext->setDemeHandle((*ioVivarium)[i]);
				Beagle_LogInfoM(
				    ioSystem->getLogger(),
				    std::string("Applying main-loop operators to the ")+uint2ordinal(i+1)+
				    std::string(" deme")
				);
				for(unsigned int j=0; j<mMainLoopSet.size(); j++) {
					Beagle_LogDetailedM(
					    ioSystem->getLogger(),
					    std::string("Applying '")+mMainLoopSet[j]->getName()+std::string("'")
					);
					mMainLoopSet[j]->operate(*(*ioVivarium)[i], *lEvolContext);

					// Check if config changed, log Evolver if it has.
					if (mConfigChanged) {
						Beagle_LogBasicM(
						    ioSystem->getLogger(),
						    "Evolver configuration changed after applying '" <<
						    mMainLoopSet[j]->getName() << "' operator from main-loop set"
						);
						Beagle_LogDetailedM(ioSystem->getLogger(), (*this));
						mConfigChanged = false;
					}
				}
				if(lEvolContext->getContinueFlag() == false) break;
				if(i != lEvolContext->getDemeIndex()) break;
				if(lGeneration != lEvolContext->getGeneration()) break;
				if(i == (ioVivarium->size()-1)) {
					lEvolContext->setGeneration(lGeneration+1);
					lEvolContext->setDemeIndex(0);
				}
			}
		}
	}

	// Log hall-of-fames of the population.
	for(unsigned int i=0; i<ioVivarium->size(); ++i) {
		HallOfFame::Handle lDemeHoF = (*ioVivarium)[i]->getHallOfFame();
		if((lDemeHoF!=NULL) && (lDemeHoF->size()>0)) {
			Beagle_LogInfoM(ioSystem->getLogger(), "Logging hall-of-fame of the " << uint2ordinal(i+1) << " deme");
			Beagle_LogInfoM(ioSystem->getLogger(), *lDemeHoF);
		}
	}
	HallOfFame::Handle lVivaHoF = ioVivarium->getHallOfFame();
	if((lVivaHoF!=NULL) && (lVivaHoF->size()>0)) {
		Beagle_LogInfoM(
		    ioSystem->getLogger(),
		    "Logging hall-of-fame of the vivarium"
		);
		Beagle_LogInfoM(ioSystem->getLogger(), *lVivaHoF);
	}

	// Log date, time, and ending message.
	ioSystem->getLogger().logCurrentTime(Logger::eBasic);
	Beagle_LogBasicM(
	    ioSystem->getLogger(),
	    (lEvolContext->isTerminationSuccessful())?("End of (successful) evolution"):("End of (unsuccessful) evolution")
	);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the evolver, its operators and the system.
 *  \param ioSystem Handle to the system of the evolution.
 *  \param inArgc Number of elements on the command-line.
 *  \param inArgv Element on the command-line.
 */
void Evolver::initialize(System::Handle ioSystem, int inArgc, char** inArgv)
{
	Beagle_StackTraceBeginM();

	// replace system components by factory concepts
	ioSystem->replaceComponentsByConcepts();

	// register system component parameters
	ioSystem->registerComponentParams();

	// Configure evolver with default evolutionary algorithm
	configureDefaultAlgorithm(*ioSystem);

	// read default configuration file
	try {
		readFromFile("beagle.conf", *ioSystem);
	} catch(const Exception&) {}

	// register evolver parameters
	registerParams(*ioSystem);

	// read default configuration file
	try {
		ioSystem->readFromFile("beagle.conf");
	} catch(const Exception&) {}

	// read configuration file
	parseCommandLine(*ioSystem, inArgc, inArgv);

	// initialize system components
	ioSystem->initComponents();

	// initialize evolver operators
	initOperators(*ioSystem);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the evolver, its operators and the system.
 *  \param ioSystem Handle for the evolution system.
 *  \param inFilename Name of configuration file.
 */
void Evolver::initialize(System::Handle ioSystem, const std::string& inFilename)
{
	Beagle_StackTraceBeginM();

	// replace system components by factory concepts
	ioSystem->replaceComponentsByConcepts();

	// register system component parameters
	ioSystem->registerComponentParams();

	// Configure evolver with default evolutionary algorithm
	configureDefaultAlgorithm(*ioSystem);

	readFromFile(inFilename, *ioSystem);

	// register evolver parameters
	registerParams(*ioSystem);

	// Reading specified configuration file
	ioSystem->readFromFile(inFilename);

	// initialize system components
	ioSystem->initComponents();

	// initialize evolver operators
	initOperators(*ioSystem);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the operators of this evolver.
 *  \param ioSystem System to use for initialization.
 */
void Evolver::initOperators(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// initializing the bootstrap operator set
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "Initializing the bootstrap operator set"
	);
	for(unsigned int i = 0; i < mBootStrapSet.size(); i++) {
		if(mBootStrapSet[i]->isInitialized() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Initializing '") + mBootStrapSet[i]->getName() + "'"
			);
			mBootStrapSet[i]->init(ioSystem);
			mBootStrapSet[i]->setInitializedFlag(true);
		}
	}

	// initializing the main-loop operator set
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "Initializing the main-loop operator set"
	);
	for(unsigned int i = 0; i < mMainLoopSet.size(); i++) {
		if(mMainLoopSet[i]->isInitialized() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Initializing '") + mMainLoopSet[i]->getName() + "'"
			);
			mMainLoopSet[i]->init(ioSystem);
			mMainLoopSet[i]->setInitializedFlag(true);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Log welcome messages for the start of an evolutionary run.
 *  \param ioSystem Evolutionary system.
 */
void Evolver::logWelcomeMessages(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle_LogBasicM(
	    ioSystem.getLogger(),
	    "Starting an evolution"
	);

	ioSystem.getLogger().logCurrentTime(Logger::eBasic);
	Beagle_LogDetailedM(ioSystem.getLogger(), ioSystem.getRegister());
	Beagle_LogDetailedM(ioSystem.getLogger(), (*this));
	
	mConfigChanged=false;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Parse the command-line options.
 *  \param ioSystem Evolutionary system.
 *  \param inArgc Number of arguments on the command-line.
 *  \param inArgv Arguments of the command-line.
 *  \throw Exception If something bad occured.
 *
 *  Options have either of the following two formats:
 *  - [-OBparameter=value]
 *  - [-OBparam1=value1,param2=value2, ... ,paramN=valueN]
 *  .
 */
void Evolver::parseCommandLine(System& ioSystem, int inArgc, char** inArgv)
{
	Beagle_StackTraceBeginM();

	// process all command-line arguments
	std::string lCommandLine = inArgv[0];
	for(int i = 1; i < inArgc; ++i) lCommandLine += std::string(" ")+inArgv[i];
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    std::string("Parsing command-line: ")+lCommandLine
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
				if(lTokenizer.getNextToken(lToken)) {
					if(lToken != "=") {
						std::cerr << "Invalid argument format: '" << inArgv[i] << "'" << std::endl;
						showUsage(ioSystem, inArgv[0], std::cerr);
						exit(-1);
					}
					if(lTokenizer.getNextToken(lValue)) {
						if(lValue == "=" || lValue == ",") {
							std::cerr << "Invalid argument format: '" << inArgv[i] << "'" << std::endl;
							showUsage(ioSystem, inArgv[0], std::cerr);
							exit(-1);
						}
					}
					if(lTokenizer.getNextToken(lToken)) {
						if(lToken != ",") {
							std::cerr << "Invalid argument format: '" << inArgv[i] << "'" << std::endl;
							showUsage(ioSystem, inArgv[0], std::cerr);
							exit(-1);
						}
					}
				}
				if (lName == "conf" || lName == "ec.conf.file") {
					if (lName=="ec.conf.file") {
						Beagle_LogBasicM(
						    ioSystem.getLogger(),
						    "The register variable 'ec.conf.file' is DEPRECATED, please use 'conf' instead."
						);
					}
					// read evolver configuration
					readFromFile(lValue, ioSystem);
					// register the parameters of any new operator
					registerOperatorParams(ioSystem);
					// read system configuration
					ioSystem.readFromFile(lValue);
					// register the parameters of any new component
					ioSystem.registerComponentParams();
					// read register configuration only
					ioSystem.getRegister().readFromFile(lValue, ioSystem);
				} else if(lName == "dump") {
					// dump a configuration; rename old file
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
					ioSystem.write(lStreamer);
					lStreamer.closeTag();
					lStream.close();
					exit(0);
				} else if(lName == "evolver") {
					// read evolver configuration
					readFromFile(lValue, ioSystem);
					// register the parameters of any new operator
					registerOperatorParams(ioSystem);
				} else if(lName == "help") {
					// show detailed usage
					showHelp(ioSystem, inArgv[0], std::cerr);
					exit(0);
				} else if((lName == "restart") || (lName == "ms.restart.file")) {
					if (lName=="ms.restart.file") {
						Beagle_LogBasicM(
						    ioSystem.getLogger(),
						    "The register variable 'ms.restart.file' is DEPRECATED, please use 'restart' instead."
						);
					}
					// read milestone configuration
					readMilestone(lValue, ioSystem);
				} else if(lName == "register") {
					// read register configuration only
					ioSystem.getRegister().readFromFile(lValue, ioSystem);
				} else if(lName == "system") {
					// read system configuration only
					ioSystem.readFromFile(lValue);
					// register the parameters of any new component
					ioSystem.registerComponentParams();
				} else if(lName == "usage") {
					// show short usage
					showUsage(ioSystem, inArgv[0], std::cerr);
					exit(0);
				} else {
					// update register value
					if(ioSystem.getRegister().isRegistered(lName) == false) {
						std::cerr << "Keyword '" << lName << "' is not a valid command-line option!" << std::endl;
						showUsage(ioSystem, inArgv[0], std::cerr);
						exit(-1);
					}
					PACC::XML::Node lNode(lValue, PACC::XML::eString);
					ioSystem.getRegister()[lName]->read(PACC::XML::ConstIterator(&lNode));
				}
			}
		} else if(memcmp(inArgv[i], "-EC", 3) == 0) {
			// deprecated -EC option
			std::cerr << "The -EC command-line option prefix is DEPRECATED, please use -OB instead." << std::endl;
			showUsage(ioSystem, inArgv[0], std::cerr);
			exit(-1);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Reading a evolver with method read is undefined. Use readWithSystem instead.
 */
void Evolver::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read", "Evolver", "Evolver");
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a milestone from a file.
 *  \param inFilename Name of file from which the milestone is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void Evolver::readMilestone(const std::string& inFileName, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Reading milestone file
#ifdef BEAGLE_HAVE_LIBZ
	igzstream lStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	if(lStream.good() == false) {
		throw Beagle_RunTimeExceptionM(std::string("Could not open file '")+inFileName+"'");
	}
	Beagle_LogBasicM(ioSystem.getLogger(), "Reading evolver and system configuration from milestone file'" << inFileName << "'");
	PACC::XML::Document lDocument(lStream, inFileName);
	lStream.close();

	// read milestone data
	PACC::XML::ConstFinder lFinderMS(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPosMS = lFinderMS.find("/Beagle");
	const std::string& lGenerationStr = lPosMS->getAttribute("generation");
	if(lGenerationStr.empty() == false) {
		mGeneration = str2int(lGenerationStr);
	}
	const std::string& lDemeIndexStr = lPosMS->getAttribute("deme");
	if(lDemeIndexStr.empty() == false) {
		mDemeIndex = str2int(lDemeIndexStr);
	}
	mVivariumFileName = inFileName;

	// read evolver configuration
	PACC::XML::ConstFinder lFinderEvol(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPosEvol = lFinderEvol.find("/Beagle/Evolver");
	if(!lPosEvol) {
		Beagle_LogBasicM(
		    ioSystem.getLogger(),
		    "WARNING: milestone does not contain any valid evolver"
		);
	} else {
		// read all evolver instances
		readWithSystem(lPosEvol, ioSystem);
		if(lPosEvol = lFinderEvol.findNext()) {
			Beagle_LogBasicM(
			    ioSystem.getLogger(),
			    "WARNING: milestone contains multiple evolvers"
			);
			do {
				readWithSystem(lPosEvol,ioSystem);
			} while(lPosEvol = lFinderEvol.findNext());
		}
	}

	// register the parameters of any new operator
	registerOperatorParams(ioSystem);

	// read system configuration
	PACC::XML::ConstFinder lFinderSys(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPosSys = lFinderSys.find("/Beagle/System");
	if(!lPosSys) {
		Beagle_LogBasicM(
		    ioSystem.getLogger(),
		    "WARNING: milestone does not contain any valid system"
		);
	} else {
		// read all system instances
		ioSystem.read(lPosSys);
		if(lPosSys = lFinderSys.findNext()) {
			Beagle_LogBasicM(ioSystem.getLogger(), "WARNING: milestone contains multiple systems");
			do {
				ioSystem.read(lPosSys);
			} while(lPosSys = lFinderSys.findNext());
		}
	}

	// register the parameters of any new component
	ioSystem.registerComponentParams();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read an evolver configuration from a file.
 *  \param inFilename Name of file from which the evolver is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void Evolver::readFromFile(const std::string& inFileName, System& ioSystem)
{
	Beagle_StackTraceBeginM();

#ifdef BEAGLE_HAVE_LIBZ
	igzstream lStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	if(lStream.good() == false) {
		throw Beagle_RunTimeExceptionM(std::string("Could not open file '")+inFileName+"'");
	}
	Beagle_LogBasicM(
	    ioSystem.getLogger(),
	    "Reading file '" << inFileName << "' for evolver configuration"
	);
	PACC::XML::Document lDocument(lStream, inFileName);
	lStream.close();
	PACC::XML::ConstFinder lFinder(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPos = lFinder.find("/Beagle/Evolver");
	if(!lPos) {
		Beagle_LogBasicM(ioSystem.getLogger(), "WARNING: file does not contain any valid evolver");
	} else {
		// read all evolver instances
		readWithSystem(lPos, ioSystem);
		if(lPos = lFinder.findNext()) {
			Beagle_LogBasicM(ioSystem.getLogger(), "WARNING: file contains multiple evolvers");
			do {
				read(lPos);
			} while(lPos = lFinder.findNext());
		}
	}

	Beagle_StackTraceEndM();
}



/*!
 *  \brief Read a vivarium from a file.
 *  \param inFilename Name of file from which the vivarium is read.
 *  \param outVivarium Vivarium read.
 *  \param ioContext Evolutionary context.
 *  \throw IOException If a reading error occurs.
 */
void Evolver::readVivarium(const std::string& inFileName, Vivarium& outVivarium, Context& ioContext)
{
	Beagle_StackTraceBeginM();

#ifdef BEAGLE_HAVE_LIBZ
	igzstream lStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	if(lStream.good() == false) {
		throw Beagle_RunTimeExceptionM(std::string("Could not open file '")+inFileName+"'");
	}
	Beagle_LogBasicM(
	    ioContext.getSystem().getLogger(),
	    "Reading vivarium from milestone file '" << inFileName << "'"
	);
	PACC::XML::Document lDocument(lStream, inFileName);
	lStream.close();
	PACC::XML::ConstFinder lFinder(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPos = lFinder.find("/Beagle/Vivarium");
	if(!lPos) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "WARNING: file does not contain any valid vivarium"
		);
	} else {
		// read all evolver instances
		outVivarium.readWithContext(lPos, ioContext);
		if(lPos = lFinder.findNext()) {
			Beagle_LogBasicM(
			    ioContext.getSystem().getLogger(),
			    "WARNING: file contains multiple vivarium"
			);
			do {
				outVivarium.readWithContext(lPos, ioContext);
			} while(lPos = lFinder.findNext());
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read an evolver from a XML subtree.
 *  \param inIter XML iterator from which the evolver is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void Evolver::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Evolver"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Evolver> expected!");

	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			if(lChild->getValue() == "BootStrapSet") {
				mBootStrapSet.clear();
				for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
					if(lChild2->getType() == PACC::XML::eData) {
						std::string lOpName = lChild2->getValue();
						Operator::Alloc::Handle lOpAlloc =
						    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
						if(lOpAlloc == NULL) {
							std::ostringstream lOSS;
							lOSS << "Operator named '" << lOpName << "' is unknown ";
							lOSS << "to the object factory!";
							throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
						}
						Operator::Handle lOp = castHandleT<Operator>(lOpAlloc->allocate());
						lOp->setName(lOpName);
						mBootStrapSet.push_back(lOp);
						lOp->readWithSystem(lChild2, ioSystem);
						mConfigChanged = true;
					}
				}
			} else if(lChild->getValue() == "MainLoopSet") {
				mMainLoopSet.clear();
				for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
					if(lChild2->getType() == PACC::XML::eData) {
						std::string lOpName = lChild2->getValue();
						Operator::Alloc::Handle lOpAlloc =
						    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
						if(lOpAlloc == NULL) {
							std::ostringstream lOSS;
							lOSS << "Operator named '" << lOpName << "' is unknown ";
							lOSS << "to the object factory!";
							throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
						}
						Operator::Handle lOp = castHandleT<Operator>(lOpAlloc->allocate());
						lOp->setName(lOpName);
						mMainLoopSet.push_back(lOp);
						lOp->readWithSystem(lChild2, ioSystem);
						mConfigChanged = true;
					}
				}
			}
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the bootstrap and main-loop operator set parameters.
 *  \param ioSystem Evolutionary system.
 */
void Evolver::registerOperatorParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// registering the bootstrap operator set parameters
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "Registering the evolver bootstrap operator set parameters"
	);
	for(unsigned int i = 0; i < mBootStrapSet.size(); i++) {
		if(mBootStrapSet[i]->hasRegisteredParams() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    "Registering '" << mBootStrapSet[i]->getName() << "' parameters"
			);
			mBootStrapSet[i]->registerParams(ioSystem);
			mBootStrapSet[i]->setRegisteredFlag(true);
		}
	}

	// registering the main-loop operator set parameters
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "Registering the evolver main-loop operator set parameters"
	);
	for(unsigned int i = 0; i < mMainLoopSet.size(); i++) {
		if(mMainLoopSet[i]->hasRegisteredParams() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    "Registering '" << mMainLoopSet[i]->getName() << "' parameters"
			);
			mMainLoopSet[i]->registerParams(ioSystem);
			mMainLoopSet[i]->setRegisteredFlag(true);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this evolver, and those of its operators.
 *  \param ioSystem Evolutionary system.
 *
 *  This method registers the following command-line options:
 *  - ec.pop.size
 *  - evolver
 *  - restart
 *  .
 *  This method also calls Operator::registerParams for all bootstrap and main-loop
 *  operators.
 */
void Evolver::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// register evolver parameters
	{
		Register::Description lDescription;

		lDescription = Register::Description(
		                   "Vivarium and demes sizes",
		                   "UIntArray",
		                   "100",
		                   "Number of demes and size of each deme of the population. The format of an UIntArray is S1:S2:...:Sn, where Si is the ith value. The size of the UIntArray is the number of demes present in the vivarium, while each value of the vector is the size of the corresponding deme."
		               );
		mPopSize = castHandleT<UIntArray>
		           (ioSystem.getRegister().insertEntry("ec.pop.size", new UIntArray(1,100), lDescription));
	}

	// Add test individual parameter
	{
		Register::Description lDescription;
		lDescription = Register::Description(
		                   "File name containing individual to test",
		                   "String",
		                   "",
		                   "Name of file containing individual to test.  Providing such a file will mean that it is parsed, the individual will be evaluated, its fitness will be displayed, and OpenBeagle will terminate."
		               );
		mTestIndi = castHandleT<String>
		            (ioSystem.getRegister().insertEntry("ec.test.indi", new String(""), lDescription));
	}

	// register bootstrap and main-loop operator set parameters
	registerOperatorParams(ioSystem);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the long program usage.
 *  \param ioSystem Evolutionary system.
 *  \param inProgram Program name.
 *  \param outStream Output stream.
 */
void Evolver::showHelp(System& ioSystem, const std::string& inProgram, std::ostream& outStream) const
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
	outStream << "-OBconf=String         Read configuration file"     << std::endl;
	outStream << "-OBdump=String         Dump configuration file"     << std::endl;
	outStream << "-OBevolver=String      Load evolver from file"      << std::endl;
	outStream << "-OBhelp                Show detailed help"          << std::endl;
	outStream << "-OBregister=String     Load register from file"     << std::endl;
	outStream << "-OBrestart=String      Restart evolution from file" << std::endl;
	outStream << "-OBsystem=String       Load system from file"       << std::endl;
	outStream << "-OBusage               Show program usage"          << std::endl;
	outStream << std::endl;
	outStream << "Specific options for this program:" << std::endl;
	outStream << "---------------------------------"  << std::endl;
	outStream << std::endl;

	ioSystem.getRegister().showHelp(outStream);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the short program usage.
 *  \param ioSystem Evolutionary system.
 *  \param inProgram Program name.
 *  \param outStream Output stream.
 */
void Evolver::showUsage(System& ioSystem, const std::string& inProgram, std::ostream& outStream) const
{
	Beagle_StackTraceBeginM();

	outStream << "usage: " << inProgram << " [-OBparameter=value] ..." << std::endl;
	outStream << "       " << inProgram << " [-OBparam1=value1,param2=value2, ... ,paramN=valueN] ..." << std::endl;
	outStream << std::endl;
	outStream << "Common options:" << std::endl;
	outStream << "--------------" << std::endl;
	outStream << std::endl;
	outStream << "-OBdump=String         Dump configuration file"     << std::endl;
	outStream << "-OBevolver=String      Load evolver from file"      << std::endl;
	outStream << "-OBhelp                Show detailed help"          << std::endl;
	outStream << "-OBregister=String     Load register from file"     << std::endl;
	outStream << "-OBrestart=String      Restart evolution from file" << std::endl;
	outStream << "-OBsystem=String       Load system from file"       << std::endl;
	outStream << "-OBusage               Show program usage"          << std::endl;
	outStream << std::endl;
	outStream << "Specific options for this program:" << std::endl;
	outStream << "---------------------------------"  << std::endl;
	outStream << std::endl;

	ioSystem.getRegister().showUsage(outStream);

	Beagle_StackTraceEndM();
}

/*!
 *  \brief Test the individual provided by 'ec.test.indi'
 */
void Evolver::testIndividual(Vivarium::Handle ioVivarium, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    "Evaluating individual in file '" << mTestIndi->getWrappedValue() << "'"
	);

	// Allocate individual
	const Factory& lFactory = ioContext.getSystem().getFactory();
	Individual::Alloc::Handle lIndiAlloc =
	    castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
	Individual::Handle lIndi = castHandleT<Individual>(lIndiAlloc->allocate());

	// Parse individual
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "Reading and parsing XML from file '" << *mTestIndi << "'"
	);
	std::ifstream lInFile(mTestIndi->getWrappedValue().c_str());
	PACC::XML::Document lXML;
	lXML.parse(lInFile,mTestIndi->getWrappedValue());
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "Reading individual from XML"
	);
	ioContext.setIndividualHandle(lIndi);
	lIndi->readWithContext(lXML.getFirstDataTag(), ioContext);

	// Evaluate individual
	EvaluationOp::Alloc::Handle lEvalOpAlloc =
	    castHandleT<EvaluationOp::Alloc>(ioContext.getSystem().getFactory().getConceptAllocator("EvaluationOp"));
	EvaluationOp::Handle lEvalOp = castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	if(lEvalOp==NULL)
		throw Beagle_RunTimeExceptionM("Individual cannot be evaluated because no evaluation operator has been specified.  Consider your call to Evolver's constructor.");
	lEvalOp->registerParams(ioContext.getSystem());
	lEvalOp->init(ioContext.getSystem());
	lEvalOp->test(lIndi, ioContext.getSystemHandle());

	Beagle_StackTraceEndM();
}



/*!
*  \brief Write an evolver into a XML streamer.
 *  \param ioStreamer XML streamer in which the evolver is written.
 *  \param inIndent Whether XML output should be indented.
 */
void Evolver::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag("Evolver", inIndent);
	ioStreamer.openTag("BootStrapSet", inIndent);
	for(unsigned int i=0; i<mBootStrapSet.size(); i++) {
		mBootStrapSet[i]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	ioStreamer.openTag("MainLoopSet", inIndent);
	for(unsigned int j=0; j<mMainLoopSet.size(); j++) {
		mMainLoopSet[j]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	ioStreamer.closeTag();

	Beagle_StackTraceEndM();
}
