/*
 *  MonitorComm.hpp
 *  Copyright (C) 2004 by Marc Dubreuil
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

#ifndef __MONITORCOMM_SMON_HH
#define __MONITORCOMM_SMON_HH

#include "dags/Group.hpp"
#include "dags/Job.hpp"
#include "PACC/Socket/Address.hpp"
#include "PACC/XML/Streamer.hpp"
#include "PACC/XML/Document.hpp"

using namespace PACC;

#include <string>
#include <vector>

/*! \namespace SMON
\brief Simple MONitor
 
This namespace contains various classes related to the monitoring of the dags-server.
*/
namespace SMON
{

/*! \brief SMON Monitor communication layer
*
*  This class talks with the server and return useful information that a simple monitor could parse.
*/

class MonitorComm
{

public:

	/*! \brief constructor with arguments
	 *
	 *  \param inAppName: application's name
	 *  \param inIPAddress: ip address of the DAGS server
	 *  \param inPort: port number of the DAGS server
	 */
	MonitorComm(std::string inAppName, std::string inIPAddress, int inPort);

	/*! \brief get the group for a milestone (read only).
	 *
	 *  \param inGroupId: the group's id to get
	 *  \param outGroup: group's informations (Environment, etc.) in DAGS format
	 *  \param outJobs: vector of jobs in DAGS format
	 *  \return true if everything is ok, false otherwise.  If false, check getErrorMessage()
	 */
	bool getGroup(unsigned int inGroupId,
	              DAGS::Group& outGroup,
	              std::vector<std::pair<std::string, DAGS::Job> > &outJobs);


	/*! \brief Get the state of the server
	 *
	 *  \param inStreamer: XML informations will be added
	 *  \return The number of groups present.
	 */
	unsigned int getState(XML::Streamer& inStreamer);


	/*! \brief return the error message
	 */
	std::string getErrorMessage()
	{
		return mErrorMessage;
	}

	/*! \brief Do a request to the server and receive an answer.
	*
	*  The request must be in XML format.  The answer will have been parsed untill the
	*  inDontParseTag are reached.
	*
	*  \param ioRequest: input/output request/answer
	*  \param inID: if an id is required (group's id or client id)
	*  \param inStreamer: XML informations will be added
	*/
	void request(XML::Streamer& inStreamer, std::string& ioRequest, int inID = 1);

private:

	/*! \brief add a XML header before sending to the DAGS server
	 *
	 *  \param ioXMLString: input/output message with the DAGS header
	 *  \param inRequestType: request type to send to the DAGS server,
	 *                        usualy "monitor"
	 */
	void addXMLHeader(std::string &ioXMLString, const std::string inRequestType);

	/*! \brief Application's name
	 */
	std::string mAppName;

	/*! \brief Server's address.
	 */
	Socket::Address mServerAdd;

	/*! \brief Error Message
	 */
	std::string mErrorMessage;

	/*! \brief Parser object.
	 *
	 *  Object that contains information about the XML.  It creates the root node.
	 */
	XML::Document* mParser;

	/*! \brief if request information in the DAGS XML tag are present and can be examined.
	 */
	bool mRequest;

	//forbid default constructor
	MonitorComm();
};
}

#endif //__MONITORCOMM_SMON_HH
