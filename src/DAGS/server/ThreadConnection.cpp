/*
 *  ThreadConnection.cpp
 *  Copyright (C) 2003-2004 by Marc Dubreuil
 *  Copyright (C) 2001-2002 by Marc Dubreuil, Jacques Labrie, Helene Torresan and Frederic Jean
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


#include "ThreadConnection.hpp"
#include "dags/Request.hpp"
#include "dags/Job.hpp"
#include "dags/Group.hpp"
#include "dags/Subgroup.hpp"
#include "dags/Distribution.hpp"
#include "dags/Monitor.hpp"
#include "PACC/Socket/Cafe.hpp"
#include "PACC/Socket/Exception.hpp"
#include "PACC/Threading/Exception.hpp"
#include "PACC/XML/Streamer.hpp"
#include "PACC/Util/SignalHandler.hpp"

#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>

using namespace std;
using namespace PACC;

DAGS::ThreadConnection::ThreadConnection(int inSocketDescriptor,
        int inThreadIdNumber,
        DAGS::CommonServerData inCSD,
        int inVerbose)
		: mThreadIdNum(inThreadIdNumber),
		mServerSocketComm(inSocketDescriptor),
		mClientId(-1),
		mNumberOfGroups(-1),
		mNewClient(false),
		mGroupId(-1),
		mSubGroupId(-1),
		mGroupGeneration(-1),
		mRequestError(0),
		mCSD(inCSD),
		mSocketDescriptor(inSocketDescriptor),
		mDatabase(inCSD.getDatabaseName()),
		mVerbose(inVerbose),
		mOSSLog(""),
		mStreamerLog(mOSSLog),
		mThreadTime(0.0),
		mParseTime(0.0),
		mSetTime(0.0),
		mGetTime(0.0),
		mReceiveTime(0.0),
		mSendTime(0.0),
		mDBTime(0.0),
		mGetGroupId(false),
		mDiffJobsGroup(0),
		mCompression(0)
{
	mClientIpAddress   = getIpAddress();
	mClientConnectTime = time(0);
	if(mVerbose > 2) {
		mStreamerLog.openTag("Client");
		mStreamerLog.insertAttribute("ip", mClientIpAddress);
	}
}


DAGS::ThreadConnection::~ThreadConnection()
{}


void DAGS::ThreadConnection::start()
{
//  PACC::Threading::Thread::sleep(10);
	std::string lXMLStringFromClient, lXMLStringToClient;
	DAGS::Group lGroup;

	if(mVerbose > 2)
		pushTimeValue();

	//Get XML String from client
	if(getXMLStringFromClient(lXMLStringFromClient) != 1) {
		mServerSocketComm.close();
		if(mVerbose > 2) {
			mThreadTime = popTime();
			setXMLTime();
			mStreamerLog.closeTag();  //close Client tag
			DAGS::CommonServerData::logMessages(mOSSLog.str());
		}
		return;
	}

	//Get the mutex for the Universe
	mUniverseMutex = DAGS::CommonServerData::getUniverseMutex();

	// new client if mClientId == -1 and no stats will be performed on the client.
	// so give it any valid id.
	if(mClientId == -1 && mCSD.getEnableClientStat() == false && mRequestError != NothingToReceive)
		mClientId = 1;

	//CLIENT SEND A GROUP AND REQUEST ANOTHER GROUP
	if(mRequestType == "group") {
		//wait until every score has been sync-ed with the DB before continuing
		while(true) {
			if(DAGS::CommonServerData::getScoreSync()) break;
			PACC::Threading::Thread::sleep(1);
		}

		std::string lReady("READY TO RECEIVE INFO");
		addXMLHeader(lReady);

		try {
			mServerSocketComm.sendMessage(lReady);
		} catch(Socket::Exception inError) {
			std::ostringstream lErrorMessage;
			lErrorMessage << "Can't communicate with the client to give it the error.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		}

		//receive the Group from the client.
		lXMLStringFromClient = "";
		if(getXMLStringFromClient(lXMLStringFromClient, false) != 1) {
			mServerSocketComm.close();
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}

		//no stats about the client of a group request, so give it any so it's not tagged as a new client
		mClientId = 0;

		if(doGroupRequest(lXMLStringFromClient)!=1) {
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}
	}

	//CLIENT SEND A SUBGROUP AND REQUEST ANOTHER SUBGROUP
	else if(mRequestType == "jobs") {
		//Check for error : if it is a new client and it said that it have sent something, there is a problem!!
		//A client cannot send new Jobs to Group by the way of a subgroup.  It can only send a new demme.
		if(mNewClient == true && mRequestError != NothingToSend) {
			std::ostringstream lErrorMessage;
			lErrorMessage << "The client does not have a valid ID and it sent a subgroup of Jobs to be stored.";
			mRequestError = SendGroupInvalidClientId;
			lXMLStringToClient=std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
			addXMLHeader(lXMLStringToClient);
			try {
				mServerSocketComm.sendMessage(lXMLStringToClient);
			} catch(Socket::Exception inError) {
				lErrorMessage.str("");
				lErrorMessage << "Can't communicate with the client to give it the error.";
			}
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			mServerSocketComm.close();
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}

		if(doSubGroupRequest(lXMLStringFromClient) != 1) {
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}
	}

	//CLIENT REQUEST AN ENTIRE UNIVERSE
	else if(mRequestType == "monitor") {
		if(doMonitorRequest()!=1) {
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}
	}
	else if(mRequestType == "reseed") {
		std::string lReady("reseed accepted.");
		addXMLHeader(lReady);
		try {
			mServerSocketComm.sendMessage(lReady);
			mServerSocketComm.close();
		} catch(Socket::Exception inError) {
			std::ostringstream lErrorMessage;
			lErrorMessage << "Can't communicate with the client to give it the error.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			mServerSocketComm.close();
			return;
		}

		//no need to talk with the client anymore, stop connection with it
		if(doReseedRequest(lXMLStringFromClient)!=1) {
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}
	}

	// clean terminate of the process
	else if(mRequestType == "terminate") {
		//wait until every score has been sync-ed with the DB before continuing
		while(true) {
			if(DAGS::CommonServerData::getScoreSync())
				break;
			PACC::Threading::Thread::sleep(1);
		}

		std::string lReady("READY TO RECEIVE INFO");
		addXMLHeader(lReady);

		try {
			mServerSocketComm.sendMessage(lReady);
		} catch(Socket::Exception inError) {
			std::ostringstream lErrorMessage;
			lErrorMessage << "Can't communicate with the client to give it the error.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			return;
		}

		//receive the Group from the client.
		lXMLStringFromClient = "";
		if(getXMLStringFromClient(lXMLStringFromClient, false) != 1) {
			mServerSocketComm.close();
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}

		//no stats about the client of a group request, so give it any so it's not tagged as a new client
		mClientId = 0;

		mXMLUtils.getAttributeGroup(lGroup);

		mAppName = lGroup.getAppName();
		mGroupId = lGroup.getDBId();
		mGroupGeneration  = lGroup.getGeneration();
		mGroupEnvironment = lGroup.getEnvironment();
		mDistributeEnv = lGroup.getDistributeEnv();

		//update the value of the environment to reflect jobs Score + Data
		mUniverseMutex->lock();
		unsigned int lJobsNumber = DAGS::CommonServerData::getCommonGroup(mGroupId).getNbJobs();
		if(mVerbose > 3)
			pushTimeValue();
		DAGS::CommonServerData::setDataVersion(mVersion);
		DAGS::CommonServerData::setAppName(mAppName);
		if(mCSD.getDatabaseName() != "") {
			updateGroupDB(lJobsNumber);
			insertJobsDB(lJobsNumber);
		}
		if(mVerbose > 3)
			mDBTime += popTime();
		//set the group
		lGroup.setTime(0);
		lGroup.setStatus(0);
		lGroup.setCounter(0);
		DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);
		DAGS::CommonServerData::setGroupReady(lGroup.getDBId());
		mUniverseMutex->unlock();
	}

	//a group is quiting, update the time group was last sent so another
	//group request can take it.
	else if(mRequestType == "quit") {
		//wait until every score has been sync-ed with the DB before continuing
		while(true) {
			if(DAGS::CommonServerData::getScoreSync())
				break;
			PACC::Threading::Thread::sleep(1);
		}

		std::string lReady("READY TO RECEIVE INFO");
		addXMLHeader(lReady);

		try {
			mServerSocketComm.sendMessage(lReady);
		} catch(Socket::Exception inError) {
			std::ostringstream lErrorMessage;
			lErrorMessage << "Can't communicate with the client to give it the error.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			return;
		}

		//receive the Group from the client.
		lXMLStringFromClient = "";
		if(getXMLStringFromClient(lXMLStringFromClient, false) != 1) {
			mServerSocketComm.close();
			if(mVerbose > 2) {
				mThreadTime = popTime();
				setXMLTime();
				mStreamerLog.closeTag();  //close Client tag
				DAGS::CommonServerData::logMessages(mOSSLog.str());
			}
			return;
		}

		//no stats about the client of a group request, so give it any so it's not tagged as a new client
		mClientId = 0;

		mXMLUtils.getAttributeGroup(lGroup);
		if(lGroup.getDBId() >= 0) {
			DAGS::Group lGroupTmp(DAGS::CommonServerData::getCommonGroup(lGroup.getDBId()));
			//if the group received is of the same generation than the one in the database
			if(lGroupTmp.getGeneration() == lGroup.getGeneration() &&
			        lGroupTmp.getStatus() == 1) {
				int lCounter = lGroupTmp.getCounter();
				if(--lCounter == 0) {
					lGroupTmp.setTime(0);
					lGroupTmp.setStatus(0);
					DAGS::CommonServerData::setGroupReady(lGroupTmp.getDBId());
				}
				lGroupTmp.setCounter(--lCounter);
				DAGS::CommonServerData::setCommonGroup(lGroupTmp, lGroupTmp.getDBId());
			} else {
				mServerSocketComm.close();
				return;
			}
		} else {
			mServerSocketComm.close();
			return;
		}
	}
	else if(mRequestType == "state") {
		mAppName = DAGS::CommonServerData::getAppName();
		mNumberOfGroups = DAGS::CommonServerData::getNumberOfGroups();

		std::string lState;
		addXMLHeader(lState);
		try {
			mServerSocketComm.sendMessage(lState);
		} catch(Socket::Exception inError) {
			std::ostringstream lErrorMessage;
			lErrorMessage << "Can't communicate with the client to give it the error.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		}
	}
	else if(mRequestType == "setEnv") {
		//no stats about the client of a group request, so give it any so it's not tagged as a new client
		mClientId = 0;

		mXMLUtils.getAttributeGroup(lGroup);

		mAppName = lGroup.getAppName();
		mGroupId = lGroup.getDBId();
		mGroupGeneration  = lGroup.getGeneration();
		mGroupEnvironment = lGroup.getEnvironment();
		mDistributeEnv = lGroup.getDistributeEnv();

		DAGS::Group lGroupData(DAGS::CommonServerData::getCommonGroup(mGroupId));

		if(mGroupGeneration == lGroupData.getGeneration()) {
			//update the value of the environment to reflect jobs Score + Data
			mUniverseMutex->lock();

			unsigned int lJobsNumber = DAGS::CommonServerData::getCommonGroup(mGroupId).getNbJobs();
			DAGS::CommonServerData::setDataVersion(mVersion);
			DAGS::CommonServerData::setAppName(mAppName);
			if(mCSD.getDatabaseName() != "") {
				updateGroupDB(lJobsNumber);
				insertJobsDB(lJobsNumber);
			}
			lGroupData.setEnvironment(mGroupEnvironment);
			DAGS::CommonServerData::setCommonGroup(lGroupData, lGroupData.getDBId());

			mUniverseMutex->unlock();

			lXMLStringToClient = "Environment set in the database";
			try {
				mServerSocketComm.sendMessage(lXMLStringToClient);
			} catch(Socket::Exception inError) {
				std::ostringstream lErrorMessage;
				lErrorMessage.str("");
				lErrorMessage << "Can't communicate with the client to give it the error.";
			}
		} else {
			std::ostringstream lErrorMessage;
			lErrorMessage.str("");
			lErrorMessage << "Error while trying to set the environment.  The generation is not the same";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			lXMLStringToClient = "Error while trying to set the environment.  The generation is not the same";
			try {
				mServerSocketComm.sendMessage(lXMLStringToClient);
			} catch(Socket::Exception inError) {
				lErrorMessage.str("");
				lErrorMessage << "Can't communicate with the client to give it the error.";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			}
		}
	}

	//CLIENT ASK AN INVALID REQUEST
	else {
		std::ostringstream lErrorMessage;
		lErrorMessage << "invalid request :" << mRequestType;
		mRequestError = InvalidRequest;
		lXMLStringToClient = std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage.str("");
			lErrorMessage << "Can't communicate with the client to give it the error.";
		}
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::start()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		mServerSocketComm.close();
		if(mVerbose > 2) {
			mThreadTime = popTime();
			setXMLTime();
			mStreamerLog.closeTag();  //close Client tag
			DAGS::CommonServerData::logMessages(mOSSLog.str());
		}
		return;
	}


	//If all tasks are done and no error happened, then the thread terminate correctly
	//and the request from the client is ok.
	mServerSocketComm.close();

	if(mVerbose > 2) {
		mThreadTime = popTime();
		setXMLTime();
		mStreamerLog.closeTag();  //close Client tag
		DAGS::CommonServerData::logMessages(mOSSLog.str());
	}
	return;
}


std::string DAGS::ThreadConnection::getIpAddress()
{
	if(mClientIpAddress != "")
		return mClientIpAddress;
	std::string outIPAddress;
	try {
		outIPAddress = mServerSocketComm.getPeerAddress().getIPAddress();
	} catch (Socket::Exception &inError) {
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getIpAddress()", inError.what(), mThreadIdNum);
	}
	return outIPAddress;
}


int DAGS::ThreadConnection::connectDB()
{
	try {
		mDatabase.openConnection();
		DAGS::CommonServerData::lockSQL();
	} catch (std::runtime_error & inError) {
		std::ostringstream lErrorMessage;
		std::string lXMLStringToClient;
		lErrorMessage << "Can't connect to database.";
		lXMLStringToClient = std::string("Can't connect to database.");
		mRequestError = ThreadCannotConnectToDb;
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage.str("");
			lErrorMessage << "Can't communicate with the client to give it the error(Can't connect to database).";
		}
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::connectDB()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		mServerSocketComm.close();
		if(mVerbose > 2)
			DAGS::CommonServerData::logMessages(mOSSLog.str());
		return -1;
	}
	return 1;
}

void DAGS::ThreadConnection::disconnectDB()
{
	DAGS::CommonServerData::unlockSQL();
	mDatabase.closeConnection();
}


void DAGS::ThreadConnection::updateDistributionInfo(std::vector<double>& inStats,
        unsigned int inLoadBalancing)
{
	if(!mNewClient) {
		DAGS::CommonServerData::setClientDistribution(mClientIpAddress,
		        time(0),
		        inStats,
		        mClientId,
		        inLoadBalancing);
	} else {
		if(mVersion != DAGS::CommonServerData::getDataVersion() && DAGS::CommonServerData::getDataVersion() != "") {
			std::ostringstream lOSS;
			lOSS << "Client Version (" << mVersion << ") is different than the expected one ("
			<< DAGS::CommonServerData::getDataVersion() << "). "
			<< "Make sure it is normal.";
			if(mVerbose > 0)
				DAGS::CommonServerData::logConsole(lOSS.str());
			DAGS::CommonServerData::logErrors("Warning", lOSS.str(), mThreadIdNum, mClientIpAddress);
		}
		mClientId = DAGS::CommonServerData::setNewClientDistribution(mClientIpAddress,
		            time(0),
		            inStats,
		            inLoadBalancing);
		if(mVerbose > 2) {
			std::ostringstream lOSS, lErrorMessage;
			lErrorMessage << "New client connected with IP address: " << mClientIpAddress;
			XML::Streamer lStreamer(lOSS);
			lStreamer.openTag("Log");
			lStreamer.insertStringContent(lErrorMessage.str());
			lStreamer.closeTag();
			DAGS::CommonServerData::logMessages(lOSS.str());
		}
	}
}


void DAGS::ThreadConnection::addXMLHeader(std::string &ioXMLString)
{
	std::ostringstream lErrorMessage;

	std::ostringstream lOStringStreamHeader;

	XML::Streamer lStreamer(lOStringStreamHeader, 0);
	lStreamer.openTag("DAGS");                       //DAGS communication
	lStreamer.insertAttribute("id", mClientId);      //client id
	lStreamer.insertAttribute("rt", mRequestType);   //request type
	lStreamer.insertAttribute("err", mRequestError); //error number
	lStreamer.insertAttribute("app", mAppName);      //application name
	if(mNumberOfGroups > -1)
		lStreamer.insertAttribute("gid", mNumberOfGroups);//number of groups in the application name
	lStreamer.insertStringContent(ioXMLString, false);
	lStreamer.closeTag();                       //close DAGS
	ioXMLString = lOStringStreamHeader.str();
}


int DAGS::ThreadConnection::getGroup(std::string &ioXMLGroup)
{
	if(mVerbose > 3)
		pushTimeValue();

	//error message
	ostringstream lErrorMessage;

	//the query sent to the database
	int lGeneration = -1;
	int lCounter = -1;
	std::string lEnvironment;
	//to be sure it is empty
	ioXMLGroup = "";
	DAGS::Group lGroup;

	try {
		//lock the mutex
		mUniverseMutex->lock();
		int lGroupId = 0;

		if(!mGetGroupId) {
			lGroupId = DAGS::CommonServerData::getGroupReady();
			if(lGroupId > -1)
				lGroup = DAGS::CommonServerData::getCommonGroup(lGroupId);
			else {
				lGroup = DAGS::CommonServerData::getAnyGroup(1, true, true, mCSD.getIdealTime());

				// no busy Group, error
				if(lGroup.getDBId() == -1) {
					// no Group in the Universe, please create one
					if(DAGS::CommonServerData::getGroupVector().empty()) {
						//unlock the mutex
						mUniverseMutex->unlock();

						if(mVerbose > 3) {
							mStreamerLog.openTag("Log");
							mStreamerLog.insertStringContent("No Group in Universe");
							mStreamerLog.closeTag();
							mGetTime = popTime();
						}
						return NoGroupInDB;
					}

					//else, return an error for the Group
					//unlock the mutex
					mUniverseMutex->unlock();
					if(mVerbose > 3) {
						mStreamerLog.openTag("Log");
						mStreamerLog.insertStringContent("No Group's ready");
						mStreamerLog.closeTag();
						mGetTime = popTime();
					}
					return GetGroupError;
				}
			}
		}
		//a get group request has been performed with a specific id
		else {
			lGroup = DAGS::CommonServerData::getCommonGroup(mGroupId);
			//can't be send since it is beeing modified
			if(lGroup.getStatus() >= 2) {
				//send a busy message back to the client
				if(mVerbose >= 3) {
					std::ostringstream lOSS;
					lOSS << "Get group asked for specific group(" << mGroupId << ") but this groups is beeing processed.";
					mStreamerLog.openTag("Log");
					mStreamerLog.insertStringContent(lOSS.str());
					mStreamerLog.closeTag();
					if(mVerbose > 3)
						mGetTime = popTime();
				}
				mUniverseMutex->unlock();
				return GetGroupError;
			}
			if(mCSD.getMemoryShortMode() && DAGS::CommonServerData::getIfJobsPresent(mGroupId) == false) {
				//send a busy message back to the client
				if(mVerbose >= 3) {
					std::ostringstream lOSS;
					lOSS << "In Memory Short mode, get group asked for specific group(" << mGroupId << ") but this group is not in memory.";
					mStreamerLog.openTag("Log");
					mStreamerLog.insertStringContent(lOSS.str());
					mStreamerLog.closeTag();
					if(mVerbose > 3)
						mGetTime = popTime();
				}
				mUniverseMutex->unlock();
				return GetGroupError;
			}
		}

		// at least 1 Group is "ready"
		mGroupId     = lGroup.getDBId();
		lGeneration  = lGroup.getGeneration();
		lCounter     = lGroup.getCounter();
		lEnvironment = lGroup.getEnvironment();

		// update the mCSD value of the Group
		if(!mGetGroupId) {
			lGroup.setStatus(1);
			lGroup.setCounter(++lCounter);
			lGroup.setTime(time(0));
			DAGS::CommonServerData::setCommonGroup(lGroup, lGroupId);
		}

		std::string lJobsString;
		//construct the XML string
		std::ostringstream lOSS;
		XML::Streamer lStreamer(lOSS, 0);
		lStreamer.openTag("G");  //Group
		lStreamer.insertAttribute("id", mGroupId); //group's id
		lStreamer.insertAttribute("app", mAppName); //application name
		lStreamer.insertAttribute("gen", lGeneration); //generation
		lStreamer.openTag("Env"); //Environment
		lStreamer.insertStringContent(lEnvironment, false);
		lStreamer.closeTag(); //close Environment

		std::vector<std::pair<std::string, DAGS::Job> >& lJob(DAGS::CommonServerData::getJobs(mGroupId));

		if(!lJob.empty()) {
			for(unsigned int i = 0; i < lJob.size(); ++i) {
				lStreamer.openTag("J"); //Job
				lStreamer.insertAttribute("id", lJob[i].second.getId()); //job id
				lStreamer.openTag("Sc"); //Score
				lStreamer.insertAttribute("eval", "no"); //evaluate
				lStreamer.insertStringContent(lJob[i].second.getScore(), false);
				lStreamer.closeTag(); //close Score
				lStreamer.openTag("Dt"); //Data
				lStreamer.insertStringContent(lJob[i].second.getData(), false);
				lStreamer.closeTag(); //close Data
				lStreamer.closeTag(); //close Job
			}
		}
		//SLOWWWWW, but must be done if short on memory.  For normal case, lJob != empty.
		else {
			if(mVerbose > 3)
				pushTimeValue();
			lStreamer.insertStringContent(getJobsDB(mGroupId), false);
			if(mVerbose > 3)
				mDBTime += popTime();
		}
		//end the XML String with the close tag
		lStreamer.closeTag(); //close Group
		ioXMLGroup = lOSS.str();

		//unlock the database
		mUniverseMutex->unlock();

		// if memory is short, erase previous group (including job's data and score)
		if(mCSD.getMemoryShortMode() && !mGetGroupId)
			DAGS::CommonServerData::eraseAllGroupData();
	} //try

	catch (std::runtime_error & inError) {
		lErrorMessage << inError.what();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getGroup()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		if(mVerbose > 3) {
			mStreamerLog.openTag("Log");
			mStreamerLog.insertStringContent("ERROR CATCHED: See error log file for info.");
			mStreamerLog.closeTag();
			mGetTime = popTime();
		}
		return -1;
	}


	if(mGetGroupId && mVerbose >= 3) {
		std::ostringstream lOSS;
		lOSS << "The client wanted to consult a group (" << mGroupId << "). This group won't be modified.";
		mStreamerLog.openTag("Log");
		mStreamerLog.insertStringContent(lOSS.str());
		mStreamerLog.closeTag();
	}

	if(mVerbose >= 3) {
		std::ostringstream lOSS;
		lOSS << "Sending group's id: " << mGroupId;
		mStreamerLog.openTag("Log");
		mStreamerLog.insertStringContent(lOSS.str());
		mStreamerLog.closeTag();
		if(mVerbose > 3)
			mGetTime = popTime();
	}
	return 1;
}


int DAGS::ThreadConnection::getSubGroup(std::string &ioXMLSubGroup)
{
	if(mVerbose > 3)
		pushTimeValue();

	//error message
	ostringstream lErrorMessage, lXMLSubGroup;

	int lCounter = -1;
	int lNbJobsInSubGroup = -1;
	if(mCSD.getEnableClientStat() == false)
		lNbJobsInSubGroup = mCSD.getNbJobsInSubGroup();
	else {
		if(!mNewClient)
			lNbJobsInSubGroup = DAGS::CommonServerData::getLoadBalancing(mClientId);
		else {
			//std::vector<double> lStats(mCSD.getNumberStatistics(),(double)mCSD.getNbJobsInSubGroup()/(double)mCSD.getIdealTime());
			std::vector<double> lStats(mCSD.getNumberStatistics(),-1);
			updateDistributionInfo(lStats, mCSD.getNbJobsInSubGroup());
			if(mVerbose > 3) {
				std::ostringstream lMessage;
				lMessage << "Client not found. Creating it";
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lMessage.str());
				mStreamerLog.closeTag();
			}
			lNbJobsInSubGroup = mCSD.getNbJobsInSubGroup();
		}
		//lNbJobsInSubGroup = loadBalancing();
	}

	std::string lEnvironment;
	int lGeneration;
	//to be sure it is empty
	ioXMLSubGroup = "";

	try {
		DAGS::Group lGroup;

		//lock the mutex
		mUniverseMutex->lock();

		//verify if the list of Jobs is empty.
		int lGroupId(DAGS::CommonServerData::getAnyGroup(2, true, false).getDBId());

		//a list is present, just send these Jobs
		if(lGroupId != -1) {
			lGroup = DAGS::CommonServerData::getCommonGroup(lGroupId);
			//there's a Group ready to be evaluated, take his description
			mGroupId     = lGroupId;
			lEnvironment = lGroup.getEnvironment();
			lCounter     = lGroup.getCounter();
			lGeneration  = lGroup.getGeneration();

			//take Jobs
			std::pair<std::vector<int>, std::string> lListJobs = DAGS::CommonServerData::getListJobsToEvaluate(lGroupId, lNbJobsInSubGroup);

			//update the Job table field counter
			const unsigned int lSizeIdTableSubGroup = lListJobs.first.size();

			//update the time of the Jobs that were sent to the client
			DAGS::CommonServerData::updateTimeTable(lGroupId, lListJobs.first);

			bool lAllSent = DAGS::CommonServerData::setJobsCounters(mGroupId, -lSizeIdTableSubGroup);

			//every Jobs has been sent, update the Group status to 3
			if(lAllSent == true) {
				lGroup.setStatus(3);
				lGroup.setTime(time(0));
				DAGS::CommonServerData::setCommonGroup(lGroup, lGroupId);
				//put back the ind counter for the Group state 3
				//DAGS::CommonServerData::setJobsCounters(lGroupId, lGroup.getNbScoreNeeded());
			}

			//unlock the mutex
			mUniverseMutex->unlock();

			//construct the string of XML
			XML::Streamer lStreamer(lXMLSubGroup, 0);
			lStreamer.openTag("SG"); //SubGroup
			lStreamer.insertAttribute("id", lGroupId); //group's id
			lStreamer.insertAttribute("app", mAppName);//application name
			lStreamer.insertAttribute("gen", lGeneration); //generation
			lStreamer.openTag("Env"); //Environment
			if(lGroup.getDistributeEnv())
				lStreamer.insertStringContent(lEnvironment, false);
			lStreamer.closeTag(); //close Environment
			lStreamer.insertStringContent(lListJobs.second, false);
			lStreamer.closeTag(); //close Subgroup

			// the xml string containing the SubGroup is now ready
			ioXMLSubGroup = lXMLSubGroup.str();

			if(mVerbose >= 3) {
				std::ostringstream lMessage;
				lMessage << lSizeIdTableSubGroup << " jobs of group's id " << mGroupId << " sent. State = 2";
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lMessage.str());
				mStreamerLog.closeTag();
				if(mVerbose > 3)
					mGetTime = popTime();
			}
			return 1;
		}

		//if all Group state = 3
		else {
			//no redispatch, just quit.
			if(DAGS::CommonServerData::getWaitPercent() == 0) {
				mUniverseMutex->unlock();
				return -1;
			}


			//take Group at state = 3, lowest generation, lowest counter, at least mWaitPercent*getIdealTime before sending it back
			DAGS::Group lGroup(DAGS::CommonServerData::getAnyGroup(3, false, true, mCSD.getIdealTime()));

			//no Group were either status = 2 nor status = 3
			if(lGroup.getDBId() == -1) {
				//unlock the mutex
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent("No SubGroup ready");
				mStreamerLog.closeTag();

				mUniverseMutex->unlock();
				return -1;
			}

			//there's a Group ready to be evaluated, take his description
			lEnvironment = lGroup.getEnvironment();
			lGroupId     = lGroup.getDBId();
			mGroupId     = lGroupId;
			lCounter     = lGroup.getCounter();
			lGeneration  = lGroup.getGeneration();
			if(lCounter == 0)
				DAGS::CommonServerData::setJobsCounters(lGroupId, lGroup.getNbScoreNeeded());

			int lNbJobsTaken = 0;
			std::string lJobsString(DAGS::CommonServerData::getJobsByTime(lGroupId,
			                        lNbJobsInSubGroup,
			                        mCSD.getIdealTime(),
			                        lNbJobsTaken));
			if(!lNbJobsTaken) {
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent("No SubGroup ready");
				mStreamerLog.closeTag();
				//unlock the mutex
				mUniverseMutex->unlock();
				return -1;
			}


			//verify if every Job in Group has been sent again
			bool lAllSent = DAGS::CommonServerData::setJobsCounters(lGroupId, -lNbJobsTaken);

			//every Jobs has been sent, update the Group status to 3
			if(lAllSent == true) {
				int lCounter = lGroup.getCounter();
				lGroup.setCounter(++lCounter);
				lGroup.setTime(time(0));
				DAGS::CommonServerData::setCommonGroup(lGroup, lGroupId);
				//put back the ind counter for the Group state 3
				DAGS::CommonServerData::setJobsCounters(lGroupId, lGroup.getNbScoreNeeded());
			}

			//construct the string of XML
			XML::Streamer lStreamer(lXMLSubGroup, 0);
			lStreamer.openTag("SG"); //SubGroup
			lStreamer.insertAttribute("id", lGroupId); //group's id
			lStreamer.insertAttribute("app", mAppName); //application name
			lStreamer.insertAttribute("gen", lGeneration); //generation
			lStreamer.openTag("Env"); //Environment
			if(lGroup.getDistributeEnv())
				lStreamer.insertStringContent(lEnvironment, false);
			lStreamer.closeTag(); //close Environment
			lStreamer.insertStringContent(lJobsString, false);
			lStreamer.closeTag(); //close Subgroup

			// the xml string containing the SubGroup is now ready
			ioXMLSubGroup = lXMLSubGroup.str();

			mUniverseMutex->unlock();

			if(mVerbose >= 3) {
				std::ostringstream lMessage;
				lMessage << lNbJobsTaken << " jobs of group's id " << mGroupId << " sent. State = 3";
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lMessage.str());
				mStreamerLog.closeTag();
				if(mVerbose > 3)
					mGetTime = popTime();
			}
			return 1;
		}
	}//end of try

	catch (std::runtime_error & inError) {
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getSubGroup()", inError.what(), mThreadIdNum, mClientIpAddress);
		if(mVerbose > 3)
			mGetTime = popTime();
		return -1;
	}
}


int DAGS::ThreadConnection::updateGroupDB(unsigned int inNbJobs)
{
	std::ostringstream lErrorMessage;
	std::ostringstream lQueryOSS;

	int outGroupId = mGroupId;

	try {
		connectDB();

		//update group status.
		lQueryOSS << "BEGIN TRANSACTION; ";
		if(DAGS::CommonServerData::getIfGroupExist(mGroupId)) {
			lQueryOSS << "UPDATE "
			<< DAGS_GROUP_TABLE_NAME
			<< " SET "
			<< DAGS_GROUP_FIELD_GENERATION  << "='" << mGroupGeneration  << "', "
			<< DAGS_GROUP_FIELD_ENVIRONMENT << "='" << mGroupEnvironment << "', "
			<< DAGS_GROUP_FIELD_NB_JOBS     << "='" << inNbJobs          << "'";
			if(mVersion != DAGS::CommonServerData::getDataVersion())
				lQueryOSS << ", " << DAGS_GROUP_FIELD_VERSION << "='" << mVersion << "'";
			lQueryOSS << " WHERE "
			<< DAGS_GROUP_FIELD_DB_GROUP_ID << "='" << mGroupId+1 << "';";
		} else //new client, add it to the database instead of updating it
		{
			lQueryOSS << "INSERT INTO "
			<< DAGS_GROUP_TABLE_NAME
			<< " ("
			<< DAGS_GROUP_FIELD_GENERATION  << ","
			<< DAGS_GROUP_FIELD_ENVIRONMENT << ","
			<< DAGS_GROUP_FIELD_NB_JOBS   << ","
			<< DAGS_GROUP_FIELD_DISTRIBUTE  << ","
			<< DAGS_GROUP_FIELD_APPNAME     << ","
			<< DAGS_GROUP_FIELD_VERSION
			<< ") VALUES("
			<< "'" << mGroupGeneration  << "',"
			<< "'" << mGroupEnvironment << "',"
			<< "'" << inNbJobs        << "',"
			<< "'" << mDistributeEnv    << "',"
			<< "'" << mAppName          << "',"
			<< "'" << mVersion          << "');";
		}
		lQueryOSS << "END TRANSACTION;";
		mDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");
		if(mNewClient)
			outGroupId = mDatabase.getLastInsertID()-1;

		disconnectDB();
		return outGroupId;

	} catch (std::runtime_error & inError) {
		lErrorMessage << "Unknown query sent to the database: \n" << lQueryOSS.str();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::updateGroupDB()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		disconnectDB();
		return -1;
	}
}


int DAGS::ThreadConnection::updateJobsDB(unsigned int inGroupId,
        bool inDataOnly)
{
	if(mVerbose > 3) {
		std::ostringstream lOSS;
		lOSS << "Update Jobs (";
		if(inDataOnly)
			lOSS << "Data";
		else
			lOSS << "Data and Score";
		lOSS << ") of the group " << inGroupId << " in the database";
		mStreamerLog.openTag("Log");
		mStreamerLog.insertStringContent(lOSS.str());
		mStreamerLog.closeTag();
	}

	ostringstream lErrorMessage;
	std::ostringstream lQueryOSS;

	try {
		connectDB();
		mUniverseMutex->lock();
		std::vector<std::pair<int, std::string> >& lScoreVector(DAGS::CommonServerData::getScore(inGroupId));
		std::vector<std::pair<std::string, DAGS::Job> >& lDataVector(DAGS::CommonServerData::getJobs(inGroupId));
		unsigned int lScoreVectorSize = lScoreVector.size();

		//Add Jobs to database
		lQueryOSS << "BEGIN TRANSACTION;";
		if(!inDataOnly) {
			//new Data for the job, so update it in the database
			for(unsigned int i = 0; i < lScoreVectorSize; ++i) {
				lQueryOSS << "UPDATE "
				<< DAGS_JOBS_TABLE_NAME
				<< " SET "
				<< DAGS_JOBS_FIELD_SCORE         << " = '" << lScoreVector[i].second << "',"
				<< DAGS_JOBS_FIELD_INVALID_SCORE << " = '" << false                  << "'"
				<< " WHERE "
				<< DAGS_JOBS_FIELD_DB_GROUP_ID << " = '" << mGroupId              << "'" << " AND "
				<< DAGS_JOBS_FIELD_USER_JOB_ID << " = '" << lScoreVector[i].first << "'; ";
			}
			//lScoreVector.clear();
		} else {
			for(unsigned int i = 0; i < lDataVector.size(); ++i) {
				lQueryOSS << "UPDATE "
				<< DAGS_JOBS_TABLE_NAME
				<< " SET "
				<< DAGS_JOBS_FIELD_DATA          << " = '" << lDataVector[i].second.getData() << "',"
				<< DAGS_JOBS_FIELD_INVALID_SCORE << " = '" << true                            << "'"
				<< " WHERE "
				<< DAGS_JOBS_FIELD_DB_GROUP_ID << " = '" << mGroupId << "'" << " AND "
				<< DAGS_JOBS_FIELD_USER_JOB_ID << " = '" << i        << "'; ";
			}
			for(unsigned int i = 0; i < lScoreVectorSize; ++i) {
				lQueryOSS << "UPDATE "
				<< DAGS_JOBS_TABLE_NAME
				<< " SET "
				<< DAGS_JOBS_FIELD_SCORE         << " = '" << lScoreVector[i].second << "',"
				<< DAGS_JOBS_FIELD_INVALID_SCORE << " = '" << false                  << "'"
				<< " WHERE "
				<< DAGS_JOBS_FIELD_DB_GROUP_ID << " = '" << mGroupId              << "'" << " AND "
				<< DAGS_JOBS_FIELD_USER_JOB_ID << " = '" << lScoreVector[i].first << "'; ";
			}
			DAGS::CommonServerData::setJobsScore(mGroupId, lScoreVector);
			lScoreVector.clear();
		}
		lQueryOSS << "END TRANSACTION;";
		mDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");
		mUniverseMutex->unlock();
		disconnectDB();
		return 1;
	} catch (std::runtime_error & inError) {
		lErrorMessage << "Error catched: " << inError.what() << endl;
		lErrorMessage << "Last query sent to the database: \n" << lQueryOSS.str();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::updateJobsDB()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		mUniverseMutex->unlock();
		disconnectDB();
		return -1;
	}
}

void DAGS::ThreadConnection::insertJobsDB(unsigned int inNbJobs)
{
	std::ostringstream lQueryOSS;
	try {
		connectDB();
		if(mNewClient) {
			lQueryOSS << "BEGIN TRANSACTION; ";
			for(unsigned int i = 0; i < inNbJobs; ++i) {
				lQueryOSS << "INSERT INTO "
				<< DAGS_JOBS_TABLE_NAME
				<< " ("
				<< DAGS_JOBS_FIELD_DB_GROUP_ID   << ","
				<< DAGS_JOBS_FIELD_USER_JOB_ID << ","
				<< DAGS_JOBS_FIELD_DATA          << ","
				<< DAGS_JOBS_FIELD_SCORE         << ","
				<< DAGS_JOBS_FIELD_INVALID_SCORE
				<< ") VALUES("
				<< "'" << mGroupId  << "',"
				<< "'" << i          << "',"
				<< "'" << ""         << "',"
				<< "'" << ""         << "',"
				<< "'" << true       << "');";
			}
			lQueryOSS << "END TRANSACTION;";
			mDatabase.executeQuery(lQueryOSS.str());
			lQueryOSS.str("");
		} else {
			DAGS::Group lGroup(DAGS::CommonServerData::getCommonGroup(mGroupId));
			int lPreviousNbJobs = lGroup.getNbJobs();
			//mDiffJobsGroup = inNbJobs - lPreviousNbJobs;
			if(mDiffJobsGroup == 0) {
				disconnectDB();
				return;
			} else if(mDiffJobsGroup > 0) //insert the diff
			{
				lQueryOSS << "BEGIN TRANSACTION; ";
				for(int i = 0; i < mDiffJobsGroup; ++i)
				{
					lQueryOSS << "INSERT INTO "
					<< DAGS_JOBS_TABLE_NAME
					<< " ("
					<< DAGS_JOBS_FIELD_DB_GROUP_ID   << ","
					<< DAGS_JOBS_FIELD_USER_JOB_ID   << ","
					<< DAGS_JOBS_FIELD_DATA          << ","
					<< DAGS_JOBS_FIELD_SCORE         << ","
					<< DAGS_JOBS_FIELD_INVALID_SCORE
					<< ") VALUES("
					<< "'" << mGroupId                             << "',"
					<< "'" << i + lPreviousNbJobs - mDiffJobsGroup << "',"
					<< "'" << ""                                   << "',"
					<< "'" << ""                                   << "',"
					<< "'" << true                                 << "');";
				}
				lQueryOSS << "END TRANSACTION;";
				mDatabase.executeQuery(lQueryOSS.str());
				lQueryOSS.str("");
			} else //delete the diff
			{
				mDiffJobsGroup = -mDiffJobsGroup;
				lQueryOSS << "DELETE FROM "
				<< DAGS_JOBS_TABLE_NAME
				<< " WHERE "
				<< DAGS_JOBS_FIELD_DB_GROUP_ID   << "='" << mGroupId              << "' AND "
				<< DAGS_JOBS_FIELD_USER_JOB_ID << ">'" << lPreviousNbJobs - mDiffJobsGroup -1 << "' AND "
				<< DAGS_JOBS_FIELD_USER_JOB_ID << "<'" << lPreviousNbJobs - 1 << "';";
				mDatabase.executeQuery(lQueryOSS.str());
				lQueryOSS.str("");
			}
		}
	} catch (std::runtime_error & inError) {
		std::ostringstream lErrorMessage;
		lErrorMessage << "Error catched: " << inError.what() << endl;
		lErrorMessage << "Last query sent to the database: \n" << lQueryOSS.str();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::insertJobsDB()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
	}
	disconnectDB();
}

std::string DAGS::ThreadConnection::getJobsDB(unsigned int inGroupId)
{
	ostringstream lErrorMessage;
	std::ostringstream lQueryOSS;

	try {
		//lock the database
		connectDB();

		//get the data field and the score
		lQueryOSS << "SELECT "
		<< DAGS_JOBS_FIELD_DATA        << ", "
		<< DAGS_JOBS_FIELD_SCORE       << ", "
		<< DAGS_JOBS_FIELD_USER_JOB_ID
		<< " FROM "
		<< DAGS_JOBS_TABLE_NAME
		<< " WHERE "
		<< DAGS_JOBS_FIELD_DB_GROUP_ID << "='" << inGroupId << "';";
		mDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");

		std::ostringstream outXMLJobs;
		XML::Streamer lStreamer(outXMLJobs, 0);
		for(unsigned int i = 0; i < mDatabase.getNumberLines(); ++i) {
			lStreamer.openTag("J"); //Job
			lStreamer.insertAttribute("id", *mDatabase.getValue(i,DAGS_JOBS_FIELD_USER_JOB_ID)); //job's id
			lStreamer.openTag("Sc"); //Score
			lStreamer.insertAttribute("eval", "no"); //evaluate
			lStreamer.insertStringContent(*mDatabase.getValue(i,DAGS_JOBS_FIELD_SCORE), false);
			lStreamer.closeTag(); //close Score
			lStreamer.openTag("Dt"); //Data
			lStreamer.insertStringContent(*mDatabase.getValue(i,DAGS_JOBS_FIELD_DATA), false);
			lStreamer.closeTag(); //close Data
			lStreamer.closeTag(); //close Job
		}
		disconnectDB();
		return outXMLJobs.str();
	}
	catch (std::runtime_error & inError) {
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::updateJobsDB()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		disconnectDB();
		return "";
	}
}

int DAGS::ThreadConnection::setGroup(std::vector<std::pair<std::string, DAGS::Job> > &inJobsVector,
                                     std::vector<std::pair<int, std::string> > &inScoresVector,
                                     std::list<int> &inNeedScore)
{
	if(mVerbose > 3)
		pushTimeValue();

	bool lNoScoreNeeded = inNeedScore.empty();
	unsigned int lNumberOfJobs = inJobsVector.size();
	unsigned int lNumberOfScoresNeeded = inNeedScore.size();

	// inJobsVector: first->xml string, second->DAGS::Job object containing Data/Score/Id.
	// if low memory, xml string = empty string to lower memory usage

	ostringstream lErrorMessage;

	mUniverseMutex->lock();

	if(DAGS::CommonServerData::getIfGroupExist(mGroupId)) {
		if(mVersion != DAGS::CommonServerData::getDataVersion()) {
			if(mVerbose > 0) {
				std::ostringstream lOSS;
				lOSS << "Client Version (" << mVersion << ") is different than the expected one ("
				<< DAGS::CommonServerData::getDataVersion() << "). "
				<< "Updating the version to this new value.";
				DAGS::CommonServerData::logConsole(lOSS.str());
				DAGS::CommonServerData::logErrors("Warning", lOSS.str(), mThreadIdNum, mClientIpAddress);
			}
			DAGS::CommonServerData::setDataVersion(mVersion);
		}
	} else {
		DAGS::CommonServerData::setDataVersion(mVersion);
		DAGS::CommonServerData::setAppName(mAppName);
		mGroupId = DAGS::CommonServerData::getNumberOfGroups();
	}

	if(mCSD.getDatabaseName() != "") {
		if(mVerbose > 3)
			pushTimeValue();
		mGroupId = updateGroupDB(lNumberOfJobs);
		if(mVerbose > 3)
			mDBTime += popTime();
	}

	//update data of group's jobs
	DAGS::CommonServerData::setJobs(mGroupId, inJobsVector, inNeedScore);

	if(!mNewClient)
		DAGS::CommonServerData::resetTables(mGroupId);
	else {
		DAGS::CommonServerData::createTimeTable(lNumberOfJobs);
		DAGS::CommonServerData::createTable(lNumberOfJobs - inScoresVector.size(), mGroupId);
	}

	//update the Jobs counter
	DAGS::CommonServerData::setJobsCounters(mGroupId, lNumberOfScoresNeeded);

	//update the score counter
	DAGS::CommonServerData::setScoreCounters(mGroupId, lNumberOfScoresNeeded);

	if(!mNewClient) {
		//update the data
		DAGS::Group lGroup(DAGS::CommonServerData::getCommonGroup(mGroupId));
		//if the received group's size != at the old size, resize the tables
		int lPreviousNbJobs = lGroup.getNbJobs();
		mDiffJobsGroup = lNumberOfJobs - lPreviousNbJobs;
		if(mDiffJobsGroup != 0)
			DAGS::CommonServerData::resizeTables(mGroupId, mDiffJobsGroup);

		// put already evaluated job in the scoreVector, erasing lScore when processing
		// to lower memory usage
		if(!inScoresVector.empty())
			DAGS::CommonServerData::updateScore(mGroupId, inScoresVector);

		lGroup.setGeneration(mGroupGeneration);
		lGroup.setEnvironment(mGroupEnvironment);
		lGroup.setStatus(2);
		lGroup.setCounter(0);
		//if the number of jobs in the group changed since the last version
		lGroup.setNbJobs(inJobsVector.size());
		lGroup.setNbScoreNeeded(lNumberOfScoresNeeded);
		DAGS::CommonServerData::setCommonGroup(lGroup, lGroup.getDBId());
	} else //new client, create a new entry
	{
		// put already evaluated job in the scoreVector, erasing lScore when processing
		// to lower memory usage
		if(!inScoresVector.empty())
			DAGS::CommonServerData::updateScore(mGroupId, inScoresVector);

		DAGS::Group lGroup;
		lGroup.setAppName(mAppName);
		lGroup.setDBId(mGroupId);
		lGroup.setStatus(2);
		lGroup.setEnvironment(mGroupEnvironment);
		lGroup.setDistributeEnv(mDistributeEnv);
		lGroup.setGeneration(mGroupGeneration);
		lGroup.setCounter(0);
		lGroup.setSpecialFlag(0);
		lGroup.setNbJobs(inJobsVector.size());
		lGroup.setNbScoreNeeded(lNumberOfScoresNeeded);
		DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);
	}

	if(mCSD.getMemoryShortMode())
		DAGS::CommonServerData::setScoreSync(false);

	//verify if some jobs need a score
	if(lNoScoreNeeded) {
		//update the mCSD group vector
		//put the groupid into the FIFO of ready groups.
		DAGS::CommonServerData::setGroupReady(mGroupId);
		DAGS::Group lGroup(DAGS::CommonServerData::getCommonGroup(mGroupId));
		lGroup.setStatus(0);
		lGroup.setCounter(0);
		DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);
		if(mCSD.getMemoryShortMode())
			DAGS::CommonServerData::setScoreSync(true);
		mUniverseMutex->unlock();
	} else
		mUniverseMutex->unlock();


	if(mCSD.getDatabaseName() != "") {
		//update the database with the informations of the groups received
		if(mVerbose > 3)
			pushTimeValue();
		insertJobsDB(lNumberOfJobs);
		if(mVerbose > 3)
			mDBTime += popTime();
	}
	if(mCSD.getGroupSyncMode() && mCSD.getDatabaseName() != "") {
		if(mVerbose > 3)
			pushTimeValue();
		updateJobsDB(mGroupId, true);
		if(mVerbose > 3)
			mDBTime += popTime();
	}

	if(mVerbose >= 3) {
		std::ostringstream lOSS;
		lOSS << "Received group's id " << mGroupId;
		mStreamerLog.openTag("Log");
		mStreamerLog.insertStringContent(lOSS.str());
		mStreamerLog.closeTag();

		lOSS.str("");
		lOSS << lNumberOfScoresNeeded << " out of " << lNumberOfJobs << " Jobs need a score to be computed";
		mStreamerLog.openTag("Log");
		mStreamerLog.insertStringContent(lOSS.str());
		mStreamerLog.closeTag();

		if(mVerbose > 3)
			mSetTime = popTime();
	}
	return 1;
}


int DAGS::ThreadConnection::setSubGroup(std::vector<std::pair<int, std::string> > &inScoreVector,
                                        DAGS::SubGroup& inSubGroup)
{
	if(mVerbose > 3)
		pushTimeValue();

	ostringstream lErrorMessage;
	std::ostringstream lQueryOSS;
	//#of score in the score vector
	unsigned int lNumberOfScore = inScoreVector.size();


	try {
		mUniverseMutex->lock();
		mAppName = inSubGroup.getAppName();
		mGroupId = inSubGroup.getGroupId();

		DAGS::Group lGroup = DAGS::CommonServerData::getCommonGroup(mGroupId);
		if(inSubGroup.getGeneration() != lGroup.getGeneration()) {
			//the received subgroup is from an older generation.  Don't update jobs score...
			if(mVerbose > 2) {
				std::ostringstream lOSS;
				lOSS << "The received subgroup is not from the same generation (" << inSubGroup.getGeneration()
				<< ").  Current generation is ("
				<< DAGS::CommonServerData::getCommonGroup(mGroupId).getGeneration() << ")";
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lOSS.str());
				mStreamerLog.closeTag();
			}
			//but update the client stats.
			if(mCSD.getEnableClientStat())
				updateClientStatistics(lNumberOfScore);
			mUniverseMutex->unlock();
			return 1;
		}

		//#of row affected
		int lNbUpdated = DAGS::CommonServerData::updateScore(mGroupId, inScoreVector);
		if(lNbUpdated == 0) {
			//in case a client returned not needed scores
			mUniverseMutex->unlock();
			//Update client statistics for load balancing (if activated)
			if(mCSD.getEnableClientStat())
				updateClientStatistics(lNumberOfScore);

			if(mVerbose >= 3) {
				std::ostringstream lOSS;
				lOSS << "Received a subgroup (" << lNumberOfScore << " jobs) of group's id " << mGroupId << " but all fitness were already computed.";
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lOSS.str());
				mStreamerLog.closeTag();
				if(mVerbose > 3)
					mSetTime = popTime();
			}
			return 1;
		}
		bool lAllEvaluated = DAGS::CommonServerData::setScoreCounters(mGroupId, -lNbUpdated);

		int lScoreNeeded = lGroup.getNbScoreNeeded() - lNbUpdated;
		if(lScoreNeeded < 0)
			lScoreNeeded = 0;
		lGroup.setNbScoreNeeded(lScoreNeeded);

		if(lAllEvaluated) {
			//update the mCSD group vector
			//put the groupid into the FIFO of ready groups.
			DAGS::CommonServerData::setGroupReady(mGroupId);
			lGroup.setStatus(0);
			lGroup.setCounter(0);
			DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);
			if(mCSD.getMemoryShortMode())
				DAGS::CommonServerData::setScoreSync(true);
		} else
			DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);

		if(double(DAGS::CommonServerData::getSizeScoreVector(mGroupId))/double(lGroup.getNbJobs()) >= double(mCSD.getDatabaseSync())/100 ||
		        lAllEvaluated) {
			std::vector<std::pair<int, std::string> >& lScoreVector(DAGS::CommonServerData::getScore(mGroupId));
			DAGS::CommonServerData::setJobsScore(mGroupId, lScoreVector);
			mUniverseMutex->unlock();

			if(mCSD.getDatabaseName() != "") {
				if(mVerbose > 3)
					pushTimeValue();
				updateJobsDB(mGroupId);
				if(mVerbose > 3)
					mDBTime += popTime();
			}
			lScoreVector.clear();
		} else
			mUniverseMutex->unlock();

		//Update client statistics for load balancing (if activated)
		if(mCSD.getEnableClientStat())
			updateClientStatistics(lNumberOfScore);
	} catch (std::runtime_error & inError) {
		lErrorMessage.str("");
		lErrorMessage << "Bad query while setting SubGroup in Universe.\n"
		<< "Error description : " << inError.what() <<"\n";
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::setSubGroup()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		if(mVerbose > 3)
			mSetTime = popTime();
		return -1;
	}

	if(mVerbose >= 3) {
		std::ostringstream lOSS;
		lOSS << "Received a subgroup (" << lNumberOfScore << " jobs) of group's id " << mGroupId;
		mStreamerLog.openTag("Log");
		mStreamerLog.insertStringContent(lOSS.str());
		mStreamerLog.closeTag();
		if(mVerbose > 3)
			mSetTime = popTime();
	}
	return 1;
}


int DAGS::ThreadConnection::getXMLStringFromClient(std::string &ioXMLStringFromClient, bool inLog)
{
	std::string lXMLStringToClient;
	ostringstream lErrorMessage;
	DAGS::Request lRequest;
	DAGS::Group lGroup;
	int lErrorCode=-1;

	//Receive data from client an check for errors.
	std::string lStringReceived;
	try {
		if(mVerbose > 3)
			pushTimeValue();

		mServerSocketComm.receiveMessage(ioXMLStringFromClient);

		if(mVerbose > 3)
			mReceiveTime = popTime();
	} catch(Socket::Exception inError) {
		lErrorMessage.str("");
		lErrorMessage << "An error occured while receiving data from client. The specific error is : ";
		lErrorMessage << inError.getMessage();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getXMLStringFromClient()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		return -1;
	}

	//Parse the XML string and check for errors.
	std::vector<std::string> lDontParseVector;
	lDontParseVector.push_back("Dt");
	lDontParseVector.push_back("Sc");
	lDontParseVector.push_back("Env");

	if(mVerbose > 3)
		pushTimeValue();

	lErrorCode = mXMLUtils.parseXML(ioXMLStringFromClient, lDontParseVector);

	if(mVerbose > 3)
		mParseTime = popTime();

	if(lErrorCode < 1) {
		lErrorMessage.str("");
		lErrorMessage << "An error occured (" << lErrorCode <<") while parsing the client XML string"
		<< "\nThe specific error is: " << mXMLUtils.getErrorMessage() << "\n";
		if(lErrorCode == XMLUtils::EmptyXMLError) {
			mRequestError = NoGroupInDB;
			lXMLStringToClient = std::string("NO INFORMATION SENT");
		} else {
			mRequestError = XMLError;
			lXMLStringToClient = std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
		}
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage << "Can't communicate with the client to give it the error.";
		}

		mServerSocketComm.close();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getXMLStringFromClient()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		return -1;
	}

	//Check what the client has requested
	lErrorCode = mXMLUtils.getAttributeRequest(lRequest);
	if(lErrorCode<1) {
		lErrorMessage.str("");
		lErrorMessage << "An error occured while getting attributes of the client request."
		<< "\nThe specific error is: " << mXMLUtils.getErrorMessage() << "\n";
		mRequestError = RequestAttributesError;
		lXMLStringToClient=std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage << "Can't communicate with the client to give it the error.";
		}

		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getXMLStringFromClient()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		mServerSocketComm.close();
		return -1;
	}

	mAppName      = lRequest.getAppName();
	mClientId     = lRequest.getClientId();
	mRequestType  = lRequest.getRequestType();
	mRequestError = lRequest.getError();
	mVersion      = lRequest.getVersion();
	mCompression  = lRequest.getCompressionLevel();

	if(lRequest.getGroupId() > -1) {
		mGroupId = lRequest.getGroupId();
		mGetGroupId = true;
	}

	//log appname, client id, request type, request error to the Client tag
	if(mVerbose >= 3 && inLog == true) {
		mStreamerLog.insertAttribute("id", mClientId);
		mStreamerLog.insertAttribute("request" , mRequestType);
		mStreamerLog.insertAttribute("app", mAppName);
		if(mRequestError == NothingToSend)
			mStreamerLog.insertAttribute("info", "NothingToSend");
		else if(mRequestError == NothingToReceive)
			mStreamerLog.insertAttribute("info", "NothingToReceive");
		else if(mRequestError == NoError)
			mStreamerLog.insertAttribute("info", "SendAndReceive");
		else
			mStreamerLog.insertAttribute("error", mRequestError);
	}

	if(mClientId == -1)
		mNewClient = true;

	//verify if same application's name
	if(mAppName == DAGS::CommonServerData::getAppName() ||
	        DAGS::CommonServerData::getAppName() == "" ||
	        mRequestType == "state")
		return 1;
	else {
		lErrorMessage << "The received application's name (" << mAppName << ") is not the expected one ("
		<< DAGS::CommonServerData::getAppName() << ").";
		lXMLStringToClient = std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage << "Can't communicate with the client to give it the error.";
		}
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::getXMLStringFromClient()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		mServerSocketComm.close();
		return -1;
	}
}


int DAGS::ThreadConnection::doGroupRequest(const std::string &inXMLStringFromClient)
{
	std::string lXMLStringToClient;
	ostringstream lErrorMessage;
	std::vector<std::pair<std::string, DAGS::Job> > lJobsVector;
	std::vector<std::pair<int, std::string> > lScoresVector;
	std::list<int> lNeedScore;
	DAGS::Group lGroup;
	int lErrorCode = -1, lRequestError = NoError;

	//IF THE CLIENT SEND A GROUP
	if(mRequestError != NothingToSend) {
		//Find information about the group
		lErrorCode = mXMLUtils.getAttributeGroup(lGroup);
		if(lErrorCode < 1) {
			lErrorMessage.str("");
			lErrorMessage << "An error occured while getting attributes of the group by the client.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			return -1;
		}

		//If there is no error when getting group attribute...
		mAppName = lGroup.getAppName();
		mGroupId = lGroup.getDBId();
		mGroupGeneration  = lGroup.getGeneration();
		mGroupEnvironment = lGroup.getEnvironment();
		mDistributeEnv = lGroup.getDistributeEnv();

		//Extract the group's jobs from the XML string
		lErrorCode = mXMLUtils.getJobs(lJobsVector, lScoresVector, lNeedScore, mCSD.getMemoryShortMode());
		if(lErrorCode == 1) {
			//Place the group in the database
			if(setGroup(lJobsVector, lScoresVector, lNeedScore)!=1) {
				lErrorMessage.str("");
				lErrorMessage << "An error occured while putting Jobs of the group in the database.";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
				lRequestError = SetGroupError;
				return -1;
			}
		} else {
			lErrorMessage.str("");
			lErrorMessage << "An error occured while getting Jobs of the group sent by the client."
			<< "\nThere is the XML string :\n\n" << inXMLStringFromClient << "\n";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			return -1;
		}
	}//end of if the client send a group


	//IF THE CLIENT WANTS A GROUP
	if(mRequestError != NothingToReceive) {
		lErrorCode = getGroup(lXMLStringToClient);
		if(lErrorCode == 1) {
			//Add the XML header
			mRequestError = NoError;
			addXMLHeader(lXMLStringToClient);

			//Send the XML string to the client and check for error
			try {
				if(mVerbose > 3)
					pushTimeValue();
				if(DAGS::CommonServerData::getGroupCompression() != -1)
					mServerSocketComm.sendMessage(lXMLStringToClient, DAGS::CommonServerData::getGroupCompression());
				else
					mServerSocketComm.sendMessage(lXMLStringToClient, mCompression);
				if(mVerbose > 3)
					mSendTime = popTime();
			} catch(Socket::Exception inError) {
				lErrorMessage << "Can't communicate with the client to give it the requested group.";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			}
		} else {
			if(lErrorCode == NoGroupInDB) {
				mRequestError = NoGroupInDB;
				lXMLStringToClient = std::string("NO GROUP IN DB");
			} else if(lErrorCode == GetGroupError) {
				mRequestError = GetGroupError;
				lXMLStringToClient = std::string("Group is busy.");
			} else //other type of error
			{
				mRequestError = GetGroupError;
				lXMLStringToClient = std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
			}
			addXMLHeader(lXMLStringToClient);
			try {
				mServerSocketComm.sendMessage(lXMLStringToClient);
			} catch(Socket::Exception inError) {
				lErrorMessage << "Can't communicate with the client to give it the error.";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			}

			mServerSocketComm.close();
			return -1;
		}
	}

	//If the client doesn't want another group, send an acknowledge to the client
	if(mRequestError == NothingToReceive) {
		mRequestError = lRequestError;
		lXMLStringToClient = std::string("ACKNOWLEDGE FROM THE SERVER.  CHECK ERROR CODE.");
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage << "Can't communicate with the client to give it the acknowledge.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		}
	}

	mServerSocketComm.close();
	return 1;
}


int DAGS::ThreadConnection::doSubGroupRequest(const std::string &inXMLStringFromClient)
{
	std::string lXMLStringToClient;
	ostringstream lErrorMessage;
	std::vector<std::pair<int,std::string> > lScoreVector;
	DAGS::SubGroup lSubGroup;
	int lErrorCode = -1, lRequestError = NoError;

	//IF THE CLIENT SEND A SubGroup
	if(mRequestError != NothingToSend) {
		//Find information about the SubGroup
		lErrorCode = mXMLUtils.getAttributeSubGroup(lSubGroup);
		if(lErrorCode < 1) {
			lErrorMessage.str("");
			lErrorMessage << "An error occured while getting attributes of the SubGroup sent by the client."
			<< "The request has been ignored."
			<< "\nThe XML string received :\n\n" << inXMLStringFromClient << "\n";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			lRequestError = SetSubGroupError;
			return -1;
		}

		//Extract the group from the XML string
		lErrorCode = mXMLUtils.getScore(lScoreVector);
		if(lErrorCode < 1) {
			lErrorMessage.str("");
			lErrorMessage << "An error occured while getting score of the SubGroup sent by the client"
			<< "\nThere is the XML string :\n\n" << inXMLStringFromClient << "\n";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			lRequestError = SetSubGroupError;
		}
		//if there is no error getting Jobs from xml string
		else {
			//Place the group in the database
			if(setSubGroup(lScoreVector, lSubGroup) != 1) {
				lErrorMessage.str("");
				lErrorMessage << "An error occured while putting score of the group in the intern variable."
				<< "The SubGroup has been ignored.\n";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
				lRequestError = SetSubGroupError;
			}
		}
	}//end of if client send a SubGroup


	//IF THE CLIENT WANT A SubGroup
	if(mRequestError != NothingToReceive) {
		//Get a SubGroup for the client
		if(getSubGroup(lXMLStringToClient) != 1) {
			mRequestError = GetSubGroupError;
			lXMLStringToClient=std::string("AN ERROR OCCURED AND THE REQUEST CANNOT BE PERFORMED.");
			addXMLHeader(lXMLStringToClient);
			try {
				mServerSocketComm.sendMessage(lXMLStringToClient);
			} catch(Socket::Exception inError) {
				lErrorMessage << "Can't communicate with the client to give it the error.";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			}

			mServerSocketComm.close();
			return -1;
		}

		mRequestError = NoError;

		//Add the XML header
		addXMLHeader(lXMLStringToClient);
		//Send the XML string to the client and check for error
		try {
			if(mVerbose > 3)
				pushTimeValue();
			if(DAGS::CommonServerData::getGroupCompression() != -1)
				mServerSocketComm.sendMessage(lXMLStringToClient, DAGS::CommonServerData::getSubGroupCompression());
			else
				mServerSocketComm.sendMessage(lXMLStringToClient, mCompression);
			if(mVerbose > 3)
				mSendTime = popTime();
		} catch(Socket::Exception inError) {
			lErrorMessage << "Can't communicate with the client to give it the requested SubGroup.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			return -1;
		}
	}

	//If the client doesn't want another SubGroup, send an acknowledge to the client
	else if(mRequestError == NothingToReceive) {
		mRequestError = lRequestError;
		lXMLStringToClient = std::string("ACKNOWLEDGE FROM THE SERVER.  CHECK ERROR CODE.");
		addXMLHeader(lXMLStringToClient);
		try {
			mServerSocketComm.sendMessage(lXMLStringToClient);
		} catch(Socket::Exception inError) {
			lErrorMessage << "Can't communicate with the client to give it the error.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		}
	}

	if(mCSD.getEnableClientStat()) {
		try {
			DAGS::Distribution& lDistribution = DAGS::CommonServerData::getClientDistribution(mClientId);
			lDistribution.setTimeLastJob(lSubGroup.getEvalTime());
		} catch(std::runtime_error& inError) {
			lErrorMessage << "The specified client id is not valid: " << mClientId;
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doSubGroupRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		}
	}

	mServerSocketComm.close();
	return 1;
}


int DAGS::ThreadConnection::doReseedRequest(const std::string &inXMLStringFromClient)
{
	std::string lXMLStringToClient;
	ostringstream lErrorMessage;
	std::vector<int> lReseedJobs;
	std::vector<std::pair<int, std::string> > lScoreJobs;
	DAGS::SubGroup lSubGroup;
	ostringstream lQueryOSS;
	int lErrorCode = -1, lRequestError=NoError;

	try {
		//Find information abour the SubGroup
		lErrorCode = mXMLUtils.getAttributeSubGroup(lSubGroup);
		if(lErrorCode < 1) {
			lErrorMessage.str("");
			lErrorMessage << "An error occured while getting attributes of the SubGroup sent by the client."
			<< "\nThe XML string received :\n\n" << inXMLStringFromClient << "\n";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doReseedRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			lRequestError = SetGroupError;
		}

		//If there was no error in SubGroup attribute...
		else {
			mAppName = lSubGroup.getAppName();
			mGroupId = lSubGroup.getGroupId();

			mUniverseMutex->lock();
			//the received jobs are from an older generation, don't update, just return.
			if(lSubGroup.getGeneration() != DAGS::CommonServerData::getCommonGroup(lSubGroup.getGroupId()).getGeneration()) {
				mUniverseMutex->unlock();
				return 1;
			}
			//Extract the group from the XML string
			lErrorCode = mXMLUtils.getReseed(lReseedJobs, lScoreJobs);
			if(lErrorCode < 1) {
				lErrorMessage.str("");
				lErrorMessage << "An error occured while getting Jobs of the SubGroup sent by the client."
				<< "\nThere is the XML string :\n\n" << inXMLStringFromClient << "\n";
				DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doReseedRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
				lRequestError = SetGroupError;
			}
		}//end of else

		if(mVerbose > 3) {
			mStreamerLog.openTag("DAGS::ThreadConnection::doReseedRequest");
			mStreamerLog.insertAttribute("reseed", lReseedJobs.size());
			mStreamerLog.insertAttribute("evaluated", lScoreJobs.size());
			mStreamerLog.closeTag();
		}

		//put the group state to 2, to make sure the Jobs will be evaluated in priority
		if(!lReseedJobs.empty()) {
			DAGS::Group lGroup = DAGS::CommonServerData::getCommonGroup(mGroupId);
			lGroup.setStatus(2);
			lGroup.setCounter(0);
			lGroup.setTime(0);
			DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);

			DAGS::CommonServerData::updateJobsNeedScore(mGroupId, lReseedJobs);

			//create/update an Jobs counter
			DAGS::CommonServerData::setJobsCounters(mGroupId, lReseedJobs.size());
		}

		//if the score vector is not empty, update the Score vector of the corresponding
		//Jobs ID
		if(!lScoreJobs.empty()) {
			DAGS::Group lGroup = DAGS::CommonServerData::getCommonGroup(mGroupId);

			//#of row affected
			int lNbUpdated = DAGS::CommonServerData::updateScore(mGroupId, lScoreJobs);
			bool lAllEvaluated = DAGS::CommonServerData::setScoreCounters(mGroupId, -lNbUpdated);

			if(lAllEvaluated) {
				//update the mCSD group vector
				//put the groupid into the FIFO of ready groups.
				DAGS::CommonServerData::setGroupReady(mGroupId);
				lGroup.setStatus(0);
				lGroup.setCounter(0);
				DAGS::CommonServerData::setCommonGroup(lGroup, mGroupId);
			}

			if(double(DAGS::CommonServerData::getSizeScoreVector(mGroupId))/double(lGroup.getNbJobs()) >= double(mCSD.getDatabaseSync())/100 ||
			        lAllEvaluated) {
				std::vector<std::pair<int, std::string> >& lScoreVector(DAGS::CommonServerData::getScore(mGroupId));
				DAGS::CommonServerData::setJobsScore(mGroupId, lScoreVector);
				mUniverseMutex->unlock();

				if(mCSD.getDatabaseName() != "") {
					if(mVerbose > 3)
						pushTimeValue();
					updateJobsDB(mGroupId);
					if(mVerbose > 3)
						mDBTime += popTime();
				}
				lScoreVector.clear();
			} else
				mUniverseMutex->unlock();
		} else
			mUniverseMutex->unlock();

		//put the Distribution stats to be taken again.
		if(mCSD.getEnableClientStat())
			DAGS::CommonServerData::invalidateClient(mClientId);

	}//end of try
	catch (std::runtime_error & inError) {
		lErrorMessage.str("");
		lErrorMessage << inError.what();
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doReseedRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
		return -1;
	}

	return 1;
}


int DAGS::ThreadConnection::doMonitorRequest()
{
	ostringstream lErrorMessage;

	try {
		DAGS::Monitor lMonitor;
		int lErrorCode = mXMLUtils.getAttributeMonitor(lMonitor);
		if(lErrorCode < 1) {
			lErrorMessage.str("");
			lErrorMessage << "An error occured while getting attributes of the Monitor sent by the client.";
			DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doMonitorRequest()", lErrorMessage.str(), mThreadIdNum, mClientIpAddress);
			return -1;
		}

		std::ostringstream lOSS;
		//fake xml streamer for the monitor.  Ugly hack, but visually better to look at.
		lOSS << endl;

		if(lMonitor.getRequest() == "DistributionList") {
			std::vector<DAGS::Distribution> lDistribution(DAGS::CommonServerData::getAllClientDistribution());
			if(!lDistribution.empty()) {
				XML::Streamer lStreamer(lOSS);
				for(unsigned int i = 0; i < lDistribution.size(); ++i) {
					unsigned int lNbJobsInSubGroup = DAGS::CommonServerData::getLoadBalancing(lDistribution[i].getClientID());
					lStreamer.openTag("Client"); //DAGS communication
					lStreamer.insertAttribute("id", lDistribution[i].getClientID());  //client id
					if(lDistribution[i].isValid())
						lStreamer.insertAttribute("valid", "yes");  //client id
					else
						lStreamer.insertAttribute("valid", "no");  //client id
					lStreamer.insertAttribute("ip", lDistribution[i].getIPAddress()); //ip address
					lStreamer.insertAttribute("lastcon", lDistribution[i].getTime()); //last time connected
					lStreamer.insertAttribute("loadBalancing", lNbJobsInSubGroup);
					lStreamer.insertAttribute("Tf", lDistribution[i].getTimeLastJob());
					std::vector<double> lStats(lDistribution[i].getStats());
					std::vector<double> lWeight(mCSD.getVectorWeightsStatistics());
					for(unsigned int j = 0; j < lStats.size(); ++j) {
						std::ostringstream lOSSStat;
						lOSSStat << "Stat" << j+1;
						lStreamer.openTag(lOSSStat.str());
						lStreamer.insertAttribute("value", lStats[j]);
						lStreamer.insertAttribute("weight", lWeight[j]);
						lStreamer.closeTag();
					}
					lStreamer.closeTag(); //close Client
				}
			}
			//could not find clients in the distribution list, send an empty one
			else {
				XML::Streamer lStreamer(lOSS);
				lStreamer.openTag("NoStats");
				lStreamer.closeTag();
			}
		} else if(lMonitor.getRequest() == "Distribution") {
			XML::Streamer lStreamer(lOSS);
			try {
				DAGS::Distribution& lDistribution = DAGS::CommonServerData::getClientDistribution(lMonitor.getId());

				unsigned int lNbJobsInSubGroup = DAGS::CommonServerData::getLoadBalancing(lMonitor.getId());
				lStreamer.openTag("Client"); //DAGS communication
				lStreamer.insertAttribute("id", lMonitor.getId());  //client id
				if(lDistribution.isValid())
					lStreamer.insertAttribute("valid", "yes");  //client id
				else
					lStreamer.insertAttribute("valid", "no");  //client id
				lStreamer.insertAttribute("ip", lDistribution.getIPAddress()); //ip address
				lStreamer.insertAttribute("lastcon", lDistribution.getTime()); //last time connected
				lStreamer.insertAttribute("loadBalancing", lNbJobsInSubGroup);
				lStreamer.insertAttribute("Tf", lDistribution.getTimeLastJob());
				std::vector<double> lStats(lDistribution.getStats());
				std::vector<double> lWeight(mCSD.getVectorWeightsStatistics());
				for(unsigned int i = 0; i < lStats.size(); ++i) {
					std::ostringstream lOSSStat;
					lOSSStat << "Stat" << i+1;
					lStreamer.openTag(lOSSStat.str());
					lStreamer.insertAttribute("value", lStats[i]);
					lStreamer.insertAttribute("weight", lWeight[i]);
//          lStreamer.insertAttribute("value", lStats[(lStats.size()-1)-i]);
					lStreamer.closeTag();
				}
				lStreamer.closeTag(); //close Client
			} catch(std::runtime_error& inError) {
				lStreamer.openTag("Error");
				lStreamer.insertStringContent("Could not find information about that client's id");
				lStreamer.closeTag();
			}
		} else if(lMonitor.getRequest() == "EnvironmentList") {
			std::vector<DAGS::Group> lGroups(DAGS::CommonServerData::getGroupVector());
			if(!lGroups.empty()) {
				XML::Streamer lStreamer(lOSS);
				for(unsigned int i = 0; i < lGroups.size(); ++i) {
					lStreamer.openTag("Group");  //Group
					lStreamer.insertAttribute("id", lGroups[i].getDBId()); //group's id
					lStreamer.insertAttribute("generation", lGroups[i].getGeneration()); //generation
					lStreamer.insertAttribute("status", lGroups[i].getStatus()); //generation
					lStreamer.openTag("Env"); //Environment
					lStreamer.insertAttribute("distribute", lGroups[i].getDistributeEnv());
					lStreamer.insertStringContent(lGroups[i].getEnvironment(), false);
					lStreamer.closeTag(); //close Environment
					lStreamer.closeTag();
				}
			}
			//could not find clients in the distribution list, send an empty one
			else {
				XML::Streamer lStreamer(lOSS);
				lStreamer.openTag("NoGroup");
				lStreamer.closeTag();
			}
		} else if(lMonitor.getRequest() == "Environment") {
			std::vector<DAGS::Group> lGroups(DAGS::CommonServerData::getGroupVector());
			unsigned int lIdRequest = lMonitor.getId();
			if(!lGroups.empty() && lIdRequest < lGroups.size()) {
				XML::Streamer lStreamer(lOSS);
				lStreamer.openTag("Group");  //Group
				lStreamer.insertAttribute("id", lGroups[lIdRequest].getDBId()); //group's id
				lStreamer.insertAttribute("generation", lGroups[lIdRequest].getGeneration()); //generation
				lStreamer.insertAttribute("status", lGroups[lIdRequest].getStatus()); //generation
				lStreamer.openTag("Env"); //Environment
				lStreamer.insertAttribute("distribute", lGroups[lIdRequest].getDistributeEnv());
				lStreamer.insertStringContent(lGroups[lIdRequest].getEnvironment(), false);
				lStreamer.closeTag(); //close Environment
				lStreamer.closeTag();
			}
			//could not find clients in the distribution list, send an empty one
			else {
				XML::Streamer lStreamer(lOSS);
				lStreamer.openTag("NoGroup");
				lStreamer.closeTag();
			}
		}
		//with specific group's id
		else if(lMonitor.getRequest() == "Group") {
			if(mCSD.getMemoryShortMode() && DAGS::CommonServerData::getIfJobsPresent(mGroupId) == false) {
				std::ostringstream lOSSMessage;
				lOSSMessage << "The DAGS server is presently in low memory usage mode. "
				<< "It cannot fetch the entire group since it will more than "
				<< "double the memory usage and will probably result in a "
				<< "system crash. If you don't need the server to be in "
				<< "low memory usage mode, change the option memory short mode "
				<< "to off of the DAGS server's config file.";
				XML::Streamer lStreamer(lOSS);
				lStreamer.openTag("Log");
				lStreamer.insertStringContent(lOSSMessage.str());
				lStreamer.closeTag();
			} else {
				//sync with the database is essential since we need the score
				//of already evaluated jobs.  This should lower the time needed
				//to verify if the score are already computed.
				mGroupId = lMonitor.getId();
				if(mCSD.getDatabaseName() != "")
					updateJobsDB(lMonitor.getId(), false);

				//take what's present in memory
				mUniverseMutex->lock();

				DAGS::Group lGroup(DAGS::CommonServerData::getCommonGroup(lMonitor.getId()));
				//construct the XML string
				XML::Streamer lStreamer(lOSS);
				lStreamer.openTag("G");  //Group
				lStreamer.insertAttribute("id", lGroup.getDBId()); //group's id
				lStreamer.insertAttribute("app", lGroup.getAppName()); //application name
				lStreamer.insertAttribute("gen", lGroup.getGeneration()); //generation
				lStreamer.openTag("Env"); //Environment
				lStreamer.insertStringContent(lGroup.getEnvironment(), false);
				lStreamer.closeTag(); //close Environment

				std::vector<std::pair<std::string, DAGS::Job> >& lJob(DAGS::CommonServerData::getJobs(lMonitor.getId()));

				if(!lJob.empty()) {
					for(unsigned int i = 0; i < lJob.size(); ++i) {
						lStreamer.openTag("J"); //Job
						lStreamer.insertAttribute("id", lJob[i].second.getId()); //job id
						lStreamer.openTag("Sc"); //Score
						if(lJob[i].second.getScore() != "") {
							lStreamer.insertAttribute("eval", "no"); //valid score
							lStreamer.insertStringContent(lJob[i].second.getScore(), false);
						} else
							lStreamer.insertAttribute("eval", "yes"); //invalid score
						lStreamer.closeTag(); //close Score
						lStreamer.openTag("Dt"); //Data
						lStreamer.insertStringContent(lJob[i].second.getData(), false);
						lStreamer.closeTag(); //close Data
						lStreamer.closeTag(); //close Job
					}
				} else {
					lStreamer.openTag("Error");
					lStreamer.insertStringContent("No job found in the specified group.");
					lStreamer.closeTag();
				}
				lStreamer.closeTag(); //close Group
				mUniverseMutex->unlock();
			}
		} else if(lMonitor.getRequest() == "Universe") {
			XML::Streamer lStreamer(lOSS);
			lStreamer.openTag("Universe");
			lStreamer.insertStringContent("Not Implemented");
			lStreamer.closeTag();
		} else if(lMonitor.getRequest() == "Terminate") {
			XML::Streamer lStreamer(lOSS);
			lStreamer.openTag("Terminate");
			lStreamer.insertStringContent("Not Implemented");
			lStreamer.closeTag();
		} else if(lMonitor.getRequest() == "States") {
			XML::Streamer lStreamer(lOSS);
			mAppName = DAGS::CommonServerData::getAppName();
			mNumberOfGroups = DAGS::CommonServerData::getNumberOfGroups();

			lStreamer.openTag(mAppName);
			lStreamer.insertAttribute("numberOfGroups", mNumberOfGroups);
			for(int i = 0; i < mNumberOfGroups; ++i) {
				lStreamer.openTag("Group");
				lStreamer.insertAttribute("id", i+1);
				lStreamer.insertAttribute("generation", DAGS::CommonServerData::getCommonGroup(i).getGeneration());
				lStreamer.insertAttribute("state", DAGS::CommonServerData::getCommonGroup(i).getStatus());
				lStreamer.insertAttribute("jobsToDo", DAGS::CommonServerData::getCommonGroup(i).getNbScoreNeeded());
				lStreamer.closeTag();
			}
			lStreamer.closeTag(); //close DAGS
		}
		std::string lXMLStringToClient(lOSS.str());
		addXMLHeader(lXMLStringToClient);
		mServerSocketComm.sendMessage(lXMLStringToClient);
	} //try
	catch(Socket::Exception inError) {
		lErrorMessage.str("");
		lErrorMessage << "\nDAGS::ThreadConnection::doMonitorRequest()";
		lErrorMessage << "\nCan't communicate with the client to give it the request.";
	} catch(std::runtime_error& inError) {
		lErrorMessage.str("");
		lErrorMessage << "Error catched: " << inError.what() <<endl;
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::doMonitorRequest()", lErrorMessage.str());
	}
	return 1;
}

int DAGS::ThreadConnection::loadBalancing(std::vector<double> inStats)
{
	//error message
	ostringstream lErrorMessage;

	std::vector<double> lVectorValuesStatistic;

	try {
		unsigned int lNumberOfStats = mCSD.getNumberStatistics();
		if(DAGS::CommonServerData::clientDistributionExist(mClientId) == false) {
			updateDistributionInfo(inStats, mCSD.getNbJobsInSubGroup());
			if(mVerbose > 3) {
				std::ostringstream lMessage;
				lMessage << "Client not found. Load balancing recommend: "
				<< mCSD.getNbJobsInSubGroup() << " Jobs.";
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lMessage.str());
				mStreamerLog.closeTag();
			}
			return mCSD.getNbJobsInSubGroup();
		}

		for(unsigned int i = 0; i < lNumberOfStats; ++i) {
			if(inStats[i] != -1)
				lVectorValuesStatistic.push_back(inStats[i]);
		}

		if(lVectorValuesStatistic.empty() != true) {
			double lMeanStats = 0;
			int oBalancingValue = 1;
			unsigned int lStatsValueSize = lVectorValuesStatistic.size();

			if(lStatsValueSize != lNumberOfStats) {
				if(mVerbose >= 3) {
					std::ostringstream lOSS;
					lOSS << "Number of stats found(" << lStatsValueSize << ") is lower than the expected one("
					<< lNumberOfStats << ").  Using default value: " << mCSD.getNbJobsInSubGroup();
					mStreamerLog.openTag("Log");
					mStreamerLog.insertStringContent(lOSS.str());
					mStreamerLog.closeTag();
				}
				return mCSD.getNbJobsInSubGroup();
			} else {
				std::vector<double> lVectorWeightsStatistic = mCSD.getVectorWeightsStatistics();
				for(unsigned int i = 0; i < lStatsValueSize; ++i) {
					lMeanStats += lVectorValuesStatistic[i] * lVectorWeightsStatistic[i];
				}
			}

			oBalancingValue = (int)floor((lMeanStats * mCSD.getIdealTime())+0.5);
			if(mVerbose >= 3) {
				std::ostringstream lOSS;
				lOSS << "Number of jobs recommended: " << oBalancingValue;
				mStreamerLog.openTag("Log");
				mStreamerLog.insertStringContent(lOSS.str());
				mStreamerLog.closeTag();
			}
			return (oBalancingValue > 1 ? oBalancingValue : 1);
		} else
			return mCSD.getNbJobsInSubGroup();
	}//end of try

	catch (std::runtime_error & inError) {
		DAGS::CommonServerData::logErrors("DAGS::ThreadConnection::loadBalancing()", inError.what(), mThreadIdNum, mClientIpAddress);
		return -1;
	}
}


int DAGS::ThreadConnection::updateClientStatistics(unsigned int inNbJobs)
{
	//error message
	ostringstream lErrorMessage;

	int lLastConnect = 0;
	std::vector<double> lOldStats;
	try {
		DAGS::Distribution& lDistribution = DAGS::CommonServerData::getClientDistribution(mClientId);
		lLastConnect = lDistribution.getLastConnect();
		lOldStats = lDistribution.getStats();
	} catch(std::runtime_error& inError) {
		//should never be here, but what the hell, just to be sure
		//nobody calls setSubGroup before getSubGroup
		return -1;
	}

	long long lClientTime = mClientConnectTime - lLastConnect;
	if(lClientTime == 0)
		lClientTime = 1;

	//making sure the lClientTime is not under 0: if someone changed
	//the server's computer clock, this could happen.  So, do nothing and
	//keep old ratio data.
	if(lClientTime >= 1) {
		double lNewRatio =  (double)(inNbJobs)/(double)(lClientTime);
		std::vector<double> lNewStats;
		lNewStats.push_back(lNewRatio);
		for(unsigned int i = 1; i < lOldStats.size(); ++i) {
			lNewStats.push_back(lOldStats[i-1]);
		}
		unsigned int lLoadBalancing = loadBalancing(lNewStats);
		updateDistributionInfo(lNewStats, lLoadBalancing);
	}
	return 1;
}


void DAGS::ThreadConnection::pushTimeValue()
{
	PACC::Timer lTimeValue;
	mListTimeValues.push_back(lTimeValue);
}


double DAGS::ThreadConnection::popTime()
{
	double lTimer = mListTimeValues.back().getValue();
	mListTimeValues.pop_back();

	return lTimer;
}

void DAGS::ThreadConnection::setXMLTime()
{
	mStreamerLog.openTag("Time");
	mStreamerLog.insertAttribute("thread", mThreadTime);
	mStreamerLog.insertAttribute("parse", mParseTime);
	mStreamerLog.insertAttribute("set", mSetTime);
	mStreamerLog.insertAttribute("DB", mDBTime);
	mStreamerLog.insertAttribute("get", mGetTime);
	mStreamerLog.insertAttribute("recv", mReceiveTime);
	mStreamerLog.insertAttribute("send", mSendTime);
	mStreamerLog.closeTag();
}
