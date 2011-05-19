/*
 *  CommonServerData.hpp
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

#ifndef __COMMON_SERVER_DATA_DAGS_HH
#define __COMMON_SERVER_DATA_DAGS_HH

#include <vector>
#include <list>
#include <stack>
#include <string>
#include "dags/Group.hpp"
#include "dags/Job.hpp"
#include "dags/Distribution.hpp"
#include "PACC/Threading.hpp"
#include "PACC/XML/Streamer.hpp"

using namespace PACC;

namespace DAGS
{

/*!
*  \class CommonServerData CommonServerData.hpp "CommonServerData.hpp"
*  \brief Share data between threads.
*
*  Used by DAGS::ThreadConnection and DAGS::Server to share common data
*  between them.  This include all information about the database (port, address,
*  user, password, database name) and all the necessary mutex needed by the
*  threads (mutex for each Universe, mutex for accesing shared data (the list
*  of Universe mutex) or shared ressource(log error file)). This class have
*  static members and static functions, so there is no need to create an object
*  of this class to use shared data.
*/

class CommonServerData
{

public:

	/*! \brief Default constructor.
	*/
	CommonServerData();

	/*! \brief Destructor
	*/
	~CommonServerData();


	/*! \brief Copy constructor.
	*/
	CommonServerData(const CommonServerData& inCommonServerData);


	/*! \brief Log errors in a file.
	*
	*  This function logs errors in a file.  The error must be put in a string (std::string)
	*  and this function will add information before and after the string and then put all that
	*  in the log errors file.  The informations added are lines to separate errors and the data
	*  and the time when the error is logged. See logMessages() for messages logged
	*  by the degree of verbosity.
	*  \param inName: where the error was catched
	*  \param inErrorMessage: the string containing the error that will be logged.
	*  \param inThreadId: thread's id that had an error
	*  \param inIPAddress: the ip address of the client connected to the server
	*/
	static void logErrors(std::string inName,
	                      std::string inErrorMessage,
	                      int inThreadId = -1,
	                      std::string inIPAddress = "");

	/*! \brief Log messages in a file.
	*
	*  This function logs messages in a file.  The message must be put in a string (std::string)
	*  and this function will add information before and after the string and then put all that
	*  in the log messages file.  The informations added are lines to separate messages and the data
	*  and the time when the message is logged.
	*  \param inMessage: the string containing the message that will be logged.
	*/
	static void logMessages(std::string inMessage);


	/*! \brief Log messages on the console
	*
	*  This function logs messages on the cerr output.  The message must be put in a string
	*  and this function will add information before and after the string and then put all that
	*  in the log messages file.  The informations added are the time that it happened and
	*  a little arrow.
	*  \param inMessage: the string containing the message that will be logged on the console.
	*/
	static void logConsole(std::string inMessage);

	/*! \brief Get a mutex for a Universe
	*
	*  This function returns a pointer to a mutex associated with a Universe.
	*  If the specified Universe does not exist it will be created automatically (this is done by
	*  the function setUniverseMutex())
	*  \return a mutex pointer
	*/
	static PACC::Threading::Mutex* getUniverseMutex();

	/*! \brief Create Mutex for a Universe.
	*
	*  A function that add a mutex for the specified Universe.
	*/
	static void setUniverseMutex();

	/*! \brief return the database's name.
	*/
	std::string getDatabaseName()
	{
		return mDatabaseName;
	}

	/*! \brief Set the database's name.
	*/
	void setDatabaseName(std::string inDatabaseName)
	{
		mDatabaseName = inDatabaseName;
	}


	/*! \brief return the database's port.
	*
	*  If MySQL is supported someday, this function will be useful.
	*/
	int getDbPort()
	{
		return mDbPort;
	}

	/*! \brief Set the database's port.
	*
	*  If MySQL is supported someday, this function will be useful.
	*/
	void setDbPort(int inDbPort)
	{
		mDbPort = inDbPort;
	}

	/*! \brief Get the IP address of the database.
	*
	*  If MySQL is supported someday, this function will be useful.
	*/
	std::string getDbAddress()
	{
		return mDbAddress;
	}

	/*! \brief Set database IP address.
	*
	*  If MySQL is supported someday, this function will be useful.
	*/
	void setDbAddress(std::string inDbAddress)
	{
		mDbAddress = inDbAddress;
	}

