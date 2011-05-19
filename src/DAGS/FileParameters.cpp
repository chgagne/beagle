/*
 *  FileParameters.cpp
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


#include "dags/FileParameters.hpp"
#include "PACC/XML/Document.hpp"
#include <string>
#include <stdexcept>

using namespace PACC;
using namespace std;

DAGS::FileParameters::FileParameters(const std::string inFileName)
		:mFileName(inFileName),
		mErrorMessage("")
{}

DAGS::FileParameters::~FileParameters()
{
}

bool DAGS::FileParameters::getParameterValue(std::string inName, int& outValue)
{
	if(mParametersMap.find(inName) == mParametersMap.end()) return false;
	std::istringstream lConvert(mParametersMap[inName].c_str());
	lConvert >> outValue;
	return true;
}

bool DAGS::FileParameters::getParameterValue(std::string inName, double& outValue)
{
	if(mParametersMap.find(inName) == mParametersMap.end()) return false;
	std::istringstream lConvert(mParametersMap[inName].c_str());
	lConvert >> outValue;
	return true;
}

bool DAGS::FileParameters::getParameterValue(std::string inName, std::string& outValue)
{
	if(mParametersMap.find(inName) == mParametersMap.end()) return false;
	outValue = mParametersMap[inName];
	return true;
}

bool DAGS::FileParameters::getParameterValue(std::string inName, unsigned int& outValue)
{
	if(mParametersMap.find(inName) == mParametersMap.end()) return false;
	std::istringstream lConvert(mParametersMap[inName].c_str());
	lConvert >> outValue;
	return true;
}

bool DAGS::FileParameters::parseFile(std::string inRootNodeName)
{
	// verify the validity of the XML stream
	XML::Document* lParser;
	lParser = new XML::Document();

	try {
		lParser->parse(mFileName);
	} catch(std::runtime_error& inError) {
		mErrorMessage = "DAGS::FileParameters::loadFile() parsing XML error: " + std::string(inError.what());
		delete lParser;
		lParser = NULL;
		return false;
	}

	XML::Iterator lRootNodePtr = lParser->getFirstDataTag();
	XML::Iterator lOptionsNodePtr;
	if(lRootNodePtr) {
		lOptionsNodePtr = lRootNodePtr->getFirstChild();
	} else {
		mErrorMessage = "DAGS::FileParameters::loadFile() no root node.";
		delete lParser;
		lParser = NULL;
		return false;
	}

	//verify if the tag name of the root node is DAGS
	std::string lRootName = lRootNodePtr->getValue();
	if(lRootName != inRootNodeName) {
		mErrorMessage = "DAGS::FileParameters::loadFile() root node name (" + lRootName + ") should be DAGS.";
		delete lParser;
		lParser = NULL;
		return false;
	}

	while(lOptionsNodePtr) {
		std::string lParameterName(lOptionsNodePtr->getValue());
		std::string lParameterValue(lOptionsNodePtr->getAttribute("value"));
		mParametersMap[lParameterName] = lParameterValue;

		if(lParameterValue == "") {
			mErrorMessage = "DAGS::FileParameters::loadFile() option " + lParameterName + " has no value.";
			delete lParser;
			lParser = NULL;
			return false;
		}
		lOptionsNodePtr = ++lOptionsNodePtr;
	}

	delete lParser;
	lParser = NULL;
	return true;
}
