/*
 *  Client.hpp
 *  Copyright (C) 2003-2004 by Marc Dubreuil
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


#ifndef __CLIENT_DAGS_HH
#define __CLIENT_DAGS_HH

#include <string>
#include <vector>
#include "PACC/Socket/Address.hpp"
#include "dags/Group.hpp"
#include "dags/Subgroup.hpp"
#include "dags/Job.hpp"

using namespace PACC;

namespace DAGS
{

/*!
 *  \class Client dags/Client.hpp "dags/Client.hpp"
 *  \brief  Classe that encapsulate communications with the server
 *
 *  Used by DAGS to encapsulate communications with the server.
 *  The constructor with arguments needs the server's
 *  IP and port and the compression level used (optional).
 *  To set a group of a particular Universe on the server
 *  it should call setGroup().  When asking for a group,
 *  getGroup() should be used.  If a client's purpose is to evaluate
 *  jobs fitness, it should call getJobs(). If a client wants
 *  every Groups (including Jobs), getUniverse() should be called.
 */

class Client
{
public:

	/*! \brief Construct a client that will connect to a server.
	 *
	 *  The server's IP and port are the arguments.
	 *  The mClientId is equal to -1.
	 *  The application name is empty.
	 *  The mRequestError value is 0, a value not recognized by the server.
	 *  \param inIpServer: server's IP (ex: 192.168.1.1)
	 *  \param inPortServer: server's port.  The default server's port is 9123.
	 *  \param inCompressionLevel: the level of compression used in communication.
	 *  \param inVersion: Client's version.
	 */
	Client(std::string inIpServer,
	       int inPortServer,
	       unsigned int inCompressionLevel = 0,
	       std::string inVersion = "");

	/*!
	 *  \brief Destruct client.
	 */
	virtual ~Client()
	{ }

	/*! \brief Get all Groups on the server
	 *
	 *  This function ask the server for a specific Universe with the inUniverseName.
	 *  This method can't be used to store a Universe in the database. If the
	 *  ioUniverseXMLString is not empty, the function will empty it.  The proper
	 *  way to create a Universe is using the setGroup() function, one Group at a time.
	 *  This function connect to the server, send the request, receive the Universe and
	 *  close the connection with the server.  After verifying if an error occured on
	 *  the server's side, it returns by reference the Universe in the ioUniverseXMLString.
	 *  \param &outUniverseXMLString : reference that will contains the Universe in XML format.
	 *  \param inUniverseName : Universe's name.
	 *  \return <ul>
	 *  <li> 1 if everything went fine;
	 *  <li> < 1 if an error occured. The number is defined in the enum eRequestError
	 *  </ul>
	 */
	int getUniverse(std::string &outUniverseXMLString,
	                std::string inUniverseName);

	/*! \brief Get any Group in the server that are ready to be evolved
	 *
	 *  This method ask the server for a Group in the specified inUniverseName.
	 *  It can also be used to set the value of an existing Group. This function
	 *  can't be used to create a new Group (to create a new Group, setGroup()).
	 *  The function connect to the server, send the request for a Group, receive the
	 *  Group and close the conneection.  After verifying if an error occured on the
	 *  server's side, it returns the Group by reference in outJobs.
	 *  \param &inGroupXMLString : reference that contains a Group in XML format that will be updated.
	 *  \param &outGroup : parameters of the Group
	 *  \param &outJobs : Jobs of the Group.
	 *  \param inUniverseName : Universe's name.
	 *  \return <ul>
	 *  <li> 1 if everything went fine;
	 *  <li> < 1 if an error occured. The number is defined in the enum eRequestError
	 *  </ul>
	 */
	int getGroup(std::string& inGroupXMLString,
	             DAGS::Group&  outGroup,
	             std::vector<std::pair<std::string, DAGS::Job> > &outJobs,
	             std::string inUniverseName);

	/*! \brief Get a specific Group in the server that is ready to be taken
	 *
	 *  This method ask the server for a specific Group in the specified inUniverseName.
	 *  In normal running mode, the other method getGroup should be used.
	 *  The function connect to the server, send the request for a Group, receive the
	 *  Group and close the conneection.  After verifying if an error occured on the
	 *  server's side, it returns the Group by reference in outJobs.
	 *  \param inGroupId : get the specific group asked
	 *  \param &outGroup : parameters of the Group
	 *  \param &outJobs : Jobs of the Group.
	 *  \param inUniverseName : Universe's name.
	 *  \return <ul>
	 *  <li> 1 if everything went fine;
	 *  <li> < 1 if an error occured. The number is defined in the enum eRequestError
	 *  </ul>
	 */
	int getGroup(unsigned int inGroupId,
	             DAGS::Group& outGroup,
	             std::vector<std::pair<std::string, DAGS::Job> > &outJobs,
	             std::string inUniverseName);


