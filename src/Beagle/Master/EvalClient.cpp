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
 *  \file   beagle/Distrib/Master/src/EvalClient.cpp
 *  \brief  Source code of class EvalClient.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.6 $
 *  $Date: 2007/09/24 20:33:01 $
 */

#include "beagle/Distrib/Master.hpp"

#ifdef BEAGLE_HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace Beagle;
using namespace Beagle::Distrib;


Master::EvalClient* Master::EvalClient::smSingleton(NULL);

/*!
 *  \brief get the instance of EvalClient
 */
Master::EvalClient* Master::EvalClient::getSingleton()
{
	Beagle_StackTraceBeginM();

	return smSingleton;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Log the error message.
 *  \param inMessage Log the message on the cerr output.
 */
void Master::EvalClient::logError(std::string inMessage)
{
	Beagle_StackTraceBeginM();

	// create the time struct of current time
	time_t lRawTime;
	struct tm * lTimeInfo;
	time ( &lRawTime );
	lTimeInfo = localtime ( &lRawTime );
	std::cerr << std::endl << asctime(lTimeInfo) <<"--> " << inMessage << std::endl;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the instance of EvalClient
 *  \param inSingleton the instance EvalClient
 */
void Master::EvalClient::setSingleton(Master::EvalClient* inSingleton)
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
void Master::EvalClient::main(PACC::SignalHandler::SignalType inSignal)
{
	Beagle_StackTraceBeginM();

	if(inSignal == SignalHandler::eSigInt) {
		logError("SIGINT received.  Informing the server...");
	} else if(inSignal == SignalHandler::eSigTerm) {
		logError("SIGTERM received.  Informing the server...");
	}
	if(getSingleton() != NULL) {
		EvalClient* lClient = getSingleton();
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
 *  \brief Construct the evaluation client that will talk with the server.
 *  \param inServerIP Server's IP address.
 *  \param inServerPort Server's port number.
 *  \param inCompressLevel zlib compression level (0 no compression, 9 full compression).
 *  \param inVersion Client version number.
 */
Master::EvalClient::EvalClient(std::string inServerIP,
                               int inServerPort,
                               unsigned int inCompressLevel,
                               std::string inVersion) :
		DAGS::Client(inServerIP, inServerPort, inCompressLevel, inVersion),
		mEnvManager(new Master::EnvironmentManager),
		mDemeID(0),
		mGeneration(0),
		mDemeStored(false),
		mTimeStart()
{ }


/*!
 *  \brief Send a receive a deme.
 *
 *  A deme here is more a sub-deme since it only has a portion of
 *  the total individuals of a deme.
 *  \throw Beagle_InternalExceptionM Bad application name received
 */
void Master::EvalClient::sendAndReceiveDeme()
{
	Beagle_StackTraceBeginM();

	Beagle::String::Handle lAppName=
	    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

	const Factory& lFactory = mContext->getSystem().getFactory();
	Individual::Alloc::Handle lIndividualAlloc =
		castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));

	std::ostringstream lOSS;
	lOSS << setprecision(10);

	if(mDemeStored) {
		double lTimeEval = mTimeStart.getValue()/(double)mDeme->size();
		std::ostringstream lOSSTime;
		lOSSTime << lTimeEval;

		PACC::XML::Streamer lStreamer(lOSS, 0);
		lStreamer.openTag("SG");
		lStreamer.insertAttribute("id", Beagle::uint2str(mDemeID));
		lStreamer.insertAttribute("app", lAppName->getWrappedValue());
		lStreamer.insertAttribute("gen", Beagle::uint2str(mGeneration));
		lStreamer.insertAttribute("tf", lOSSTime.str());

		// Send evaluated scores
		for(unsigned int i=0; i<mDeme->size(); ++i) {
			lStreamer.openTag("J"); //Job
			lStreamer.insertAttribute("id", Beagle::uint2str(mIndivID[i])); //job id
			lStreamer.openTag("Sc"); //Score
			lStreamer.insertAttribute("eval", "no"); //evaluation
			(*mDeme)[i]->getFitness()->write(lStreamer, false);
			lStreamer.closeTag();
			lStreamer.closeTag();
		}

		lStreamer.closeTag();
	}

	std::string lDemeSerialized = lOSS.str();

	DAGS::SubGroup lSubGroupDAGS;
	std::vector< std::pair<std::string,DAGS::Job> > lJobs;
	std::vector<std::string> lDontParseVector;
	lDontParseVector.push_back("Dt");  //Data
	lDontParseVector.push_back("Sc");  //Score
	lDontParseVector.push_back("Env"); //Environment
	doNotParseXMLTags(lDontParseVector);

	mDemeStored = false;
	while(true) {
		int lErrorNbr = getJobs(lDemeSerialized, lSubGroupDAGS, lJobs, lAppName->getWrappedValue());
		if(lErrorNbr == DAGS::Client::NoError) break;
		if(lErrorNbr < -17) logError(getErrorMessage());
		lDemeSerialized = std::string("");
		PACC::Threading::Thread::sleep(1);
	}

	mTimeStart.reset();

	std::string lObtAppName = lSubGroupDAGS.getAppName();
	if(lObtAppName != lAppName->getWrappedValue()) {
		std::string lMessage = "Bad application name received (desired: '";
		lMessage += lAppName->getWrappedValue();
		lMessage += "', obtained: '";
		lMessage += lObtAppName;
		lMessage += "').";
		throw Beagle_InternalExceptionM(lMessage);
	}

	mDemeID = (unsigned int)(lSubGroupDAGS.getGroupId());
	mGeneration = (unsigned int)lSubGroupDAGS.getGeneration();

	if(mEnvManager != NULL) {
		std::string lEnvironment = lSubGroupDAGS.getEnvironment();
		std::istringstream lEnvIS(lEnvironment);
		PACC::XML::Document lParserEnv;
		lParserEnv.parse(lEnvIS, "environmnent");
		PACC::XML::Iterator lEnvNode(lParserEnv.getFirstRoot());
		if(lEnvNode) {
			mEnvManager->readEnvironment(lEnvNode, *mDeme, *mContext);
		}
	}

	// Get jobs individual
	mIndivID.clear();
	const unsigned int lOldDemeSize = mDeme->size();
	mDeme->resize(lJobs.size());
	for(unsigned int i=lOldDemeSize; i<lJobs.size(); ++i) {
		(*mDeme)[i] = castHandleT<Individual>(lIndividualAlloc->allocate());
	}
	
	for(unsigned int i=0; i<mDeme->size(); ++i) {
		mIndivID.push_back(lJobs[i].second.getId());
		std::ostringstream lOSS;
		lOSS << "<Individual>" << lJobs[i].second.getData() << "</Individual>";
		std::istringstream lGenoIS(lOSS.str());
		PACC::XML::Document lParser;
		lParser.parse(lGenoIS);
		PACC::XML::Iterator lRootNode(lParser.getFirstRoot());
		(*mDeme)[i]->readWithContext(lRootNode, *mContext);
		(*mDeme)[i]->setFitness(NULL);
	}

	mDemeStored = true;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Terminate the evolution by telling the server which individuals to reseed.
 */
void Master::EvalClient::terminate()
{
	try {
		// Reconstruct individuals and do a reseed request of individuals not evaluated,
		// only if they are valid or were processed.
		if(mDemeStored) {
			Beagle::String::Handle lAppName=
			    castHandleT<Beagle::String>(mContext->getSystem().getRegister().getEntry("db.app.name"));

			if(mDeme->empty() == false) {
				std::ostringstream lOSS;
				PACC::XML::Streamer lStreamer(lOSS, 0);
				lStreamer.openTag("SG");
				lStreamer.insertAttribute("id",  Beagle::uint2str(mDemeID));
				lStreamer.insertAttribute("app", lAppName->getWrappedValue());
				lStreamer.insertAttribute("gen", Beagle::uint2str(mGeneration));
				//lStreamer.insertAttribute("gen", Beagle::uint2str(mContext->getGeneration()));
				for(unsigned int i=0; i<mDeme->size(); ++i) {
					lStreamer.openTag("J");
					lStreamer.insertAttribute("id", Beagle::uint2str(mIndivID[i]));
					if(((*mDeme)[i]->getFitness() != NULL) && ((*mDeme)[i]->getFitness()->isValid())) {
						lStreamer.openTag("Sc");
						lStreamer.insertAttribute("eval", "no");
						(*mDeme)[i]->getFitness()->write(lStreamer, false);
						lStreamer.closeTag();
					}
					lStreamer.closeTag();
				}
				lStreamer.closeTag();

				std::string lReseedString = lOSS.str();
				reseed(lReseedString, lAppName->getWrappedValue());
			}
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





