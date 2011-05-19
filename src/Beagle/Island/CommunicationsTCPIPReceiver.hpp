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
 *  \file   beagle/CommunicationsTCPIPReceiver.hpp
 *  \brief  Definition of the class CommunicationsTCPIPReceiver.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:32 $
 */

#ifndef Beagle_Distrib_Island_CommunicationsTCPIPReceiver_hpp
#define Beagle_Distrib_Island_CommunicationsTCPIPReceiver_hpp

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
 *  \class CommunicationsTCPIPReceiver beagle/CommunicationsTCPIPReceiver.hpp "beagle/CommunicationsTCPIPReceiver.hpp"
 *  \brief Thread that receives TCP/IP messages for CommunicationsTCPIP system component.
 *  \ingroup DBIM
 */
class CommunicationsTCPIPReceiver : public PACC::Threading::Thread,
			private PACC::Socket::TCP
{

public:

	//! Typedef of a message (recipient, message-body pair)
	typedef std::pair< std::string, std::string > MessageType;
	//! Typedef of message queue
	typedef std::vector< MessageType > QueueType;

	explicit CommunicationsTCPIPReceiver(unsigned int inPortNumber);
	virtual ~CommunicationsTCPIPReceiver();

	void getQueue(QueueType& ioQueue);

	/*!
	 *  \brief Get the port number that this thread is listening on.
	 */
	inline unsigned int getPortNumber()
	{
		return mPortNumber;
	}

protected:

	virtual void main();

	//! Maximum number of senders that may wait to be recieved.
	static const unsigned int QueueMax;
	//! Queue of messages.
	QueueType mQueue;
	//! Mutex for access to the queue of messages.
	PACC::Threading::Mutex mQueueMutex;
	//! Port number on which to listen.
	unsigned int mPortNumber;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_CommunicationsTCPIPReceiver_hpp
