/*
 *  Server.cpp
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

#include "Server.hpp"
#include "SQLiteQuery.hpp"
#include "dags/FileParameters.hpp"
#include "PACC/Util/Date.hpp"
#include "PACC/Socket/Cafe.hpp"
#include "PACC/Socket/Exception.hpp"
#include "PACC/Threading.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

using namespace PACC;

PACC::Threading::Mutex DAGS::Server::mMutexStop;
bool DAGS::Server::mServerStopped = false;

DAGS::Server::Server(DAGS::FileParameters inFileParameter,
                     int inVerbose,
                     int inPort,
                     int inMaxConnections,
                     int inMaxThreads)
		:Socket::TCPServer(inPort, inMaxConnections),
		mPort(inPort),
		mNbConnection(inMaxConnections),
		mNbThreads(inMaxThreads),
		mVerbose(inVerbose),
		mFile(inFileParameter)
{
	setCustomAction(SignalHandler::eSigInt);
	pushAction(SignalHandler::eSigInt);
	setCustomAction(SignalHandler::eSigTerm);
	pushAction(SignalHandler::eSigTerm);
	mMutexServer = new PACC::Threading::Mutex();
}


DAGS::Server::~Server()
{
	if(mMutexServer) delete mMutexServer;
	mMutexServer = 0;
	mVectorThreadConnection.erase(mVectorThreadConnection.begin(),mVectorThreadConnection.end());
}

void DAGS::Server::main(PACC::SignalHandler::SignalType inSignal)
{
	mMutexStop.lock();
	if(mServerStopped == false) {
		switch (inSignal) {
		case SIGINT:
			cout << "*** DAGS Agile Grid Scheduler : Received INTERACTIVE signal... ***" << endl;
			break;
		case SIGTERM:
			cout << "*** DAGS Agile Grid Scheduler : Received TERMINATION signal... ***" << endl;
			break;
		default:
			cout << "*** DAGS Agile Grid Scheduler : Received unknown signal... ***" << endl;
		}
		mServerStopped = true;
		stop();
		SignalHandler::popAction(SignalHandler::eSigInt);
		SignalHandler::popAction(SignalHandler::eSigTerm);
	}
	mMutexStop.unlock();
}

void DAGS::Server::start()
{
	mErrorCode = 1;
	std::vector<double> lWeightsVector(DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT, (double)1/DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT);
	mCSD.setDbPort(mPort);
	mCSD.setDbAddress(DAGS_DEFAULT_DB_ADDRESS);
	mCSD.setDatabaseName(DAGS_DEFAULT_DB_NAME);
	mCSD.setDbUser(DAGS_DEFAULT_DB_USER);
	mCSD.setDbPassword(DAGS_DEFAULT_DB_PASSWORD);
	mCSD.setEnableClientStat(DAGS_DEFAULT_ENABLE_CLIENT_STAT);
	mCSD.setNbJobsInSubGroup(DAGS_DEFAULT_NUMBER_JOBS_TO_CRUNCHER);
	mCSD.setErrorFile(DAGS_DEFAULT_LOG_ERROR_FILE);
	mCSD.setMessageFile(DAGS_DEFAULT_LOG_MESSAGE_FILE);
	mCSD.setNumberStatistics(DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT);
	mCSD.setIdealTime(DAGS_DEFAULT_EXPECTED_TIME_BETWEEN_COMMS);
	mCSD.setVectorWeightsStatistics(lWeightsVector);
	mCSD.setDatabaseSync(DAGS_DEFAULT_DB_SYNC);
	DAGS::CommonServerData::setCompressionLevels(DAGS_DEFAULT_SUBGROUP_COMPRESSION, DAGS_DEFAULT_GROUP_COMPRESSION);

	mIsInit = false;

	mErrorCode = readParameter();
	int lErrorCode = 0;
	//use a database
	if(mCSD.getDatabaseName() != "") {
		if(mVerbose >= 2) {
			cout << endl;
			cout << "---------------------- Database check -----------------------" << endl;
		}

		int lDBError = verifyDatabase();
		if(lDBError != 1) {
			mErrorCode = lDBError;
			if(mVerbose >= 2) cout << "-------------------------------------------------------------" << endl << endl;
			return;
		}

		//setting the groupVector of mCSD
		lErrorCode = setGroupVector();
	}

	if(mVerbose > 2) {
		std::ostringstream lOSSMessage;
		XML::Streamer lStreamer(lOSSMessage);
		lStreamer.openTag("DAGS");
		Date lDate;
		std::string lTimeString(lDate.get());
		lStreamer.insertAttribute("start", lTimeString);
		lStreamer.closeTag(); //close DAGS
		DAGS::CommonServerData::logMessages(lOSSMessage.str()); //stop the log messages
	}

	if(mVerbose >= 2 && mCSD.getDatabaseName() != "") cout << "-------------------------------------------------------------" << endl << endl << flush;

	//a problem in the database, return to exit.
	if(lErrorCode == -1 || mErrorCode == -1) return;

	if(mVerbose > 1) cout << "--> Press CTRL-C to cleanly terminate the server" << endl << endl;

	if(mVerbose > 0) cout  << "DAGS Server is ready to accept connections." << endl << endl << flush;

	//call the new function for the socket connection.
	//this function end when the server receive a signal to stop the server.
	try {
		Socket::TCPServer::run(mNbThreads);
		//Socket::TCPServer::acceptConnections();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		std::cout << "\nConnection problem.  Error code: " << inError.getErrorCode() << std::flush << std::endl;
	}
}

void DAGS::Server::main(int inDescriptor, const Socket::ServerThread* inThread)
{
	try {
		std::string lIPAddress;
		mMutexServer->lock();
		if(mServerStopped == true) {
			cout << "DAGS Server : The server will be terminated. Please wait..." << endl << flush;
			mMutexServer->unlock();
			//the server will stop waiting for connection
			return;
		}

		int lNbFreeThreadNumber = DAGS::CommonServerData::getFreeThreadNumberSize();
		if(lNbFreeThreadNumber > 0) {
			int lFreeThreadNumber = DAGS::CommonServerData::getFreeThreadNumber();

			delete mVectorThreadConnection[lFreeThreadNumber];
			mVectorThreadConnection[lFreeThreadNumber] = new DAGS::ThreadConnection(inDescriptor,lFreeThreadNumber,mCSD, mVerbose);
			DAGS::ThreadConnection* lThreadConnectionTemp = mVectorThreadConnection[lFreeThreadNumber];
			mMutexServer->unlock();
			if(mVerbose >= 2) {
				mMutexServer->lock();
				lIPAddress = lThreadConnectionTemp->getIpAddress();
				std::cout << "DAGS Server : Connection opened with " << lIPAddress
				<< " using [Thread# " << lFreeThreadNumber << "]" << std::endl << std::flush;
				mMutexServer->unlock();
			}
			lThreadConnectionTemp->start();
			DAGS::CommonServerData::addFreeThreadNumber(lFreeThreadNumber);
			if(mVerbose >= 3) {
				mMutexServer->lock();
				std::cout << "DAGS Server : Connection closed with " << lIPAddress
				<< " using [Thread# " << lFreeThreadNumber << "]" << std::endl << std::flush;
				mMutexServer->unlock();
			}
		} else {
			//less than number of allowed threads has been started a first time
			DAGS::ThreadConnection* lThreadConnectionTemp;
			mVectorThreadConnection.push_back(lThreadConnectionTemp);
			int lFreeThreadNumber = mVectorThreadConnection.size()-1;
			mVectorThreadConnection[lFreeThreadNumber] = new DAGS::ThreadConnection(inDescriptor,lFreeThreadNumber,mCSD, mVerbose);
			lThreadConnectionTemp = mVectorThreadConnection[lFreeThreadNumber];
			mMutexServer->unlock();
			if(mVerbose >= 2) {
				mMutexServer->lock();
				lIPAddress = lThreadConnectionTemp->getIpAddress();
				std::cout << "DAGS Server : Connection opened with " << lIPAddress
				<< " using [Thread# " << lFreeThreadNumber << "]" << std::endl << std::flush;
				mMutexServer->unlock();
			}
			lThreadConnectionTemp->start();
			DAGS::CommonServerData::addFreeThreadNumber(lFreeThreadNumber);
			if(mVerbose >= 3) {
				mMutexServer->lock();
				std::cout << "DAGS Server : Connection closed with " << lIPAddress
				<< " using [Thread# " << lFreeThreadNumber << "]" << std::endl << std::flush;
				mMutexServer->unlock();
			}
		}
	} //end of try

	catch (PACC::Threading::Exception &inError) {
		std::cout << "Thread Namespace problem: " << inError.getErrorCode()
		<< "\nThe message is: " << inError.getMessage() << std::endl << std::flush;
	} catch (Socket::Exception &inError) {
		std::cout << "DAGS::Server::main problem of socket: " << inError.getErrorCode()
		<< "\nThe message is: " << inError.getMessage() << std::endl << std::flush;
	}

	return;
}


bool DAGS::Server::isInit()
{
	return mIsInit;
}

int DAGS::Server::getErrorCode()
{
	int lTempInt = mErrorCode;
	mErrorCode = 1;
	return lTempInt;
}

void DAGS::Server::setVerbose(int inVerbose)
{
	if(inVerbose < 0) {
		mVerbose = 0;
	} else if(inVerbose > 4) {
		mVerbose = 4;
	} else {
		mVerbose = inVerbose;
	}
}

int DAGS::Server::getVerbose()
{
	return mVerbose;
}


void DAGS::Server::stop()
{
	try {
		if(mIsInit == false) return;

		mServerStopped = true;
		//stop listening to connection
		if(mVerbose >= 1) cout << "-> Stop listening to connection..." << flush;
		halt();
		if(mVerbose >= 1) cout << " done." << endl;

		switch (mVerbose) {
		case 1:
			cout << "-> Waiting for client connection to terminate...";
			break;
		case 2:
		case 3:
		case 4:
			if(mVectorThreadConnection.size() != (unsigned)mNbThreads)
				cout    << "-> Waiting for " << mVectorThreadConnection.size() - DAGS::CommonServerData::getFreeThreadNumberSize();
			else cout << "-> Waiting for " << mNbThreads - DAGS::CommonServerData::getFreeThreadNumberSize();
			cout << " client(s) connection(s) to terminate..." << endl;
			break;
		}
	} catch (PACC::Threading::Exception &inError) {
		std::cout << "Thread namespace problem: " << inError.getErrorCode()
		<< "\nThe message is: " << inError.getMessage() << std::endl << std::flush;
	}

	if(mVerbose > 2) {
		std::ostringstream lOSSMessage;
		XML::Streamer lStreamer(lOSSMessage);
		lStreamer.openTag("DAGS");
		Date lDate;
		std::string lTimeString(lDate.get());
		lStreamer.insertAttribute("finish", lTimeString);
		lStreamer.closeTag(); //close DAGS

		//loggin stats
		std::vector<DAGS::Distribution> lDistribution(DAGS::CommonServerData::getAllClientDistribution());
		lStreamer.openTag("Distribution");
		if(!lDistribution.empty()) {
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
				for(unsigned int j = 0; j < lStats.size(); ++j) {
					std::ostringstream lOSSStat;
					lOSSStat << "Stat" << j+1;
					lStreamer.openTag(lOSSStat.str());
					lStreamer.insertAttribute("value", lStats[(lStats.size()-1)-j]);
					lStreamer.closeTag();
				}
				lStreamer.closeTag(); //close Client
			}
		}
		//could not find clients in the distribution list, send an empty one
		else {
			lStreamer.openTag("NoStats");
			lStreamer.closeTag();
		}
		lStreamer.closeTag(); //close Distribution
		DAGS::CommonServerData::logMessages(lOSSMessage.str()); //stop the log messages
	}

	return;
}


int DAGS::Server::readParameter()
{
	if(mVerbose >= 1) {
		cout << endl << DAGS_SERVER_NAME_VERSION_STRING << endl << endl;
	}

	//database name setting (or empty if no database used)
	std::string lDbName("");
	mFile.getParameterValue(DAGS_DB_NAME, lDbName);
	mCSD.setDatabaseName(lDbName);

	//if client statistic (for load balancing) is used
	std::string lBoolString;
	if(mFile.getParameterValue(DAGS_ENABLE_CLIENT_STAT, lBoolString) == true);
	{
		(strcmp(lBoolString.c_str(), DAGS_TRUE) == 0) ? mCSD.setEnableClientStat(true) : mCSD.setEnableClientStat(false);
	}

	//number of jobs sent to the cruncher when starting the server of when no stats are used
	int lJobs = 0;
	if(mFile.getParameterValue(DAGS_NUMBER_JOBS_TO_CRUNCHER, lJobs) == true) {
		mCSD.setNbJobsInSubGroup(lJobs);
	}

	//log error filename
	std::string lErrorFileName;
	if(mFile.getParameterValue(DAGS_LOG_ERROR_FILE, lErrorFileName) == true) {
		mCSD.setErrorFile(lErrorFileName);
	}

	//log messages filename
	std::string lLogFileName;
	if(mFile.getParameterValue(DAGS_LOG_MESSAGE_FILE, lLogFileName) == true) {
		mCSD.setMessageFile(lLogFileName);
	}

	//expected time betwwen communications (clients to server)
	int lTime = 0;
	if(mFile.getParameterValue(DAGS_EXPECTED_TIME_BETWEEN_COMMS, lTime) == true) {
		mCSD.setIdealTime(lTime);
	}

	//timeout value before sending back the data to another client
	double lTimeout = 0.0;
	if(mFile.getParameterValue(DAGS_CLIENT_TIMEOUT, lTimeout) == true) {
		mCSD.setWaitPercent(lTimeout);
	}

	//number of statistics used
	int lNumberOfStats = 0;
	if(mFile.getParameterValue(DAGS_CLIENT_NUMBER_USED_STAT, lNumberOfStats) == true) {
		mCSD.setNumberStatistics(lNumberOfStats);
	}

	//set the value of each statistics
	std::vector<double> lStatsVector(lNumberOfStats,(double)1/lNumberOfStats);
	for(int i = 1; i <= lNumberOfStats; ++i) {
		double lStatsValue;
		std::ostringstream lOSS;
		lOSS << DAGS_CLIENT_STAT_WEIGHT << i;
		if(mFile.getParameterValue(lOSS.str(),lStatsValue) == true)
			lStatsVector[i-1] = lStatsValue;
		else
			cerr << "WARNING: SOME STATS VALUE WERE NOT DEFINED!" << endl;
	}
	mCSD.setVectorWeightsStatistics(lStatsVector);

	//value of database sync-ing
	int lDatabaseSyncValue = 100;
	if(mFile.getParameterValue(DAGS_DB_SYNC, lDatabaseSyncValue) == true) {
		mCSD.setDatabaseSync(lDatabaseSyncValue);
	}

	//if the memory short mode is activated
	std::string lMemoryShort;
	if(mFile.getParameterValue(DAGS_MEMORY_SHORT, lMemoryShort) == true) {
		(strcmp(lMemoryShort.c_str(), DAGS_TRUE) == 0) ? mCSD.setMemoryShortMode(true) : mCSD.setMemoryShortMode(false);
		DAGS::CommonServerData::setScoreSync(true);
	}

	//if the group client when returning data will be sync-ing in the database
	std::string lGroupDatabaseSync;
	if(mFile.getParameterValue(DAGS_GROUP_DB_SYNC, lGroupDatabaseSync) == true) {
		(strcmp(lGroupDatabaseSync.c_str(), DAGS_TRUE) == 0) ? mCSD.setGroupSyncMode(true) : mCSD.setGroupSyncMode(false);
	}

	//set communication compression level
	int lGroupCompression = DAGS_DEFAULT_GROUP_COMPRESSION;
	int lSubGroupCompression = DAGS_DEFAULT_SUBGROUP_COMPRESSION;
	mFile.getParameterValue(DAGS_GROUP_COMPRESSION, lGroupCompression);
	mFile.getParameterValue(DAGS_SUBGROUP_COMPRESSION, lSubGroupCompression);
	DAGS::CommonServerData::setCompressionLevels(lSubGroupCompression, lGroupCompression);

	if(mVerbose >= 2) {
		cout << endl;
		cout << "---------------------- Server settings ----------------------" << endl;
		cout << "Listening port              : " << mPort << endl;
		cout << "Simultaneous connections    : " << mNbConnection << endl;
		cout << "Max running threads         : " << mNbThreads    << endl;
		if(mCSD.getDatabaseName() != "")
			cout << "Database filename           : " << mCSD.getDatabaseName() << endl;
		else
			cout << "Database                    : " << "disabled" << endl;
		if(DAGS::CommonServerData::getGroupCompression()) {
			cout << "Group Compression level     : " << DAGS::CommonServerData::getGroupCompression() << endl;
		} else cout << "Group Compression level     : disabled" << endl;
		if(DAGS::CommonServerData::getSubGroupCompression()) {
			cout << "SubGroup Compression level  : " << DAGS::CommonServerData::getSubGroupCompression() << endl;
		} else cout << "SubGroup Compression level  : disabled"  << endl;
		cout << "Jobs to cruncher per cycle  : " << mCSD.getNbJobsInSubGroup() << endl;
		if(mCSD.getWaitPercent() == 0)
			cout << "Timeout                     : " << "disabled" << endl;
		else
			cout << "Timeout                     : " << mCSD.getWaitPercent() << endl;
		cout << "Database sync percent       : " << mCSD.getDatabaseSync() << endl;
		cout << "Memory Short mode           : " << ((mCSD.getMemoryShortMode() == true) ? "enabled" : "disabled") << endl;
		cout << "Database group sync         : " << ((mCSD.getGroupSyncMode() == true) ? "enabled" : "disabled") << endl;
		cout << "Load balancing              : " << ((mCSD.getEnableClientStat() == true) ? "enabled" : "disabled") << endl;
		if(mCSD.getEnableClientStat() == true) {
			cout <<"Expected time between comms : " << mCSD.getIdealTime() << " sec." << endl;
			cout <<"Number of statistics used   : " << mCSD.getNumberStatistics() << endl;
			for(unsigned int i = 0; i < lStatsVector.size(); ++i) {
				cout <<"Weight of stat. " << i+1 << "           : " << lStatsVector[i] << endl;
			}
		}
		cout << "Errors log filename         : " << mCSD.getErrorFile() << endl;
		cout << "Messages log filename       : " << mCSD.getMessageFile() << endl;
		cout << "-------------------------------------------------------------" << endl << endl;
	}

	if(mCSD.getDatabaseName() == "" && mCSD.getMemoryShortMode() == true) {
		cout << "CANNOT use option " << DAGS_MEMORY_SHORT << " mode at true without a database... please, specify a name for the database." << endl;
		return -1;
	}

	if(mCSD.getGroupSyncMode() == false && mCSD.getDatabaseSync() < 100) {
		cout << "Disabling option " << DAGS_GROUP_DB_SYNC << " and using lower than 100 value for "
		<< DAGS_DB_SYNC << " can corrupt the population states in case of a server restart."
		<< " Use at your own risk!" << endl;
	}

	mIsInit = true;
	return 1;
}

int DAGS::Server::verifyDatabase()
{
	if(mVerbose == 1) {
		std::cout << "Checking for database integrity..." << std::flush;
	}
	if(mVerbose >= 2) {
		std::cout << "Checking if Database can be opened..." << std::flush;
	}

	SQLQuery lDatabase(mCSD.getDatabaseName());

	try {
		lDatabase.openConnection();
	} catch (std::runtime_error & inError) {
		std::cout << inError.what() << std::endl;
		return CannotConnectToDatabaseError;
	}

	//set the cache_size 10 megs of memory footprint
	//seam to slow down if not default value.
	//lDatabase.setCacheSize(10000);

	try {
		if(mVerbose >= 2) {
			std::cout << " done" << std::flush << std::endl;
			std::cout << "Checking if the tables exist... "<< std::flush;
		}

		std::ostringstream lQueryOSS;
		lQueryOSS << "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
		lDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");

		if(lDatabase.getNumberLines() != 0) {
			if(mVerbose >= 2) {
				cout << "Some tables already exist!" << endl;
				cout << "Verifying the validity of the tables...";
			}
			for(unsigned int i = 0; i < lDatabase.getNumberRows(); ++i) {
				std::string lTableName = lDatabase.getRowName(i,0);
				if(lTableName == DAGS_GROUP_TABLE_NAME) {
					if(mVerbose >= 3) {
						cout << endl << " Table 'groups' found. ";
					}
				} else if(lTableName == DAGS_JOBS_TABLE_NAME) {
					if(mVerbose >= 3) {
						cout << endl << " Table 'jobs' found. ";
					}
				} else {
					cout << " Table (" << lTableName << ") is unknown to dags-server." << endl;
					lDatabase.closeConnection();
					return CannotConnectToDatabaseError;
				}
			}
			if(mVerbose == 1 || mVerbose == 2) cout << " done" << endl;
			else if(mVerbose >= 3) cout << endl;
			lDatabase.closeConnection();
			return 1;
		}
	} catch (std::runtime_error & inError) {
		std::cout << inError.what() << std::endl;
		return CannotConnectToDatabaseError;
	}


	//create the Tables
	try {
		//table distribution
		std::ostringstream lQueryOSS;

		//table group
		lQueryOSS << "CREATE TABLE "
		<< DAGS_GROUP_TABLE_NAME
		<< " ("
		<< DAGS_GROUP_FIELD_ENVIRONMENT << " " << "blob"                << ","
		<< DAGS_GROUP_FIELD_GENERATION  << " " << "int unsigned"        << ","
		<< DAGS_GROUP_FIELD_NB_JOBS     << " " << "int unsigned"        << ","
		<< DAGS_GROUP_FIELD_DISTRIBUTE  << " " << "blob"                << ","
		<< DAGS_GROUP_FIELD_APPNAME     << " " << "blob"                << ","
		<< DAGS_GROUP_FIELD_VERSION     << " " << "blob"                << ","
		<< DAGS_GROUP_FIELD_DB_GROUP_ID << " " << "INTEGER PRIMARY KEY"
		<< ");";
		lDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");

		//table JOBS
		lQueryOSS << "CREATE TABLE "
		<< DAGS_JOBS_TABLE_NAME
		<< " ("
		<< DAGS_JOBS_FIELD_DB_GROUP_ID   << " " << "int unsigned"     << ","
		<< DAGS_JOBS_FIELD_DATA          << " " << "blob"             << ","
		<< DAGS_JOBS_FIELD_SCORE         << " " << "blob"             << ","
		<< DAGS_JOBS_FIELD_USER_JOB_ID   << " " << "int unsigned"     << ","
		<< DAGS_JOBS_FIELD_INVALID_SCORE << " " << "blob"             << ","
		<< DAGS_JOBS_FIELD_DB_JOB_ID     << " " << "INTEGER PRIMARY KEY"
		<< ");";
		lDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");

	} //end of try to create table
	catch (std::runtime_error & inError) {
		if(mVerbose >= 2) {
			std::cout << "table already exist!" << std::flush << std::endl;
		}
		lDatabase.closeConnection();
		return 1;
	}

	if(mVerbose >= 2) {
		std::cout << "done"<< std::flush << std::endl;
		cout << "Checking for Indexes and Keys in tables... "<<flush;
	}

	try {
		//index for the Job table
		std::ostringstream lQueryOSS;

		lQueryOSS << "CREATE UNIQUE INDEX "
		<< DAGS_JOBS_INDEX
		<< " ON "
		<< DAGS_JOBS_TABLE_NAME
		<< "("
		<< DAGS_JOBS_FIELD_DB_GROUP_ID   << ", "
		<< DAGS_JOBS_FIELD_USER_JOB_ID
		<< ");";
		lDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");

		//index for the Group table
		lQueryOSS << "CREATE UNIQUE INDEX "
		<< DAGS_GROUP_INDEX
		<< " ON "
		<< DAGS_GROUP_TABLE_NAME
		<< "("
		<< DAGS_GROUP_FIELD_DB_GROUP_ID
		<< ");";
		lDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");
	} catch (std::runtime_error & inError) {
		std::cout << "FAILED!" << std::endl << inError.what() << std::endl;
		lDatabase.closeConnection();
		return CannotCreateIndexError;
	}

	if(mVerbose >= 2) {
		cout << "done" << endl;
	}

	lDatabase.closeConnection();
	return 1;
}


int DAGS::Server::setGroupVector()
{
	SQLQuery lDatabase(mCSD.getDatabaseName());
	try {
		lDatabase.openConnection();
	} catch (std::runtime_error & inError) {
		std::cout << inError.what() << std::endl;
		return -1;
	}

	if(mVerbose >= 1) {
		cout << "Verifying the SQL file... " << flush;
	}

	try {
		std::ostringstream lQueryOSS;

		// take the information of the current groups, to lower the SQL query numbers
		// during thread connections
		lQueryOSS << "SELECT "
		<< DAGS_GROUP_FIELD_DB_GROUP_ID << ", "
		<< DAGS_GROUP_FIELD_ENVIRONMENT << ", "
		<< DAGS_GROUP_FIELD_GENERATION  << ", "
		<< DAGS_GROUP_FIELD_NB_JOBS     << ", "
		<< DAGS_GROUP_FIELD_DISTRIBUTE  << ", "
		<< DAGS_GROUP_FIELD_APPNAME     << ", "
		<< DAGS_GROUP_FIELD_VERSION
		<< " FROM "
		<< DAGS_GROUP_TABLE_NAME
		<< " ORDER BY "
		<< DAGS_GROUP_FIELD_DB_GROUP_ID
		<< ";";
		lDatabase.executeQuery(lQueryOSS.str());
		lQueryOSS.str("");
		if(lDatabase.getNumberLines() == 0) {
			if(mVerbose >= 2) {
				cout << " done.  No group present." << endl;
			}
			lDatabase.closeConnection();
			return 1;
		}

		if(mVerbose >= 2) {
			cout << lDatabase.getNumberLines() << " group(s) found. " << flush;
		}

		std::vector<DAGS::Group> lGroupVector;
		for (unsigned int i = 0; i < lDatabase.getNumberLines(); ++i) {
			DAGS::Group lGroup;
			lGroup.setDBId(atoi((*lDatabase.getValue(i,DAGS_GROUP_FIELD_DB_GROUP_ID)).c_str())-1);
			lGroup.setEnvironment(*lDatabase.getValue(i,DAGS_GROUP_FIELD_ENVIRONMENT));
			lGroup.setGeneration(atoi((*lDatabase.getValue(i,DAGS_GROUP_FIELD_GENERATION)).c_str()));
			lGroup.setNbJobs(atoi((*lDatabase.getValue(i,DAGS_GROUP_FIELD_NB_JOBS)).c_str()));
			if(atoi((*lDatabase.getValue(i,DAGS_GROUP_FIELD_DISTRIBUTE)).c_str()) == 1) lGroup.setDistributeEnv(true);
			lGroup.setAppName(*lDatabase.getValue(i,DAGS_GROUP_FIELD_APPNAME));
			DAGS::CommonServerData::setAppName(*lDatabase.getValue(i,DAGS_GROUP_FIELD_APPNAME));
			DAGS::CommonServerData::setDataVersion(*lDatabase.getValue(i,DAGS_GROUP_FIELD_VERSION));
			lGroup.setStatus(0); //ready to be taken by a group request
			lGroup.setCounter(0);

			lGroupVector.push_back(lGroup);
		}

		mCSD.setGroupVector(lGroupVector);

		//create the JOBS counters
		for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
			//I first need the number of JOBS in the group
			int lNbJobs = lGroupVector[i].getNbJobs();
			if(lNbJobs > 0)
				DAGS::CommonServerData::setJobsCounters(lGroupVector[i].getDBId(), lNbJobs);
		}

		//create the score counters
		for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
			//I first need the number of JOBS in the group
			int lNbJobs = lGroupVector[i].getNbJobs();
			if(lNbJobs > 0)
				DAGS::CommonServerData::setScoreCounters(lGroupVector[i].getDBId(), lNbJobs);
		}

		//creating the vector of Score + Data, and the ScoreReceived(bool) Vector
		for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
			DAGS::CommonServerData::createTable(lGroupVector[i].getNbJobs(), i);
			//create the time table
			DAGS::CommonServerData::createTimeTable(lGroupVector[i].getNbJobs());
		}

		//if the request for a group also sync with the database
		if(mCSD.getGroupSyncMode()) {
			//it means that some jobs score won't be valid with the corresponding data.
			//find those jobs, and evaluate them first before sending them back to
			//a group's request.

			//memory is short, just take the first group
			if(mCSD.getMemoryShortMode()) {
				lQueryOSS << "SELECT "
				<< DAGS_JOBS_FIELD_DB_GROUP_ID
				<< " FROM "
				<< DAGS_JOBS_TABLE_NAME
				<< " Where "
				<< DAGS_JOBS_FIELD_INVALID_SCORE << "='" << true << "';";
				lDatabase.executeQuery(lQueryOSS.str());
				lQueryOSS.str("");

				//every individuals are ready to be taken by a group request
				//since we are in a memory short mode, take the first one.
				if(lDatabase.getNumberLines() == 0) {
					lQueryOSS << "SELECT "
					<< DAGS_JOBS_FIELD_DATA          << ", "
					<< DAGS_JOBS_FIELD_SCORE         << ", "
					<< DAGS_JOBS_FIELD_USER_JOB_ID
					<< " FROM "
					<< DAGS_JOBS_TABLE_NAME
					<< " WHERE "
					<< DAGS_JOBS_FIELD_DB_GROUP_ID << "='" << lGroupVector[0].getDBId() << "';";
					lDatabase.executeQuery(lQueryOSS.str());
					lQueryOSS.str("");

					//add them to the list of JOBS
					std::vector<std::pair<string, DAGS::Job> > lVectorJobs;
					for(unsigned int j = 0; j < lDatabase.getNumberLines(); ++j) {
						DAGS::Job lJob;
						lJob.setId(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
						lJob.setScore(*lDatabase.getValue(j,DAGS_JOBS_FIELD_SCORE));
						lJob.setData(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA));
						lVectorJobs.push_back(std::pair<string, DAGS::Job>("", lJob));

						if(lJob.getData() == "") {
							cout << endl << "--> The group number " << lGroupVector[0].getDBId() << " is invalid in the database.  \n--> Please erase it first." << endl;
							lDatabase.closeConnection();
							return -1;
						}

					}
					std::list<int> lEmptyScore;
					DAGS::CommonServerData::setJobs(lGroupVector[0].getDBId(), lVectorJobs, lEmptyScore);
					//put the score and jobs counters to zero of every group since they are all have a valid score
					for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
						DAGS::CommonServerData::setGroupReady(lGroupVector[i].getDBId());
						int lNbJobs = lGroupVector[i].getNbJobs();
						DAGS::CommonServerData::setScoreCounters(lGroupVector[i].getDBId(), -lNbJobs);
						DAGS::CommonServerData::setJobsCounters(lGroupVector[i].getDBId(), -lNbJobs);
					}
				}
				//take only the first group.
				else {
					//group's id
					unsigned int lGroupId = atoi((*lDatabase.getValue(0,DAGS_GROUP_FIELD_DB_GROUP_ID)).c_str());

					lQueryOSS << "SELECT "
					<< DAGS_JOBS_FIELD_DATA          << ", "
					<< DAGS_JOBS_FIELD_SCORE         << ", "
					<< DAGS_JOBS_FIELD_USER_JOB_ID   << ", "
					<< DAGS_JOBS_FIELD_INVALID_SCORE
					<< " FROM "
					<< DAGS_JOBS_TABLE_NAME
					<< " WHERE "
					<< DAGS_JOBS_FIELD_DB_GROUP_ID << "='" << lGroupId << "';";
					lDatabase.executeQuery(lQueryOSS.str());
					lQueryOSS.str("");

					//add them to the list of JOBS
					std::vector<std::pair<string, DAGS::Job> > lVectorJobs;
					std::list<int> lScoreList;
					for(unsigned int j = 0; j < lDatabase.getNumberLines(); ++j) {
						DAGS::Job lJob;
						lJob.setId(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
						//the score is valid, take it
						if(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_INVALID_SCORE)).c_str()) == 0)
							lJob.setScore(*lDatabase.getValue(j,DAGS_JOBS_FIELD_SCORE));
						else lScoreList.push_back(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
						lJob.setData(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA));

						//construct the string
						std::ostringstream lXMLJob;
						if(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_INVALID_SCORE)).c_str()) == 1) {
							XML::Streamer lStreamer(lXMLJob, 0);
							lStreamer.openTag("J"); //Job
							lStreamer.insertAttribute("id", atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str())); //job's id
							lStreamer.openTag("Sc"); //score
							lStreamer.insertAttribute("eval", "yes");
							lStreamer.closeTag();
							lStreamer.openTag("Dt"); //Data
							lStreamer.insertStringContent(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA), false);
							lStreamer.closeTag(); //close Data
							lStreamer.closeTag(); //close Job
						}

						lVectorJobs.push_back(std::pair<string, DAGS::Job>(lXMLJob.str(), lJob));

						if(lJob.getData() == "") {
							cout << endl << "--> The group number " << lGroupId << " is invalid in the database.  \n--> Please erase it first." << endl;
							lDatabase.closeConnection();
							return -1;
						}
					}
					DAGS::CommonServerData::setJobs(lGroupId, lVectorJobs, lScoreList);
					DAGS::Group lGroup(DAGS::CommonServerData::getCommonGroup(lGroupId));
					if(lScoreList.size() > 0) {
						lGroup.setStatus(2);
						lGroup.setNbScoreNeeded(lScoreList.size());
					}
					DAGS::CommonServerData::setCommonGroup(lGroup, lGroupId);

					//put the score and jobs counters of every group to what it should be
					if(lScoreList.size() == 0) {
						DAGS::CommonServerData::setGroupReady(lGroupVector[lGroupId].getDBId());
					} else {
						int lNbJobs = lGroupVector[lGroupId].getNbJobs();
						DAGS::CommonServerData::setScoreCounters(lGroupVector[lGroupId].getDBId(), -lNbJobs + lScoreList.size());
						DAGS::CommonServerData::setJobsCounters(lGroupVector[lGroupId].getDBId(), -lNbJobs + lScoreList.size());
					}
				}
			}
			//there is plenty of memory, take every individuals of every groups
			else {
				//take every group ID that has invalid jobs's score to modify their counters
				for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
					lQueryOSS << "SELECT "
					<< DAGS_JOBS_FIELD_DATA          << ", "
					<< DAGS_JOBS_FIELD_SCORE         << ", "
					<< DAGS_JOBS_FIELD_USER_JOB_ID   << ", "
					<< DAGS_JOBS_FIELD_INVALID_SCORE
					<< " FROM "
					<< DAGS_JOBS_TABLE_NAME
					<< " WHERE "
					<< DAGS_JOBS_FIELD_DB_GROUP_ID << "='" << lGroupVector[i].getDBId() << "';";
					lDatabase.executeQuery(lQueryOSS.str());
					lQueryOSS.str("");

					//add them to the list of JOBS
					std::vector<std::pair<string, DAGS::Job> > lVectorJobs;
					std::list<int> lScoreList;
					for(unsigned int j = 0; j < lDatabase.getNumberLines(); ++j) {
						DAGS::Job lJob;
						lJob.setId(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
						//the score is valid, take it
						if(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_INVALID_SCORE)).c_str()) == 0)
							lJob.setScore(*lDatabase.getValue(j,DAGS_JOBS_FIELD_SCORE));
						else lScoreList.push_back(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
						lJob.setData(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA));

						//construct the string
						std::ostringstream lXMLJob;
						if(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_INVALID_SCORE)).c_str()) == 1) {
							XML::Streamer lStreamer(lXMLJob, 0);
							lStreamer.openTag("J"); //Job
							lStreamer.insertAttribute("id", atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str())); //job's id
							lStreamer.openTag("Sc"); //score
							lStreamer.insertAttribute("eval", "yes");
							lStreamer.closeTag();
							lStreamer.openTag("Dt"); //Data
							lStreamer.insertStringContent(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA), false);
							lStreamer.closeTag(); //close Data
							lStreamer.closeTag(); //close Job
						}

						lVectorJobs.push_back(std::pair<string, DAGS::Job>(lXMLJob.str(), lJob));
						if(lJob.getData() == "") {
							cout << endl << "--> The group number " << lGroupVector[i].getDBId() << " is invalid in the database.  \n--> Please erase it first." << endl;
							lDatabase.closeConnection();
							return -1;
						}
					}
					DAGS::CommonServerData::setJobs(lGroupVector[i].getDBId(), lVectorJobs, lScoreList);
					DAGS::Group lGroup(DAGS::CommonServerData::getCommonGroup(lGroupVector[i].getDBId()));
					if(lScoreList.size() > 0) {
						lGroup.setStatus(2);
						lGroup.setNbScoreNeeded(lScoreList.size());
					}
					DAGS::CommonServerData::setCommonGroup(lGroup, lGroupVector[i].getDBId());
					if(lScoreList.size() == 0) {
						DAGS::CommonServerData::setGroupReady(lGroupVector[i].getDBId());
					} else {
						int lNbJobs = lGroupVector[i].getNbJobs();
						DAGS::CommonServerData::setScoreCounters(lGroupVector[i].getDBId(), -lNbJobs + lScoreList.size());
						DAGS::CommonServerData::setJobsCounters(lGroupVector[i].getDBId(), -lNbJobs + lScoreList.size());
					}
				}
			}
		}
		//the database Data and Score are valid with each others
		else {
			//memory is short, just take the first group
			if(mCSD.getMemoryShortMode()) {
				lQueryOSS << "SELECT "
				<< DAGS_JOBS_FIELD_DATA        << ", "
				<< DAGS_JOBS_FIELD_SCORE       << ", "
				<< DAGS_JOBS_FIELD_USER_JOB_ID
				<< " FROM "
				<< DAGS_JOBS_TABLE_NAME
				<< " WHERE "
				<< DAGS_JOBS_FIELD_DB_GROUP_ID << "='" << lGroupVector[0].getDBId() << "';";
				lDatabase.executeQuery(lQueryOSS.str());
				lQueryOSS.str("");

				//add them to the list of JOBS
				std::vector<std::pair<string, DAGS::Job> > lVectorJobs;
				std::list<int> lScoreList;
				for(unsigned int j = 0; j < lDatabase.getNumberLines(); ++j) {
					DAGS::Job lJob;
					lJob.setId(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
					//the score is valid, take it
					lJob.setScore(*lDatabase.getValue(j,DAGS_JOBS_FIELD_SCORE));
					lJob.setData(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA));
					lVectorJobs.push_back(std::pair<string, DAGS::Job>("", lJob));
					if(lJob.getData() == "") {
						cout << endl << "--> The group number " << lGroupVector[0].getDBId() << " is invalid in the database.  \n--> Please erase it first." << endl;
						lDatabase.closeConnection();
						return -1;
					}
				}
				DAGS::CommonServerData::setJobs(lGroupVector[0].getDBId(), lVectorJobs, lScoreList);
				for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
					int lNbJobs = lGroupVector[i].getNbJobs();
					DAGS::CommonServerData::setScoreCounters(lGroupVector[i].getDBId(), -lNbJobs);
					DAGS::CommonServerData::setJobsCounters(lGroupVector[i].getDBId(), -lNbJobs);
					DAGS::CommonServerData::setGroupReady(lGroupVector[i].getDBId());
				}
			}
			//memory is not short, take every groups/jobs
			else {
				for(unsigned int i = 0; i < lGroupVector.size(); ++i) {
					lQueryOSS << "SELECT "
					<< DAGS_JOBS_FIELD_DATA        << ", "
					<< DAGS_JOBS_FIELD_SCORE       << ", "
					<< DAGS_JOBS_FIELD_USER_JOB_ID
					<< " FROM "
					<< DAGS_JOBS_TABLE_NAME
					<< " WHERE "
					<< DAGS_JOBS_FIELD_DB_GROUP_ID << "='" << lGroupVector[i].getDBId() << "';";

					lDatabase.executeQuery(lQueryOSS.str());
					lQueryOSS.str("");

					//add them to the list of JOBS
					std::vector<std::pair<string, DAGS::Job> > lVectorJobs;
					std::list<int> lScoreList;
					for(unsigned int j = 0; j < lDatabase.getNumberLines(); ++j) {
						DAGS::Job lJob;
						lJob.setId(atoi((*lDatabase.getValue(j,DAGS_JOBS_FIELD_USER_JOB_ID)).c_str()));
						//the score is valid, take it
						lJob.setScore(*lDatabase.getValue(j,DAGS_JOBS_FIELD_SCORE));
						lJob.setData(*lDatabase.getValue(j,DAGS_JOBS_FIELD_DATA));
						lVectorJobs.push_back(std::pair<string, DAGS::Job>("", lJob));
						if(lJob.getData() == "") {
							cout << endl << "--> The group number " << lGroupVector[i].getDBId() << " is invalid in the database.  \n--> Please erase it first." << endl;
							lDatabase.closeConnection();
							return -1;
						}
					}
					DAGS::CommonServerData::setJobs(lGroupVector[i].getDBId(), lVectorJobs, lScoreList);
					DAGS::CommonServerData::setGroupReady(lGroupVector[i].getDBId());
					int lNbJobs = lGroupVector[i].getNbJobs();
					DAGS::CommonServerData::setScoreCounters(lGroupVector[i].getDBId(), -lNbJobs);
					DAGS::CommonServerData::setJobsCounters(lGroupVector[i].getDBId(), -lNbJobs);
				}
			}
		}

	}//end of try

	catch (std::runtime_error & inError) {
		std::cout << "FAILED! " << std::endl << inError.what() << std::endl;
		return -1;
	}

	if(mVerbose >= 1) cout << " done" << endl;
	lDatabase.closeConnection();
	return 1;
}



