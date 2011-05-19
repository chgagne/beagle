/*
 *  main.cpp
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


#include "DefineSMON.h"
#include "MonitorRequest.hpp"
#include "PACC/XML/Streamer.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace PACC;
using namespace std;

//Default path et file of configuration file
#define DAGS_MONITOR_FILE_PARAMETERS_NAME  "./dags-monitor.conf"



void usage()
{
	cout << "Usage: dags-monitor [options]" << endl <<endl;
	cout << "    -f filename : Path and name of monitor's configuration file." << endl;
	cout << "    -c filename : Create a default monitor's configuration file." << endl;
	cout << "    -D          : Stats about all clients." << endl;
	cout << "    -d clientID : Stats about the client with id." << endl;
	cout << "    -E          : All environments of every groups." << endl;
	cout << "    -e groupID  : Environment of the groupID." << endl;
	cout << "    -g groupID  : Get the entire groupID." << endl;
	cout << "    -s          : Get DAGS state." << endl;
	cout << "    -m filename : Write a milestone." << endl;
	cout << "    -v          : Version number and configuration file." << endl;
}

void createConfigFile(std::string inFileName);

int main(int argc, char** argv)
{
	std::string lFileParameters(DAGS_MONITOR_FILE_PARAMETERS_NAME);
	std::string lFileMilestone(SMON_DEFAULT_MILESTONE);
	bool lMilestone = false;
	bool lVerbose = false;
	std::string lRequestType;
	int lID = 0;

	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] != '-') break;
		if(argv[i][1] == 'f') {
			lFileParameters = &argv[++i][0];
		} else if(argv[i][1] == 'c') {
			lFileParameters = &argv[++i][0];
			createConfigFile(lFileParameters);
			return 0;
		} else if(argv[i][1] == 'D') {

			lRequestType = "DistributionList";
		} else if(argv[i][1] == 'd') {
			if(i+1 >= argc) {
				cerr << "The option 'd' need the argument id" << endl;
				return 0;
			}
			lID = atoi(&argv[++i][0]);
			lRequestType = "Distribution";
		} else if(argv[i][1] == 'E') {
			lRequestType = "EnvironmentList";
		} else if(argv[i][1] == 'e') {
			if(i+1 >= argc) {
				cerr << "The option 'e' need the argument id." << endl;
				return 0;
			}
			lID = atoi(&argv[++i][0]);
			lRequestType = "Environment";
		} else if(argv[i][1] == 'g') {
			if(i+1 >= argc) {
				cerr << "The option 'g' need the argument id." << endl;
				return 0;
			}
			lID = atoi(&argv[++i][0]);
			lRequestType = "Group";
		} else if(argv[i][1] == 's') {
			lRequestType = "States";
		} else if(argv[i][1] == 'm') {
			lMilestone = true;
			if(i+1 >= argc) {
				cerr << "The option 'm' need a filename." << endl;
				return 0;
			}
			lFileMilestone = &argv[++i][0];
		} else if(argv[i][1] == 'v') {
			lVerbose = true;
		} else {
			usage();
			return 0;
		}
	}

	SMON::MonitorRequest lMonitorRequest;
	bool lParsed = lMonitorRequest.readFileParameters(lFileParameters);
	if(lParsed == false) {
		cout << "*** SMON Simple MONitor for DAGS : Config file error. ***" << endl;
		return 0;
	}
	if(lVerbose) {
		cout << SMON_DAGS_SERVER_NAME_VERSION_STRING << endl;
		lMonitorRequest.printFileParameters();
		cout << endl;
		return 0;
	}

	if(!lMilestone) lMonitorRequest.requestInformation(lRequestType, lID);
	else {
		lMonitorRequest.writeMilestone(lFileMilestone);
	}

	return 0;
}

void createConfigFile(std::string inFileName)
{
	std::ostringstream lOSS;
	XML::Streamer lStreamer(lOSS);
	lStreamer.openTag("SMON");

	//every option supported by DAGS with its corresponding value
	lStreamer.openTag(SMON_APPNAME);
	lStreamer.insertAttribute("value", SMON_DEFAULT_APPNAME);
	lStreamer.closeTag();
	lStreamer.openTag(SMON_IPADDRESS);
	lStreamer.insertAttribute("value", SMON_DEFAULT_IPADDRESS);
	lStreamer.closeTag();
	lStreamer.openTag(SMON_PORT);
	lStreamer.insertAttribute("value", SMON_DEFAULT_PORT);
	lStreamer.closeTag();

	lStreamer.closeTag();//close SMON

	//write into the file name specified.
	ofstream outFile(inFileName.c_str());
	outFile << lOSS.str();
	outFile.close();

	cout << "*** SMON Simple MONitor for DAGS : Config file created. ***" << endl;
}
