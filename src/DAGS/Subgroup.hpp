/*
 *  SubGroup.hpp
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

#ifndef __SUBGROUP_DAGS_HH
#define __SUBGROUP_DAGS_HH
#include <string>

namespace DAGS
{

/*!
*  \class SubGroup dags/Subgroup.hpp "dags/Subgroup.hpp"
*  \brief DAGS SubGroup class
*
*  This class is used to help parse the XML code returned by the server
*  or to help rebuild a subgroup to send to the server.
*  getGroupId() returns the group's id getAppName returns the
*  application's name.
*/

class SubGroup
{

public:

	/*! \brief Default constructor
	*/
	SubGroup()
			: mGroupId(-1),
			mAppName(""),
			mEnvironment(""),
			mGeneration(-1)
	{}

	/*! \brief get group id
	*/
	int getGroupId()
	{
		return mGroupId;
	}

	/*! \brief get application name
	*/
	std::string getAppName()
	{
		return mAppName;
	}

	/*! \brief get the group's environment
	 */
	std::string getEnvironment()
	{
		return mEnvironment;
	}

	/*! \brief get the group's generation
	 */
	int getGeneration()
	{
		return mGeneration;
	}

	/*! \brief Return the time it took to evaluate 1 job.
	 */
	double getEvalTime()
	{
		return mEvalTime;
	}

	/*! \brief set group's id
	*/
	void setGroupId(const int inGroupId)
	{
		mGroupId = inGroupId;
	};

	/*! \brief set application's name
	*/
	void setAppName(const std::string& inAppName)
	{
		mAppName = inAppName;
	}

	/*! \brief set group's environment
	*/
	void setEnvironment(const std::string inEnvironment)
	{
		mEnvironment = inEnvironment;
	}

	/*! \brief set group's generation
	*/
	void setGeneration(const int inGeneration)
	{
		mGeneration = inGeneration;
	}

	/*! \brief set the time it took to evaluate 1 job
	*/
	void setEvalTime(const double inEvalTime)
	{
		mEvalTime = inEvalTime;
	}

	/*! \brief copy constructor
	*/
	SubGroup(const SubGroup& inSubGroup)
	{
		mGroupId = inSubGroup.mGroupId;
		mAppName = inSubGroup.mAppName;
		mEnvironment = inSubGroup.mEnvironment;
		mGeneration = inSubGroup.mGeneration;
		mEvalTime = inSubGroup.mEvalTime;
	}

private:

	/*! \brief  group's id
	*/
	int mGroupId;

	/*! \brief  application's name
	*/
	std::string mAppName;

	/*! \brief  group's environment
	*/
	std::string mEnvironment;

	/*! \brief  group's generation
	*/
	int mGeneration;

	/*! \brief mean time to evaluate 1 job (usually called Tf)
	*/
	double mEvalTime;
};
}

#endif
