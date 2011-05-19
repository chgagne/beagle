/*
 *  CommonServerData.cpp
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

#include "CommonServerData.hpp"
#include "PACC/Threading/Exception.hpp"
#include "PACC/Util/Date.hpp"
#include "PACC/Util/Timer.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace PACC;

//static mutex
PACC::Threading::Mutex* DAGS::CommonServerData::mUniverseMutex;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexLogErrors;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexGetUniverseMutex;
PACC::Threading::Mutex DAGS::CommonServerData::mFreeThreadNumberMutex;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexGroupVector;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexDistribution;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexJobsID;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexJobsCounter;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexScoreCounter;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexSQL;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexListJobs;
PACC::Threading::Mutex DAGS::CommonServerData::mMutexTimeTable;
PACC::Threading::Mutex DAGS::CommonServerData::mTableMutex;
bool DAGS::CommonServerData::mTerminate = false;
std::list<unsigned int> DAGS::CommonServerData::mListGroupsReady;
std::list<unsigned int> DAGS::CommonServerData::mReseedDistribution;
bool DAGS::CommonServerData::mScoreSync = true;
bool DAGS::CommonServerData::mMemoryShort = false;
bool DAGS::CommonServerData::mGroupDBSync = false;
unsigned int DAGS::CommonServerData::mDatabaseSync = 100;
int DAGS::CommonServerData::mGroupCompression = -1;
int DAGS::CommonServerData::mSubGroupCompression = -1;

//other static data
std::stack<int> DAGS::CommonServerData::mFreeThreadNumber = std::stack<int>();
std::string DAGS::CommonServerData::mLogErrorFile = std::string("");
std::string DAGS::CommonServerData::mLogMessageFile = std::string("");
std::vector<DAGS::Group> DAGS::CommonServerData::mGroupVector = std::vector<DAGS::Group>();
std::vector<DAGS::Distribution> DAGS::CommonServerData::mDistributionVector = std::vector<DAGS::Distribution>();
double DAGS::CommonServerData::mWaitPercent = 100;
std::vector<int> DAGS::CommonServerData::mJobsCounters;
std::vector<int> DAGS::CommonServerData::mScoreCounters;
std::vector<std::list<std::pair<int, std::string> > > DAGS::CommonServerData::mListJobs;
std::vector<std::list<std::pair<int, int> > > DAGS::CommonServerData::mTimeFIFO;
std::vector<std::vector<int> > DAGS::CommonServerData::mTimeTable;
//std::vector<std::vector<std::string> > DAGS::CommonServerData::mDataVector;
std::vector<std::pair<unsigned int, std::vector<std::string> > > DAGS::CommonServerData::mDataVector;
std::vector<std::vector<std::pair<int, std::string> > > DAGS::CommonServerData::mScoreVector;
std::vector<std::vector<bool> > DAGS::CommonServerData::mScoreReceived;
std::vector<std::vector<std::pair<std::string, DAGS::Job> > > DAGS::CommonServerData::mJobsVector;
std::vector<std::list<int> > DAGS::CommonServerData::mJobsNeedScore;
std::string DAGS::CommonServerData::mAppName = "";
std::string DAGS::CommonServerData::mDataVersion = "";

DAGS::CommonServerData::CommonServerData()
{}

DAGS::CommonServerData::~CommonServerData()
{}

DAGS::CommonServerData::CommonServerData(const CommonServerData& inCommonServerData)
{
	mDatabaseName            = std::string(inCommonServerData.mDatabaseName.c_str());
	mDbPort                  = inCommonServerData.mDbPort;
	mDbAddress               = std::string(inCommonServerData.mDbAddress.c_str());
	mDbUser                  = std::string(inCommonServerData.mDbUser.c_str());
	mDbPassword              = std::string(inCommonServerData.mDbPassword.c_str());
	mEnableClientStat        = inCommonServerData.mEnableClientStat;
	mNbJobsInSubGroup      = inCommonServerData.mNbJobsInSubGroup;
	mNumberStatistics        = inCommonServerData.mNumberStatistics;
	mIdealTime               = inCommonServerData.mIdealTime;
	mVectorWeightsStatistics = inCommonServerData.mVectorWeightsStatistics;
}


void DAGS::CommonServerData::logErrors(std::string inName,
                                       std::string inErrorMessage,
                                       int inThreadId,
                                       std::string inIPAddress)
{
	try {
		std::ostringstream lOSSMessage;
		XML::Streamer lStreamer(lOSSMessage);
		lStreamer.openTag("Error");
		Date lDate;
		std::string lTimeString(lDate.get());
		lStreamer.insertAttribute("time", lTimeString);
		if(inThreadId > -1)   lStreamer.insertAttribute("threadID", inThreadId);
		if(inIPAddress != "") lStreamer.insertAttribute("ip", inIPAddress);
		lStreamer.openTag(inName);
		lStreamer.insertStringContent(inErrorMessage, false);
		lStreamer.closeTag();
		lStreamer.closeTag(); //close Error

		//write the errors into the file
		mMutexLogErrors.lock();

		std::ofstream lOutFile;
		lOutFile.open(mLogErrorFile.c_str(), std::ofstream::out | std::ofstream::app);
		lOutFile << lOSSMessage.str();
		lOutFile.close();

		mMutexLogErrors.unlock();
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::logErrors()", lErrorString);
	}
}


void DAGS::CommonServerData::logMessages(std::string inMessage)
{
	try {
		mMutexLogErrors.lock();

		//write the threads messages into the file
		std::ofstream lOutFile;
		lOutFile.open(mLogMessageFile.c_str(), std::ofstream::out | std::ofstream::app);
		lOutFile << inMessage;
		lOutFile.close();

		mMutexLogErrors.unlock();
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::logMessages()", lErrorString);
	}
}


void DAGS::CommonServerData::logConsole(std::string inMessage)
{
	try {
		mMutexLogErrors.lock();

		//write the threads messages into the file
		Date lDate;
		std::string lTimeString(lDate.get());
		cerr << lTimeString << endl
		<< "--> "      << inMessage   << endl;

		mMutexLogErrors.unlock();
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::logMessages()", lErrorString);
	}
}



PACC::Threading::Mutex* DAGS::CommonServerData::getUniverseMutex()
{
	try {
		mMutexGetUniverseMutex.lock();
		PACC::Threading::Mutex* outMutex = 0;

		if(mUniverseMutex == 0) setUniverseMutex();
		outMutex = mUniverseMutex;

		mMutexGetUniverseMutex.unlock();
		return outMutex;
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::getUniverseMutex()", lErrorString);
		return NULL;
	}
}


void DAGS::CommonServerData::setUniverseMutex()
{
	try {
		mUniverseMutex = new PACC::Threading::Mutex();
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::setUniverseMutex()", lErrorString);
	}
}

void DAGS::CommonServerData::addFreeThreadNumber(int inFreeThreadNumber)
{
	try {
		mFreeThreadNumberMutex.lock();
		mFreeThreadNumber.push(inFreeThreadNumber);
		mFreeThreadNumberMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::addFreeThreadNumber()", lErrorString);
	}
}

int DAGS::CommonServerData::getFreeThreadNumber()
{
	try {
		mFreeThreadNumberMutex.lock();
		int lIntTemp;
		lIntTemp = mFreeThreadNumber.top();
		mFreeThreadNumber.pop();
		mFreeThreadNumberMutex.unlock();
		return lIntTemp;
	} catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::getFreeThreadNumber()", lErrorString);
		return 0;
	}
}

long DAGS::CommonServerData::getFreeThreadNumberSize()
{
	try {
		mFreeThreadNumberMutex.lock();
		long lLongTemp;
		lLongTemp = mFreeThreadNumber.size();
		mFreeThreadNumberMutex.unlock();
		return lLongTemp;
	}
	catch(PACC::Threading::Exception & inException) {
		std::string lErrorString(inException.getMessage());
		logErrors("DAGS::CommonServerData::getFreeThreadNumberSize()", lErrorString);
		return 0;
	}
}


void DAGS::CommonServerData::setGroupVector(std::vector<DAGS::Group>& inGroupVector)
{
	lockGroupVector();
	mGroupVector = inGroupVector;
	unlockGroupVector();
}

void DAGS::CommonServerData::setCommonGroup(DAGS::Group inGroup, unsigned int inGroupId)
{
	lockGroupVector();

	if(inGroupId < mGroupVector.size()) mGroupVector[inGroupId] = inGroup;
	else mGroupVector.push_back(inGroup);

	unlockGroupVector();
	return;
}

DAGS::Group DAGS::CommonServerData::getCommonGroup(unsigned int inGroupId)
{
	DAGS::Group outGroup;
	if(inGroupId >= mGroupVector.size()) {
		std::ostringstream lOSS;
		lOSS << "Group's id (" << inGroupId << ") is invalid.  It should be less or equal to " << mGroupVector.size()-1;
		logErrors("DAGS::CommonServerData::getCommonGroup()", lOSS.str());
		return outGroup;
	}
	lockGroupVector();
	outGroup = mGroupVector[inGroupId];
	unlockGroupVector();
	return outGroup;
}

DAGS::Group DAGS::CommonServerData::getAnyGroup(int inStatus,
        bool inGeneration,
        bool inCounter,
        long long inIdealTime)
{
	lockGroupVector();
	int lGroupID = -1;
	int lGeneration = 999999999;
	int lCounter = 999999999;
	int lPosition = -1;

	//if inVerifyTime is true, verify to make sure the Group has spent an amout of time before
	//saying it's ready again to be active (returned by getAnyGroup).
	long long lPresentTime = time(0);
	double lWait = inIdealTime*getWaitPercent()/100;

	for(unsigned int i = 0; i < mGroupVector.size(); ++i) {
		if(inGeneration == true && inCounter == false) {
			if(mGroupVector[i].getStatus() == inStatus &&
			        mGroupVector[i].getGeneration() < lGeneration) {
				lGeneration = mGroupVector[i].getGeneration();
				lGroupID = mGroupVector[i].getDBId();
				lPosition = i;
			}
		} else if(inGeneration == true && inCounter == true) {
			if(mGroupVector[i].getStatus() == inStatus &&
			        mGroupVector[i].getGeneration()  < lGeneration &&
			        mGroupVector[i].getCounter() < lCounter &&
			        lPresentTime - mGroupVector[i].getTime() > lWait) {
				lCounter = mGroupVector[i].getCounter();
				lGeneration = mGroupVector[i].getGeneration();
				lGroupID = mGroupVector[i].getDBId();
				lPosition = i;
			}
		} else if(inGeneration == false && inCounter == true) {
			if(inIdealTime > -1) {
				if(mGroupVector[i].getStatus() == inStatus &&
				        mGroupVector[i].getCounter() < lCounter &&
				        lPresentTime - mGroupVector[i].getTime() > lWait) {
					lCounter = mGroupVector[i].getCounter();
					lGroupID = mGroupVector[i].getDBId();
					lPosition = i;
				}
			} else {
				if(mGroupVector[i].getStatus() == inStatus &&
				        mGroupVector[i].getCounter() < lCounter) {
					lCounter = mGroupVector[i].getCounter();
					lGroupID = mGroupVector[i].getDBId();
					lPosition = i;
				}
			}
		}
	}

	if(lPosition > -1) {
		unlockGroupVector();
		return mGroupVector[lPosition];
	}

	//cant find any
	unlockGroupVector();
	return DAGS::Group();
}



void DAGS::CommonServerData::setClientDistribution(std::string inIPAddress,
        int inLastConnect,
        std::vector<double> inStats,
        unsigned int inClientId,
        unsigned int inLoadBalancing)
{
	lockDistribution();
	if(inClientId < mDistributionVector.size()) {
		mDistributionVector[inClientId].setLastConnect(inLastConnect);
		mDistributionVector[inClientId].setStats(inStats);
		mDistributionVector[inClientId].setLoadBalancing(inLoadBalancing);
		mDistributionVector[inClientId].setCurrentTime();
		unlockDistribution();
		return;
	}
	std::ostringstream lOSS;
	lOSS << "Client's id (" << inClientId << ") is invalid.  It should be less or equal to " << mDistributionVector.size()-1;
	logErrors("DAGS::CommonServerData::setClientDistribution()", lOSS.str());
	unlockDistribution();
	return;
}


unsigned int DAGS::CommonServerData::setNewClientDistribution(std::string inIPAddress,
        int inLastConnect,
        std::vector<double> inStats,
        unsigned int inLoadBalancing)
{
	lockDistribution();
	int lClientId = 0;
	if(mReseedDistribution.empty()) {
		DAGS::Distribution lNewClient;
		lNewClient.setIPAddress(inIPAddress);
		lNewClient.setLastConnect(inLastConnect);
		lNewClient.setStats(inStats);
		lNewClient.setLoadBalancing(inLoadBalancing);
		lNewClient.setValid();
		lNewClient.setCurrentTime();
		mDistributionVector.push_back(lNewClient);
		lClientId = mDistributionVector.size()-1;
		mDistributionVector[lClientId].setClientID(lClientId);
	} else {
		lClientId = mReseedDistribution.front();
		mReseedDistribution.pop_front();
		mDistributionVector[lClientId].setIPAddress(inIPAddress);
		mDistributionVector[lClientId].setLastConnect(inLastConnect);
		mDistributionVector[lClientId].setStats(inStats);
		mDistributionVector[lClientId].setLoadBalancing(inLoadBalancing);
		mDistributionVector[lClientId].setValid();
		mDistributionVector[lClientId].setCurrentTime();
	}
	unlockDistribution();
	return lClientId;
}

DAGS::Distribution& DAGS::CommonServerData::getClientDistribution(unsigned int inClientId)
{
	if(inClientId < mDistributionVector.size())
		return mDistributionVector[inClientId];
	else
		throw runtime_error("DAGS::CommonServerData::getClientDistribution() invalid client ID");
}


void DAGS::CommonServerData::invalidateClient(unsigned int inClientId)
{
	if(inClientId >= mDistributionVector.size())
		throw runtime_error("DAGS::CommonServerData::invalidateClient() invalid client ID");

	mReseedDistribution.push_back(inClientId);
	lockDistribution();
	mDistributionVector[inClientId].setInvalid();
	unlockDistribution();
}

bool DAGS::CommonServerData::clientDistributionExist(unsigned int inClientId)
{
	lockDistribution();
	bool outBool = false;
	if(inClientId >= mDistributionVector.size())
		outBool = false;
	else outBool = true;
	unlockDistribution();
	return outBool;
}

unsigned int DAGS::CommonServerData::getLoadBalancing(unsigned int inClientId)
{
	lockDistribution();

	unsigned int outLoadBalancing = mDistributionVector[inClientId].getLoadBalancing();
	unlockDistribution();
	return outLoadBalancing;
}


bool DAGS::CommonServerData::setJobsCounters(unsigned int inGroupID, int inNB)
{
	lockJobsCounter();
	if(inGroupID < mJobsCounters.size()) {
		mJobsCounters[inGroupID] += inNB;
		if(mJobsCounters[inGroupID] > 0) {
			unlockJobsCounter();
			return false;
		} else if(mJobsCounters[inGroupID] == 0) {
			unlockJobsCounter();
			return true;
		}
		//Should not happen
		else if(mJobsCounters[inGroupID] < 0) {
			mJobsCounters[inGroupID] = 0;
			unlockJobsCounter();
			std::ostringstream lOSS;
			lOSS << "Jobs counter (" << inNB << ") is smaller than zero in Group's id: " << inGroupID;
			logErrors("DAGS::CommonServerData::setJobsCounters()", lOSS.str());
			return true;
		}
	}

	//did not find the inGroupID, create it
	if(inNB > 0) {
		mJobsCounters.push_back(inNB);
		unlockJobsCounter();
		return false;
	}

	//cannot create a Group counter without Jobs.
	std::ostringstream lOSS;
	lOSS << "Trying to change a indiviudals Group counter (" << inNB << ") but the Groupid is invalid: " << inGroupID;
	logErrors("DAGS::CommonServerData::setJobsCounters()", lOSS.str());
	unlockJobsCounter();
	return false;
}

bool DAGS::CommonServerData::setScoreCounters(unsigned int inGroupID, int inNB)
{
	lockScoreCounter();
	if(inGroupID < mScoreCounters.size()) {
		mScoreCounters[inGroupID] += inNB;
		if(mScoreCounters[inGroupID] > 0) {
			unlockScoreCounter();
			return false;
		} else if(mScoreCounters[inGroupID] == 0) {
			unlockScoreCounter();
			return true;
		}
		//Should not happen
		else if(mScoreCounters[inGroupID] < 0) {
			mScoreCounters[inGroupID] = 0;
			unlockScoreCounter();
			std::ostringstream lOSS;
			lOSS << "Score counter (" << inNB << ") is smaller than 0 on the group's id: " << inGroupID;
			logErrors("DAGS::CommonServerData::setScoreCounters()", lOSS.str());
			return true;
		}
	}

	//did not find the inGroupID, create it
	if(inNB > 0) {
		mScoreCounters.push_back(inNB);
		unlockScoreCounter();
		return false;
	}

	//cannot create a Group counter without Jobs.
	std::ostringstream lOSS;
	lOSS << "Trying to change a score counter (" << inNB << ") but the Groupid is invalid: " << inGroupID;
	logErrors("DAGS::CommonServerData::setScoreCounters()", lOSS.str());
	unlockScoreCounter();
	return false;
}

void DAGS::CommonServerData::setListJobsToEvaluate(unsigned int inGroupId,
        std::vector<std::pair<std::string,DAGS::Job> > &inJobsVector)
{
	try {
		mMutexListJobs.lock();

		//the corresponding list does not exist, creating it.
		if(inGroupId > mListJobs.size()) {
			mListJobs.resize(mListJobs.size()+1);
		}

		//add the DAGS::Jobs to the list
		for(unsigned int i = 0; i < inJobsVector.size(); ++i) {
			mListJobs[inGroupId].push_back(std::pair<int, std::string> (inJobsVector[i].second.getId(), inJobsVector[i].first));
		}
		mMutexListJobs.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::setListJobsToEvaluate()", inException.getMessage());
	}
}


void DAGS::CommonServerData::setListJobsToEvaluate(unsigned int inGroupId,
        std::vector<std::pair<int, std::string> > &inJobsVector)
{
	try {
		mMutexListJobs.lock();

		//the corresponding list does not exist, creating it.
		if(inGroupId > mListJobs.size()) {
			mListJobs.resize(mListJobs.size()+1);
		}

		//add the DAGS::Jobs to the empty list
		for(unsigned int i = 0; i < inJobsVector.size(); ++i) {
			mListJobs[inGroupId].push_back(std::pair<int, std::string> (inJobsVector[i].first, inJobsVector[i].second));
		}
		mMutexListJobs.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::setListJobsToEvaluate()", inException.getMessage());
	}
}


std::pair<std::vector<int>, std::string> DAGS::CommonServerData::getListJobsToEvaluate(int inGroupId, int inNbJobs)
{
	try {
		mMutexListJobs.lock();

		std::pair<std::vector<int>, std::string> outPairJobs;
		std::ostringstream lOSS;

		//pop the Jobs of the list
		if(!mMemoryShort) {
			for(int i = 0; i < inNbJobs; ++i) {
				if(!mJobsNeedScore[inGroupId].empty()) {
					lOSS << mJobsVector[inGroupId][mJobsNeedScore[inGroupId].front()].first;
					outPairJobs.first.push_back(mJobsNeedScore[inGroupId].front());
					mJobsNeedScore[inGroupId].pop_front();
				} else {
					break;
				}
			}
		} else //slow if memory is short
		{
			XML::Streamer lStreamer(lOSS, 0);
			for(int i = 0; i < inNbJobs; ++i) {
				if(!mJobsNeedScore[inGroupId].empty()) {
					lStreamer.openTag("J"); //job
					lStreamer.insertAttribute("id", mJobsVector[inGroupId][mJobsNeedScore[inGroupId].front()].second.getId());
					lStreamer.openTag("Sc"); //score
					lStreamer.insertAttribute("eval", "no");
					lStreamer.insertStringContent(mJobsVector[inGroupId][mJobsNeedScore[inGroupId].front()].second.getScore(), false);
					lStreamer.closeTag(); //close Score
					lStreamer.openTag("Dt"); //data
					lStreamer.insertStringContent(mJobsVector[inGroupId][mJobsNeedScore[inGroupId].front()].second.getData(), false);
					lStreamer.closeTag(); //close Data
					lStreamer.closeTag(); //close Job
					mJobsNeedScore[inGroupId].pop_front();
				}
			}
		}
		outPairJobs.second = lOSS.str();
		mMutexListJobs.unlock();
		return outPairJobs;
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::getListJobsToEvaluate()", inException.getMessage());
	}
	std::pair<std::vector<int>, std::string> lEmpty;
	return lEmpty;
}


bool DAGS::CommonServerData::getIfJobsPresent(unsigned int inGroupId)
{
	if(mJobsNeedScore[inGroupId].empty() || inGroupId >= mJobsNeedScore.size())
		return false;
	else return true;
}

void DAGS::CommonServerData::updateJobsNeedScore(unsigned int inGroupId, std::vector<int> inJobsNeedScore)
{
	try {
		mTableMutex.lock();

		for(unsigned int i = 0; i < inJobsNeedScore.size(); ++i) {
			mJobsNeedScore[inGroupId].push_back(inJobsNeedScore[i]);
		}

		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::updateJobsNeedScore()", inException.getMessage());
	}

}


void DAGS::CommonServerData::createTimeTable(int inDimension)
{
	try {
		mMutexTimeTable.lock();
		mTimeTable.resize(mTimeTable.size()+1);
		mTimeTable[mTimeTable.size()-1].resize(inDimension, 0);
		mTimeFIFO.resize(mTimeFIFO.size()+1);
		mMutexTimeTable.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::createTimeTable()", inException.getMessage());
	}
}


void DAGS::CommonServerData::updateTimeTable(unsigned int inGroupId, const std::vector<int>& inJobsId)
{
	try {
		mMutexTimeTable.lock();
		int lTime = time(0);
		for(unsigned int i = 0; i < inJobsId.size(); ++i) {
			mTimeTable[inGroupId][(inJobsId[i])] = lTime;
			mTimeFIFO[inGroupId].push_back(std::pair<int, int>(inJobsId[i], lTime));
		}
		mMutexTimeTable.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::updateTimeTable()", inException.getMessage());
	}
}

std::string DAGS::CommonServerData::getJobsByTime(unsigned int inGroupId, int inNbJobs, int inWait, int& outNbTaken)
{
	try {
		mMutexTimeTable.lock();
		std::ostringstream outJobs;
		XML::Streamer lStreamer(outJobs, 0);
		int lPresentTime = time(0);

		while(!mTimeFIFO[inGroupId].empty()) {
			if((lPresentTime - mTimeFIFO[inGroupId].front().second) > inWait) {
				int lJobId (mTimeFIFO[inGroupId].front().first);
				//if the score of this jobs was not received
				if(mScoreReceived[inGroupId][lJobId] == false) {
					if(mTimeFIFO[inGroupId].front().second == mTimeTable[inGroupId][lJobId]) {
						++outNbTaken;
						//reset value of TimeTable and FIFO
						mTimeFIFO[inGroupId].pop_front();
						mTimeTable[inGroupId][lJobId] = lPresentTime;
						mTimeFIFO[inGroupId].push_back(std::pair<int, int>(lJobId, lPresentTime));
						//construction of this Job
						lStreamer.openTag("J"); //Job
						lStreamer.insertAttribute("id", lJobId);
						lStreamer.openTag("Sc"); //score
						lStreamer.closeTag(); //close Score
						lStreamer.openTag("Dt"); //data
						lStreamer.insertStringContent(mJobsVector[inGroupId][lJobId].second.getData(), false);
						lStreamer.closeTag();// close data
						lStreamer.closeTag(); //close Job
					}
				} else {
					mTimeTable[inGroupId][lJobId] = 0;
					mTimeFIFO[inGroupId].pop_front();
				}
			} else {
				break;
			}

			//enough Jobs, so stop fetching
			if(!(--inNbJobs)) {
				break;
			}

		}
		mMutexTimeTable.unlock();
		return outJobs.str();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::getJobsByTime()", inException.getMessage());
		return "";
	}
}

int DAGS::CommonServerData::updateScore(unsigned int inGroupId, std::vector<std::pair<int, std::string> >& inScore)
{
	try {
		mTableMutex.lock();
		int lNbNewScore = 0;
		while(!inScore.empty()) {
			if(!mScoreReceived[inGroupId][(inScore.back().first)]) {
				++lNbNewScore;
				mScoreReceived[inGroupId][(inScore.back().first)] = true;
				mScoreVector[inGroupId].push_back(inScore.back());
			}
			inScore.pop_back();
		}
		mTableMutex.unlock();
		return lNbNewScore;
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::updateScore()", inException.getMessage());
	}
	return 0;
}

std::vector<std::pair<int, std::string> >& DAGS::CommonServerData::getScore(unsigned int inGroupId)
{
	return mScoreVector[inGroupId];
}

void DAGS::CommonServerData::setJobs(unsigned int inGroupId,
                                     std::vector<std::pair<std::string, DAGS::Job> >& inJobs,
                                     std::list<int>& inNeedScore)
{
	try {
		mTableMutex.lock();
		if(inGroupId >= mJobsVector.size()) {
			mJobsVector.resize(inGroupId+1);
			mJobsNeedScore.resize(inGroupId+1);
		}

		//if memory is short, erase old values
		if(mMemoryShort) {
			for(unsigned int i = 0; i < mJobsVector.size(); ++i) {
				mJobsVector[i].clear();
			}
		}

		//erase old data
		mJobsVector[inGroupId].clear();

		//set the value received
		mJobsVector[inGroupId] = inJobs;

		//set job's id that need to be calculated
		mJobsNeedScore[inGroupId] = inNeedScore;

		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::setJobs()", inException.getMessage());
	}
}

std::vector<std::pair<std::string, DAGS::Job> >& DAGS::CommonServerData::getJobs(unsigned int inGroupId)
{
	try {
		mTableMutex.lock();
		if(inGroupId >= mJobsVector.size()) {
			std::ostringstream lOSS;
			lOSS << "group's id(" << inGroupId << ") is invalid.";
			runtime_error("DAGS::CommonServerData::getJobs()" + lOSS.str());
		}
		mTableMutex.unlock();
		return mJobsVector[inGroupId];
	} catch(PACC::Threading::Exception & inException) {
		throw runtime_error("DAGS::CommonServerData::getJobs()" + inException.getMessage());
	}
}

void DAGS::CommonServerData::setJobsScore(unsigned int inGroupId,
        std::vector<std::pair<int, std::string> >& inScore)
{
	try {
		mTableMutex.lock();
		if(inGroupId >= mJobsVector.size()) {
			std::ostringstream lOSS;
			lOSS << "Group's id is invalid: " << inGroupId << endl;
			logErrors("DAGS::CommonServerData::setJobsScore()", lOSS.str());
			mTableMutex.unlock();
			return;
		}

		for(unsigned int i = 0; i < inScore.size(); ++i) {
			mJobsVector[inGroupId][inScore[i].first].second.setScore(inScore[i].second);
		}
		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::setJobsScore()", inException.getMessage());
	}
}

unsigned DAGS::CommonServerData::getSizeScoreVector(unsigned int inGroupId)
{
	return mScoreVector[inGroupId].size();
}



void DAGS::CommonServerData::createTable(int inDimension, unsigned int inGroupId)
{
	try {
		mTableMutex.lock();

		//create the vector of received scores
		std::vector<bool> lBoolVector(inDimension, false);
		mScoreReceived.push_back(lBoolVector);

		//create the Score vector of an empty list
		mScoreVector.resize(mScoreVector.size()+1);

		//create the Data Vector of empty string
		std::vector<std::string> lStringVector(inDimension, "");
		mDataVector.push_back(std::pair<int, std::vector<std::string> >(inGroupId, lStringVector));

		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::createTable()", inException.getMessage());
	}
}

void DAGS::CommonServerData::resetTables(int inGroupId)
{
	try {
		mTableMutex.lock();
		for(unsigned int i = 0; i < mScoreReceived[inGroupId].size(); ++i) {
			mScoreReceived[inGroupId][i] = false;
		}
		mScoreVector[inGroupId].clear();
		mTimeFIFO[inGroupId].clear();
		mJobsCounters[inGroupId] = 0;
		mScoreCounters[inGroupId] = 0;
		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::resetTables()", inException.getMessage());
	}
}

void DAGS::CommonServerData::resizeTables(int inGroupId, int inDiffJobsGroup)
{
	try {
		mTableMutex.lock();
		mScoreReceived[inGroupId].resize(mScoreReceived[inGroupId].size() + inDiffJobsGroup, false);
		mTimeTable[inGroupId].resize(mTimeTable[inGroupId].size() + inDiffJobsGroup, 0);
		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::resizeTables()", inException.getMessage());
	}
}


bool DAGS::CommonServerData::getIfGroupExist(unsigned int inGroupId)
{
	if(inGroupId >= mGroupVector.size()) return false;

	return true;
}

void DAGS::CommonServerData::eraseAllGroupData()
{
	try {
		mTableMutex.lock();
		for(unsigned int i = 0; i < mJobsVector.size(); ++i) {
			mJobsVector[i].clear();
		}
		mTableMutex.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::eraseAllGroupData()", inException.getMessage());
	}
}

void DAGS::CommonServerData::lockSQL()
{
	try {
		mMutexSQL.lock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::lockSQL()", inException.getMessage());
	}
}

void DAGS::CommonServerData::unlockSQL()
{
	try {
		mMutexSQL.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::unlockSQL()", inException.getMessage());
	}
}

void DAGS::CommonServerData::lockJobsCounter()
{
	try {
		mMutexJobsCounter.lock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::lockJobsCounter()", inException.getMessage());
	}
}

void DAGS::CommonServerData::unlockJobsCounter()
{
	try {
		mMutexJobsCounter.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::unlockJobsCounter()", inException.getMessage());
	}
}

void DAGS::CommonServerData::lockScoreCounter()
{
	try {
		mMutexScoreCounter.lock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::lockScoreCounter()", inException.getMessage());
	}
}

void DAGS::CommonServerData::unlockScoreCounter()
{
	try {
		mMutexScoreCounter.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::unlockScoreCounter()", inException.getMessage());
	}
}


void DAGS::CommonServerData::setTerminate()
{
	mTerminate = true;
}


bool DAGS::CommonServerData::getTerminate()
{
	return mTerminate;
}

void DAGS::CommonServerData::setGroupReady(unsigned int inGroupID)
{
	lockGroupVector();
	mListGroupsReady.push_back(inGroupID);
	unlockGroupVector();
}

int DAGS::CommonServerData::getGroupReady()
{
	if(mListGroupsReady.empty()) return -1;
	lockGroupVector();
	unsigned int outGroupId = mListGroupsReady.front();
	mListGroupsReady.pop_front();
	unlockGroupVector();
	return outGroupId;
}

void DAGS::CommonServerData::lockGroupVector()
{
	try {
		mMutexGroupVector.lock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::lockGroupVector()", inException.getMessage());
	}

}

void DAGS::CommonServerData::unlockGroupVector()
{
	try {
		mMutexGroupVector.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::unlockGroupVector()", inException.getMessage());
	}
}

void DAGS::CommonServerData::lockDistribution()
{
	try {
		mMutexDistribution.lock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::lockDistribution()", inException.getMessage());
	}

}

void DAGS::CommonServerData::unlockDistribution()
{
	try {
		mMutexDistribution.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::unlockDistribution()", inException.getMessage());
	}
}


void DAGS::CommonServerData::lockJobsID()
{
	try {
		mMutexJobsID.lock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::lockJobsID()", inException.getMessage());
	}
}


void DAGS::CommonServerData::unlockJobsID()
{
	try {
		mMutexJobsID.unlock();
	} catch(PACC::Threading::Exception & inException) {
		logErrors("DAGS::CommonServerData::unlockJobsID()", inException.getMessage());
	}
}


