/*
 *  MonitorComm.cpp
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

#include "MonitorComm.hpp"
#include "dags/XMLUtils.hpp"
#include "dags/Request.hpp"
#include "PACC/Socket/Cafe.hpp"
#include "PACC/Socket/Exception.hpp"
#include "PACC/Threading/Exception.hpp"
#include "PACC/XML/Streamer.hpp"

#include <sstream>
#include <string>

using namespace PACC;
using namespace std;

SMON::MonitorComm::MonitorComm(std::string inAppName, std::string inIPAddress, int inPort)
		: mAppName(inAppName),
		mServerAdd(inPort, inIPAddress),
		mErrorMessage(""),
		mParser(0),
		mRequest(false)
{
}

void SMON::MonitorComm::addXMLHeader(std::string &ioXMLString,
                                     const std::string inRequestType)
{
	std::ostringstream lXmlHeader;

	XML::Streamer lStreamer(lXmlHeader, 0);
	lStreamer.openTag("DAGS");
	lStreamer.insertAttribute("id", -1); //client id
	lStreamer.insertAttribute("rt", inRequestType); //request type
	lStreamer.insertAttribute("err", 1); //error number
	lStreamer.insertAttribute("app", mAppName); //application name
	lStreamer.insertStringContent(ioXMLString);
	lStreamer.closeTag(); //close DAGS

	ioXMLString = lXmlHeader.str();
}

bool SMON::MonitorComm::getGroup(unsigned int inGroupId,
                                 DAGS::Group& outGroup,
                                 std::vector<std::pair<std::string, DAGS::Job> >& outJobs)
{
	std::ostringstream lOSS;

	XML::Streamer lStreamer(lOSS, 0);
	lStreamer.openTag("M"); //monitor
	lStreamer.insertAttribute("rt", "Group"); //request type
	lStreamer.insertAttribute("id", inGroupId); //group's id for the request
	lStreamer.closeTag(); //close monitor

	std::string lStringMessage = lOSS.str();
	;
	addXMLHeader(lStringMessage, "monitor");

	try {
		Socket::Cafe lConnection(mServerAdd);

		// send request to the server
		lConnection.sendMessage(lStringMessage);

		// receive the answer from the server
		lConnection.receiveMessage(lStringMessage);

		//close the connection.  Cafe destructor should be auto called
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = "SMON::SMonitor::getGroup Error: ";
		mErrorMessage += inError.getMessage();
		return false;
	}

	std::vector<std::string> lDoNotParse;
	lDoNotParse.push_back("Sc");
	lDoNotParse.push_back("Env");
	lDoNotParse.push_back("Dt");
	DAGS::XMLUtils lParser;
	int lErrorCode = lParser.parseXML(lStringMessage, lDoNotParse);
	if(lErrorCode < 0) {
		mErrorMessage = "SMON::SMonitor::getGroup Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return false;
	}

	DAGS::Request lRequest;
	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage = "SMON::SMonitor::getGroup Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return false;
	}
	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "SMON::SMonitor::getGroup Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return false;
	}

	//get Attributes Group
	lErrorCode = lParser.getAttributeGroup(outGroup);
	if(lErrorCode < 0) {
		mErrorMessage = "SMON::SMonitor::getGroup Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return false;
	}

	//get the vector of Jobs
	std::vector<std::pair<int, std::string> > lScore;
	std::list<int> lNeedScore;
	lErrorCode = lParser.getJobs(outJobs, lScore, lNeedScore);
	if(lErrorCode < 0) {
		mErrorMessage = "SMON::SMonitor::getGroup Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return false;
	}

	return true;
}

unsigned int SMON::MonitorComm::getState(XML::Streamer& inStreamer)
{
	std::string lStateString;
	addXMLHeader(lStateString, "state");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		// send request to the server
		lConnection.sendMessage(lStateString);

		// receive the answer from the server
		lConnection.receiveMessage(lStateString);

		//dont wait for an answer/process from the server and close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		inStreamer.openTag("Error");
		mErrorMessage = "SMON::SMonitor::getState() Error ";
		// error message of Exception.
		mErrorMessage += inError.getMessage();
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return 0;
	}

	//parse the received message
	std::istringstream lISSXML(lStateString);
	if(mParser) delete mParser;
	mParser = new XML::Document();

	try {
		mParser->parse(lISSXML);
	} catch(std::runtime_error& inError) {
		inStreamer.openTag("Error");
		mErrorMessage = "SMON::SMonitor::request() parsing XML error: " + std::string(inError.what());
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return 0;
	}

	XML::Iterator lRootNodePtr;
	lRootNodePtr = mParser->getFirstDataTag();

	if(lRootNodePtr->getValue() != "DAGS") {
		inStreamer.openTag("Error");
		mErrorMessage = "SMON::SMonitor::getState() XML Error.  Root name is invalid: " + lRootNodePtr->getValue();
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return 0;
	}

	inStreamer.openTag("DAGS");
	inStreamer.insertAttribute("application", lRootNodePtr->getAttribute("app"));
	inStreamer.insertAttribute("groups", lRootNodePtr->getAttribute("gid"));
	inStreamer.closeTag();

	return atoi(lRootNodePtr->getAttribute("gid").c_str());
}

void SMON::MonitorComm::request(XML::Streamer& inStreamer,
                                std::string& inRequest,
                                int inID)
{
	std::ostringstream lOSS;

	XML::Streamer lStreamer(lOSS, 0);
	lStreamer.openTag("M"); //monitor
	lStreamer.insertAttribute("rt", inRequest); //request type
	lStreamer.insertAttribute("id", inID); //group's id for the request
	lStreamer.closeTag(); //close monitor

	inRequest = lOSS.str();

	addXMLHeader(inRequest, "monitor");

	try {
		Socket::Cafe lConnection(mServerAdd);

		// send request to the server
		lConnection.sendMessage(inRequest);

		// receive the answer from the server
		lConnection.receiveMessage(inRequest);

		//close the connection.  Cafe destructor should be auto called
		lConnection.close();
	} catch(Socket::Exception inError) {
		inStreamer.openTag("Error");
		// error message of Exception.
		mErrorMessage = "SMON::SMonitor::request Error: ";
		mErrorMessage += inError.getMessage();
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return;
	}

	//parse the received message
	std::istringstream lISSXML(inRequest);
	if(mParser) delete mParser;
	mParser = new XML::Document();

	mParser->setNoParse("DAGS");

	try {
		mParser->parse(lISSXML);
	} catch(std::runtime_error& inError) {
		inStreamer.openTag("Error");
		mErrorMessage = "SMON::SMonitor::request() parsing XML error: " + std::string(inError.what());
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return;
	}

	XML::Iterator lRootNodePtr;
	lRootNodePtr = mParser->getFirstDataTag();

	if(!lRootNodePtr) {
		inStreamer.openTag("Error");
		mErrorMessage = "SMON::SMonitor::request() no root node.";
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return;
	}

	//verify if the tag name of the root node is DAGS
	std::string lRootName = lRootNodePtr->getValue();
	if(lRootName == "DAGS") mRequest = true;
	else {
		inStreamer.openTag("Error");
		mErrorMessage = "SMON::SMonitor::request() root node should be DAGS, found " + lRootName;
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return;
	}

	if(atoi(lRootNodePtr->getAttribute("err").c_str()) != 1) {
		std::ostringstream lOSSError;
		inStreamer.openTag("Error");
		lOSSError << "SMON::SMonitor::request() the request had a problem " << lRootNodePtr->getAttribute("err");
		mErrorMessage = lOSSError.str();
		inStreamer.insertStringContent(mErrorMessage);
		inStreamer.closeTag();
		mErrorMessage.resize(0);
		return;
	}

	XML::Iterator lInsideRoot;
	lInsideRoot = lRootNodePtr->getFirstChild();
	//return what's inside the DAGS tag.
	inStreamer.insertStringContent(lInsideRoot->getValue());
}
