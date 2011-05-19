/*
 *  XMLUtils.hpp
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

#ifndef XMLUtils_DAGS_HPP
#define XMLUtils_DAGS_HPP

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include "dags/Request.hpp"
#include "dags/Group.hpp"
#include "dags/Subgroup.hpp"
#include "dags/Job.hpp"
#include "dags/Monitor.hpp"
#include "PACC/XML.hpp"

using namespace PACC;

namespace DAGS
{

/*!
 *  \class XMLUtils dags/XMLUtils.hpp "dags/XMLUtils.hpp"
 *  \brief A class that parse and analyze the XML stream
 *   This class parse the XML stream. It is used by the Master and the slaves. By request, it will fill objects ( DAGS::Request , DAGS::Group , DAGS::SubGroup and DAGS::Jobs ).
 */
class XMLUtils
{

public:

	/*! \brief Default constructor
	 */
	XMLUtils();

	/*! \brief Destructor
	 */
	~XMLUtils();

	/*! \brief Function to fill a vector of Jobs
	 *
	 *  This function fill vector  of Jobs. In each element there is pair of sub-elements. The first one is the XML string of the Job. The second one is the DAGS::Job object related to the Job's string.
	 *  \param outJobsVector: vector<pair< string, DAGS::Job > > The vector is received by reference.
	 *  \param outScore: already computed score
	 *  \param outNeedScore: a vector of job id that need to be computed.
	 *  \param inLowMemory: if the server is in low memory mode.
	 *  \return a eErrorCode if there is one error and 1 if there is no error.
	 *  \sa DAGS::Job
	 *  \sa eErrorCode
	 */
	int getJobs(std::vector<std::pair<std::string,DAGS::Job> > &outJobsVector,
	            std::vector<std::pair<int, std::string> > &outScore,
	            std::list<int> &outNeedScore,
	            bool inLowMemory = false);

	/*! \brief Function to fill a vector of score corresponding to an id
	 *
	 *  This function fill a vector of pair with first beeing the Jobs id and second, it's score
	 */
	int getScore(std::vector<std::pair<int, std::string> > &outScore);

	/*! \brief Function to fill a vector of Id of Jobs and the corresponding score is available
	 *
	 *  This function is used when a reseed is needed.  The Jobs are empty
	 *  and only the Jobs id is received. If a score is present, then a list
	 *  of it is made.
	 */
	int getReseed(std::vector<int>& outId, std::vector<std::pair<int, std::string> > &outScore);

	/*! \brief Get the group's attributes.
	 *
	 *  This function fill a DAGS::Group object with the attributes at the <group> tag in the XML string.
	 *  \param outGroup: a DAGS::Group object. The object is modified by reference.
	 *  \return a eErrorCode if there is one error and 1 if there is no error.
	 *  \sa DAGS::Group
	 */
	int getAttributeGroup(DAGS::Group &outGroup);

	/*! \brief Get the group's attributes
	 *
	 *  This function fill a DAGS::SubGroup object with the attributes at the <subgroup> tag in the XML string.
	 *  \param outSubGroup: a DAGS::SubGroup object. The object is modified by reference.
	 *  \return a eErrorCode if there is one error and 1 if there is no error.
	 *  \sa DAGS::SubGroup
	*/
	int getAttributeSubGroup(DAGS::SubGroup &outSubGroup);

	/*! \brief Get the request's attributes
	 *
	 *  This function fill a DAGS::Request object with the attributes at the <DAGS> tag in the XML string.
	 *  \param outRequest: a DAGS::Request object. The object is modified by reference.
	 *  \return a eErrorCode if there is one error and 1 if there is no error.
	 *  \sa DAGS::Request
	 */
	int getAttributeRequest(DAGS::Request &outRequest);

	/*! \brief Get the monitor's request attributes
	 *
	 *  This function fill a DAGS::monitor object with the attributes at the <DAGS> tag in the XML string.
	 *  \param outMonitor: a DAGS::Monitor object. The object is modified by reference.
	 *  \return a eErrorCode if there is an error and 1 if there is no error.
	 *  \sa DAGS::Monitor
	 */
	int getAttributeMonitor(DAGS::Monitor& outMonitor);

	/*! \brief Parse the XML String and fill a tree.
	 *
	 *  This function parse the xml string and fill a DOM Tree. mDoc is the root. This function also catch all syntax error in the xml string
	 *  \param inXMLString: XML string to be parsed
	 *  \param inDontParseTag: tag that will not be parsed.
	 */
	int parseXML(const std::string& inXMLString,
	             const std::vector<std::string>& inDontParseTag = std::vector<std::string>());

	/*! \brief Return the error message
	*/
	std::string getErrorMessage()
	{
		return mErrorMessage;
	}

	/*! \brief Error codes
	 *
	 *  Error codes related to the parser and DOM Tree manipulation.  they are always lower than 0 to easily
	 *  find if a function returns an error.
	 */
	enum eErrorCode
	{
	    InitError=-6,/*!<XCERCES-DOM Initialisation error*/
	    ParseXMLError,/*!<XML Parsing Error*/
	    ParseDOMError,/*!<DOM Error*/
	    ParseUndefinedError,/*!<Unknown error about the parsing of the XML String*/
	    NoNameElementError,/*!<A TAG is missing on the XML String*/
	    NoNameAttributeError,/*!<An attribute have an error*/
	    EmptyXMLError/*!<There is no child XML tag inside the Connection TAG*/
	};

private:

	/*! \brief Error Code returned to the caller.
	 * \sa eErrorCode
	 */
	int mErrorCode;

	/*! \brief Parser object. Object that contains information about the XML.  It creates the root node.
	 */
	XML::Document* mParser;

	/*! \brief The root node, a node containing the DAGS tag and the information tag.
	 */
	XML::Iterator mRootNodePtr;

	/*! \brief The node that contains the information inside the information (G, SG) tag.
	 */
	XML::Iterator mNodeInformationPtr;

	/*! \brief The node that contains the information inside the Job (T) tag.
	 */
	XML::Iterator mNodeJobsPtr;

	/*! \brief Specify if the Connection tag is valid.
	 */
	bool mRequest;

	/*! \brief Specify if the information tag is a group of Jobs.
	 */
	bool mSubGroup;

	/*! \brief Specify if the information tag is a group.
	 */
	bool mGroup;

	/*! \brief Specify if the information tag is a Monitor request.
	 */
	bool mMonitor;

	/*! \brief Specify if the node Job has been set.
	 */
	bool mJob;

	/*! \brief Error message.
	 */
	std::string mErrorMessage;

};

}

#endif //XMLUtils_DAGS_HH




