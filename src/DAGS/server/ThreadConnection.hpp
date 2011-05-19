/*
 *  ThreadConnection.hpp
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

#ifndef __THREADCONNECTION_DAGS_HPP
#define __THREADCONNECTION_DAGS_HPP

#include <vector>
#include <string>
#include "DefineDAGSServer.h"
#include "SQLiteQuery.hpp"
#include "CommonServerData.hpp"
#include "dags/Group.hpp"
#include "dags/Subgroup.hpp"
#include "dags/Job.hpp"
#include "dags/XMLUtils.hpp"
#include "PACC/Util/Timer.hpp"
#include "PACC/Threading.hpp"
#include "PACC/Socket/Cafe.hpp"

using namespace PACC;

namespace DAGS
{

/*!
*  \class ThreadConnection ThreadConnection.hpp "ThreadConnection.hpp"
*  \brief Every threads have access to the methods in ThreadConnection.
*
*  For each incoming connection from a client, the server create an object of this class
*  and give it the connection.  All request from the client are treated
*  here.  Each object have its own connection to the data base, and use
*  mutex to avoid conflict between other threads when changes are done
*  to the data base.
*/
class ThreadConnection
{

public:

	/*! \brief  Constructor with arguments.
	*
	*  \param inSocketDescriptor: is a socket descriptor that represent a connection.
	*  \param inThreadIdNumber: is the index of the thread in the global static vector gVectorThreadConnection.
	*  \param inCSD: is the object common server data that the thread can use
	*  \param inVerbose : the degree of verbosity of the log
	*/
	ThreadConnection(int inSocketDescriptor, int inThreadIdNumber, DAGS::CommonServerData inCSD, int inVerbose = 2);

	/*! \brief  Destructor.
	*/
	~ThreadConnection();

	/*! \brief  Method to start what the thread will do.
	*/
	void start();

	/*! \brief  Get the IP address
	*
	*  This function return the IP address of the client connected to the socket.
	*  \return a string (std::string) that contains the IP address.
	*/
	std::string getIpAddress();

private:

	/*! \brief  Connect to data base.
	*
	*  \return represent what happened :
	*  <ul>
	*  <li>1 if connection is successful.
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int connectDB();

	/*! \brief  Disconnect from data base.
	*/
	void disconnectDB();


	/*! \brief  Update the informations of the client
	*
	*  Update the informations of the client: client IP address, last time it was connected
	*  and the vector of stats of the time needed to evaluate Jobs.  This function
	*  is called when an evaluation client received some Jobs.  The evolver client
	*  does not call this function.
	*  \param inStats: statistics of the client that will replace the old ones.
	*  \param inLoadBalancing: number of jobs to send to the client
	*/
	void updateDistributionInfo(std::vector<double>& inStats, unsigned int inLoadBalancing);


	/*! \brief  Add the XML header to the XML String.
	*/
	void addXMLHeader(std::string &ioXMLString);

	/*! \brief  Get a Group.
	*
	*  This function returns a group that is ready to be evolved.
	*  \param outXMLGroup reference that will contain the group in XML format.
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int getGroup(std::string &outXMLGroup);

	/*! \brief  Get a SubGroup of Jobs.
	*
	*  This function returns the XML string of a SubGroups of Jobs to be evaluated.
	*  \param outXMLSubGroup: reference that will contain the SubGroup in XML.
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int getSubGroup(std::string &outXMLSubGroup);

	/*! \brief update the database group information
	*
	*  This function will change the information of the group in the database.
	*  \param inNbJobs: number of jobs in the group
	*  \return groupId if everything went fine, -1 otherwise
	*/
	int updateGroupDB(unsigned int inNbJobs = 0);

	/*! \brief update the database jobs information
	*
	*  This function will change the information of the jobs in the database.
	*  \param inGroupId: the group's id that will be sync in the database
	*  \param inDataOnly:  if true, only the data tag will be updated in the database
	*  \return number of updated jobs if everything went fine, -1 otherwise
	*/
	int updateJobsDB(unsigned int inGroupId, bool inDataOnly = false);

	/*! \brief insert empty jobs in the database
	*
	*  This function will put empty jobs in the database.
	*  \param inNbJobs:  number of jobs to be created
	*/
	void insertJobsDB(unsigned int inNbJobs);