	/*! \brief get the database user's name.
	*
	*  If MySQL is supported someday, this function will be useful.
	*/
	std::string getDbUser()
	{
		return mDbUser;
	}

	/*! \brief Set the database user's name.
	*
	*  If MySQL is supported someday, this function will be useful.
	*/
	void setDbUser(std::string inDbUser)
	{
		mDbUser = inDbUser;
	}

	/*! \brief Get database's password.
	*
	*/
	std::string getDbPassword()
	{
		return mDbPassword;
	}

	/*! \brief Set database password.
	*
	*  Not supported in SQLite for the moment.
	*/
	void setDbPassword(std::string inDbPassword)
	{
		mDbPassword = inDbPassword;
	}

	/*! \brief Return the default number of Jobs that are sent to clients.
	*
	*  \return a number of Jobs.
	*  \sa setNbJobsInSubGroup()
	*/
	int getNbJobsInSubGroup()
	{
		return mNbJobsInSubGroup;
	}

	/*! \brief Set the default number of Jobs that will be sent to clients.
	*
	*  \param inNbJobsInSubGroup: number of Jobs.
	*  \sa getNbJobsInSubGroup()
	*/
	void setNbJobsInSubGroup(int inNbJobsInSubGroup)
	{
		mNbJobsInSubGroup = inNbJobsInSubGroup;
	}

	/*! \brief Return if the server use clients statistics for load balancing.
	*
	*  \return TRUE if enabled (use client load balancing), otherwise FALSE.
	*  \sa setNbJobsInSubGroup()
	*/
	bool getEnableClientStat()
	{
		return mEnableClientStat;
	}

	/*! \brief Enable or disable clients load balancing.
	*
	*  \param inEnableClientStat TRUE to enable (use client load balancing), else FALSE.
	*  \sa getNbJobsInSubGroup()
	*/
	void setEnableClientStat(bool inEnableClientStat)
	{
		mEnableClientStat = inEnableClientStat;
	}

	/*! \brief Add a free Thread ID number on the stack.
	*
	*  This function is called by a DAGS::ThreadConnection for adding its thread number on the stack when
	*  it have finish its task.  It's useful when the degree of verbosity is greater than
	*  2.  This is an approximation of threads that are in use.
	*  \param inFreeThreadNumber : Thread ID number
	*/
	static void addFreeThreadNumber(int inFreeThreadNumber);

	/*! \brief Get a free Thread ID number.
	*
	*  This function is called by a DAGS::Server for getting a free Thread ID number to use.
	*  \return Thread ID number not used anymore.
	*/
	static int getFreeThreadNumber();

	/*! \brief Return the number of free Thread ID number.
	*
	*  This function is called by a DAGS::Server for getting the number of free Thread ID number.
	*  \return number of free Threads.
	*/
	static long getFreeThreadNumberSize();

	/*! \brief Set the error file name
	*
	*  This method is used to set the name and the path to the log error file.
	*  This function is NOT protected by a mutex, so it must be called when there is only
	*  one thread running or when it is sure that no thread will call the function logErrors().
	*  \param inFile: file's name.
	*  \sa logErrors()
	*/
	static void setErrorFile(std::string inFile)
	{
		mLogErrorFile = inFile;
	}

	/*! \brief get the error file's name
	*
	*  \return the error file's name
	*/
	static std::string getErrorFile()
	{
		return mLogErrorFile;
	}

	/*! \brief Set the Message file's name
	*
	*  This method is used to set the name and the path to the log error file.
	*  This function is NOT protected by a mutex, so it must be called when there is only
	*  one thread running or when it is sure that no thread will call the function logErrors().
	*  \param inFile: file's name.
	*  \sa logMessages()
	*/
	static void setMessageFile(std::string inFile)
	{
		mLogMessageFile = inFile;
	}

	/*! \brief Get the message file's name
	*
	*  \return the message file's name
	*/
	static std::string getMessageFile()
	{
		return mLogMessageFile;
	}

	/*! \brief Get number of statistics in load balancing.
	*
	*  \return the number of statistics used (1-5)
	*/
	int getNumberStatistics()
	{
		return mNumberStatistics;
	}

