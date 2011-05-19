/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/Distrib/Master/src/EvolClient.cpp
 *  \brief  Source code of class EvolClient.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.7 $
 *  $Date: 2007/09/27 14:58:01 $
 */

#include "beagle/Distrib/Master.hpp"

#ifdef BEAGLE_HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace Beagle;
using namespace Beagle::Distrib;


Master::EvolClient* Master::EvolClient::smSingleton(NULL);


/*!
 *  \brief get the instance of EvolClient
 */
Master::EvolClient* Master::EvolClient::getSingleton()
{
	Beagle_StackTraceBeginM();

	return smSingleton;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Log the error message.
 *  \param inMessage Log the message on the cerr output.
 */
void Master::EvolClient::logError(std::string inMessage)
{
	Beagle_StackTraceBeginM();

	// create the time struct of current time
	time_t lRawTime;
	struct tm * lTimeInfo;
	time(&lRawTime);
	lTimeInfo = localtime(&lRawTime);
	std::cerr << std::endl << asctime(lTimeInfo) << "--> " << inMessage << std::endl;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the instance of EvolClient
 *  \param inSingleton the instance EvolClient
 */
void Master::EvolClient::setSingleton(Master::EvolClient* inSingleton)
{
	Beagle_StackTraceBeginM();

	if((smSingleton==NULL) && (inSingleton!=NULL)) {
		smSingleton = inSingleton;
		// Catch signals SIGINT and SIGTERM
		inSingleton->setCustomAction(SignalHandler::eSigInt);
		pushAction(SignalHandler::eSigInt);
		inSingleton->setCustomAction(SignalHandler::eSigTerm);
		pushAction(SignalHandler::eSigTerm);
	} else if((smSingleton!=NULL) && (inSingleton==NULL)) {
		// Restore signals SIGINT and SIGTERM
		popAction(SignalHandler::eSigInt);
		popAction(SignalHandler::eSigTerm);
		smSingleton = inSingleton;
	} else {
		smSingleton = inSingleton;
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief When the signal inSignal is received, this function is called.
 *  \param inSignal Signal number
 */
void Master::EvolClient::main(PACC::SignalHandler::SignalType inSignal)
{
	Beagle_StackTraceBeginM();

	if(inSignal == SignalHandler::eSigInt) {
		logError("SIGINT received.  Informing the server...");
	} else if(inSignal == SignalHandler::eSigTerm) {
		logError("SIGTERM received.  Informing the server...");
	}
	if(getSingleton() != NULL) {
		EvolClient* lClient = getSingleton();
		lClient->terminate();
		setSingleton(NULL);
	}

	std::cerr << "--> " << "Server as been advised. Terminating the client." << std::endl;

	// small ugly patch.  It seams that some client could not be soft killed by signals.  I have no idea why.
	// see man 2 _exit for more information on bsd/linux.  I'm not sure if Windows had that problem
	// but anyway, it should be fine now.
	_exit(0);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Construct the evoluation client that will talk with the server.
 *  \param inServerIP Server's IP address.
 *  \param inServerPort Server's port number.
 *  \param inCompressLevel zlib compression level (0 no compression, 9 full compression)
 *  \param inVersion client's version
 */
Master::EvolClient::EvolClient(std::string inServerIP,
                               int inServerPort,
                               unsigned int inCompressLevel,
                               std::string inVersion) :
		DAGS::Client(inServerIP, inServerPort, inCompressLevel, inVersion),
		mEnvManager(new Master::EnvironmentManager),
		mDemeID(0),
		mDemeIDEvol(0),
		mGeneration(0),
		mDemeStored(false)
{ }


/*!
 *  \brief  Get the number of demes that are present on the DAGS server.
 *  \return Number of demes.
 *  \throw Beagle_RunTimeExceptionM Problem that need to be addressed.
 */
unsigned int Master::EvolClient::getNumberOfDemes()
{
	Beagle_StackTraceBeginM();

	std::pair<std::string, int> lState = getState();

	if(getErrorMessage().size() > 0) {
		std::ostringstream lMessage;
		lMessage << "Problem when getting evolution state from server: ";
		lMessage << getErrorMessage();
		throw Beagle_RunTimeExceptionM(lMessage.str());
	}
	if(lState.second < -1) {
		std::ostringstream lMessage;
		lMessage << "Number of demes on server is less than 0 (";
		lMessage << lState.second << ")!";
		throw Beagle_RunTimeExceptionM(lMessage.str());
	}

	//should not happen.
	if(lState.second == -1)
		lState.second = 0;

	return (unsigned int)lState.second;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Consult a read-only deme.
 *  \param inDemeID the deme's id.
 *  \throw Beagle_RunTimeExceptionM Problem that need to be addressed.
 */
void Master::EvolClient::consultDeme(unsigned int inDemeID)
{
	Beagle_StackTraceBeginM();

	Beagle_AssertM(mDemeStored == false);
	mDemeID = inDemeID;

	Beagle::String::Handle lAppName=
	    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

	const Factory& lFactory = mContext->getSystem().getFactory();
	Individual::Alloc::Handle lIndividualAlloc =
		castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));

	std::string lDemeSerialized;
	DAGS::Group lGroupDAGS;
	std::vector<std::pair<std::string,DAGS::Job> > lJobs;
	std::vector<std::string> lDontParseVector;
	lDontParseVector.push_back("Dt");
	lDontParseVector.push_back("Sc");
	lDontParseVector.push_back("Env");
	doNotParseXMLTags(lDontParseVector);

	while(true) {
		int lErrorCode = getGroup(mDemeID, lGroupDAGS, lJobs, lAppName->getWrappedValue());
		if(lErrorCode == DAGS::Client::NoError) {
			break;
		}
		else if(lErrorCode == DAGS::Client::NoGroupInDB) {
			throw Beagle_RunTimeExceptionM("No group in server's database, couldn't receive!");
		}
		else if(lErrorCode < -17) {
			logError(getErrorMessage());
		}
		lDemeSerialized = std::string("");
		PACC::Threading::Thread::sleep(1);
	}

	std::string lObtAppName = lGroupDAGS.getAppName();
	if(lObtAppName != lAppName->getWrappedValue()) {
		std::string lMessage = "Bad application name received (desired: '";
		lMessage += lAppName->getWrappedValue();
		lMessage += "', obtained: '";
		lMessage += lObtAppName;
		lMessage += "').";
		throw Beagle_RunTimeExceptionM(lMessage);
	}

	Beagle_AssertM((lGroupDAGS.getDBId()) == (int)mDemeID);
	mGeneration = (unsigned int)lGroupDAGS.getGeneration();

	if(mEnvManager!=NULL) {
		std::string lEnvironment(lGroupDAGS.getEnvironment());
		std::istringstream lEnvIS(lEnvironment);
		PACC::XML::Document lParserEnv;
		lParserEnv.parse(lEnvIS, "environmnent");
		PACC::XML::Iterator lEnvNode(lParserEnv.getFirstRoot());
		mEnvManager->readEnvironment(lEnvNode, *mDeme, *mContext);
	}

	const unsigned int lOldDemeSize = mDeme->size();
	mDeme->resize(lJobs.size());
	for(unsigned int i=lOldDemeSize; i<lJobs.size(); ++i) {
		(*mDeme)[i] = castHandleT<Individual>(lIndividualAlloc->allocate());
	}

	for(unsigned int i=0; i<mDeme->size(); ++i) {
		// Read genotypes
		std::ostringstream lOSS;
		lOSS << "<Individual>" << lJobs[i].second.getData() << "</Individual>";
		std::istringstream lGenoIS(lOSS.str());
		PACC::XML::Document lParserGeno;
		lParserGeno.parse(lGenoIS, "genotypes");
		PACC::XML::Iterator lGenoNode(lParserGeno.getFirstRoot());
		(*mDeme)[i]->readWithContext(lGenoNode, *mContext);

		// Read fitness of individual.
		std::string lFitnessStr = lJobs[i].second.getScore();
		std::istringstream lFitnessIS(lFitnessStr);
		PACC::XML::Document lParserFitness;
		lParserFitness.parse(lFitnessIS, "fitness");
		std::string lFitnessType = lParserFitness.getFirstRoot()->getAttribute("type");
		Fitness::Alloc::Handle lFitnessAlloc = NULL;
		if(lFitnessType.empty()) {
			lFitnessAlloc = castHandleT<Fitness::Alloc>(lFactory.getConceptAllocator("Fitness"));
		}
		else {
			lFitnessAlloc = castHandleT<Fitness::Alloc>(lFactory.getAllocator(lFitnessType));
		}
		Fitness::Handle lFitness = castHandleT<Fitness>(lFitnessAlloc->allocate());
		lFitness->read(lParserFitness.getFirstRoot());
		(*mDeme)[i]->setFitness(lFitness);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Receive any deme that are ready to be evolved.
 *  \throw Beagle_RunTimeExceptionM Problem that need to be addressed.
 */
void Master::EvolClient::receiveDeme()
{
	Beagle_StackTraceBeginM();

	Beagle::String::Handle lAppName=
	    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

	const Factory& lFactory = mContext->getSystem().getFactory();
	Individual::Alloc::Handle lIndividualAlloc =
		castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));

	std::string lDemeSerialized;
	DAGS::Group lGroupDAGS;
	std::vector<std::pair<std::string,DAGS::Job> > lJobs;
	std::vector<std::string> lDontParseVector;
	lDontParseVector.push_back("Dt");
	lDontParseVector.push_back("Sc");
	lDontParseVector.push_back("Env");
	doNotParseXMLTags(lDontParseVector);

	while(true) {
		int lErrorCode = getGroup(lDemeSerialized, lGroupDAGS, lJobs, lAppName->getWrappedValue());
		if(lErrorCode == DAGS::Client::NoError) {
			break;
		}
		else if(lErrorCode == DAGS::Client::NoGroupInDB) {
			throw Beagle_RunTimeExceptionM("No group in server's database, couldn't receive!");
		}
		else if(lErrorCode < -17) {
			logError(getErrorMessage());
		}
		lDemeSerialized = std::string("");
		PACC::Threading::Thread::sleep(1);
	}

	std::string lObtAppName = lGroupDAGS.getAppName();
	if(lObtAppName != lAppName->getWrappedValue()) {
		std::string lMessage = "Bad application name received (desired: '";
		lMessage += lAppName->getWrappedValue();
		lMessage += "', obtained: '";
		lMessage += lObtAppName;
		lMessage += "').";
		throw Beagle_RunTimeExceptionM(lMessage);
	}

	mDemeID     = (unsigned int)(lGroupDAGS.getDBId());
	mGeneration = (unsigned int)lGroupDAGS.getGeneration();

	if(mEnvManager!=NULL) {
		std::string lEnvironment(lGroupDAGS.getEnvironment());
		std::istringstream lEnvIS(lEnvironment);
		PACC::XML::Document lParserEnv;
		lParserEnv.parse(lEnvIS, "environmnent");
		PACC::XML::Iterator lEnvNode(lParserEnv.getFirstRoot());
		mEnvManager->readEnvironment(lEnvNode, *mDeme, *mContext);
	}

	const unsigned int lOldDemeSize = mDeme->size();
	mDeme->resize(lJobs.size());
	for(unsigned int i=lOldDemeSize; i<lJobs.size(); ++i) {
		(*mDeme)[i] = castHandleT<Individual>(lIndividualAlloc->allocate());
	}

	for(unsigned int i=0; i<mDeme->size(); ++i) {
		// Read genotypes
		std::ostringstream lOSS;
		lOSS << "<Individual>" << lJobs[i].second.getData() << "</Individual>";
		std::istringstream lGenoIS(lOSS.str());
		PACC::XML::Document lParserGeno;
		lParserGeno.parse(lGenoIS, "genotypes");
		PACC::XML::Iterator lGenoNode(lParserGeno.getFirstRoot());
		(*mDeme)[i]->readWithContext(lGenoNode, *mContext);

		// Read fitness of individual.
		std::string lFitnessStr = lJobs[i].second.getScore();
		std::istringstream lFitnessIS(lFitnessStr);
		PACC::XML::Document lParserFitness;
		lParserFitness.parse(lFitnessIS, "fitness");
		std::string lFitnessType = lParserFitness.getFirstRoot()->getAttribute("type");
		Fitness::Alloc::Handle lFitnessAlloc = NULL;
		if(lFitnessType.empty()) {
			lFitnessAlloc = castHandleT<Fitness::Alloc>(lFactory.getConceptAllocator("Fitness"));
		}
		else {
			lFitnessAlloc = castHandleT<Fitness::Alloc>(lFactory.getAllocator(lFitnessType));
		}
		Fitness::Handle lFitness = castHandleT<Fitness>(lFitnessAlloc->allocate());
		lFitness->read(lParserFitness.getFirstRoot());
		(*mDeme)[i]->setFitness(lFitness);
	}

	mDemeStored = true;    // A deme is currently stored

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Send a deme and receive another one that is ready to be evolved.
 *  \throw Beagle_RunTimeExceptionM Problem that need to be addressed.
 */
void Master::EvolClient::sendAndReceiveDeme()
{
	Beagle_StackTraceBeginM();

	Beagle::String::Handle lAppName=
	    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

	const Factory& lFactory = mContext->getSystem().getFactory();
	Individual::Alloc::Handle lIndividualAlloc =
		castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));

	std::ostringstream lOSS;
	lOSS << setprecision(10);
	PACC::XML::Streamer lStreamer(lOSS, 0);
	lStreamer.openTag("G"); //group
	lStreamer.insertAttribute("id",  Beagle::uint2str(mDemeID));
	lStreamer.insertAttribute("app", lAppName->getWrappedValue());
	lStreamer.insertAttribute("gen", Beagle::uint2str(mGeneration));

	lStreamer.openTag("Env"); //environment
	if(mEnvManager!=NULL)
		mEnvManager->writeEnvironment(lStreamer, *mDeme, *mContext);
	lStreamer.closeTag(); //close Environment

	// construct the individuals to send to the server
	for(unsigned int i=0; i<mDeme->size(); ++i) {
		lStreamer.openTag("J"); //job
		lStreamer.insertAttribute("id", Beagle::uint2str(i)); //job's id

		// Write Score
		lStreamer.openTag("Sc"); //score
		if(((*mDeme)[i]->getFitness() != NULL) && ((*mDeme)[i]->getFitness()->isValid())) {
			lStreamer.insertAttribute("eval", "no");
			(*mDeme)[i]->getFitness()->write(lStreamer, false);
		} else
			lStreamer.insertAttribute("eval", "yes");
		lStreamer.closeTag(); //close score

		// Write Data
		lStreamer.openTag("Dt"); //data
		for(unsigned int j=0; j<(*mDeme)[i]->size(); ++j) {
			(*(*mDeme)[i])[j]->write(lStreamer, false);
		}
		lStreamer.closeTag(); //close Data

		// Close job
		lStreamer.closeTag();
	}

	// Close Group
	lStreamer.closeTag();
	std::vector<std::string> lDontParseVector;
	lDontParseVector.push_back("Dt"); //data
	lDontParseVector.push_back("Sc"); //score
	lDontParseVector.push_back("Env"); //environment
	doNotParseXMLTags(lDontParseVector);

	std::string lStringToSend(lOSS.str());

	mDemeStored = false;
	mDeme->clear();

	DAGS::Group lGroupDAGS;
	std::vector<std::pair<std::string,DAGS::Job> > lJobs;

	while(true) {
		int lErrorCode = getGroup(lStringToSend, lGroupDAGS, lJobs, lAppName->getWrappedValue());
		if(lErrorCode == DAGS::Client::NoError) {
			break;
		}
		else if(lErrorCode == DAGS::Client::NoGroupInDB) {
			throw Beagle_RunTimeExceptionM("No group in server's database, couldn't receive!");
		}
		else if(lErrorCode == DAGS::Client::GetGroupError) {
			lStringToSend = std::string("");
		}
		else if(lErrorCode < -17) {
			logError(getErrorMessage());
		}
		PACC::Threading::Thread::sleep(1);
	}
	lStringToSend = std::string("");

	std::string lObtAppName = lGroupDAGS.getAppName();
	if(lObtAppName != lAppName->getWrappedValue()) {
		std::string lMessage = "Bad application name received (desired: '";
		lMessage += lAppName->getWrappedValue();
		lMessage += "', obtained: '";
		lMessage += lObtAppName;
		lMessage += "').";
		throw Beagle_RunTimeExceptionM(lMessage);
	}

	mDemeID     = (unsigned int)(lGroupDAGS.getDBId());
	mGeneration = (unsigned int)lGroupDAGS.getGeneration();

	if(mEnvManager!=NULL) {
		std::string lEnvironment(lGroupDAGS.getEnvironment());
		std::istringstream lEnvIS(lEnvironment);
		PACC::XML::Document lParserEnv;
		lParserEnv.parse(lEnvIS, "environmnent");
		PACC::XML::Iterator lEnvNode(lParserEnv.getFirstRoot());
		mEnvManager->readEnvironment(lEnvNode, *mDeme, *mContext);
	}

	const unsigned int lOldDemeSize = mDeme->size();
	mDeme->resize(lJobs.size());
	for(unsigned int i=lOldDemeSize; i<lJobs.size(); ++i) {
		(*mDeme)[i] = castHandleT<Individual>(lIndividualAlloc->allocate());
	}

	for(unsigned int i=0; i<mDeme->size(); ++i) {
		// Read genotypes
		std::ostringstream lOSS;
		lOSS << "<Individual>" << lJobs[i].second.getData() << "</Individual>";
		std::istringstream lGenoIS(lOSS.str());
		PACC::XML::Document lParserGeno;
		lParserGeno.parse(lGenoIS, "genotypes");
		PACC::XML::Iterator lGenoNode(lParserGeno.getFirstRoot());
		(*mDeme)[i]->readWithContext(lGenoNode, *mContext);

		// Read fitness of individual.
		std::string lFitnessStr = lJobs[i].second.getScore();
		std::istringstream lFitnessIS(lFitnessStr);
		PACC::XML::Document lParserFitness;
		lParserFitness.parse(lFitnessIS, "fitness");
		if((*mDeme)[i]->getFitness() == NULL) {
			Fitness::Alloc::Handle lFitnessAlloc =
				castHandleT<Fitness::Alloc>(lFactory.getConceptAllocator("Fitness"));
			(*mDeme)[i]->setFitness(castHandleT<Fitness>(lFitnessAlloc->allocate()));
		}
		PACC::XML::Iterator lFitnessNode(lParserFitness.getFirstRoot());
		(*mDeme)[i]->getFitness()->read(lFitnessNode);
		(*mDeme)[i]->getFitness()->setValid();
	}

	mDemeStored = true;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Send a deme to the DAGS server.
 *  \param inDemeType Specify the deme's type (NewGroup, ExistingGroup)
 */
void Master::EvolClient::sendDeme(DAGS::Client::eGroupType inDemeType)
{
	Beagle_StackTraceBeginM();

	Beagle::String::Handle lAppName=
	    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

	std::ostringstream lOSS;
	lOSS << setprecision(10);
	PACC::XML::Streamer lStreamer(lOSS, 0);
	lStreamer.openTag("G");
	lStreamer.insertAttribute("id",  Beagle::uint2str(mDemeID));
	lStreamer.insertAttribute("app", lAppName->getWrappedValue());
	lStreamer.insertAttribute("gen", Beagle::uint2str(mGeneration));
	lStreamer.openTag("Env");
	if(mEnvManager!=NULL)
		mEnvManager->writeEnvironment(lStreamer, *mDeme, *mContext);
	lStreamer.closeTag();

	// construct the individuals to send to the server
	for(unsigned int i=0; i<mDeme->size(); ++i) {
		lStreamer.openTag("J");
		lStreamer.insertAttribute("id", Beagle::uint2str(i));

		// Write Score
		lStreamer.openTag("Sc"); //score
		if(((*mDeme)[i]->getFitness() != NULL) && ((*mDeme)[i]->getFitness()->isValid())) {
			lStreamer.insertAttribute("eval", "no");
			(*mDeme)[i]->getFitness()->write(lStreamer, false);
		} else
			lStreamer.insertAttribute("eval", "yes");
		lStreamer.closeTag(); //close score

		// Write Data
		lStreamer.openTag("Dt"); //data
		for(unsigned int j=0; j<(*mDeme)[i]->size(); ++j) {
			(*(*mDeme)[i])[j]->write(lStreamer, false);
		}
		lStreamer.closeTag(); //close Data

		// Close job
		lStreamer.closeTag();
	}

	// Close Group
	lStreamer.closeTag();
	std::vector<std::string> lDontParseVector;
	lDontParseVector.push_back("Dt"); //data
	lDontParseVector.push_back("Sc"); //score
	lDontParseVector.push_back("Env"); //environment
	doNotParseXMLTags(lDontParseVector);

	std::string lStringToSend(lOSS.str());
	mDemeStored = false;
	int lErrorCode = setGroup(lStringToSend, lAppName->getWrappedValue(), inDemeType);
	if(lErrorCode < -17)
		logError(getErrorMessage());

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Tell the server that the evolClient is quiting and it did not evolve the received deme.
 */
void Master::EvolClient::terminate()
{
	try {
		if(mDemeStored) {
			Beagle::String::Handle lAppName=
			    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

			std::ostringstream lOSS;
			lOSS << setprecision(10);
			PACC::XML::Streamer lStreamer(lOSS, 0);

			// just send the Environment to DAGS server
			lStreamer.openTag("G");
			lStreamer.insertAttribute("id",  Beagle::uint2str(mDemeID));
			lStreamer.insertAttribute("app", lAppName->getWrappedValue());
			lStreamer.insertAttribute("gen", Beagle::uint2str(mGeneration));
			lStreamer.openTag("Env");
			lStreamer.closeTag();
			lStreamer.closeTag();

			// Don't send any individuals/DAGS::jobs to the server since it already has them
			// and no evolution took place
			std::string lStringToSend(lOSS.str());
			mDemeStored = false;
			int lErrorCode = setGroup(lStringToSend, lAppName->getWrappedValue(), DAGS::Client::Quit);
			if(lErrorCode < -17)
				logError(getErrorMessage());
		}
	}//try
	catch(Beagle::Exception& inException) {
		cerr << "Exception catched with the signal handler: '" << inException.what()
		<< "'.  The server was not informed!" << endl << flush;
		exit(1);
	} catch(std::exception& inException) {
		cerr << "Standard exception catched with the signal handler: '" << inException.what()
		<< "'.  The server was not informed!" << endl << flush;
		exit(1);
	} catch(...) {
		cerr << "Unknown exception catched with the signal handler."
		<< " The server was not informed!" << endl << flush;
		exit(1);
	}
}


/*!
 *  \brief Update the environment of the deme on the DAGS server.
 */
void Master::EvolClient::updateEnv()
{
	Beagle_StackTraceBeginM();

	Beagle::String::Handle lAppName=
	    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

	std::ostringstream lOSS;
	lOSS << setprecision(10);
	PACC::XML::Streamer lStreamer(lOSS, 0);

	// just send the Environment to DAGS server
	lStreamer.openTag("G");
	lStreamer.insertAttribute("id",  Beagle::uint2str(mDemeID));
	lStreamer.insertAttribute("app", lAppName->getWrappedValue());
	lStreamer.insertAttribute("gen", Beagle::uint2str(mGeneration));
	lStreamer.openTag("Env");
	if(mEnvManager!=NULL)
		mEnvManager->writeEnvironment(lStreamer, *mDeme, *mContext);
	lStreamer.closeTag();
	lStreamer.closeTag();

	// Don't send any individuals/DAGS::jobs to the server since it already has them
	// and no evolution took place
	std::string lStringToSend(lOSS.str());
	updateEnvironment(lStringToSend);

	Beagle_StackTraceEndM();
}




