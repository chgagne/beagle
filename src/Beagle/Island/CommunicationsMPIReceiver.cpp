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
 *  \file   beagle/Distrib/Island/src/CommunicationsMPIReceiver.cpp
 *  \brief  Source code of class CommunicationsMPIReceiver.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.3 $
 *  $Date: 2007/08/08 19:26:32 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


namespace
{

std::string getMPIError(int inErrorNum)
{
	static char lString[MPI_MAX_ERROR_STRING];
	static int lLength;
	static int lRet;
	lRet = MPI_Error_string( inErrorNum, lString, &lLength );
	if (lRet != MPI_SUCCESS)
		return "Error while obtaining error description (probably an unknown error number)";
	return std::string(lString);
}

}


/*!
 *  \brief Constructor for CommunicationsMPIReceiver
 */
Island::CommunicationsMPIReceiver::CommunicationsMPIReceiver()
{}


/*!
 *  \brief CommunicationsMPIReceiver destructor.
 */
Island::CommunicationsMPIReceiver::~CommunicationsMPIReceiver()
{
	// Ask the thread to terminate
	std::cerr << "Cancelling thread from ~CommunicationsMPIReceiver()" << std::endl;
	cancel();

	// Send message, just so that the call in main() to MPI_Probe() returns
	std::cerr << "Obtaining my rank" << std::endl;
	int lMyRank;
	int lRet = MPI_Comm_rank(MPI_COMM_WORLD, &lMyRank);
	std::cerr << "Rank obtained.  It is " << lMyRank << std::endl;
	if (lRet!=MPI_SUCCESS) {
		std::cerr << "Throwing exception.  Error is " << getMPIError(lRet) << std::endl;
		throw Beagle_RunTimeExceptionM
		("Failed to get MPI rank of current process while destroying CommunicationsMPIReceiver");
	}
	std::cerr << "Sending message" << std::endl;
	lRet = MPI_Send(&lMyRank,
	                1,
	                MPI_INT,
	                lMyRank,
	                CommunicationsMPI::eMessage,
	                MPI_COMM_WORLD);
	std::cerr << "Message sent" << std::endl;
	if (lRet!=MPI_SUCCESS) {
		std::cerr << "Throwing exception.  Error is " << getMPIError(lRet) << std::endl;
		throw Beagle_RunTimeExceptionM
		("Failed to send message while destroying CommunicationsMPIReceiver");
	}
	std::cerr << "Message sent successfully" << std::endl;

	// Wait for the thread to terminate
	std::cerr << "Waiting for thread to terminate in ~CommunicationsMPIReceiver()" << std::endl;
	wait();
	if (isRunning()) {
		std::cerr << "Thread is still running" << std::endl;
	} else {
		std::cerr << "Thread is not running" << std::endl;
	}
}


/*!
 *  \brief Obtains the queue of messages.
 *
 *  Note: This method obtains the queue mutex and clears the internal
 *  queue.
 */
Island::CommunicationsMPIReceiver::QueueType CommunicationsMPIReceiver::getQueue()
{
	mQueueMutex.lock(); // Ensure that data isn't written during read
	QueueType lQueue = mQueue;
	mQueue.clear();// Clear queue so that it's not read again
	mQueueMutex.unlock();
	return lQueue;
}

/*!
 *  \brief The main method of the thread
 *
 *  This method accepts connections; receives messages and stores them in the queue.
 */