	/*! \brief Set number of statistics in load balancing.
	*
	*  \param inNumberStatistics: the number of statistics used (1-5)
	*/
	void setNumberStatistics(int inNumberStatistics)
	{
		mNumberStatistics = inNumberStatistics;
	}

	/*! \brief Get ideal compute time for client.
	*
	*  \return number of seconds clients should have finish evaluating fitness. Used in a load balancing environment.
	*/
	int getIdealTime()
	{
		return mIdealTime;
	}

	/*! \brief Set ideal compute time for client
	*
	*  \param inIdealTime: number of seconds clients should take before calling back the server.
	*/
	void setIdealTime(int inIdealTime)
	{
		mIdealTime = inIdealTime;
	}

	/*! \brief Get statistics weight in load balancing environment.
	*
	*  The vector contains informations on how to calculate the load balancing. It's based
	*  on pondered statistics of the time it took the client to evaluate the Jobs
	*  it received.
	*  \return values used for load balancing. Value range between 0 and 1, 1 means that
	*   statistic is the only one used.  All values added together is 1.
	*/
	std::vector<double> getVectorWeightsStatistics()
	{
		return mVectorWeightsStatistics;
	}

	/*! \brief Set statistics weight in load balancing environment.
	*
	*  \param inVectorWeightsStatistics: vector of pondered statistics.
	*/
	void setVectorWeightsStatistics(std::vector<double> inVectorWeightsStatistics)
	{
		mVectorWeightsStatistics = inVectorWeightsStatistics;
	}

	/*! \brief get percent needed before syncing jobs in Database
	*
	*  \return percent between 0 and 100. 0 beeing a parano mode.
	*/
	unsigned int getDatabaseSync()
	{
		return mDatabaseSync;
	}

	/*! \brief Set percent needed before syncing jobs in Database
	*
	*  \param inPercent: percent between 0 and 100. 0 beeing a parano mode.
	*/
	void setDatabaseSync(unsigned int inPercent)
	{
		mDatabaseSync = inPercent;
	}


	/*! \brief Get if the memory is short mode
	*
	*  \return mMemoryShort: true means that the memory is indeed short, false otherwise
	*/
	bool getMemoryShortMode()
	{
		return mMemoryShort;
	}

	/*! \brief Set if the memory is short mode
	*
	*  \param inMemoryShort: true means that the memory is indeed short, false otherwise
	*/
	void setMemoryShortMode(bool inMemoryShort)
	{
		mMemoryShort = inMemoryShort;
	}

	/*! \brief Get if the group must be sync-ed.
	*
	*  \return mMemoryShort: true means that the group must also sync when received by
	*  the server, false means that only subgroup will be.
	*/
	bool getGroupSyncMode()
	{
		return mGroupDBSync;
	}

	/*! \brief Set if the group must be sync.
	*
	*  \param inGroupDBSync: true means that the group must also sync when received by
	*  the server, false means that only subgroup will be.
	*/
	void setGroupSyncMode(bool inGroupDBSync)
	{
		mGroupDBSync = inGroupDBSync;
	}

	/*! \brief Get if the Score has been all sync with the database
	*
	*  \return true if all sync, false otherwise.
	*/
	static bool getScoreSync()
	{
		return mScoreSync;
	}

	/*! \brief Set if the Score has been all sync with the database
	*
	*  \param inScoreSync: true if all sync, false otherwise.
	*/
	static void setScoreSync(bool inScoreSync)
	{
		mScoreSync = inScoreSync;
	}


	/*! \brief set the compression levels of zlib for communications.
	*
	*  \param inSubGroupCompression: compression level of a subgroup request
	*  \param inGroupCompression: compression level of a group request
	*/
	static void setCompressionLevels(int inSubGroupCompression = -1, int inGroupCompression = -1)
	{
		mSubGroupCompression = inSubGroupCompression;
		mGroupCompression = inGroupCompression;
	}


	/*! \brief get the compression levels of zlib for communications of sub group.
	*/
	static int getSubGroupCompression()
	{
		return mSubGroupCompression;
	}

	/*! \brief get the compression levels of zlib for communications of sub group.
	*/
	static int getGroupCompression()
	{
		return mGroupCompression;
	}

