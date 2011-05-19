/*
 *  Server.hpp
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
#ifndef __SERVER_DAGS_HPP
#define __SERVER_DAGS_HPP

#include "DefineDAGSServer.h"
#include "ThreadConnection.hpp"
#include "CommonServerData.hpp"
#include "dags/FileParameters.hpp"
#include "PACC/Util/SignalHandler.hpp"
#include "PACC/Threading.hpp"
#include "PACC/Socket/TCPServer.hpp"
#include <string>
#include <vector>

using namespace PACC;

namespace DAGS
{

/*!
*  \class Server Server.hpp "Server.hpp"
*  \brief Server's class that wait for connections of clients.
*
*  To create a server, the default constructor or the constructor
*  with arguments can be used.  The default constructor needs to call init().  The constructor
*  with arguments call init() internally.
*  The function start() starts the server (if the constructor by default is
*  used, the function init()  must be called before).  The function stop() stop the server
*  (the server stop listening for connection). The enumeration eErrorCode contains the
*  error returned by most public methods.
*/
class Server : public Socket::TCPServer, public SignalHandler
{

public:

	/*! \brief Constructor with arguments.
	*
	*  This constructor don't need to call init() since it's calling it internally.
	*  \param inFileParameter : Name and the full path of the parameter file for the server.
	*  \param inVerbose : Verbose (0(no verbose) 1(simple output) 2(full output) 3(log threads time) 4(log everything)).
	*  \param inPort: Server's port.
	*  \param inMaxConnections: Maximum number of connections allowed.
	*  \param inMaxThreads: Maximum number of threads that can run at the same time.
	*/
	Server(DAGS::FileParameters inFileParameter,
	       int inVerbose,
	       int inPort,
	       int inMaxConnections,
	       int inMaxThreads);

	/*! \brief Destructor.
	*/
	~Server();

	/*! \brief Handle the receiving signal.
	*
	*  \param inSignal signal number
	*/
	virtual void main(PACC::SignalHandler::SignalType inSignal);

	/*! \brief start the serverThread.
	*
	*  This function must be called if the default constructor is used.  If the constructor by arguments is
	*  used, this function is internally called in it. This function initialize some variables
	*  and a CommonServerData object.
	*/
	void start();

	/*! \brief main function of server when it receives connection
	*
	*  Creates an adequate client for TCP socket.
	*/
	void main(int inDescriptor, const Socket::ServerThread* inThread);

	/*! \brief Returns the init state of the server
	*
	*  \return initialization of the server (if init or not)
	*  \sa mIsInit
	*/
	bool isInit();

	/*! \brief Get the error code.
	*
	*  This function returns mErrorCode and then put mErrorCode equal to 1 to reset it.
	*  \return the mErrorCode
	*/
	int getErrorCode();

	/*! \brief Set verbosity of the server
	*
	*  Level 0 means no comment, 1 only the minimal console output, 2 connections output,
	*  3 log the threads time in a file and 4 log everything in a file.
	*  \param inVerbose : level of verbosity.
	*/
	void setVerbose(int inVerbose);

	/*! \brief Get verbosity level of the server
	*/
	int getVerbose();


	/*! \brief Stop the server.
	*
	*  Stop principals functions of the server, like listening for connections and wait for
	*  connected clients to terminate. This function is called to manually close the server by
	*  code.  Doing a kill or a ctrl-c can also call this function.
	*/
	void stop();


	/*! \brief Error code returned by methods.
	*
	*  These errors codes can help to know what is the specific problem when a function cannot perform its task.
	*/
	enum eErrorCode
	{
	    ServerStopped=-16,/*!<The server is stopped*/
	    DatabaseIndexError,/*!<An index in the data base doesn't have the required structure*/
	    DatabaseTableError,/*!<A table in the data base doesn't have the required structure */
	    CannotCreateIndexError,/*!<The user supplied doesn't have permission to create index in database */
	    CannotCreateDatabaseError,/*!<The user supplied doesn't have permission to create(or modify) database*/

	    CannotCreateTableError,/*!<The user supplied doesn't have permission to create(or modify) table
	                                  in database*/
	    CannotConnectToDatabaseError,/*!<The server cannot connect to the database (bad address,
	                                        bad user, bad password, sql server not running)*/
	    DatabaseOperationError,/*!<An error occur while verifying database*/
	    NotInitError,/*!<This error occur when the object is not initialized and a function
	                          that need the object initialized is called.*/
	    FileParametersError,/*!<This error occur when there is a problem while loading parameter.*/
	    SocketAcceptError,/*!<This error occur when there is a problem when accepting a connection.*/
	    SocketAlreadyInitError,/*!<This error occur when an initialization function is called and the object
	                                  is already initialized.*/
	    SocketDescriptorError,/*!<This error occur when a function can't get a socket descriptor from system.*/
	    SocketBindError,/*!<This error occur when a function can't bind to a specified port.*/
	    SocketListenError,/*!<This error occur when a function can't listen to a specified port.*/
	    UnknownError/*!<This is an unknown error.*/
	};


private:

	/*! \brief Read the parameters
	*
	*  Read the parameters in the server parameter file and associate the different variables to these values.
	*  \return 0 if all is ok or an integer that is the error code if there is an error.
	*/
	int readParameter();

	/*! \brief Verify database
	*
	*  Verify the connection to the database for missing tables, etc.
	*  \return 0 if all is ok, or an integer that represents the error code.
	*/
	int verifyDatabase();

	/*! \brief Set the Group vector of mCSD (common server data)
	*
	*  \return 0 if all is ok, or -1 if an error happened and the server should be shut down.
	*/
	int setGroupVector();

	/*! \brief Error code.
	 */
	int mErrorCode;

	/*! \brief Server's port it is listening to.
	 */
	int mPort;

	/*! \brief Number of connections allowed
	*/
	int mNbConnection;

	/*! \brief Number of threads allowed
	*/
	int mNbThreads;

	/*! \brief Init state
	*
	*  Is the state of the initialisation of the server (true = is init, false = is not init).
	*  \sa isInit()
	*/
	bool mIsInit;

	/*! \brief Degree of verbosity
	*
	*  If mVerbose == 0, no output will be made, if mVerbose == 1, small output will be made, and if
	*  mVerbose == 2, everything will be displayed, mVerbose == 3, log threads time in file,
	*  mVerbose == 4, log everything in a file.
	*/
	int mVerbose;

	/*! \brief File which contains parameters
	*
	*  This is a string that contain the file name and the path to the file which contain parameters.
	*/
	DAGS::FileParameters mFile;

	/*! \brief ThreadConnection Vector
	*
	*  A Vector that contain the pointers threads of differrent clients that are connected with the server.
	*  \sa ThreadConnection
	*/
	std::vector<DAGS::ThreadConnection*> mVectorThreadConnection;

	/*! \brief Common Server Data
	*
	*  Server class and ThreadConnection can share common data.
	*/
	DAGS::CommonServerData mCSD;

	/*! \brief Mutex for the variable mServerStopped
	*/
	PACC::Threading::Mutex* mMutexServer;

	/*! \brief Mutex to be certain no other threads will execute the same function twice (kernel 2.6.x)
	*/
	static PACC::Threading::Mutex mMutexStop;

	/*! \brief Represent if the server is stopped
	*
	*  This variable is set to TRUE if a signal is received (SIGINT and SIGTERM)
	*/
	static bool mServerStopped;

	/*! \brief Default constructor disabled.
	 */
	Server();
};
}

#endif




