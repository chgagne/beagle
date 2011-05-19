/*
 *  Monitor.hpp
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

#ifndef __MONITOR_DAGS_HH
#define __MONITOR_DAGS_HH
#include <string>

namespace DAGS
{

/*!
*  \class Monitor dags/Monitor.hpp "dags/Monitor.hpp" 
*  \brief DAGS Monitor class
*
*  This class is used to help parse the XML code.  The monitor can ask for
*  a distribution list, a distribution, a group list, a group, the universe or
*  to terminate cleanly.
*/

class Monitor
{

public:

	/*! \brief Default constructor
	*/
	Monitor()
			: mId(-1),
			mRequest("")
	{}

	/*! \brief get group id
	*/
	int getId()
	{
		return mId;
	}

	/*! \brief get application name
	*/
	std::string getRequest()
	{
		return mRequest;
	}

	/*! \brief set group's id
	*/
	void setId(const int inId)
	{
		mId = inId;
	};

	/*! \brief set application's name
	*/
	void setRequest(const std::string& inRequest)
	{
		mRequest = inRequest;
	}

	/*! \brief copy constructor
	*/
	Monitor(const Monitor& inMonitor)
	{
		mId = inMonitor.mId;
		mRequest = inMonitor.mRequest;
	}

private:

	/*! \brief  group's id or client's id
	*/
	int mId;

	/*! \brief  request from the monitor
	*/
	std::string mRequest;
};
}

#endif