	/*! \brief get the application's name.
	*/
	static std::string getAppName()
	{
		return mAppName;
	}

	/*! \brief set the application's name.
	*/
	static void setAppName(std::string inAppName)
	{
		mAppName = inAppName;
	}

	/*! \brief get the data version (what client version created these data)
	*/
	static std::string getDataVersion()
	{
		return mDataVersion;
	}

	/*! \brief set the data version (what client version created these data)
	*/
	static void setDataVersion(std::string inDataVersion)
	{
		mDataVersion = inDataVersion;
	}

	/*! \brief get the number of groups.
	*/
	static unsigned int getNumberOfGroups()
	{
		return mGroupVector.size();
	}

	/*! \brief Return Groups parameters as seen in the database.
	*
	*  \return vector of Groups parameters.
	*/
	static std::vector<DAGS::Group>& getGroupVector()
	{
		return mGroupVector;
	}

	/*! \brief Set Groups parameters as seen in the database.
	*
	*  \param inGroupVector: Groups parameters as seen in the database
	*/
	static void setGroupVector(std::vector<DAGS::Group>& inGroupVector);

	/*! \brief Set a Group in mGroupVector
	*
	*  \param inGroup: the Group parameters
	*  \param inGroupId: Group's ID (corresponding to the position in mGroupVector)
	*/
	static void setCommonGroup(DAGS::Group inGroup, unsigned int inGroupId);

	/*! \brief Get a particular Group
	*
	*  \param inGroupId: Group's ID (corresponding to the position+1 in mGroupVector)
	*/
	static DAGS::Group getCommonGroup(unsigned int inGroupId);

	/*! \brief Get any Group with lower generation
	*
	*  \param inStatus: status of the Group <ul><li>0: ready to be evolved </li>
	*                                          <li>1: beeing evolved </li>
	                                           <li>2: ready to be evaluated </li>
	                                           <li>3: all Jobs sent to clients </li>
	                                        </ul>
	*  \param inGeneration: if the lowest Group's generation should be used to retrieve a Group.
	*  \param inCounter: if the lowest Group's counter should be used to retrieve a Group
	*  \param inIdealTime: number of seconds a Group should have all be sent to clients before starting to send again.
	*  \return Group parameters.
	*/
	static DAGS::Group getAnyGroup(int inStatus, bool inGeneration = true, bool inCounter = false, long long inIdealTime = -1);

	/*! \brief Set the client distribution statistics if load balancing is enabled.
	*
	*  \param inIPAddress: client IP address.
	*  \param inLastConnect: the time value that the client will be marked as the last connection time (time(0)).
	*  \param inStats: the stats vector of the time needed to evaluate Jobs.
	*  \param inClientId: the client's ID.
	*  \param inLoadBalancing: value of the load balancing (usually number of jobs to send)
	*  \return clientId if its a new client, 0 otherwise (same ID as the inClientId)
	*/
	static void setClientDistribution(std::string inIPAddress,
	                                  int inLastConnect,
	                                  std::vector<double> inStats,
	                                  unsigned int inClientId,
	                                  unsigned int inLoadBalancing);

	/*! \brief Set a new client distribution statistics if load balancing is enabled.
	 *
	 *  \param inIPAddress: client IP address.
	 *  \param inLastConnect: the time value that the client will be marked as the last connection time (time(0)).
	 *  \param inStats: the stats vector of the time needed to evaluate Jobs.
	 *  \param inLoadBalancing: load balancing value (usually number of jobs)
	 *  \return clientId
	 */
	static unsigned int setNewClientDistribution(std::string inIPAddress,
	        int inLastConnect,
	        std::vector<double> inStats,
	        unsigned int inLoadBalancing);

	/*! \brief Get the distribution statistics of the client.
	*
	*  \param inClientId: the client's ID.
	*  \return Distribution info
	*/
	static DAGS::Distribution& getClientDistribution(unsigned int inClientId);

	/*! \brief Invalidate the stats of the client.
	 *
	 *  The client told the server that it could not process Jobs anymore.
	 *  It sent a reseed request.  The client ID will be taken again next time
	 *  a new client connect.
	 *
	 *  \param inClientId: client's id.
	 *  \throw runtime_error invalid client ID.
	 */
	static void invalidateClient(unsigned int inClientId);

