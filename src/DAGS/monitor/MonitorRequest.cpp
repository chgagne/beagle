/*
*  MonitorRequest.cpp
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

#include "MonitorRequest.hpp"
#include "MonitorComm.hpp"
#include "DefineSMON.h"
#include "dags/FileParameters.hpp"
#include "PACC/XML/Streamer.hpp"
#include <sstream>

using namespace PACC;

SMON::MonitorRequest::MonitorRequest()
		: mAppName(SMON_DEFAULT_APPNAME),
		mPort(SMON_DEFAULT_PORT),
		mIPAddress(SMON_DEFAULT_IPADDRESS)
{}

SMON::MonitorRequest::~MonitorRequest()
{}


void SMON::MonitorRequest::printFileParameters()
{
	cout << "-------------------------- SMON settings --------------------------" << endl;
	cout << "Configuration's file : " << mConfFileName << endl;
	cout << "Application's name   : " << mAppName      << endl;
	cout << "Server's port        : " << mPort         << endl;
	cout << "Server's ip address  : " << mIPAddress    << endl;
	cout << "-------------------------------------------------------------------" << endl;
}

bool SMON::MonitorRequest::readFileParameters(std::string inConfFileName)
{
	mConfFileName = inConfFileName;
	DAGS::FileParameters lFile(mConfFileName);
	bool lParsedFile(lFile.parseFile("SMON"));
	if(!lParsedFile) {
		return false;
	}

	//parse item in the config file and set variables
	if(lFile.getParameterValue(SMON_APPNAME, mAppName) == false) {
		cerr << "WARNING: application's name was not found in the configuration file." << endl;
	}
	if(lFile.getParameterValue(SMON_PORT, mPort) == false) {
		cerr << "Server's port was not found in the configuration file." << endl;
		return false;
	}
	if(lFile.getParameterValue(SMON_IPADDRESS, mIPAddress) == false) {
		cerr << "Server's ip address was not found in the configuration file." << endl;
		return false;
	}
	return true;
}


void SMON::MonitorRequest::requestInformation(std::string inRequestType, unsigned int inID)
{
	std::ostringstream lOSS;
	XML::Streamer lStreamer(lOSS);

	SMON::MonitorComm lMonitorComm(mAppName, mIPAddress, mPort);

	if(inRequestType == "Distribution" || inRequestType == "Environment" || inRequestType == "Group") {
		lMonitorComm.request(lStreamer, inRequestType, inID);
	} else if(inRequestType == "States" || inRequestType == "DistributionList" || inRequestType == "EnvironmentList") {
		lMonitorComm.request(lStreamer, inRequestType);
	}
	//not define
	else {
		std::string lError("Invalid request type: ");
		lError += inRequestType;
		lStreamer.openTag("Error");
		lStreamer.insertStringContent(lError);
		lStreamer.closeTag();
	}
	cout << lOSS.str() << endl;
}


void SMON::MonitorRequest::writeMilestone(std::string inMilestoneName)
{
	//get the number of groups on the server
	std::ostringstream lOSS;
	XML::Streamer lStreamer(lOSS);
	SMON::MonitorComm lMonitorComm(mAppName, mIPAddress, mPort);
	unsigned int lNumberOfGroups = lMonitorComm.getState(lStreamer);
	if(lNumberOfGroups == 0) {
		cout << lOSS.str() << endl;
		return;
	}
	lOSS.str("");

	lStreamer.insertHeader("ISO-8859-1");
	lStreamer.openTag("Beagle");
	lStreamer.insertAttribute("version", "2.1.1");
	lStreamer.openTag("Vivarium");
	lStreamer.openTag("Population");
	lStreamer.insertAttribute("size", lNumberOfGroups);

	//for every groups, get every jobs.
	for(unsigned int i = 0; i < lNumberOfGroups; ++i) {
		DAGS::Group lGroup;
		std::vector<std::pair<std::string, DAGS::Job> > lJobs;

		//fetch the jobs of the group
		lMonitorComm.getGroup(i, lGroup, lJobs);

		lStreamer.openTag("Deme");
		//write the Environment
		lStreamer.insertStringContent(lGroup.getEnvironment());

		//write the individuals
		lStreamer.openTag("Population");
		lStreamer.insertAttribute("size", lJobs.size());
		for(unsigned int j = 0; j < lJobs.size(); ++j) {
			lStreamer.openTag("Individual");
			//write the Score/Fitness
			if(lJobs[j].second.getScore() != "")
				lStreamer.insertStringContent(lJobs[j].second.getScore());
			else {
				lStreamer.openTag("Fitness");
				lStreamer.insertAttribute("valid", "no");
				lStreamer.closeTag();
			}

			//write the Data/Genotype
			lStreamer.insertStringContent(lJobs[j].second.getData());
			lStreamer.closeTag();//close Individual
		}
		lStreamer.closeTag();//close Population
		lStreamer.closeTag();//close Deme
	}
	lStreamer.closeTag(); //close Population
	lStreamer.closeTag(); //close Vivarium
	lStreamer.closeTag(); //close Beagle

	//write to the file
	std::ofstream lOutFile;
	lOutFile.open(inMilestoneName.c_str(), std::ofstream::out);
	lOutFile << lOSS.str();
	lOutFile.close();
}