	/*! \brief Set a Group in the server or create it if it does not exist.
	 *
	 *  If the inGroupType variable is set to NewGroup, this function create a Group in the
	 *  specified inUniverseName.  If the inGroupType variable is set to ExistingGroup, it
	 *  will try to update the Group corresponding to the inUniverseName.  If the inUniverseName
	 *  does not exist, the server will create the corresponding Universe.  If the
	 *  Universe exist, it create a new Group for it or update the value of an existing
	 *  Group.
	 *  The function connect to the server, send the request to set a new Group, receive
	 *  the confirmation from the server and close the connection.  It returns an error
	 *  if the server could not create the Group or returns 1 if everything went fine.
	 *  Normally, this method is called only the first time to create a Group. Otherwise,
	 *  getGroup() should be used as it minimize the number of connections needed.
	 *  \param inGroupXMLString : The Group to create (or update) on the server
	 *  \param inUniverseName : Universe's name.
	 *  \param inGroupType : Three types: NewGroup means that it will create a new Group
	 *  in the Universe, ExistingGroup means that it will update the Group in the
	 *  Universe and Terminate means that the server will stop accepting connection
	 *  and will terminate cleanly.
	 *  \return <ul>
	 *  <li> 1 if everything went fine;
	 *  <li> < 1 if an error occured. The number is defined in the enum eRequestError
	 *  </ul>
	 */
	int setGroup(std::string& inGroupXMLString,
	             std::string  inUniverseName,
	             int inGroupType = NewGroup);

	/*! \brief Ask for a bunch of Jobs to evaluate their fitness.
	 *
	 *  This function get a group of Jobs in the corresponding inUniverseName.
	 *  It can also be used to set a group of Jobs on the server (after
	 *  calculating the fitness).  If the ioJobsXMLString is empty, it will
	 *  just ask for a group of Jobs.  Otherwise, it will update the value
	 *  of Jobs in the server and ioJobsXMLString will returns
	 *  by reference a group of Jobs if some are ready to be evaluated.
	 *  This function connect to the server, send the request, receive
	 *  a group of Jobs and close the connection with the server.
	 *  After verifying if an error occured on the server's side, it returns by reference
	 *  the group of Jobs in the outJobs.
	 *  \param &ioJobsXMLString : reference of Jobs in XML format.
	 *  \param &outGroups : Group's parameters
	 *  \param &outJobs : bunch of Jobs
	 *  \param inUniverseName : std::string of the Universe's name.
	 *  \return <ul>
	 *  <li> 1 if everything went fine;
	 *  <li> < 1 if an error occured. The number is defined in the enum eRequestError
	 *  </ul>
	 */
	int getJobs(std::string &ioJobsXMLString,
	            DAGS::SubGroup &outGroups,
	            std::vector<std::pair<std::string,DAGS::Job> > &outJobs,
	            std::string inUniverseName);

	/*! \brief Reseed the Jobs so another client will calculate their fitness.
	 *
	 *  This method is used when a software failure occurs. A software failure could be
	 *  the deactivation of a screensaver similar to SETI@Home.  It send back the
	 *  Jobs that were computed with their score and the other Jobs
	 *  are sent without the (old) score.  These Jobs do not contain the
	 *  Jobs data.
	 *  This function connect to the server, send the Jobs and close the
	 *  connection with the server.
	 *  \param &inJobsXMLString : Reference of a group of Jobs in XML format.
	 *  \param inUniverseName : Universe's name.
	 *  \return <ul>
	 *  <li> 1 if everything went fine;
	 *  <li> < 1 if an error occured. The number is defined in the enum eRequestError
	 *  </ul>
	 */
	int reseed(std::string &inJobsXMLString,
	           std::string inUniverseName);


	/*! \brief The following XML tags won't be parsed
	 *
	 */
	void doNotParseXMLTags(std::vector<std::string> inDontParse);


