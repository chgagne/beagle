/*
 *  FileParameters.hpp
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

#ifndef _FILEPARAMETERS
#define _FILEPARAMETERS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace DAGS
{
/*!
 *  \class FileParameters dags/FileParameters.hpp "dags/FileParameters.hpp"
 *  \brief Read parameters in a file.
 *
 *  This class provides functions to read a DAGS configurations file.
 */
class FileParameters
{

public:

	/*! \brief Constructor with a file as argument.
	 *
	 *  This constructor load directly the specified file when an object is created.
	 *  \param inFile is a string(std::string) object containing the file name with the path to the file.
	 */
	FileParameters(std::string inFile);

	/*! \brief Destructor.
	 */
	~FileParameters();

	/*! \brief get the parameter's value (int)
	 *
	 *  If the parameter is not found, the outValue will not be modified.  If it is found, outValue will take the parameter's value.
	 *  \param inName: the parameter's name
	 *  \param outValue: the value of the parameter
	 *  \return return true if the parameter exist, false otherwise.  The parameter's value, if it exist, is returned in the double outValue.
	 */
	bool getParameterValue(const std::string inName, int& outValue);

	/*! \brief get the parameter's value (double)
	 *
	 *  If the parameter is not found, the outValue will not be modified.  If it is found, outValue will take the parameter's value.
	 *  \param inName: the parameter's name
	 *  \param outValue: the value of the parameter
	 *  \return return true if the parameter exist, false otherwise.  The parameter's value, if it exist, is returned in the double outValue.
	 */
	bool getParameterValue(const std::string inName, double& outValue);

	/*! \brief get the parameter's value (string)
	 *
	 *  If the parameter is not found, the outValue will not be modified.  If it is found, outValue will take the parameter's value.
	 *  \param inName: the parameter's name
	 *  \param outValue: the value of the parameter    
	 *  \return return true if the parameter exist, false otherwise.  The parameter's value, if it exist, is returned in the string outValue.
	 */
	bool getParameterValue(const std::string inName, std::string& outValue);

	/*! \brief get the parameter's value (int)
	 *
	 *  If the parameter is not found, the outValue will not be modified.  If it is found, outValue will take the parameter's value.
	 *  \param inName: the parameter's name
	 *  \param outValue: the value of the parameter
	 *  \return return true if the parameter exist, false otherwise.  The parameter's value, if it exist, is returned in the double outValue.
	 */
	bool getParameterValue(const std::string inName, unsigned int& outValue);

	/*! \brief Return the error message.
	 */
	std::string getErrorMessage()
	{
		return mErrorMessage;
	}

	/*! \brief Load/parse file parameters.
	 *
	 *  Load the specified file
	 *  \param inRootNodeName: the root name of the xml file
	 *  \return TRUE if the file could be loaded and parsed correctly, FALSE otherwise.
	 */
	bool parseFile(std::string inRootNodeName);

private:

	/*! \brief Map containing parameters name with its value.
	 */
	std::map<std::string, std::string> mParametersMap;

	/*! \brief file's name
	 */
	std::string mFileName;

	/*! \brief Error Message
	 */
	std::string mErrorMessage;

	//disable default constructor
	FileParameters();
};
}

#endif