	/*! \brief get the jobs that are present in the database of a corresponding groupid
	*
	*  this function slow down the server.  It should only be called when memory is short
	*  and you don't mind taking long before getting an answer.
	*  \param inGroupId: group's id
	*  \return Jobs in XML format, ready to be sent
	*/
	std::string getJobsDB(unsigned int inGroupId);


	/*! \brief  Place a group in the database.
	*
	*  This function place a group in the database.  It also check for database error and integrity.
	*  \param inJobsVector: a vector of object pair that contain a string representing the xml string of a
	*  Job and an object DAGS::Job that contain informations about the Job.
	*  \param inScoresVector: the score of already evaluated jobs. first->job id, second->string of the Score
	*  \param inNeedScore: a list of job's id of job that need to be evaluated
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*  \sa setSubGroup().
	*/
	int setGroup(std::vector<std::pair<std::string,DAGS::Job> > &inJobsVector,
	             std::vector<std::pair<int, std::string> > &inScoresVector,
	             std::list<int> &inNeedScore);

	/*! \brief  Place a SubGroup of Job in the database.
	*
	*  This function place a SubGroup of Job in the database.  It also check for database
	*  error and integrity.
	*  \param inScoreVector: a vector of object pair that contain a string representing the xml string of an
	*  Job and an object DAGS::Job that contain informations about the Job.
	*  \param inSubGroup: subgroup object
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*  \sa setGroup().
	*/
	int setSubGroup(std::vector<std::pair<int, std::string> > &inScoreVector,
	                DAGS::SubGroup& inSubGroup);


	/*! \brief  Get the XML string from the client an parse it.
	*
	*  This function wait for the data from the client and parse the received XML string.
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int getXMLStringFromClient(std::string &ioXMLStringFromClient, bool inLog = true);

	/*! \brief  Do a group request from the client
	*
	*  This function do a group request from the client. It store in database the group
	*  sent by the client (if the client send a group) and retrieve a group from datbase
	*  and send it to client (if the client wants a group).
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int doGroupRequest(const std::string &inXMLStringFromClient);

	/*! \brief  Do a SubGroup request from the client
	*
	*  This function do a SubGroup request from the client. It store in database the SubGroup
	*  sent by the client (if the client send a SubGroup) and retrieve a SubGroup from datbase
	*  and send it to client (if the client wants a SubGroup).
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int doSubGroupRequest(const std::string &inXMLStringFromClient);

	/*! \brief  Do a Reseed request from the client
	*  This function do a reseed request from the client. It means that the client can't finish
	*  evaluating the Jobs fitness and that it's Jobs must be reseed to another
	*  client.
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int doReseedRequest(const std::string &inXMLStringFromClient);

	/*! \brief  Do a Monitor request from the client
	*  This function do a monitor request from the client.  Request can ask for
	*  a distribution list, a distribution, a group list, a group, the universe
	*  or to terminate cleanly
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	int doMonitorRequest();

	/*! \brief  Do a terminate request to cleanly close the server
	*  This function will terminate cleanly DAGS.
	*  \return an integer that represent what happened :
	*  <ul>
	*  <li>1 if it worked
	*  <li>-1 if there is an error
	*  </ul>
	*/
	//int doTerminateRequest(const std::string& inXMLStringFromClient);

	/*! \brief  Compute the number of Jobs to give to the client when the performance is enabled
	*
	*  This function compute the number of Jobs to give to the client when the performance is enabled.
	*  \param inStats statistics that will be used to compute a load balancing functions for the current clients.
	*  \return -1 if an error occured or the number of the error :
	*/
	int loadBalancing(std::vector<double> inStats);

	/*! \brief  Update client statistics
	*
	*  This function compute performances of the client and update database with
	*  these results.  It is called only if the loadBalancing is activated.
	*  \return 1 if there are no errors.
	*  \sa loadBalancing().
	*/
	int updateClientStatistics(unsigned int inNbJobs);

	/*! \brief  push_back the time value on the FIFO mListTimeValues
	*/
	void pushTimeValue();

	/*! \brief  pop_back the last value of the FIFO mListTimeValues and return.  don't use popTime with popTimeValue.
	*/
	double popTime();

	void setXMLTime();

