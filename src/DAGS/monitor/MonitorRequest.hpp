/*
 *  MonitorRequest.hpp
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

#ifndef __MONITORREQUEST_SMON_HH
#define __MONITORREQUEST_SMON_HH

#include <string>
#include <vector>

using namespace std;


//namespace Simple MONitor
namespace SMON
{

/*! \brief Create XML request and return answers in XML format.
*
*  This class displays information to the console.
*/

class MonitorRequest
{

public:


	/*! \brief Constructor
	 *
	 */
	MonitorRequest();

	/*! \brief Destructor
	 *
	 */
	~MonitorRequest();

	/*! \brief print parameters of the conf file
	 */
	void printFileParameters();

	/*! \brief read parameters for the monitor in the file mConfFileName
	 *
	 *  \param inConfFileName: config file name
	 */
	bool readFileParameters(std::string inConfFileName);

	/*! \brief print informations requested
	 *  \param inRequestType: request type to the server
	 *  \param inID: ID number of the request type (group id for exemple).
	 */
	void requestInformation(std::string inRequestType, unsigned int inID);

	/*! \brief write a milestone that Open BEAGLE can read
	 *
	 *  This function will fetch every groups/jobs from DAGS
	 *  and will convert them so that it can be read from Open Beagle
	 *
	 *  \param inMilestoneName: milestone file name that will be saved
	 */
	void writeMilestone(std::string inMilestoneName);

private:

	std::string mConfFileName;
	std::string mAppName;
	unsigned int mPort;
	std::string mIPAddress;
};

}

#endif // __MONITORREQUEST_SMON_HH