	/*! \brief Verify if the client distribution exist.
	 *
	 *  \param inClientId: client's id.
	 *  \return true if it exists, false otherwise.
	 */
	static bool clientDistributionExist(unsigned int inClientId);

	/*! \brief Get the number of jobs that could be send to a client
	*
	*  \param inClientId: the client id
	*  \return number of jobs.
	*/
	static unsigned int getLoadBalancing(unsigned int inClientId);

	/*! \brief Get the distribution statistics of all clients.
	*
	*  \return vector of all client distriution statistics.
	*/
	static std::vector<DAGS::Distribution> getAllClientDistribution()
	{
		return mDistributionVector;
	}

	/*! \brief Set the percent of time the server will wait before sending back Groups.
	*
	*  \param inWaitPercent: percent.
	*/
	static void setWaitPercent(double inWaitPercent)
	{
		mWaitPercent = inWaitPercent;
	}

	/*! \brief Return the wait percent the server will wait before sending back Groups.
	*
	*  \return percent.
	*/
	static double getWaitPercent()
	{
		return mWaitPercent;
	}


	/*! \brief Set the number of Jobs that are still needed to be evaluated
	*
	*  \param inGroupID: the Group's id
	*  \param inNB: the number that will be added to the counter.  If > 0 will be used in setGroup/reseed
	*  and if < 0, in getSubGroup.
	*  \return if the counter of the Group is == 0, then return true.  return false otherwise.
	*/
	static bool setJobsCounters(unsigned int inGroupID, int inNB);

	/*! \brief Set the number of Jobs that were not fitness evaluated
	*
	*  \param inGroupID: the Group id
	*  \param inNB: the number that will be added to the counter.  If > 0 will be used in setGroup/reseed
	*  and if < 0, in getSubGroup.
	*  \return true if the counter of the Group is == 0. Return false otherwise.
	*/
	static bool setScoreCounters(unsigned int inGroupID, int inNB);

	/*! \brief Set the list of Jobs to be evaluated.
	*
	*  It limits the number of times the server will search the database.
	*  \param inGroupId: group's id
	*  \param inJobsVector: vector of pair of Jobs(second) and its XML string(first)
	*/
	static void setListJobsToEvaluate(unsigned int inGroupId, std::vector<std::pair<std::string,DAGS::Job> > &inJobsVector);

	/*! \brief Set the list of Jobs to be evaluated.
	*
	*  It limits the number of times the server will search the database.
	*  \param inGroupId: group's id
	*  \param inJobsVector: vector of pair of Jobs. First is the JobsId, second it's XML string
	*/
	static void setListJobsToEvaluate(unsigned int inGroupId, std::vector<std::pair<int, std::string> > &inJobsVector);


	/*! \brief Get a bunch of Jobs on the list that are ready to be evaluated.
	*
	*  \param inGroupId: Group's id.
	*  \param inNbJobs: The number of Jobs that need to be evaluated
	*  \return pair of a vector of Jobs id (first) and a string of the Data of the Jobs of the list
	*   that will be evaluated(second).  This is in XML.
	*/
	static std::pair<std::vector<int>, std::string> getListJobsToEvaluate(int inGroupId, int inNbJobs);

	/*! \brief get if the jobs are present in memory
	*
	*  \param inGroupId: Group's id.
	*  \return true if the jobs are present in memory, false otherwise
	*/
	static bool getIfJobsPresent(unsigned int inGroupId);

	/*! \brief update the jobs that need a score so that they can be redispatched to another client
	 *
	 *  \param inGroupId: Group's id.
	 *  \param inJobsNeedScore: job's id that need to be evaluated
	 */
	static void updateJobsNeedScore(unsigned int inGroupId, std::vector<int> inJobsNeedScore);

	/*! \brief Create the time table with the corresponding dimension
	*
	*  \param inDimension: the number of Jobs in the Group
	*/
	static void createTimeTable(int inDimension);


	/*! \brief Update the time table
	*
	*  \param inGroupId: the Group id that will be updated
	*  \param inJobsId: a list of Jobs id that will be updated wih the current time
	*/
	static void updateTimeTable(unsigned int inGroupId, const std::vector<int>& inJobsId);


