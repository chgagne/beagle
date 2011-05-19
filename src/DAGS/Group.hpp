/*
 *  Group.hpp
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
#ifndef __Group_DAGS_HH
#define __Group_DAGS_HH
#include <string>
#include <iostream>
using namespace std;

namespace DAGS
{

/*!
*  \class Group dags/Group.hpp "dags/Group.hpp"
*  \brief DAGS Group class.
*
*  This class is used to help parse the XML code returned by the server
*  or to help rebuild a Group to send to the server.  The default constructor
*  should be used.  The other functions are there to get the specific xml tag
*  or to set them.  GetDBId() returns the id of the Group, getGeneration() get
*  the generation number, getAppName() returns the universe's name or the
*  application's name and getEnvironment() returns the environment. The function
*  setId() sets the Group id, setGeneration() sets the generation number, setAppName()
*  sets the application's name and setEnvironment() sets the environment of the Group. This class
*  is often used with the conjonction of XMLUtils class to parse the xml tags.
*/
class Group
{

public:

	/*! Default Constructor
	*/
	Group()
			: mDBId(-1),
			mGeneration(-1),
			mAppName(""),
			mEnvironment(""),
			mStatus(-1),
			mCounter(-1),
			mSpecialFlag(-1),
			mTime(-1),
			mNbJobs(0),
			mNbScoreNeeded(0),
			mDistribute(false)
	{}

	/*! \brief Get the Group's id
	*/
	int getDBId()
	{
		return mDBId;
	}

	/*! \brief Get the Group's generation
	*/
	int getGeneration()
	{
		return mGeneration;
	}

	/*! \brief Get the application's name
	*/
	std::string getAppName()
	{
		return mAppName;
	}

	/*! \brief Get the Group's environment
	*/
	std::string getEnvironment()
	{
		return mEnvironment;
	}

	/*! \brief Get the Group's status.
	*/
	int getStatus()
	{
		return mStatus;
	}

	/*! \brief Get the Group's counter.
	*/
	int getCounter()
	{
		return mCounter;
	}

	/*! \brief Get the Group's special flag
	*/
	int getSpecialFlag()
	{
		return mSpecialFlag;
	}

	/*! \brief Get the Group's time that it was last sent to clients
	*
	*  \return the number of seconds since 1970
	*/
	long long getTime()
	{
		return mTime;
	}

	/*! \brief Get the number of Jobs that are present in the Group
	*/
	int getNbJobs()
	{
		return mNbJobs;
	}

	/*! \brief Get the number of score needed for the group
	*/
	int getNbScoreNeeded()
	{
		return mNbScoreNeeded;
	}

	/*! \brief If the environment must be sent to clients.
	 */
	bool getDistributeEnv()
	{
		return mDistribute;
	}

	/*! \brief Set the Group's id
	*/
	void setDBId(int inId)
	{
		mDBId = inId;
	}

	/*! \brief Set the application's name
	*/
	void setAppName(const std::string& inAppName)
	{
		mAppName=std::string(inAppName.c_str());
	}

	/*! \brief Set the Group's environment
	*/
	void setEnvironment(const std::string& inEnvironment)
	{
		mEnvironment=std::string(inEnvironment.c_str());
	}

	/*! \brief Set the Group's generation
	*/
	void setGeneration(int inGeneration)
	{
		mGeneration=inGeneration;
	}

	/*! \brief Set the Group's state
	*/
	void setStatus(int inStatus)
	{
		mStatus=inStatus;
	}

	/*! \brief Set the Group's counter
	*/
	void setCounter(int inCounter)
	{
		mCounter=inCounter;
	}

	/*! \brief Set the Group's counter
	*/
	void setSpecialFlag(int inFlag)
	{
		mSpecialFlag = inFlag;
	}

	/*! \brief Set the time the Group was all sent to clients
	*
	*  \param inTime: number of seconds since 1970
	*/
	void setTime(long long inTime)
	{
		mTime = inTime;
	}


	/*! \brief Set the number of Jobs that are present in the Group
	*/
	void setNbJobs(int inNbJobs)
	{
		mNbJobs = inNbJobs;
	}

	/*! \brief Set the number of Score that are needed for the group
	*/
	void setNbScoreNeeded(int inNbScore)
	{
		mNbScoreNeeded = inNbScore;
	}


	/*! \brief Set if the environment must be sent to the clients.
	*/
	void setDistributeEnv(bool inDistribute)
	{
		mDistribute = inDistribute;
	}

	/*! \brief Copy constructor
	*/
//       Group(const Group& inGroup)
//       {
//         mDBId        = inGroup.mDBId;
//         mGeneration  = inGroup.mGeneration;
//         mAppName     = inGroup.mAppName;
//         mEnvironment = inGroup.mEnvironment;
//         mStatus      = inGroup.mStatus;
//         mCounter     = inGroup.mCounter;
//         mSpecialFlag = inGroup.mSpecialFlag;
//         mTime        = inGroup.mTime;
//         mNbJobs    = inGroup.mNbJobs;
//         mDistribute  = inGroup.mDistribute;
//       }

	/*! \brief Operator =
	*/
//       const Group &operator =(const Group& inGroup)
//       {
//         if(&inGroup != this)
//         {
//           mDBId        = inGroup.mDBId;
//           mGeneration  = inGroup.mGeneration;
//           mAppName     = inGroup.mAppName;
//           mEnvironment = inGroup.mEnvironment;
//           mStatus      = inGroup.mStatus;
//           mCounter     = inGroup.mCounter;
//           mSpecialFlag = inGroup.mSpecialFlag;
//           mTime        = inGroup.mTime;
//           mNbJobs    = inGroup.mNbJobs;
//           mDistribute  = inGroup.mDistribute;
//         }
//         return *this;
//       }

private:

	/*! \brief Group's id
	*/
	int mDBId;

	/*! \brief Group's generation
	*/
	int mGeneration;

	/*! \brief Application's name of the Group
	*/
	std::string mAppName;

	/*! \brief Group environment
	*/
	std::string mEnvironment;

	/*! \brief Group's state.
	*
	*  <lu><li>0: ready to be evolved. </li>
	       <li>1: presently beeing evolved. </li>
	       <li>2: ready to be evaluated. </li>
	       <li>3: presently beeing evaluated. </li>
	   </lu>
	*/
	int mStatus;

	/*! \brief Group's counter
	*/
	int mCounter;

	/*! \brief Group's special flag
	*/
	int mSpecialFlag;

	/*! \brief The last time the Group was sent to clients
	*/
	long long mTime;

	/*! \brief Number of Jobs that are in the Group.
	*/
	int mNbJobs;

	/*! \brief Number of scores that are needed.
	*/
	int mNbScoreNeeded;

	/*! \brief If the environment of the group must be distributed to clients.
	*/
	bool mDistribute;
};
}
#endif
