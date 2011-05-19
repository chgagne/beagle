/*
 *  Job.hpp
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
#ifndef __JOB_DAGS_HH
#define __JOB_DAGS_HH
#include <string>
namespace DAGS
{

/*!
*  \class Job dags/Job.hpp "dags/Job.hpp"
*  \brief Job class
*
*  This class is used to help parse the XML code returned by the server
*  or to help rebuild a subgroups, or a group to send to the server.
*  GetId() returns the id of the Job, getScore() get
*  the score of the Job, getData() returns the Data of the Job. The function
*  setId() sets the Job's id, setScore() sets the score of the Job, setData()
*  sets the Job's Data.
*/

class Job
{

public:

	/*! \brief Default constructor
	*/
	Job()
			: mId(-1),
			mScore(""),
			mData(""),
			mEvaluate(false)
	{}

	/*! \brief Get the Job ID
	*/
	int getId()
	{
		return mId;
	}

	/*! \brief Get the Job score
	*/
	std::string getScore()
	{
		return mScore;
	}

	/*! \brief Get the Data of the Job
	*/
	std::string getData()
	{
		return mData;
	}

	/*! \brief Get if the data must be evaluated.  True if it must be evaluated.
	*/
	bool getEvaluate()
	{
		return mEvaluate;
	}

	/*! \brief Set the Job ID
	*/
	void setId(int inId)
	{
		mId = inId;
	}

	/*! \brief Set the score
	*/
	void setScore(const std::string& inScore)
	{
		mScore = inScore;
	}

	/*! \brief Set the Data
	*/
	void setData(const std::string& inData)
	{
		mData = inData;
	}

	/*! \brief Set the evaluate variable to true so that the corresponding Data must be evaluated.
	*/
	void setEvaluate(bool inEvaluate)
	{
		mEvaluate = inEvaluate;
	}


	/*! \brief Copy constructor
	*/
	Job(const Job& inJob)
	{
		mId       = inJob.mId;
		mScore    = inJob.mScore;
		mData     = inJob.mData;
		mEvaluate = inJob.mEvaluate;
	}

private:

	/*! \brief Job's ID
	*/
	int mId;

	/*! \brief Job's Score
	*/
	std::string mScore;

	/*! \brief Job's Data
	*/
	std::string mData;

	/*! \brief true if the mData must be evaluated, false otherwise.
	*/
	bool mEvaluate;
};

}

#endif
