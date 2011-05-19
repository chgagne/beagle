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
 *  \file   beagle/Distrib/Island/src/CommunicationsMPI.cpp
 *  \brief  Source code of class CommunicationsMPI.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:10 $
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
 *  \brief Construct CommunicationsMPI component.
 *  \param inName Name of the Communications component.
 */
Island::CommunicationsMPI::CommunicationsMPI(const std::string inName) :
		Communications(inName),
		mReceiver(new CommunicationsMPIReceiver)
{
	// Create a receiver
}

/*!
 *  \brief CommunicationsMPI destructor.
 *
 *  Calls MPI_Finalize().
 */
Island::CommunicationsMPI::~CommunicationsMPI()
{
	delete mReceiver;
	MPI_Finalize();
}


/*!
 *  \brief Add address to list of known addresses.
 *  \param inName Name of address.
 *  \param inAddress TCP/IP address.
 *
 *  Note that addresses are read from the parameters file during
 *  System initialization (see readWithSystem()).
 */
void Island::CommunicationsMPI::addAddress(const std::string& inName,
        const int inAddress)
{
	Beagle_StackTraceBeginM();
	// Ensure that inAddress is not invalid
	int lSize;
	MPI_Comm_size(MPI_COMM_WORLD, &lSize);
	if (inAddress >= lSize) {
		Beagle_RunTimeExceptionM
		( string("In CommunicationsMPI::addAddress(), the value of inAddress was invalid ")+
		  "as there were only "+int2str(lSize)+" processors" );
	}
	mAddressToMPIMap.insert
	(mAddressToMPIMapType::value_type(inName, inAddress));
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Get message queue from receiving thread.
 *  \return Message queue.
 */
Island::Communications::MessageQueueType CommunicationsMPI::getMessageQueue()
{
	Beagle_StackTraceBeginM();
	if (mReceiver==NULL) {
		throw Beagle_RunTimeExceptionM
		(string("In CommunicationsMPI: An attempt was made to obtain received messages")+
		 " but the receiver thread had not been started.");
	}
	Beagle_AssertM( mReceiver->isRunning() );
	return mReceiver->getQueue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get MPI rank given a name.
 *  \param inName Name to search for.
 *  \param outAddress MPI rank for given name (valid only if method returns true).
 *  \return True if the name was found, false otherwise.
 */
bool Island::CommunicationsMPI::getMPIAddress(const std::string& inName,
        int& outAddress)
{
	Beagle_StackTraceBeginM();
	mAddressToMPIMapType::iterator lItr;
	lItr = mAddressToMPIMap.find(inName);
	if (lItr == mAddressToMPIMap.end()) return false;
	outAddress = lItr->second;
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize operator.
 *  \param ioSystem Evolutionary system.
 *
 *  Calls MPI_Init();
 */
void Island::CommunicationsMPI::initialize(System &ioSystem, int& ioArgc, char**& ioArgv)
{
	// Initialize MPI
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsMPI",
	    "Initializing MPI"
	);
	std::cout << "Calling MPI_Init()..." << std::endl;
	int lRet = MPI_Init(&ioArgc, &ioArgv);
	if (lRet!=MPI_SUCCESS) {
		throw Beagle_RunTimeExceptionM(string("Failed to initialize MPI.  Error was ")+
		                               getMPIError(lRet));
	}

	// Use default MPI error handler to allow the return of error codes.
	MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
	/*
	MPI_File_set_errhandler(MPI_ERRORS_RETURN);
	MPI_Win_set_errhandler(MPI_ERRORS_RETURN);
	*/
}



/*!
 *  \brief Initialize operator.
 *  \param ioSystem Evolutionary system.
 */
void Island::CommunicationsMPI::init(System &ioSystem)
{
	Beagle_StackTraceBeginM();

	// Check MPI has been initialized.
	int lFlag;
	int lRet;
	lRet = MPI_Initialized( &lFlag );
	if (lRet != MPI_SUCCESS) {
		throw Beagle_RunTimeExceptionM
		(string("In CommunicationsMPI::init(): Failed to successfully check if MPI has been ")+
		 " initialized.  Error was "+getMPIError(lRet));
	}
	if (lFlag!=true) {
		throw Beagle_RunTimeExceptionM("In CommunicationsMPI::init(): MPI has not been initialized");
	}
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsMPI",
	    "MPI has been initialized"
	);

	// List known addresses
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsMPI",
	    string("CommunicationsMPI knows ")+uint2str(mAddressToMPIMap.size())+
	    " address(es)"
	);
	unsigned int lIndex=0;
	for (mAddressToMPIMapType::iterator lItr = mAddressToMPIMap.begin();
	        lItr != mAddressToMPIMap.end(); ++lItr) {
		Beagle_LogDebugM(
		    ioSystem.getLogger(),
		    "communications", "Beagle::CommunicationsMPI",
		    uint2ordinal(lIndex+1)+" address named '"+lItr->first+"' is rank "+
		    int2str(lItr->second)
		);
		lIndex++;
	}

	// Start the receiver
	Beagle_NonNullPointerAssertM( mReceiver );
	mReceiver->run();
	Beagle_AssertM( mReceiver->isRunning() );
	Beagle_LogTraceM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsMPI",
	    string("MPI receiver thread is now running")
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Reads MPI rank from XML source.
 *  \param inIter Iterator to XML node from which to read address.
 *  \param ioSystem Evolutionary system.
 *
 *  Address tag should have a 'name' attribute and 'Rank' tag.
 *
 *  Here is an example:
 *  \code
 *  <Address name="target">
 *    <Rank>1</Rank>
 *  </Address>
 *  \endcode
 */
void Island::CommunicationsMPI::readAddress(PACC::XML::ConstIterator inIter,
        Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsMPI",
	    "Reading address for CommunicationsMPI component"
	);

	// Get name (an attribute of the 'Address' tag)
	std::string lName;
	if (inIter->isDefined("name")) {
		lName = inIter->getAttribute("name");
		Beagle_LogDebugM(
		    ioSystem.getLogger(),
		    "communications", "Beagle::CommunicationsMPI",
		    string("Reading address named '")+lName+"'"
		);
	} else {
		throw Beagle_RunTimeExceptionM
		(string("While reading an address for component '")+getName()+
		 "', the address that was being read was found not to have an associated name.");
	}

	// Read rank
	Int::Handle lRank;

	for (PACC::XML::ConstIterator lNode=inIter->getFirstChild(); lNode; ++lNode) {
		if (lNode->getType()==PACC::XML::eData) {
			if (lNode->getValue()=="Rank") {
				// Get rank number
				lRank = new Int;
				PACC::XML::ConstIterator lRankNode = lNode->getFirstChild();
				Beagle_AssertM( lRankNode->getType() == PACC::XML::eString );
				std::istringstream lISS( lRankNode->getValue() );
				lISS >> lRank->getWrappedValue();
				Beagle_LogDebugM(
				    ioSystem.getLogger(),
				    "communications", "Beagle::CommunicationsMPI",
				    string("Address named '")+lName+"' has rank number "+
				    uint2str(lRank->getWrappedValue())
				);
			}
		}
	}

	// Check a rank number was read
	if (lRank==NULL) {
		throw Beagle_RunTimeExceptionM
		(string("Failed to read a rank number for address named '")+lName+"'");
	}

	// Add address
	addAddress( lName, lRank->getWrappedValue() );

	Beagle_StackTraceEndM();
}



