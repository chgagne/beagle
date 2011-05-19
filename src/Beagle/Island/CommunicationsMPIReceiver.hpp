/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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

/*!
 *  \file   beagle/CommunicationsMPIReceiver.hpp
 *  \brief  Definition of the class CommunicationsMPIReceiver.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:32 $
 */

#ifndef Beagle_Distrib_Island_CommunicationsMPIReceiver_hpp
#define Beagle_Distrib_Island_CommunicationsMPIReceiver_hpp

#include <string>
#include <vector>
#include "PACC/Threading.hpp"
#include "PACC/Socket.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class CommunicationsMPIReceiver beagle/CommunicationsMPIReceiver.hpp "beagle/CommunicationsMPIReceiver.hpp"
 *  \brief Thread that receives TCP/IP messages for CommunicationsMPI system component.
 *  \ingroup DBIM
 */
class CommunicationsMPIReceiver : public PACC::Threading::Thread
{

public:

	//! Typedef of a message (recipient, message-body pair)
	typedef std::pair< std::string, std::string > MessageType;
	//! Typedef of message queue
	typedef std::vector< MessageType > QueueType;

	CommunicationsMPIReceiver();
	virtual ~CommunicationsMPIReceiver();

	QueueType getQueue();

protected:

	virtual void main();

	//! Maximum number of senders that may wait to be recieved.
	static const unsigned int QueueMax;
	//! Queue of messages.
	QueueType mQueue;
	//! Mutex for access to the queue of messages.
	PACC::Threading::Mutex mQueueMutex;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_CommunicationsMPIReceiver_hpp
