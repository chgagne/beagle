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
 *  \file   beagle/Distrib/Island/src/CommunicationsTCPIPReceiver.cpp
 *  \brief  Source code of class CommunicationsTCPIPReceiver.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:33 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


const unsigned int Island::CommunicationsTCPIPReceiver::QueueMax = 10;

/*!
 *  \brief Constructor for CommunicationsTCPIPReceiver
 *  \param inPortNumber The port number on which to listen.
 *
 *  The constructor sets the send/receive timeout.  It binds to the
 *  given port number and starts listening.  Connections will not be
 *  accepted until the thread is started.
 *
 *  \sa main()
 */
Island::CommunicationsTCPIPReceiver::CommunicationsTCPIPReceiver(unsigned int inPortNumber) :
		mPortNumber(inPortNumber)
{
	// Set to send/receive timeout to 15 seconds.
	setSockOpt( PACC::Socket::eRecvTimeOut, 15.0 );
	setSockOpt( PACC::Socket::eSendTimeOut, 15.0 );

	/* Allow the reuse of addresses and ports.

	   According to the entry in 'man 7 ip', the use of SO_REUSEADDR
	   causes a reduction in reliability of TCP/IP.  However the failure
	   to use this option means that the island-model code cannot be
	   restarted on the same address and port combination for between 1
	   and 4 minutes after the program has terminated.  This is an
	   unacceptably long time given that it is common for many runs to
	   be executed sequentially.

	   The use of SO_REUSEADDR is discussed in "TCP/IP Illustrated Vol
	   1" (pages 241--245).  Matthew Walker believes that the reduced
	   reliability is not really of a practical concern.  Although it
	   may open up a potential security flaw, the benefit of not having
	   to wait 1--4 minutes greatly outweighs this risk.  He believes
	   the use of this flag will not cause any corruption of data.
	*/
	setSockOpt(PACC::Socket::eReuseAddress, true);

	// Bind to port and start listening.
	bind(mPortNumber);
	listen(QueueMax);
}


/*!
 *  \brief CommunicationsTCPIPReceiver destructor.
 */
Island::CommunicationsTCPIPReceiver::~CommunicationsTCPIPReceiver()
{
	cancel();

	// It is necessary to wait for the thread to terminate befor closing
	// the connection.  Otherwise the thread can be receiving a message,
	// the destructor can close the thread during the receive, and when
	// the thread finishes the receive it crashes as it doesn't expect
	// the connection to be closed while it's doing its thing.
	wait();
	close();
}


/*!
 *  \brief Obtains the queue of messages.
 *
 *  Note: This method obtains the queue mutex and clears the internal
 *  queue.
 */
void Island::CommunicationsTCPIPReceiver::getQueue(Island::CommunicationsTCPIPReceiver::QueueType& ioQueue)
{
	mQueueMutex.lock(); // Ensure that data isn't written during read
	ioQueue.insert(ioQueue.end(), mQueue.begin(), mQueue.end());   // Copy queue
	mQueue.clear();// Clear queue so that it's not read again
	mQueueMutex.unlock();
}

/*!
 *  \brief The main method of the thread
 *
 *  This method accepts connections; receives messages and stores them in the queue.
 */
void Island::CommunicationsTCPIPReceiver::main()
{
	try {
		// Wait for activity
		while (true) {
			while(!waitForActivity(10)) {
				if (mCancel) break;
			}
			if (mCancel) break;

			try {
				int lDescriptor = accept();
				PACC::Socket::Cafe lConnection(lDescriptor);

				// Who's the connection from?
				PACC::Socket::Address lAddress = lConnection.getPeerAddress();

				// Get message
				std::string lRecipient;
				std::string lMessageBody;
				lConnection.receiveMessage(lRecipient);
				lConnection.receiveMessage(lMessageBody);
				MessageType lMessage(lRecipient, lMessageBody);

				// Close connection
				lConnection.close();

				// Store message
				mQueueMutex.lock();
				mQueue.push_back( lMessage );
				mQueueMutex.unlock();
			} catch (PACC::Socket::Exception& inException) {
				if (inException.getErrorCode() == PACC::Socket::eBadMessage) {
					// The signature (Cafe) was invalid.  Oh well, just ignore the message.
				} else if (inException.getErrorCode() == PACC::Socket::eConnectionClosed) {
					// The connection was closed at the sending end.  Oh well, just ignore the message.
				} else if (inException.getErrorCode() == PACC::Socket::eTimeOut) {
					// Send or receive timed out.  Oh well, just ignore the message.
				} else throw; // Rethrow exception
			}
		}
		close(); // Close the receiving port
	} catch (...) {
		std::cerr << "\n\nUnknown exception caught in CommunicationsTCPIPReceiver!" << std::endl;
		throw;
	}
}