/*!
 *  \brief Send message to inAddress
 *  \param inAddress The name of the receiver.
 *  \param inRecipient The name of the recipient within the receiver.
 *  \param inMessage The body of the message.
 *  \param ioContext Evolutionary context of the operation.
 *
 *  The message is sent immediately.
 */
void Island::CommunicationsMPI::sendMessage(const std::string& inAddress,
        const std::string& inRecipient,
        const std::string& inMessage,
        Beagle::Context& ioContext )
{
	Beagle_StackTraceBeginM();
	// Get MPI rank to send to
	int lRank;
	bool lValid = getMPIAddress(inAddress, lRank);
	if (!lValid)
		throw Beagle_RunTimeExceptionM(std::string("Address identifer (")+
		                               inAddress+std::string(") unknown"));

	// Send MPI message to lRank

	// Ideally, it would be best to use MPI_Bsend() (a basic buffered
	// send) which would mean the send would not need to wait for an
	// associated receive.  However, because there seems to be no way to
	// check if the buffer is full, the MPI_Recv() call has been placed
	// in a separate thread, and that means a standard MPI_Send() is all
	// that is required.

	// My assumption that it is not possible to check if a buffer is
	// full comes from the sentence, "It does not seem feasible to
	// provide a portable mechanism for querying or controlling
	// buffering" (section "Buffered allocation and usage" of MPI: A
	// Message-Passing Interface Standard (version 1.1) available at
	// mpi-forum.org).

	// Send two ints, the first is the length of the recipient, the
	// second is the lenght of the message body.
	try {
		int lRet;
		int lLengths[2];
		lLengths[0] = inRecipient.size();
		lLengths[1] = inMessage.size();
		lRet = MPI_Send(lLengths,
		                2,
		                MPI_INT,
		                lRank,
		                eLengths,
		                MPI_COMM_WORLD );
		if (lRet!=MPI_SUCCESS) {
			throw Beagle_RunTimeExceptionM
			(string("CommunicationsMPI::sendMessage(): Failed to send the lengths of recipient ")+
			 "and message body.  Error was "+getMPIError(lRet));
		}

		// Send recipient
		lRet = MPI_Send(const_cast<char*>(inRecipient.data()),
		                lLengths[0],
		                MPI_CHAR,
		                lRank,
		                eRecipient,
		                MPI_COMM_WORLD);
		if (lRet!=MPI_SUCCESS) {
			throw Beagle_RunTimeExceptionM
			(string("CommunicationsMPI::sendMessage(): Failed to send recipient.  Error was")+
			 getMPIError(lRet));
		}

		// Send message body
		lRet = MPI_Send(const_cast<char*>(inMessage.data()),
		                lLengths[1],
		                MPI_CHAR,
		                lRank,
		                eMessage,
		                MPI_COMM_WORLD );
		if (lRet==MPI_SUCCESS) {
			// Sucessfully sent message
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "communications", "Beagle::CommunicationsMPI",
			    std::string("Message sent")
			);
		} else {
			throw Beagle_RunTimeExceptionM
			(string("CommunicationsMPI::sendMessage(): Failed to send the message body.  ")+
			 "Error was"+getMPIError(lRet));
		}
	} catch (...) {
		// Failed to send message
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::CommunicationsMPI",
		    std::string("WARNING: failed to send message via MPI to '")+inAddress+"'"
		);
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::CommunicationsMPI",
		    std::string("Recipient was '")+inRecipient+
		    "'.  Message was '"+inMessage+"'"
		);
		throw;
	}

	/* Send a packed message
	// Pack message (length of recipient, recipient, length of message body, message body)

	// Calculate memory requirements
	int lRet;
	int lSize;
	int lTotalSize = 0;
	int lRecipientCount = inRecipient.size();
	int lMessageCount = inMessage.size();
	lRet = MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &lSize);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to calculate packing size of recipient length");
	}
	lTotalSize+=lSize;
	lRet = MPI_Pack_size(lRecipientCount, MPI_CHAR, MPI_COMM_WORLD, &lSize);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to calculate packing size of recipient");
	}
	lTotalSize+=lSize;
	lRet = MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &lSize);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to calculate packing size of message body length");
	}
	lTotalSize+=lSize;
	lRet = MPI_Pack_size(lMessageCount, MPI_CHAR, MPI_COMM_WORLD, &lSize);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to calculate packing size of message body");
	}
	lTotalSize+=lSize;

	char* lBuffer;
	try {
	// Allocate sufficient memory
	lBuffer = new char[lTotalSize];

	// Pack message
	int lPosition;
	lRet = MPI_Pack(&lRecipientCount, 1, MPI_INT, lBuffer, lTotalSize, &lPosition, MPI_COMM_WORLD);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to pack recipient count");
	}
	lRet = MPI_Pack(const_cast<char*>(inRecipient.data()), lRecipientCount, MPI_CHAR,
	lBuffer, lTotalSize, &lPosition, MPI_COMM_WORLD);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to pack recipient");
	}
	lRet = MPI_Pack(&lMessageCount, 1, MPI_INT, lBuffer, lTotalSize, &lPosition, MPI_COMM_WORLD);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to pack message body count");
	}
	lRet = MPI_Pack(const_cast<char*>(inMessage.data()), lMessageCount, MPI_CHAR,
	lBuffer, lTotalSize, &lPosition, MPI_COMM_WORLD);
	if (lRet!=MPI_SUCCESS) {
	throw Beagle_RunTimeExceptionM
	("CommunicationsMPI::sendMessage(): Failed to pack message body");
	}

	// Send message
	lRet = MPI_Send( lBuffer, lPosition, MPI_PACKED, lRank, 0, MPI_COMM_WORLD );
	if (lRet==MPI_SUCCESS) {
	// Sucessfully sent message
	Beagle_LogVerboseM(
	ioContext.getSystem().getLogger(),
	"communications", "Beagle::CommunicationsMPI",
	std::string("Message sent")
	);
	}
	else {
	// Failed to send message
	Beagle_LogBasicM(
	ioContext.getSystem().getLogger(),
	"communications", "Beagle::CommunicationsMPI",
	std::string("Warning: failed to send message via MPI to '")+inAddress+"'"
	);
	Beagle_LogDebugM(
	ioContext.getSystem().getLogger(),
	"communications", "Beagle::CommunicationsMPI",
	std::string("Recipient was '")+inRecipient+
	"'.  Message was '"+inMessage+"'"
	);
	}
	}
	catch (...) {
	// Clean up memory
	delete[] lBuffer;
	throw;
	}
	delete[] lBuffer;
	*/

	Beagle_StackTraceEndM();
}






