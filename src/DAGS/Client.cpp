/*
 *  Client.cpp
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

#include <sstream>
#include "dags/Client.hpp"
#include "dags/Request.hpp"
#include "dags/XMLUtils.hpp"
#include "PACC/Socket/Cafe.hpp"
#include "PACC/Socket/Exception.hpp"

using namespace PACC;

DAGS::Client::Client(std::string inIpServer,
                     int inPortServer,
                     unsigned int inCompressionLevel,
                     std::string inVersion)
		: mErrorMessage(""),
		mIpServer(inIpServer),
		mPort(inPortServer),
		mClientId(-1),
		mRequestError(0),
		mAppName(""),
		mCompressionLevel(inCompressionLevel),
		mServerAdd(mPort, mIpServer),
		mVersion(inVersion)
{}

int DAGS::Client::getUniverse(std::string &ioUniverseXMLString,
                              std::string inUniverseName)
{
	mErrorMessage = "";
	mRequestError = NothingToSend;
	mAppName = inUniverseName;
	int lErrorCode = 0;
	addXMLHeader(ioUniverseXMLString, "universe");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		// send request to the server
		lConnection.sendMessage(ioUniverseXMLString, mCompressionLevel);

		// receive the answer from the server
		lConnection.receiveMessage(ioUniverseXMLString);

		//close the connection.  Cafe destructor should be auto called
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = "DAGS::Client::getUniverse Error: ";
		mErrorMessage += inError.getMessage();
		return adjustSocketError(inError.getErrorCode());
	}

	// object to help parsing
	DAGS::XMLUtils lParser;
	DAGS::Request lRequest;

	// parse ioUniverseXMLString
	lErrorCode = lParser.parseXML(ioUniverseXMLString, mNoParseTags);

	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getUniverse Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getUniverse Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}
	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "DAGS::Client::getUniverse Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return lErrorCode;
	}

	return 1;
}



int DAGS::Client::getGroup(std::string& inGroupXMLString,
                           DAGS::Group& outGroup,
                           std::vector<std::pair<std::string, DAGS::Job> > &outJobs,
                           std::string inUniverseName)
{
	mErrorMessage = "";
	std::string lGroupReceived;
	if(inGroupXMLString == std::string("")) {
		mRequestError = NothingToSend;
	} else {
		mRequestError = NoError;
	}
	mAppName = inUniverseName;
	int lErrorCode = 0;
	addXMLHeader(inGroupXMLString, "group");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		//ask if it's ok to send the group
		//as soon the server send something, it means it's ready to accept data
		std::string lRequestGroup("READY TO SEND INFO");
		addXMLHeader(lRequestGroup, "group");
		lConnection.sendMessage(lRequestGroup, mCompressionLevel);
		lConnection.receiveMessage(lRequestGroup);

		// send request to the server
		lConnection.sendMessage(inGroupXMLString, mCompressionLevel);

		// receive the answer from the server
		lConnection.receiveMessage(lGroupReceived);

		//close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += inError.getMessage();
		return adjustSocketError(inError.getErrorCode());
	}

	// object to help parsing
	DAGS::XMLUtils lParser;
	DAGS::Request lRequest;

	// parse ioGroupXMLString
	lErrorCode = lParser.parseXML(lGroupReceived, mNoParseTags);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage += "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//modify the clientId
	mClientId = lRequest.getClientId();
	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return lErrorCode;
	}

	//get Attributes Group
	lErrorCode = lParser.getAttributeGroup(outGroup);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//get the vector of Jobs
	std::vector<std::pair<int, std::string> > lScore;
	std::list<int> lNeedScore;
	lErrorCode = lParser.getJobs(outJobs, lScore, lNeedScore);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	return 1;
}


int DAGS::Client::getGroup(unsigned int inGroupId,
                           DAGS::Group& outGroup,
                           std::vector<std::pair<std::string, DAGS::Job> > &outJobs,
                           std::string inUniverseName)
{
	mErrorMessage = "";
	//specify to DAGS that nothing will be send
	mRequestError = NothingToSend;

	mAppName = inUniverseName;
	int lErrorCode = 0;
	std::string lGroupReceived;

	//the client id will be used to specify which groupid is needed.
	int lClientId = mClientId;
	mClientId = inGroupId;
	addXMLHeader(lGroupReceived, "group", inGroupId);
	//put back the client Id so that it does not point to a group id
	mClientId = lClientId;

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		//ask if it's ok to send the group
		//as soon the server send something, it means it's ready to accept data
		std::string lRequestGroup("READY TO SEND INFO");
		addXMLHeader(lRequestGroup, "group");
		lConnection.sendMessage(lRequestGroup);
		lConnection.receiveMessage(lRequestGroup);

		// send request to the server
		lConnection.sendMessage(lGroupReceived);

		// receive the answer from the server
		lConnection.receiveMessage(lGroupReceived);

		//close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = "DAGS::Client::getGroup(inGroupId) Error: ";
		mErrorMessage += inError.getMessage();
		return adjustSocketError(inError.getErrorCode());
	}

	// object to help parsing
	DAGS::XMLUtils lParser;
	DAGS::Request lRequest;

	// parse ioGroupXMLString
	lErrorCode = lParser.parseXML(lGroupReceived, mNoParseTags);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage += "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//modify the clientId
	mClientId = lRequest.getClientId();
	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return lErrorCode;
	}

	//get Attributes Group
	lErrorCode = lParser.getAttributeGroup(outGroup);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//get the vector of Jobs
	std::vector<std::pair<int, std::string> > lScore;
	std::list<int> lNeedScore;
	lErrorCode = lParser.getJobs(outJobs, lScore, lNeedScore);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getGroup(string) Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	return 1;
}


int DAGS::Client::setGroup(std::string& inGroupXMLString,
                           std::string inUniverseName,
                           int inGroupType)
{
	mErrorMessage = "";
	int lClientId = mClientId;
	mAppName = inUniverseName;
	mRequestError = NothingToReceive;
	int lErrorCode = 0;

	std::string lRequestGroup("READY TO SEND INFO");

	if(inGroupType == Terminate) {
		addXMLHeader(inGroupXMLString, "terminate");
		addXMLHeader(lRequestGroup, "terminate");
	} else if(inGroupType == NewGroup) {
		mClientId = -1;
		addXMLHeader(inGroupXMLString, "group");
		addXMLHeader(lRequestGroup, "group");
	} else if(inGroupType == ExistingGroup) {
		addXMLHeader(inGroupXMLString, "group");
		addXMLHeader(lRequestGroup, "group");
	} else if(inGroupType == Quit) {
		addXMLHeader(inGroupXMLString, "quit");
		addXMLHeader(lRequestGroup, "quit");
	} else {
		mErrorMessage = "DAGS::Client::setGroup Error: ";
		mErrorMessage += "Group type unknown.";
		return GroupTypeError;
	}
	//if a user want to call getGroup after setGroup
	mClientId = lClientId;

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		//ask if it's ok to send the group
		//as soon the server send something, it means it's ready to accept data
		lConnection.sendMessage(lRequestGroup, mCompressionLevel);
		lConnection.receiveMessage(lRequestGroup);

		// send request to the server
		lConnection.sendMessage(inGroupXMLString, mCompressionLevel);

		// receive the answer from the server
		if(inGroupType != Terminate && inGroupType != Quit)
			lConnection.receiveMessage(inGroupXMLString);

		//close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = inError.getMessage();
		return adjustSocketError(inError.getErrorCode());
	}

	// object to help parsing
	DAGS::XMLUtils lParser;
	DAGS::Request lRequest;

	// parse inGroupXMLString
	lErrorCode = lParser.parseXML(inGroupXMLString, mNoParseTags);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::setGroup Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::setGroup Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "DAGS::Client::setGroup Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return lErrorCode;
	}

	return 1;
}


int DAGS::Client::getJobs(std::string &ioJobsXMLString,
                          DAGS::SubGroup &outSubGroups,
                          std::vector<std::pair<std::string,DAGS::Job> > &outJobs,
                          std::string inUniverseName)
{
	mErrorMessage = "";
	if(ioJobsXMLString == std::string("")) {
		mRequestError = NothingToSend;
	} else {
		mRequestError = NoError;
	}
	mAppName = inUniverseName;
	int lErrorCode = 0;
	addXMLHeader(ioJobsXMLString, "jobs");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);
		lConnection.setSockOpt(Socket::eLinger, 0);

		// send request to the server
		lConnection.sendMessage(ioJobsXMLString, mCompressionLevel);

		// receive the answer from the server
		lConnection.receiveMessage(ioJobsXMLString);

		//close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = "DAGS::Client::getJobs Error: ";
		mErrorMessage += inError.getMessage();
		return adjustSocketError(inError.getErrorCode());
	}

	// object to help parsing
	DAGS::XMLUtils lParser;
	DAGS::Request lRequest;

	// parse ioJobsXMLString
	lErrorCode = lParser.parseXML(ioJobsXMLString, mNoParseTags);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getJobs Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getJobs Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//modify the clientId
	mClientId = lRequest.getClientId();
	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "DAGS::Client::getJobs Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return lErrorCode;
	}

	//get Attributes Group
	lErrorCode = lParser.getAttributeSubGroup(outSubGroups);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getJobs Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	//get the vector of Jobs
	std::vector<std::pair<int, std::string> > lScore;
	std::list<int> lNeedScore;
	lErrorCode = lParser.getJobs(outJobs, lScore, lNeedScore);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getJobs Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return adjustParserError(lErrorCode);
	}

	return 1;
}


int DAGS::Client::reseed(std::string &ioJobsXMLString,
                         std::string inUniverseName)
{
	mErrorMessage = "";
	mRequestError = NoError;
	mAppName = inUniverseName;
	addXMLHeader(ioJobsXMLString, "reseed");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);
		lConnection.setSockOpt(Socket::eLinger, 0);
		lConnection.setSockOpt(Socket::eRecvTimeOut, 1);
		lConnection.setSockOpt(Socket::eSendTimeOut, 4);

		// send request to the server
		lConnection.sendMessage(ioJobsXMLString, mCompressionLevel);

		lConnection.receiveMessage(ioJobsXMLString);

		//dont wait for an answer/process from the server and close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		mErrorMessage = "DAGS::Client::reseed Error: ";
		// error message of Exception.
		mErrorMessage += inError.getMessage();
		return adjustSocketError(inError.getErrorCode());
	}

	//if the client ask for another subgroup, it will be a new client
	mClientId = -1;


	return 1;
}

void DAGS::Client::doNotParseXMLTags(std::vector<std::string> inDontParse)
{
	//remove the old values of XML tags
	mNoParseTags.clear();

	mNoParseTags = inDontParse;

}

std::string DAGS::Client::getErrorMessage()
{
	std::string outError = mErrorMessage;
	mErrorMessage = "";
	return outError;
}

std::pair<std::string, int> DAGS::Client::getState()
{
	mErrorMessage = "";
	std::pair<std::string, int> outState;
	int lErrorCode;

	mRequestError = NoError;
	std::string lStateString;
	addXMLHeader(lStateString, "state");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		// send request to the server
		lConnection.sendMessage(lStateString, mCompressionLevel);

		// receive the answer from the server
		lConnection.receiveMessage(lStateString);

		//dont wait for an answer/process from the server and close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		mErrorMessage = "DAGS::Client::getState Error: ";
		// error message of Exception.
		mErrorMessage += inError.getMessage();
		return outState;
	}

	// object to help parsing
	DAGS::XMLUtils lParser;
	DAGS::Request lRequest;

	// parse ioJobsXMLString
	lErrorCode = lParser.parseXML(lStateString, mNoParseTags);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getState Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return outState;
	}

	// get the requestga attributes
	lErrorCode = lParser.getAttributeRequest(lRequest);
	if(lErrorCode < 0) {
		mErrorMessage = "DAGS::Client::getState Error: ";
		mErrorMessage += lParser.getErrorMessage();
		return outState;
	}

	//verify if error sent by server
	lErrorCode = lRequest.getError();
	if(lErrorCode != 1) {
		mErrorMessage = "DAGS::Client::getState Error: ";
		std::ostringstream lOSS;
		lOSS << "The server returned an error: " << lErrorCode;
		mErrorMessage += lOSS.str();
		return outState;
	}

	outState.first = lRequest.getAppName();
	outState.second = lRequest.getGroupId();

	return outState;
}

void DAGS::Client::updateEnvironment(std::string& inEnvironment)
{
	mRequestError = NothingToReceive;
	addXMLHeader(inEnvironment, "setEnv");

	//connect to the server
	try {
		Socket::Cafe lConnection(mServerAdd);

		// send request to the server
		lConnection.sendMessage(inEnvironment);

		// receive the answer from the server
		lConnection.receiveMessage(inEnvironment);

		//close the connection
		lConnection.close();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		mErrorMessage = inError.getMessage();
	}
}



void DAGS::Client::addXMLHeader(std::string &ioXMLString,
                                const std::string inRequestType,
                                int inGroupId)
{
	std::ostringstream lXmlHeader;

	XML::Streamer lStreamer(lXmlHeader, 0);
	lStreamer.openTag("DAGS");
	lStreamer.insertAttribute("id", mClientId); //client id
	lStreamer.insertAttribute("rt", inRequestType); //request type
	lStreamer.insertAttribute("err", mRequestError); //error number
	lStreamer.insertAttribute("app", mAppName); //application name
	lStreamer.insertAttribute("ver", mVersion); //Client's version
	if(inGroupId > -1)
		lStreamer.insertAttribute("gid", inGroupId); //group's id of the request
	lStreamer.insertAttribute("comp", mCompressionLevel); //group's id of the request
	lStreamer.insertStringContent(ioXMLString, false);
	lStreamer.closeTag(); //close DAGS

	ioXMLString = lXmlHeader.str();
}


int DAGS::Client::adjustSocketError(const int inErrorSocket) const
{
	return (inErrorSocket - 31);
}


int DAGS::Client::adjustParserError(const int inParserError) const
{
	return (inParserError - 32);
}
