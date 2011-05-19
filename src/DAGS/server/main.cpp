/*
 *  main.cpp
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

#include "dags/FileParameters.hpp"
#include "PACC/XML/Streamer.hpp"
#include "Server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



void usage(char** argv);
void createConfigFile(std::string inFileName);

//Default path and name of the configuration file (if none specified on command line).
#define DEFAULT_CONFIGURATION_FILE_PATH_NAME  "dags-server.conf"

int main(int argc, char** argv)
{
	int lVerbose = 2;
	std::string lFileParameter(DEFAULT_CONFIGURATION_FILE_PATH_NAME);
	int lNbConnections = DAGS_DEFAULT_NB_MAX_CONNECTION;
	int lNbThreads = DAGS_DEFAULT_NB_MAX_THREADS;
	bool lConnectionsBool = false;
	bool lThreadsBool = false;

	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] != '-') break;
		if(argv[i][1] == 'v') {
			if(++i >= argc) usage(argv);
			lVerbose = atoi(&argv[i][0]);
			if(lVerbose < 0)
				lVerbose = 0;
			if(lVerbose > 4)
				lVerbose = 4;
		} else if(argv[i][1] == 'f') {
			if(++i >= argc) usage(argv);
			lFileParameter = &argv[i][0];
		} else if(argv[i][1] == 'c') {
			if(++i >= argc) usage(argv);
			lNbConnections = atoi(&argv[i][0]);
			lConnectionsBool = true;
		} else if(argv[i][1] == 't') {
			if(++i >= argc) usage(argv);
			lNbThreads = atoi(&argv[i][0]);
			lThreadsBool = true;
		} else if(argv[i][1] == 'd') {
			if(++i >= argc) usage(argv);
			createConfigFile(&argv[i][0]);
			return 0;
		} else {
			usage(argv);
		}
	}

	DAGS::FileParameters lFile(lFileParameter);
	bool lParsedFile(lFile.parseFile("DAGS"));
	if(!lParsedFile) {
		cout << lFile.getErrorMessage() << endl;
		return 0;
	}

	int lPort = DAGS_DEFAULT_PORT;
	lFile.getParameterValue(DAGS_PORT, lPort);
	if(!lConnectionsBool) lFile.getParameterValue(DAGS_NB_MAX_CONNECTION, lNbConnections);
	if(!lThreadsBool) lFile.getParameterValue(DAGS_NB_MAX_THREADS, lNbThreads);

	try {
		DAGS::Server lServer(lFile, lVerbose, lPort, lNbConnections, lNbThreads);
		lServer.start();
		lServer.wait();
	} catch(Socket::Exception inError) {
		// error message of Exception.
		std::cout << "Can't create a server object.  Error message is: \n" << inError.getMessage() << std::flush << std::endl;
		exit(1);
	}
	cout << "*** DAGS Agile Grid Scheduler stopped. ***" << endl;

	return 0;
}

void usage(char** argv)
{
	cout << "Usage: " << argv[0] << " [options]" << endl << endl;
	cout << "    -v degree      : Verbose. " << "0: default value (no verbose)";
	cout << ", 1: simple output, 2: full console output, 3: time of thread, 4: time of every methods" << endl;
	cout << "    -f filename    : Path and name of DAGS's configuration file." << endl;
	cout << "    -c connections : number of connections allowed." << endl;
	cout << "    -t threads     : number of threads allowed." << endl;
	cout << "    -d dump file   : Filename of DAGS's configuration file dump." << endl;
	exit(1);
}


void createConfigFile(std::string inFileName)
{
	std::ostringstream lOSS;
	XML::Streamer lStreamer(lOSS);
	lStreamer.openTag("DAGS");

	//every option supported by DAGS with its corresponding value
	lStreamer.openTag(DAGS_PORT);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_PORT);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_NB_MAX_CONNECTION);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_NB_MAX_CONNECTION);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_NB_MAX_THREADS);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_NB_MAX_THREADS);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_DB_NAME);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_DB_NAME);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_ENABLE_CLIENT_STAT);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_ENABLE_CLIENT_STAT);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_NUMBER_JOBS_TO_CRUNCHER);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_NUMBER_JOBS_TO_CRUNCHER);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_EXPECTED_TIME_BETWEEN_COMMS);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_EXPECTED_TIME_BETWEEN_COMMS);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_CLIENT_TIMEOUT);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_CLIENT_TIMEOUT);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_CLIENT_NUMBER_USED_STAT);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT);
	lStreamer.closeTag();
	for(int i = 1; i <= DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT; ++i) {
		std::ostringstream lOSSWeight;
		lOSSWeight << DAGS_CLIENT_STAT_WEIGHT << i;
		lStreamer.openTag(lOSSWeight.str());
		lStreamer.insertAttribute("value", (double)1/DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT);
		lStreamer.closeTag();
	}
	lStreamer.openTag(DAGS_DB_SYNC);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_DB_SYNC);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_GROUP_DB_SYNC);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_GROUP_DB_SYNC);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_MEMORY_SHORT);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_MEMORY_SHORT);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_GROUP_COMPRESSION);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_GROUP_COMPRESSION);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_SUBGROUP_COMPRESSION);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_SUBGROUP_COMPRESSION);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_LOG_ERROR_FILE);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_LOG_ERROR_FILE);
	lStreamer.closeTag();
	lStreamer.openTag(DAGS_LOG_MESSAGE_FILE);
	lStreamer.insertAttribute("value", DAGS_DEFAULT_LOG_MESSAGE_FILE);
	lStreamer.closeTag();

	lStreamer.closeTag();//close DAGS

	//write into the file name specified.
	ofstream outFile(inFileName.c_str());
	outFile << lOSS.str();
	outFile.close();

	cout << "*** DAGS Agile Grid Scheduler : Config file created. ***" << endl;
}