	/*! \brief Returns the error message given in Try/Catch
	 */
	std::string getErrorMessage();

	/*! \brief Get the application name and the number of groups in the universe.
	 *  \return the application name for .first, the number of groups for .second
	 */
	std::pair<std::string, int> getState();

	/*! \brief Update only the environment of a group.
	 */
	void updateEnvironment(std::string& inEnvironment);

	/*! \brief Error code when sending or receiving XML from the server
	 *
	 *  These errors codes tell the server or the client what to expect. This enumeration must be
	 *  similar to the one found in ThreadConnection. Some of these errors refer to the socket
	 *  error found in Socket::Cafe.  The other are errors sent by the server to tell what
	 *  was the problem.  If the error number is lower than 1, then it means something did go
	 *  wrong.  This means that the data could not be sent, receive, or that the server could
	 *  not work on the database.  If the error number is greater or equal to 1, it means that
	 *  no error occured (no socket error or server error).  The NothingtoSend and NothingToReceive
	 *  value in the enumeration are there to tell the server what to expect.  Their values are
	 *  NothingToSend = 2 and NothingtoReceive = 3.  Since they are only usefull for the server,
	 *  no user should see them because the server never return these values.
	 */
	enum eRequestError{
	    InitError = -38,/*!<XCERCES-DOM Initialisation error*/
	    ParseXMLError,/*!<XML Parsing Error*/
	    ParseDOMError,/*!<DOM Error*/
	    ParseUndefinedError,/*!<Unknown error about the parsing of the XML String*/
	    NoNameElementError,/*!<A TAG is missing on the XML String*/
	    NoNameAttributeError,/*!<An attribute have an error*/
	    GroupTypeError,/*!<-32 This error occur when the specified SubGroup type is not recognized.
	                          It should be either NewGroup or ExistingGroup. The only place this error occurs
	                          is in the function setGroup(), the third parameter.*/
	    eAddressInUse, //!<-31 Address is already in use (bind & connect).
	    eAddressNotAvailable, //!< Address not available on machine (bind & connect).
	    eBadDescriptor, //!< Invalid socket descriptor (socket).
	    eBadMessage, //!< Message signature is invalid.
	    eConnectionClosed, //!< Connection was closed (or broken) by other party.
	    eConnectionRefused, //!< Connection refused by server.
	    eDatagramTooLong, //!< Datagram too long to send atomically.
	    eInvalidOption, //!< Invalid option for socket protocol.
	    eIsConnected, //!< Socket is already connected.
	    eNotConnected, //!< Socket is not connected.
	    eOpNotSupported, //!< Operation is not supported for this socket.
	    ePrivilegedPort, //!< User does not have acces to privileged ports (bind).
	    eTimeOut, //!< Time out was reached for operation (receive & send).
	    eOtherError, //!< Any other OS specific error. (-18)

	    NoGroupInDB,/*!<Message to specufy that there is no Group in the database so the client can create one.*/
	    SetSubGroupError,/*!<-16 This error occurs when there's a problem setting a group in the database.*/
	    SetGroupError,/*!<This error occurs when there's a problem setting a Group in the database.*/
	    UniverseQueryError,/*!<This error occurs when the query sent to the database is not recognized by the database.*/
	    UniverseNotExist,/*!<This error occurs when the Universe's name specified does not exist.*/
	    AddClientToDbError,/*!<This error occurs when the server could not add a client to the database.*/
	    GetUniverseError,/*!<This error occurs when the server could not get the Universe.*/
	    GetSubGroupError,/*!<This error occurs when the server could not get a group.*/
	    GetGroupError,/*!<This error occurs when the server could not get a Group to send to the client.*/
	    GroupAlReadyExist,/*!<This error occurs when the specified Group already exist in the database.*/
	    SendSubGroupInvalidClientId,/*!<This error occurs when the client send a group but it's clientId is invalid. This
	                                      id is specified in mClientId.*/
	    JobsError,/*!<This error occurs when there's is a problem with Jobs.*/
	    SubGroupAttributesError,/*!<This error occurs when the group XML tag has attributes that could not be recognized.*/
	    GroupAttributesError,/*!<This error occurs when the Group XML tag  has attributes that could not be recognized.*/
	    RequestAttributesError,/*!<This error occurs when the requestGA XML tag has attributes that could not be recognized.*/
	    XMLError,/*!<This error occurs when the XML parser makes an unknown error.*/
	    InvalidRequest,/*!<This error occurs if the user send an invalid request type.  This type is specified
	                             in the function addXMLHeader().  This error should not happen since this function is private
	                             and is called only in getGroup(), setGroup(), getUniverse() and getJobs().*/
	    ThreadCannotConnectToDb,/*!<This error occur when the thread could not connect itself to the database.*/
	    NoError,/*!<Message to specify that there is No error send to the server or returned from the server.*/
	    NothingToSend,/*!<Message to specify that there is nothing to send to the server.*/
	    NothingToReceive/*!<Message to specify that there is nothing to receive from the server.*/
	};


