/*
 *  Distribution.hpp
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


#ifndef __DISTRIBUTION_DAGS_HH
#define __DISTRIBUTION_DAGS_HH
#include "PACC/Util/Date.hpp"
#include <string>
#include <vector>

namespace DAGS
{

/*!
*  \class Distribution dags/Distribution.hpp "dags/Distribution.hpp"
*  \brief DAGS Distribution class to provide clients statistics
*
*  It's used to keep statistics for clients.  Statistics include
*  the IP address (mIPAddress), the last time it connected with
*  the server (mLastConnect), the client ID (mClientID),
*  and stats for load balancing (mStats).
*/

class Distribution
{

public:

	/*! \brief Default Constructor
	*/
	Distribution()
			: mIPAddress(""),
			mLastConnect(0),
			mTime(""),
			mStats(5, 0),
			mClientID(-1),
			mLoadBalancing(0),
			mValid(false),
			mTimeLastJob(0.0)
	{}

	/*! \brief Set the client's IP address
	*/
	void setIPAddress(const std::string& inIPAddress)
	{
		mIPAddress = inIPAddress;
	}

	/*! \brief Set the last connect time (seconds since 1970)
	*/
	void setLastConnect(long inLastConnect)
	{
		mLastConnect = inLastConnect;
	}

	/*! \brief Set the current time in a string format.
	*/
	void setCurrentTime()
	{
		PACC::Date lDate;
		mTime = lDate.get();
	}

	/*! \brief Set the vector of stats
	*/
	void setStats(const std::vector<double>& inStats)
	{
		mStats = inStats;
	}

	/*! \brief Set the client's ID
	*/
	void setClientID(int inClientID)
	{
		mClientID = inClientID;
	}

	/*! \brief set the load balancing value (number of jobs to send)
	*/
	void setLoadBalancing(unsigned int inLoadBalancing)
	{
		mLoadBalancing = inLoadBalancing;
	}

	/*! \brief set the distribution to valid (still crunching).
	*/
	void setValid()
	{
		mValid = true;
	}

	/*! \brief set the distribution to invalid (still crunching).
	*/
	void setInvalid()
	{
		mValid = false;
	}

	/*! \brief set the time to evaluate 1 job.
	*/
	void setTimeLastJob(double inTime)
	{
		mTimeLastJob = inTime;
	}

	/*! \brief Get the client IP address
	*/
	std::string getIPAddress()
	{
		return mIPAddress;
	}

	/*! \brief Get the last connection time
	*/
	long getLastConnect()
	{
		return mLastConnect;
	}

	/*! \brief Get the last connection time in a string format
	*/
	std::string getTime()
	{
		return mTime;
	}

	/*! \brief Get the vector of stats
	*/
	std::vector<double> getStats()
	{
		return mStats;
	}

	/*! \brief Get the client's ID
	*/
	int getClientID()
	{
		return mClientID;
	}

	/*! \brief get the load balancing value (number of jobs to send)
	*/
	unsigned int getLoadBalancing()
	{
		return mLoadBalancing;
	}

	/*! \brief return if the Distribution is valid (client still crunching)
	*/
	bool isValid()
	{
		return mValid;
	}

	/*! \brief return the time to evaluate 1 job.
	*/
	double getTimeLastJob()
	{
		return mTimeLastJob;
	}

	/*! \brief Copy constructor
	*/
	Distribution(const Distribution& inDistribution)
	{
		mIPAddress     = inDistribution.mIPAddress;
		mLastConnect   = inDistribution.mLastConnect;
		mTime          = inDistribution.mTime;
		mStats         = inDistribution.mStats;
		mClientID      = inDistribution.mClientID;
		mLoadBalancing = inDistribution.mLoadBalancing;
		mValid         = inDistribution.mValid;
		mTimeLastJob = inDistribution.mTimeLastJob;
	}

	/*! \brief operator =
	*/
	const Distribution &operator =(const Distribution& inDistribution)
	{
		if(&inDistribution != this) {
			mIPAddress     = inDistribution.mIPAddress;
			mLastConnect   = inDistribution.mLastConnect;
			mTime          = inDistribution.mTime;
			mStats         = inDistribution.mStats;
			mClientID      = inDistribution.mClientID;
			mLoadBalancing = inDistribution.mLoadBalancing;
			mValid         = inDistribution.mValid;
			mTimeLastJob = inDistribution.mTimeLastJob;
		}
		return *this;
	}

private:

	/*! \brief Client IP address
	*/
	std::string mIPAddress;
	/*! \brief Last connection with the client (sec since 1970)
	*/
	long mLastConnect;
	/*! \brief Last connection with the client (sec since 1970)
	*/
	std::string mTime;
	/*! \brief Vector of client's stats
	*/
	std::vector<double> mStats;
	/*! \brief Client ID
	*/
	int mClientID;

	/*! \brief value of the number of jobs to send to the client
	*/
	unsigned int mLoadBalancing;

	/*! \brief true if the Distribution data is valid, false otherwise
	*/
	bool mValid;

	/*! \brief Time to evaluate 1 job (no communication/latency time included)
	*  This value is returned directly by the client.
	*/
	double mTimeLastJob;
};
}
#endif