	/*! \brief Get some Jobs that were already sent if they were sent more than inWait time ago
	*
	*  \param inGroupId: the Group id of the Jobs needed
	*  \param inNbJobs: the max number of Jobs wanted
	*  \param inWait: the time the Jobs is supposed to have waited before beeing sent again
	*  \param outNbTaken: the number of Jobs that were returned
	*  \return every Jobs in XML format.
	*/
	static std::string getJobsByTime(unsigned int inGroupId, int inNbJobs, int inWait, int& outNbTaken);


	/*! \brief Update the score vector
	*
	*  This function is called by evaluation threads to update the score data of Jobs.
	*  \param inGroupId: Group's id.
	*  \param inScore: score of the Jobs.
	*/
	static int updateScore(unsigned int inGroupId, std::vector<std::pair<int, std::string> >& inScore);

	/*! \brief Return the score
	*
	*  This function returns the score of the corresponding Group's id.
	*  \param: Group's id of the needed score.
	*  \return list of jobs's id (first) with its corresponding score.
	*/
	static std::vector<std::pair<int, std::string> >& getScore(unsigned int inGroupId);

	/*! \brief Set the jobs vector mJobsVector.
	*
	*  \param inGroupId: Group's id.
	*  \param inJobs: a vector containing DAGS::Job (ie: Data, Score, id)
	*  \param inNeedScore: job id that need to be evaluated
	*/
	static void setJobs(unsigned int inGroupId,
	                    std::vector<std::pair<std::string, DAGS::Job> >& inJobs,
	                    std::list<int>& inNeedScore);

	/*! \brief Return a vector of DAGS::Job of mJobsVector
	*
	*  \param inGroupId: Group's id.
	*  \return a vector of jobs (include the Data, Score and id)
	*  \throw runtime_error Problem while getting jobs.
	*/
	static std::vector<std::pair<std::string, DAGS::Job> >& getJobs(unsigned int inGroupId);


	/*! \brief Set the Score of jobs in mJobsVector
	*
	*  \param inGroupId: Group's id.
	*  \param inScore: first is the Job id, second is Score
	*/
	static void setJobsScore(unsigned int inGroupId, std::vector<std::pair<int, std::string> >& inScore);

	/*! \brief Return the size of the score vector
	*
	*  The size of the score vector is the number of score that are ready
	*  to be sync with the database.
	*  \param: Group's id of the needed score vector.
	*  \return number of score in the data mScoreVector
	*/
	static unsigned int getSizeScoreVector(unsigned int inGroupId);

	/*! \brief Create the following tables/vector: mScoreReceived, mScoreVector, mDataVector.
	*/
	static void createTable(int inDimension, unsigned int inGroupId);

	/*! \brief Reset the table to their default value: mScoreReceived=false, mScoreVector.clear, mTimeFiFO.clear
	*/
	static void resetTables(int inGroupId);

	/*! \brief Resize the tables for the new dimension.
	*
	*  \param inGroupId: group's id
	*  \param inDiffJobsGroup: diff of the size of the number of jobs in the groups
	*/
	static void resizeTables(int inGroupId, int inDiffJobsGroup);

	/*! \brief return true if the group already exist in the DB, false otherwise
	*/
	static bool getIfGroupExist(unsigned int inGroupId);

	/*! \brief erase all existing groups data.
	*
	*  This function is used when the memory is short and that the user does not mind spending time
	*  getting groups information from the database.
	*/
	static void eraseAllGroupData();

	/*! \brief Lock the mutex mMutexSQL.
	*/
	static void lockSQL();

	/*! \brief Unlock the mutex mMutexSQL.
	*/
	static void unlockSQL();

	/*! \brief Lock the mutex mMutexJobsCounter
	*/
	static void lockJobsCounter();

	/*! \brief Unlock the mutex mMutexJobsCounter
	*/
	static void unlockJobsCounter();

	/*! \brief Lock the mutex mMutexScoreCounter
	*/
	static void lockScoreCounter();

	/*! \brief Unlock the mutex mMutexScoreCounter
	*/
	static void unlockScoreCounter();

	/*! \brief set the terminate bool so every clients can cleanly terminate
	*/
	static void setTerminate();

	/*! \brief get the terminate bool.
	*
	* \return true if it should terminate, false otherwise.
	*/
	static bool getTerminate();