void Island::CommunicationsMPIReceiver::main()
{
	try {
		while (true) {
			// Probe to see if a message can be received.
			int lRet;
			MPI_Status lStatus;
			lRet = MPI_Probe(MPI_ANY_SOURCE,
			                 CommunicationsMPI::eMessage,
			                 MPI_COMM_WORLD,
			                 &lStatus);
			if(lRet!=MPI_SUCCESS) {
				throw Beagle_RunTimeExceptionM("Failed to probe for message in CommunicationsMPIReceiver::main()");
			}

			// Check to see if thread termination has been requested
			if (mCancel) break;

			// Ensure that an "eLengths" message is available
			int lFlag;
			lRet = MPI_Iprobe(MPI_ANY_SOURCE,
			                  CommunicationsMPI::eLengths,
			                  MPI_COMM_WORLD,
			                  &lFlag,
			                  &lStatus);
			if (lRet!=MPI_SUCCESS) {
				throw Beagle_RunTimeExceptionM("Failed to probe for 'eLengths' message in CommunicationsMPIReceiver::main()");
			}
			if (lFlag!=true) {
				throw Beagle_RunTimeExceptionM("Probe failed to find 'eLengths' message in CommunicationsMPIReceiver::main()");
			}

			// Get lengths
			int lLengths[2];
			lRet = MPI_Recv(lLengths,
			                2,
			                MPI_INT,
			                MPI_ANY_SOURCE,
			                CommunicationsMPI::eLengths,
			                MPI_COMM_WORLD,
			                &lStatus );
			if(lRet!=MPI_SUCCESS) {
				throw Beagle_RunTimeExceptionM("Failed to receive 'eLengths' message in CommunicationsMPIReceiver::main()");
			}


			// Allocate space to receive strings
			char* lRecipient = new char[lLengths[0]+1];
			char* lMessageBody = new char[lLengths[1]+1];

			// Ensure that an "eRecipient" message is available
			lRet = MPI_Iprobe(MPI_ANY_SOURCE,
			                  CommunicationsMPI::eRecipient,
			                  MPI_COMM_WORLD,
			                  &lFlag,
			                  &lStatus);
			if (lRet!=MPI_SUCCESS) {
				throw Beagle_RunTimeExceptionM("Failed to probe for 'eRecipient' message in CommunicationsMPIReceiver::main()");
			}
			if (lFlag!=true) {
				throw Beagle_RunTimeExceptionM("Probe failed to find 'eRecipient' message in CommunicationsMPIReceiver::main()");
			}

			// Receive recipient
			lRet = MPI_Recv(lRecipient,
			                lLengths[0],
			                MPI_CHAR,
			                MPI_ANY_SOURCE,
			                CommunicationsMPI::eRecipient,
			                MPI_COMM_WORLD,
			                &lStatus);
			if (lRet!=MPI_SUCCESS) {
				throw Beagle_RunTimeExceptionM("Failed to receive 'eRecipient' in CommunicationsMPIReceiver::main()");
			}
			lRecipient[lLengths[0]] = '\0';


			// Receive message (probe has already returned successfully)
			lRet = MPI_Recv(lMessageBody,
			                lLengths[1],
			                MPI_CHAR,
			                MPI_ANY_SOURCE,
			                CommunicationsMPI::eMessage,
			                MPI_COMM_WORLD,
			                &lStatus);
			if (lRet!=MPI_SUCCESS) {
				throw Beagle_RunTimeExceptionM("Failed to receive 'eRecipient' in CommunicationsMPIReceiver::main()");
			}
			lMessageBody[lLengths[1]] = '\0';


			// Store the message
			MessageType lMessage(lRecipient, lMessageBody);
			mQueueMutex.lock();
			mQueue.push_back( lMessage );
			mQueueMutex.unlock();
		}
	} catch (Beagle::Exception& inException) {
		std::cerr << "A Beagle exception was thrown:" << std::endl;
		inException.explain();
		throw;
	} catch (...) {
		std::cerr << "\n\nUnknown exception caught in CommunicationsMPIReceiver!" << std::endl;
		throw;
	}

	/*
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
	    }
	    catch (PACC::Socket::Exception& inException) {
	if (inException.getErrorCode() == PACC::Socket::eBadMessage) {
	 // The signature (Cafe) was invalid.  Oh well, just ignore the message.
	}
	else if (inException.getErrorCode() == PACC::Socket::eConnectionClosed) {
	 // The connection was closed at the sending end.  Oh well, just ignore the message.
	}
	else if (inException.getErrorCode() == PACC::Socket::eTimeOut) {
	 // Send or receive timed out.  Oh well, just ignore the message.
	}
	else throw; // Rethrow exception
	    }
	  }
	  close(); // Close the receiving port
	}
	catch (...) {
	  std::cerr << "\n\nUnknown exception caught in CommunicationsMPIReceiver!" << std::endl;
	  throw;
	}
	*/
}