	/*! \brief Enumeration of Group type.
	 *
	 * The type of Group is only used in the function setGroup() as parameters 3: inGroupType.
	 * Refer to the function setGroup() to see how it should be used.
	 */
	enum eGroupType
	{
	    NewGroup,/*!< The Group specified is a new Group and should be added to the Universe.*/
	    ExistingGroup,/*!< The Group specified is an existing Group
	                           in the Universe and should be updated in the database.*/
	    Terminate,/*!<The group specified is the final group and the server must be terminated.*/
	    Quit/*!<The group specified must be reseed to another group request client.*/
	};

private:

	/*! \brief Error's message given in Try/Catch
	 */
	std::string mErrorMessage;

	/*! \brief Server's IP Address
	 */
	std::string mIpServer;

	/*! \brief Server Port
	 */
	int mPort;

	/*! \brief The Client ID
	 *
	 *  This is the client id returned by the server. This is not the same id specified in the XML tag
	 *  by the user.  This id is used in getGroup, setGroup, getJobs and getUniverse and is used
	 *  to construct a communication tag between the server and the client.
	 */
	int mClientId;

	/*! \brief Request Error
	 *
	 *  Tell what kind of data the client wants and what type of data it is sending.
	 *  <ul>
	 *  <li> 1: everything went fine
	 *  <li> 2: nothing to send, need something
	 *  <li> 3: nothing to receive, give something
	 *  </ul>
	 */
	int mRequestError;

	/*! \brief Application name
	 */
	std::string mAppName;

	/*! \brief level of compression during communication. 0 means no compression.
	*/
	unsigned int mCompressionLevel;

	/*! \brief Construct XML header tag for communications with the server
	 *
	 *  The XML header is a protocol tag.  This tag include the client id and the request
	 *  type.  It specifies when the XML message that the client send start and end,
	 *  tell the server the client id, the request type, the request error (i.e. NoError)
	 *  and the Universe's name.
	 *  \param ioXMLString: the header will be added to this std::string.
	 *  \param inRequestType: the type of request (i.e. Group, Job, Universe).
	 *  \param inGroupId: group's id of the request (getGroup(unsigned int))
	 */
	void addXMLHeader(std::string &ioXMLString,
	                  const std::string inRequestType,
	                  int inGroupId = -1);

	/*! \brief Adjust the error of the socket comm
	 *
	 *  This function translate the error received by socket communication to the enumeration
	 *  eRequestError.  It means that if the enumeration is modified, this function must also be modified.
	 *  For now, it only need to add the number -31, the maximum value of the socket error in the enum
	 *  with the number of socket errors (9). If the enumaration is changed, don't forget to modify
	 *  this function because any returned error will not return the true error.
	 *  \param inErrorSocket: ClientSocketComm error
	 *  \return the new error number
	 */
	int adjustSocketError(const int inErrorSocket) const;

	/*! \brief Adjust the error of the parser of XMLUtil
	 *
	 *  This function translate the error received from XMLUtil object contained in enumeration
	 *  eRequestError.  It means that if the enumeration is modified, this function must also be modified.
	 *  For now, it only need to add the number -32. If the enumaration is changed, don't forget to modify
	 *  this function because any returned error will not return the true error.
	 *  \param inParserError: XMLUtil error
	 *  \return the new error number
	 */
	int adjustParserError(const int inParserError) const;

	/*! \brief Server's address: ip address and port.
	 */
	Socket::Address mServerAdd;

	/*! \brief These tags won't be parsed.
	 */
	std::vector<std::string> mNoParseTags;

	/*! \brief Client's version
	 */
	std::string mVersion;

	/*! \brief Deactivate default constructor
	 */
	Client();

};

}

#endif //__CLIENT_DAGS_HH