	/*! \brief set the group as ready
	*
	*  Add the group id to the end of the list of groups that are ready. mListGroupsReady
	*  \param inGroupID: the group's id that is all evaluated
	*/
	static void setGroupReady(unsigned int inGroupID);

	/*! \brief get a group ID that is all evaluated
	*
	* \return the group's ID.
	*/
	static int getGroupReady();

private:

	/*! \brief Lock the mutex mMutexGroupVector
	*/
	static void lockGroupVector();

	/*! \brief Unlock the mutex mMutexGroupVector
	*/
	static void unlockGroupVector();

	/*! \brief Lock the mutex mMutexDistribution
	*/
	static void lockDistribution();

	/*! \brief Unlock the mutex mMutexDistribution
	*/
	static void unlockDistribution();

	/*! \brief Lock the mutex mMutexJobsID
	*/
	static void lockJobsID();

	/*! \brief Unlock the mutex mMutexJobsID
	*/
	static void unlockJobsID();

	/*! \brief The name of the database that the server is connected on
	*/
	std::string mDatabaseName;

	/*! \brief Port of the database server to connect on.
	 */
	int mDbPort;

	/*! \brief Database IP Address
	*/
	std::string mDbAddress;

	/*! \brief The username used to connect to the database server
	*/
	std::string mDbUser;

	/*! \brief The password used with the username to connect to the database server.
	*/
	std::string mDbPassword;

	/*! \brief Indicate if the client performance and other stats are enabled
	*/
	bool mEnableClientStat;

	/*! \brief Number of Jobs that are included in a SubGroup when getSubGroup() is called.
	*/
	int mNbJobsInSubGroup;

	/*! \brief Vector of pair that contain pointer to mutex associated with a Universe name
	*/
	static PACC::Threading::Mutex* mUniverseMutex;

	/*! \brief Mutex for accessing the JobsCounter
	*/
	static PACC::Threading::Mutex mMutexJobsCounter;

	/*! \brief Mutex for accessing the ScoreCounter
	*/
	static PACC::Threading::Mutex mMutexScoreCounter;

	/*! \brief Mutex for logging errors in file
	*/
	static PACC::Threading::Mutex mMutexLogErrors;

	/*! \brief Mutex for gettting a Universe mutex
	*/
	static PACC::Threading::Mutex mMutexGetUniverseMutex;

	/*! \brief Mutex for gettting Jobs if they were updated or not
	*/
	static PACC::Threading::Mutex mMutexJobsID;

	/*! \brief Mutex for free Thread ID number
	*/
	static PACC::Threading::Mutex mFreeThreadNumberMutex;

	/*! \brief Mutex for modifying the GroupVector;
	*/
	static PACC::Threading::Mutex mMutexGroupVector;

	/*! \brief Mutex for modifying the DistributionVector;
	*
	*/
	static PACC::Threading::Mutex mMutexDistribution;

	/*! \brief Mutex for calling for calling the SQL database and retriving the answer;
	*/
	static PACC::Threading::Mutex mMutexSQL;

	/*! \brief Mutex for getting/setting the list of Jobs that need to be evaluated;
	*/
	static PACC::Threading::Mutex mMutexListJobs;


	/*! \brief Mutex for getting/setting the time table of Jobs with the current time;
	*/
	static PACC::Threading::Mutex mMutexTimeTable;


	/*! \brief list of Jobs that need to be evaluated; First is the Groupid;
	*
	*  Groupid = first, the list: first is the Jobs ID, second is the XML of the individual
	*/
	static std::vector<std::list<std::pair<int, std::string> > > mListJobs;

	/*! \brief a FIFO of Jobs with the time they were sent to the client
	*
	*  vector correspond to the Groupid, the list of pair: first = individualID, second: time(0)
	*/
	static std::vector<std::list<std::pair<int, int> > > mTimeFIFO;

	/*! \brief a time table of Jobs with the time they were sent to the client
	*
	*  first dim of the vector correspond to the Groupid, second dim correspond to the individualid
	*/
	static std::vector<std::vector<int> > mTimeTable;

	/*! \brief Contains the path and the name of the errors log file.
	 */
	static std::string mLogErrorFile;