	enum eRequestError{
	    NoGroupInDB=-17,
	    SetSubGroupError,
	    SetGroupError,
	    UniverseQueryError,
	    UniverseNotExist,
	    AddClientToDbError,
	    GetUniverseError,
	    GetSubGroupError,
	    GetGroupError,
	    GroupAlReadyExist,
	    SendGroupInvalidClientId,
	    JobsError,
	    SubGroupAttributesError,
	    GroupAttributesError,
	    RequestAttributesError,
	    XMLError,
	    InvalidRequest,
	    ThreadCannotConnectToDb,
	    NoError,//1
	    NothingToSend, //2
	    NothingToReceive //3
	};

	/*! \brief  The Thread ID number given by the server
	 */
	int mThreadIdNum;

	/*! \brief  Pointer to a Universe mutex
	 */
	PACC::Threading::Mutex* mUniverseMutex;

	/*! \brief  Handle connections with the client
	*/
	Socket::Cafe mServerSocketComm;

	/*! \brief  Database client ID
	 */
	int mClientId;

	/*! \brief  number of groups present in the application's name.
	 */
	int mNumberOfGroups;

	/*! \brief  If the client is a new client
	*/
	bool mNewClient;

	/*! \brief  The user group ID
	*/
	int mGroupId;

	/*! \brief  The databse SubGroup ID
	*/
	int mSubGroupId;

	/*! \brief  Generation of the group
	*/
	int mGroupGeneration;

	/*! \brief  If the environment must be distributed to clients when they ask for subgroup
	*/
	bool mDistributeEnv;

	/*! \brief  Error number
	*/
	int mRequestError;

	/*! \brief  Environment of the group (XML)
	*/
	std::string mGroupEnvironment;

	/*! \brief  IP address of the client
	*/
	std::string mClientIpAddress;

	/*! \brief Request type of the client
	*/
	std::string mRequestType;

	/*! \brief  Application name (name of the problem)
	*/
	std::string mAppName;

	/*! \brief  Common data shared between DAGS::ThreadConnection and DAGS::Server
	*/
	DAGS::CommonServerData mCSD;

	/*! \brief  Socket descriptor
	*/
	int mSocketDescriptor;

	/*! \brief  Object that parse XML string from the client
	*/
	DAGS::XMLUtils mXMLUtils;

	/*! \brief  Object that handle connections do MySQL database
	*/
	SQLQuery mDatabase;

	/*! \brief  Current time when the client connect
	*
	*  This value is used for the load balancing
	*/
	long long mClientConnectTime;

	/*! \brief  Log the time of each methods, to see where the threads lose much of its time
	*/
	std::ostringstream mTimeLog;

	/*! \brief  the degree of verbosity.  The default one is 2
	*
	*  degree 2: no output of the thread.
	*  degree 3: the time in the main method is written in a log file.
	*  degree 4: the time that is spent in each methods is written in a log file.
	*/
	int mVerbose;

	/*! \brief  a FIFO of time each methods time spent.  Always use push_back and pop_back
	*/
	std::vector<PACC::Timer> mListTimeValues;

	/*! \brief  a way of accessing the mStreamerLog object with ostringstream.
	 */
	std::ostringstream mOSSLog;

	/*! \brief  a XML streamer object of the log that will be written
	 */
	XML::Streamer mStreamerLog;


	/*! \brief  time spent for the entire thread
	*/
	double mThreadTime;
	/*! \brief  time spent for parsing the XML
	*/
	double mParseTime;
	/*! \brief  time spent for setting a group or subgroup
	*/
	double mSetTime;
	/*! \brief  time spent for getting a group or subgroup
	*/
	double mGetTime;
	/*! \brief  time spent while receiving with DACC
	*/
	double mReceiveTime;
	/*! \brief  time spent while sending with DACC
	*/
	double mSendTime;
	/*! \brief  time spent to do Database update
	*/
	double mDBTime;

	/*! \brief  true if getGroup is asking for a specific group, false otherwise
	*/
	bool mGetGroupId;

	/*! \brief  The difference of jobs between 2 generations of a same group.
	*/
	int mDiffJobsGroup;

	/*! \brief  The client's version
	*/
	std::string mVersion;

	/*! \brief  Compression level used by the client.
	*/
	unsigned int mCompression;
};
}

#endif




