/*
 *  XMLUtils.cpp
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


#include "dags/XMLUtils.hpp"
#include <stdexcept>

DAGS::XMLUtils::XMLUtils():
		mErrorCode(1),
		mParser(0),
		mRequest(false),
		mSubGroup(false),
		mGroup(false),
		mMonitor(false),
		mJob(false),
		mErrorMessage("")
{}

DAGS::XMLUtils::~XMLUtils()
{
	if(mParser) delete mParser;
}


int DAGS::XMLUtils::getAttributeGroup(DAGS::Group &outGroup)
{
	// the information tag is not a Group
	if(mGroup == false) {
		mErrorMessage = "DAGS::XMLUtils::getAttributeGroup error: There's no Group tag and attributes.";
		return ParseXMLError;
	}

	//attributes of the Group tag
	std::string lGroupIDStr = mNodeInformationPtr->getAttribute("id"); //group's id
	std::string lAppNameStr = mNodeInformationPtr->getAttribute("app"); //application's name
	//set the value of the generation
	std::string lGeneration = mNodeInformationPtr->getAttribute("gen"); //generation
	outGroup.setGeneration(atoi(lGeneration.c_str()));

	if(lGroupIDStr == "" || lAppNameStr == "") {
		mErrorMessage = "DAGS::XMLUtils::getAttributeGroup error: group's id or the application's name is invalid.";
		return ParseXMLError;
	}

	//set the attributes of the Group in a DAGS::Group object
	outGroup.setDBId(atoi(lGroupIDStr.c_str()));
	outGroup.setAppName(lAppNameStr);

	//get the environment of the Group
	std::ostringstream lEnvironmentTag;
	XML::Iterator lNodeEnvironmentPtr = mNodeInformationPtr->getFirstChild();
	//the environment will be transfered to the clients, so keep it.
	if(lNodeEnvironmentPtr->getAttribute("dist") == "yes")  //distribute
	{
		outGroup.setDistributeEnv(true);
	}
	XML::Iterator lNodeInsideEnvPtr = lNodeEnvironmentPtr->getFirstChild();
	if(lNodeInsideEnvPtr) outGroup.setEnvironment(lNodeInsideEnvPtr->getValue());

	//setting the node Job to be parsed
	mNodeJobsPtr = ++lNodeEnvironmentPtr;
	if(mNodeJobsPtr) mJob = true;
	else {
		mErrorMessage = "DAGS::XMLUtils::getAttributeGroup error: there's no job XML after the Environment tag.";
		return ParseXMLError;
	}
	return 1;
}

int DAGS::XMLUtils::getAttributeSubGroup(DAGS::SubGroup &outSubGroup)
{
	// the information tag is not a SubGroup
	if(mSubGroup == false) {
		mErrorMessage = "DAGS::XMLUtils::getAttributeSubGroup error: There's no SubGroup tag and attributes.";
		return ParseXMLError;
	}

	//attributes of the SubGROUP tag
	std::string lGroupIDStr    = mNodeInformationPtr->getAttribute("id"); //group's id
	std::string lAppNameStr    = mNodeInformationPtr->getAttribute("app"); //application's name
	//set the value of the generation
	std::string lGeneration = mNodeInformationPtr->getAttribute("gen"); //generation
	outSubGroup.setGeneration(atoi(lGeneration.c_str()));
	std::string lEvalTime = mNodeInformationPtr->getAttribute("tf");
	outSubGroup.setEvalTime(atof(lEvalTime.c_str()));

	if(lGroupIDStr == "" || lAppNameStr == "") {
		mErrorMessage = "DAGS::XMLUtils::getAttributeSubGroup error: group's id or the application's name is invalid.";
		return ParseXMLError;
	}

	//set the attributes of the SubGroup in a DAGS::SubGroup object
	outSubGroup.setGroupId(atoi(lGroupIDStr.c_str()));
	outSubGroup.setAppName(lAppNameStr);

	//setting the node Job to be parsed
	XML::Iterator lNodeEnvironmentPtr = mNodeInformationPtr->getFirstChild();
	if(lNodeEnvironmentPtr->getValue() == "Env") //environment
	{
		XML::Iterator lNodeInsideEnvPtr = lNodeEnvironmentPtr->getFirstChild();
		if(lNodeInsideEnvPtr) outSubGroup.setEnvironment(lNodeInsideEnvPtr->getValue());
		mNodeJobsPtr = ++lNodeEnvironmentPtr;
	}
	//no Environment returned, the next child is a <Job>
	else mNodeJobsPtr = mNodeInformationPtr->getFirstChild();


	if(mNodeJobsPtr) mJob = true;
	else {
		std::ostringstream lOSS;
		lOSS << "DAGS::XMLUtils::getAttributeSubGroup error: there's no job XML." << endl;
		mErrorMessage = lOSS.str();
		return ParseXMLError;
	}

	if(mNodeJobsPtr->getValue() != "J") {
		mErrorMessage = "DAGS::XMLUtils::getAttributeSubGroup error: xml tag name is invalid (" + mNodeJobsPtr->getValue() + ")";
		return ParseXMLError;
	}

	return 1;
}

int DAGS::XMLUtils::getAttributeRequest(DAGS::Request &outRequest)
{
	if(mRequest == false) {
		mErrorMessage = "DAGS::XMLUtils::getAttributeRequest error: There's no DAGS tag and attributes.";
		return ParseXMLError;
	}

	//attributes of the DAGS tag
	std::string lClientIDStr    = mRootNodePtr->getAttribute("id"); //client id
	std::string lRequestTypeStr = mRootNodePtr->getAttribute("rt"); //request type
	std::string lErrorStr       = mRootNodePtr->getAttribute("err");//error number
	std::string lAppNameStr     = mRootNodePtr->getAttribute("app");//application name
	std::string lGroupId        = mRootNodePtr->getAttribute("gid");//group's id
	std::string lVersion        = mRootNodePtr->getAttribute("ver");//client's version
	std::string lCompression    = mRootNodePtr->getAttribute("comp");//client's compression level

	//setting the variables of the DAGS::Request object
	outRequest.setClientId(atoi(lClientIDStr.c_str()));
	outRequest.setRequestType(lRequestTypeStr);
	outRequest.setError(atoi(lErrorStr.c_str()));
	outRequest.setAppName(lAppNameStr);
	if(lGroupId != "") outRequest.setGroupId(atoi(lGroupId.c_str()));
	if(lVersion != "") outRequest.setVersion(lVersion);
	if(lCompression != "") outRequest.setCompressionLevel(atoi(lCompression.c_str()));

	return 1;
}

int DAGS::XMLUtils::getAttributeMonitor(DAGS::Monitor& outMonitor)
{
	if(mMonitor == false) {
		mErrorMessage = "DAGS::XMLUtils::getAttributeMonitor error: There's no Monitor tag and attributes.";
		return ParseXMLError;
	}

	//attributes of the Monitor tag
	std::string lRequestStr = mNodeInformationPtr->getAttribute("rt"); //request Type
	std::string lIdStr = mNodeInformationPtr->getAttribute("id");

	//setting the variables of the DAGS::Request object
	outMonitor.setId(atoi(lIdStr.c_str()));
	outMonitor.setRequest(lRequestStr);
	return 1;
}

int DAGS::XMLUtils::getJobs(std::vector<std::pair<std::string,DAGS::Job> > &outJobVector,
                            std::vector<std::pair<int, std::string> > &outScore,
                            std::list<int> &outNeedScore,
                            bool inLowMemory)
{
	if(mJob == false) {
		mErrorMessage = "DAGS::XMLUtils::getJobs error: There's no Jobs tag and attributes.";
		return ParseXMLError;
	}

	DAGS::Job lJob;
	while(mNodeJobsPtr) {
		//attribute of the Job tag
		unsigned int lJobID = atoi(mNodeJobsPtr->getAttribute("id").c_str()); //job's id
		lJob.setId(lJobID);

		//get everything inside the Score tag
		XML::Iterator lNodeScorePtr = mNodeJobsPtr->getFirstChild();

		//there is already a Score for that job, do not evaluate the job
		if(lNodeScorePtr->getAttribute("eval") == "no") //evaluate the Data
		{
			XML::Iterator lNodeInsideScorePtr;
			std::ostringstream lScoreTag;
			if(lNodeScorePtr) lNodeInsideScorePtr = lNodeScorePtr->getFirstChild();
			while(lNodeInsideScorePtr)
			{
				XML::Streamer lStreamerScore(lScoreTag, 0);
				try {
					lNodeInsideScorePtr->serialize(lStreamerScore);
				} catch(std::runtime_error &inError) {
					mErrorMessage = "DAGS::XMLUtils::getJobs error Score tag: " + std::string(inError.what());
					return ParseXMLError;
				}
				lNodeInsideScorePtr = ++lNodeInsideScorePtr;
			}
			outScore.push_back(std::pair<int, std::string>(lJobID, lScoreTag.str()));
			lJob.setScore(lScoreTag.str());
			lJob.setEvaluate(false);
		} else {
			lJob.setEvaluate(true);
			outNeedScore.push_back(lJobID);
		}

		//get everything inside the Data tag
		std::ostringstream lDataTag;
		XML::Iterator  lNodeDataPtr = ++lNodeScorePtr;
		if(lNodeDataPtr) lNodeDataPtr = lNodeDataPtr->getFirstChild();
		while(lNodeDataPtr) {
			XML::Streamer lStreamerData(lDataTag, 0);
			try {
				lNodeDataPtr->serialize(lStreamerData);
			} catch(std::runtime_error &inError) {
				mErrorMessage = "DAGS::XMLUtils::getJobs error Data tag: " + std::string(inError.what());
				return ParseXMLError;
			}
			lNodeDataPtr = ++lNodeDataPtr;
		}
		lJob.setData(lDataTag.str());

		std::ostringstream lJobTag;
		XML::Streamer lStreamerJob(lJobTag, 0);
		try {
			mNodeJobsPtr->serialize(lStreamerJob);
		} catch(std::runtime_error &inError) {
			mErrorMessage = "DAGS::XMLUtils::getJobs error Job tag: " + std::string(inError.what());
			return ParseXMLError;
		}
		if(!inLowMemory) outJobVector.push_back(std::pair<std::string,DAGS::Job>(lJobTag.str(),lJob));
		else outJobVector.push_back(std::pair<std::string,DAGS::Job>("",lJob));

		mNodeJobsPtr = ++mNodeJobsPtr;

		//reset of values
		lJob.setId(-1);
		lJob.setScore("");
		lJob.setData("");
		lJob.setEvaluate(false);

	}

	return 1;
}

int DAGS::XMLUtils::getScore(std::vector<std::pair<int, std::string> > &outScore)
{
	if(mJob == false) {
		mErrorMessage = "DAGS::XMLUtils::getScore error: There's no Jobs tag and attributes.";
		return ParseXMLError;
	}
	while(mNodeJobsPtr) {
		//attribute of the Job tag
		unsigned int lJobID = atoi(mNodeJobsPtr->getAttribute("id").c_str()); //job's id
		//no need to check for the evaluate attribute since they were all evaluated

		//get everything inside the Score tag
		XML::Iterator lNodeScorePtr = mNodeJobsPtr->getFirstChild();
		XML::Iterator lNodeInsideScorePtr;
		std::ostringstream lScoreTag;
		if(lNodeScorePtr) lNodeInsideScorePtr = lNodeScorePtr->getFirstChild();
		while(lNodeInsideScorePtr) {
			XML::Streamer lStreamerScore(lScoreTag, 0);
			try {
				lNodeInsideScorePtr->serialize(lStreamerScore);
			} catch(std::runtime_error &inError) {
				mErrorMessage = "DAGS::XMLUtils::getScore error Score tag: " + std::string(inError.what());
				return ParseXMLError;
			}
			lNodeInsideScorePtr = ++lNodeInsideScorePtr;
		}

		outScore.push_back(std::pair<int, std::string>(lJobID, lScoreTag.str()));

		mNodeJobsPtr = ++mNodeJobsPtr;


	}

	return 1;
}


int DAGS::XMLUtils::getReseed(std::vector<int>& outId, std::vector<std::pair<int, std::string> > &outScore)
{
	if(mJob == false) {
		mErrorMessage = "DAGS::XMLUtils::getReseed error: There's no Jobs tag and attributes.";
		return ParseXMLError;
	}

	//make sure its empty
	outId.clear();
	outScore.clear();

	while(mNodeJobsPtr) {
		//attribute of the Job tag
		unsigned int lJobID = atoi(mNodeJobsPtr->getAttribute("id").c_str()); //job's id

		//verify if it has a score.
		//get everything inside the Score tag
		XML::Iterator lNodeScorePtr = mNodeJobsPtr->getFirstChild();
		//there is no Score for this Job
		if(!lNodeScorePtr) {
			outId.push_back(lJobID);
		} else {
			XML::Iterator lNodeInsideScorePtr;
			std::ostringstream lScoreTag;
			if(lNodeScorePtr) lNodeInsideScorePtr = lNodeScorePtr->getFirstChild();
			while(lNodeInsideScorePtr) {
				XML::Streamer lStreamerScore(lScoreTag, 0);
				try {
					lNodeInsideScorePtr->serialize(lStreamerScore);
				} catch(std::runtime_error &inError) {
					mErrorMessage = "DAGS::XMLUtils::getReseed error Score tag: " + std::string(inError.what());
					return ParseXMLError;
				}
				lNodeInsideScorePtr = ++lNodeInsideScorePtr;
			}

			outScore.push_back(std::pair<int, std::string>(lJobID, lScoreTag.str()));
		}


		mNodeJobsPtr = ++mNodeJobsPtr;
	}
	return 1;
}

//private function
int DAGS::XMLUtils::parseXML(const std::string& inXMLString,
                             const std::vector<std::string>& inDontParseTag)
{
	// verify the validity of the XML stream
	std::istringstream lISSXML(inXMLString);
	if(mParser) delete mParser;
	mParser = new XML::Document();


	//do not parse the XML tag included in inDontParseTag
	if(inDontParseTag.empty() != true) {
		for(unsigned int i = 0; i < inDontParseTag.size(); ++i) {
			mParser->setNoParse(inDontParseTag[i]);
		}
	}

	try {
		mParser->parse(lISSXML);
	} catch(std::runtime_error& inError) {
		mErrorMessage = "DAGS::XMLUtils::parseXML parsing XML error: " + std::string(inError.what());
		return InitError;
	}

	mRootNodePtr = mParser->getFirstDataTag();
	if(mRootNodePtr) {
		mNodeInformationPtr = mRootNodePtr->getFirstChild();
	} else {
		mErrorMessage = "DAGS::XMLUtils::parseXML no root node.";
		return ParseXMLError;
	}

	//verify if the tag name of the root node is DAGS
	std::string lRootName = mRootNodePtr->getValue();
	if(lRootName == "DAGS") mRequest = true;
	else return ParseXMLError;
	if(!mNodeInformationPtr) return 1;//request received;

	//get the tag name of the next sibling// information to be parsed
	std::string lSiblingName = mNodeInformationPtr->getValue();
	if(lSiblingName == "SG") mSubGroup = true;
	else if(lSiblingName == "G") mGroup = true;
	else if(lSiblingName == "M") mMonitor = true;
	else if(lSiblingName == "NO INFORMATION SENT") return EmptyXMLError;
	//make sure the problem is not an error
	//else return ParseXMLError;

	//no error in the XML stream
	return 1;
}






