	/*! \brief Contains the path and the name of the messages log file.
	 */
	static std::string mLogMessageFile;

	/*! \brief Free Threads ID numbers
	*/
	static std::stack<int> mFreeThreadNumber;

	/*! \brief Counter of Jobs sent
	*/
	static std::vector<int> mJobsCounters;

	/*! \brief Counter of score received after beeing evaluated.
	*/
	static std::vector<int> mScoreCounters;

	/*! \brief Number of used statistics
	*
	*  Represent how many statistics that will be used (1-5)
	*/
	int mNumberStatistics;

	/*! \brief Ideal compute time for client
	*
	*  Value in seconds use to do load balancing of the system
	*/
	int mIdealTime;

	/*! \brief Statistics Weight
	*
	*  Values use to do give a weight to statistics
	*/
	std::vector<double> mVectorWeightsStatistics;

	/*! \brief percent of needed before sync-ing in database
	*
	*  Values between 0 and 100. 0 beeing a parano mode.
	*/
	static unsigned int mDatabaseSync;

	/*! \brief Memory short mode.
	*
	*  True if the memory is short, false otherwise
	*/
	static bool mMemoryShort;

	/*! \brief group sync mode
	*
	*  Normally, only subgroup will be sync-ing with the database.
	*  If True, the group will also be sync-ing with the database, false only subgroup.
	*/
	static bool mGroupDBSync;

	/*! \brief Groups values as seen in the database
	*/
	static std::vector<DAGS::Group> mGroupVector;

	/*! \brief Distribution of the clients
	*/
	static std::vector<DAGS::Distribution> mDistributionVector;

	/*! \brief List of Distribution(client id) that can be used again.
	*/
	static std::list<unsigned int> mReseedDistribution;

	/*! \brief The pourcent the server will wait before sending back the Group/individual
	*/
	static double mWaitPercent;

	/*! \brief Vector of scores
	*
	*  vector position corresponds to the Groupid, list of pair of individual id and
	*  the corresponding score.  When the server send a Group to be evolved, it clear
	*  the corresponding list.
	*/
	static std::vector<std::vector<std::pair<int, std::string> > > mScoreVector;

	/*! \brief Vector of received scores.
	*
	*  If the value is false, then the score was not received
	*/
	static std::vector<std::vector<bool> > mScoreReceived;

	/*! \brief Vector of Datas
	*
	*  this vector is constructed when the client first set the Group in the database,
	*  or when the server starts.
	*  .first is the group's id, second is a vector of every jobs <Data>
	*/
	//static std::vector<std::vector<std::string> > mDataVector;
	static std::vector<std::pair<unsigned int, std::vector<std::string> > > mDataVector;

	/*! \brief Mutex of the table.
	*/
	static PACC::Threading::Mutex mTableMutex;

	/*! \brief If the process should be terminated
	*
	*  true if the process should be terminated, false otherwise
	*/
	static bool mTerminate;

	/*! \brief FIFO of group that are ready to be taken.
	*
	*  When a group has all Score needed, its groupID is added to the back of the list.
	*/
	static std::list<unsigned int> mListGroupsReady;

	/*! \brief Score sync status
	*
	*  true if all jobs's Score has been updated in the DB, false otherwise
	*/
	static bool mScoreSync;

	/*! \brief Group communication compression level.
	*
	*  0 if no compression is used, 1 to 9 for the other level of compression.  1 is faster but compress less, 9 is slower but compress more. -1 means compress if the client compress.
	*/
	static int mGroupCompression;

	/*! \brief  SubGroup communication compression level.
	*
	*  0 if no compression is used, 1 to 9 for the other level of compression.  1 is faster but compress less, 9 is slower but compress more. -1 means compress if the client compress.
	*/
	static int mSubGroupCompression;

	/*! \brief a vector of every Jobs in DAGS format that are ready to be sent to a getGroup request.
	*/
	static std::vector<std::vector<std::pair<std::string, DAGS::Job> > > mJobsVector;

	/*! \brief vector of list of needed scores.  the integer is the job id.
	*/
	static std::vector<std::list<int> > mJobsNeedScore;

	/*! \brief application's name
	*/
	static std::string mAppName;

	/*! \brief The data was produced with this version
	*/
	static std::string mDataVersion;

};

}

#endif
