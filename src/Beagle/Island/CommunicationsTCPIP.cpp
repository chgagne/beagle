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
 *  \file   beagle/Distrib/Island/src/CommunicationsTCPIP.cpp
 *  \brief  Source code of class CommunicationsTCPIP.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct CommunicationsTCPIP component.
 *  \param inPortNumber Port number on which to listen.  If set to -1,
 *  the receiving thread will not be started but messages can still be
 *  sent.
 *  \param inName Name of the Communications component.
 */
Island::CommunicationsTCPIP::CommunicationsTCPIP(unsigned int inPortNumber,
        const std::string inName) :
		Communications(inName),
		mPortNumber(inPortNumber),
		mReceiver(NULL)
{}

/*!
 *  \brief CommunicationsTCPIP destructor.
 *
 *  Stops the receiving thread.
 */
Island::CommunicationsTCPIP::~CommunicationsTCPIP()
{
	delete mReceiver;
}


/*!
 *  \brief Add address to list of known addresses.
 *  \param inName Name of address.
 *  \param inAddress TCP/IP address.
 *
 *  Note that addresses are read from the parameters file during
 *  System initialization (see readWithSystem()).
 */
void Island::CommunicationsTCPIP::addAddress(const std::string& inName,
        const PACC::Socket::Address& inAddress )
{
	Beagle_StackTraceBeginM();
	mAddressToTCPIPMap.insert
	(AddressToTCPIPMapType::value_type(inName, inAddress));
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Get message queue from receiving thread.
 *  \return Message queue.
 */
void Island::CommunicationsTCPIP::getMessageQueue(Island::Communications::MessageQueueType& ioQueue)
{
	Beagle_StackTraceBeginM();
	if (mReceiver==NULL) {
		throw Beagle_RunTimeExceptionM
		(std::string("In CommunicationsTCPIP: An attempt was made to obtain received messages")+
		 " but the receiver thread had not been started.  Is this because a port number"+
		 " was not specified?");
	}
	Beagle_AssertM( mReceiver->isRunning() );
	mReceiver->getQueue(ioQueue);
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Get TCP/IP address given a name.
 *  \param inName Name to search for.
 *  \param outAddress TCP/IP address of given name (valid only if method returns true).
 *  \return True if the name was found, false otherwise.
 */
bool Island::CommunicationsTCPIP::getTCPIPAddress(const std::string& inName,
        PACC::Socket::Address& outAddress)
{
	Beagle_StackTraceBeginM();
	AddressToTCPIPMapType::iterator lItr;
	lItr = mAddressToTCPIPMap.find(inName);
	if (lItr == mAddressToTCPIPMap.end()) return false;
	outAddress = lItr->second;
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Returns true if address is known.
 *  \param inAddress Address to check for.
 */
bool Island::CommunicationsTCPIP::haveAddress(const std::string& inAddress)
{
	return (mAddressToTCPIPMap.find(inAddress) != mAddressToTCPIPMap.end());
}



/*!
 *  \brief Initialize operator.
 *  \param ioSystem Evolutionary system.
 *
 *  Starts the receiving thread.  Note that changing the port number
 *  after the thread has started will not change the port number on
 *  which the thread is listening (i.e. once started, the port number
 *  is lock in and cannot be chanaged.)
 */
void Island::CommunicationsTCPIP::init(System &ioSystem)
{
	Beagle_StackTraceBeginM();
	// List known addresses
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsTCPIP",
	    std::string("CommunicationsTCPIP knows ")+uint2str(mAddressToTCPIPMap.size())+
	    " address(es)"
	);
	unsigned int lIndex=0;
	for (AddressToTCPIPMapType::iterator lItr = mAddressToTCPIPMap.begin();
	        lItr != mAddressToTCPIPMap.end(); ++lItr) {
		Beagle_LogDebugM(
		    ioSystem.getLogger(),
		    "communications", "Beagle::CommunicationsTCPIP",
		    uint2ordinal(lIndex+1)+" address named '"+lItr->first+"' is "+
		    lItr->second.getIPAddress()+":"+uint2str(lItr->second.getPortNumber())
		);
		lIndex++;
	}

	// Create and start the receiver
	try {
		if (mPortNumber!=(unsigned int)(-1)) {
			mReceiver = new CommunicationsTCPIPReceiver(mPortNumber);
		} else {
			Beagle_LogBasicM(
			    ioSystem.getLogger(),
			    "communications", "Beagle::CommunicationsTCPIP",
			    "WARNING: TCP/IP receiver thread has not been started because PortNumber is -1"
			);
			mReceiver = NULL;
			return;
		}
	} catch (PACC::Socket::Exception& inException) {
		throw Beagle_RunTimeExceptionM
		(std::string("An exception was thrown while creating the TCP/IP receiver thread.")+
		 "  The exception message was: "+inException.getMessage());
	}
	Beagle_NonNullPointerAssertM( mReceiver );
	mReceiver->run();
	Beagle_AssertM( mReceiver->isRunning() );
	Beagle_LogTraceM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsTCPIP",
	    std::string("TCP/IP receiver thread created and listening on port ")+
	    uint2str(mPortNumber)
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Reads TCP/IP address from XML source.
 *  \param inIter Iterator to XML node from which to read address.
 *  \param ioSystem Evolutionary system.
 *
 *  Address tag should have a 'name' attribute and 'Port' tag.  It may
 *  also have a 'Host' tag, but if it is not included then the host
 *  defaults to '127.0.0.1' (localhost).
 *
 *  Here is an example:
 *  \code
 *  <Address name="target">
 *    <Host>192.168.1.3</Host>
 *    <Port>4501</Port>
 *  </Address>
 *  \endcode
 */
void Island::CommunicationsTCPIP::readAddress(PACC::XML::ConstIterator inIter,
        Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::CommunicationsTCPIP",
	    "Reading address for CommunicationsTCPIP component"
	);

	// Get name (an attribute of the 'Address' tag)
	std::string lName;
	if (inIter->isDefined("name")) {
		lName = inIter->getAttribute("name");
		Beagle_LogDebugM(
		    ioSystem.getLogger(),
		    "communications", "Beagle::CommunicationsTCPIP",
		    std::string("Reading address named '")+lName+"'"
		);
	} else {
		throw Beagle_RunTimeExceptionM
		(std::string("While reading an address for component '")+getName()+
		 "', the address that was being read was found not to have an associated name.");
	}

	// Read host name and port number
	std::string lHost="127.0.0.1"; // default to localhost
	UInt::Handle lPort;

	for (PACC::XML::ConstIterator lNode=inIter->getFirstChild(); lNode; ++lNode) {
		if (lNode->getType()==PACC::XML::eData) {
			if (lNode->getValue()=="Host") {
				// Get host name
				PACC::XML::ConstIterator lHostNode = lNode->getFirstChild();
				Beagle_AssertM( lHostNode->getType() == PACC::XML::eString );
				lHost = lHostNode->getValue();
				Beagle_LogDebugM(
				    ioSystem.getLogger(),
				    "communications", "Beagle::CommunicationsTCPIP",
				    std::string("Address named '")+lName+"' has host name '"+lHost+"'"
				);
			} else if (lNode->getValue()=="Port") {
				// Get port number
				lPort = new UInt;
				PACC::XML::ConstIterator lPortNode = lNode->getFirstChild();
				Beagle_AssertM( lPortNode->getType() == PACC::XML::eString );
				std::istringstream lISS( lPortNode->getValue() );
				lISS >> lPort->getWrappedValue();
				Beagle_LogDebugM(
				    ioSystem.getLogger(),
				    "communications", "Beagle::CommunicationsTCPIP",
				    std::string("Address named '")+lName+"' has port number "+
				    uint2str(lPort->getWrappedValue())
				);
			}
		}
	}

	// Check a port number was read
	if (lPort==NULL) {
		throw Beagle_RunTimeExceptionM
		(std::string("Failed to read a port number for address named '")+lName+"'");
	}

	// Add address
	PACC::Socket::Address lAddress( lPort->getWrappedValue(), lHost );
	addAddress( lName, lAddress );

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the CommunicationsTCPIP component from and XML source.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioSystem Evolutionary system.
 *
 *  The port on which to listen may be specified through a 'Port' tag.
 *  This method calls the readAddress method for all 'Address' tags.
 *
 *  Here is an example:
 *  \code
 *  <CommunicationsTCPIP>
 *    <Port>4500</Port> <!-- The port on which this program should listen -->
 *    <Address name="target">
 *      <Host>192.168.1.3</Host>
 *      <Port>4501</Port> <!-- The port number of the receiving program -->
 *    </Address>
 *  </CommunicationsTCPIP>
 *  \endcode
 */
void Island::CommunicationsTCPIP::readWithSystem(PACC::XML::ConstIterator inIter,
        Beagle::System& ioSystem)
{
	// Read addresses
	Communications::readWithSystem(inIter, ioSystem);

	// Look to read port number on which to listen
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType()==PACC::XML::eData &&
		        lChild->getValue()=="Port") {
			// Read port number
			PACC::XML::ConstIterator lPortNode=lChild->getFirstChild();
			Beagle_AssertM( lPortNode->getType() == PACC::XML::eString );
			std::istringstream lISS( lPortNode->getValue() );
			lISS >> mPortNumber;
			Beagle_LogDebugM(
			    ioSystem.getLogger(),
			    "communications", "Beagle::Communications",
			    std::string("CommunicationsTCPIP: Port number on which to listen was read as ")+
			    uint2str(mPortNumber)
			);
		}
	}
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
void Island::CommunicationsTCPIP::sendMessage(const std::string& inAddress,
        const std::string& inRecipient,
        const std::string& inMessage,
        Beagle::Context& ioContext )
{
	Beagle_StackTraceBeginM();
	// Get TCP/IP address
	PACC::Socket::Address lAddress;
	bool lValid = getTCPIPAddress(inAddress, lAddress);
	if (!lValid) {
		throw Beagle_RunTimeExceptionM
		(std::string("In CommunicationsTCPIP::sendMessage(): Address identifer '")+
		 inAddress+std::string("' unknown"));
	}

	try {
		// Connect to address
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::CommunicationsTCPIP",
		    std::string("Connecting to ")+lAddress.getIPAddress()+":"+uint2str(lAddress.getPortNumber())
		);
		PACC::Socket::Cafe lConnection;
		lConnection.connect(lAddress);

		// Set timeout to 30 seconds
		lConnection.setSockOpt( PACC::Socket::eSendTimeOut, 30.0 );

		// Send message to receiver
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::CommunicationsTCPIP",
		    std::string("Sending message")
		);
		lConnection.sendMessage(inRecipient);
		lConnection.sendMessage(inMessage);
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::CommunicationsTCPIP",
		    std::string("Message sent successfully")
		);
	} catch (PACC::Socket::Exception& inException) {
		if (inException.getErrorCode() == PACC::Socket::eConnectionRefused) {
			Beagle_LogBasicM(
			    ioContext.getSystem().getLogger(),
			    "communications", "Beagle::CommunicationsTCPIP",
			    std::string("WARNING: failed to send message to '")+inAddress+
			    "' (recipient '"+inRecipient+"'); couldn't connect to "+lAddress.getIPAddress()+":"+
			    uint2str(lAddress.getPortNumber())
			);
		} else if (inException.getErrorCode() == PACC::Socket::eConnectionClosed) {
			Beagle_LogBasicM(
			    ioContext.getSystem().getLogger(),
			    "communications", "Beagle::CommunicationsTCPIP",
			    std::string("WARNING: failed to send message to '")+inAddress+
			    "' (recipient '"+inRecipient+"'); connection was closed by the receiving end"
			);
		} else {
			throw;
		}
	}
	Beagle_StackTraceEndM();
}


void Island::CommunicationsTCPIP::writeContent(PACC::XML::Streamer& ioStreamer,
        bool inIndent) const
{
	// Write port number
	ioStreamer.insertComment("If port number is set to -1; listening is deactivated");
	ioStreamer.openTag("Port", false);
	ioStreamer.insertStringContent(int2str(mPortNumber));
	ioStreamer.closeTag(); // Port

	// Write addresses
	if (mAddressToTCPIPMap.empty()) {
		ioStreamer.insertComment("No addresses have been configured");
	} else {
		for (AddressToTCPIPMapType::const_iterator lItr=mAddressToTCPIPMap.begin();
		        lItr!=mAddressToTCPIPMap.end(); ++lItr) {
			ioStreamer.openTag("Address", inIndent);
			ioStreamer.insertAttribute("name", lItr->first);
			ioStreamer.openTag("Host", false);
			ioStreamer.insertStringContent(lItr->second.getIPAddress());
			ioStreamer.closeTag(); // Host
			ioStreamer.openTag("Port", false);
			ioStreamer.insertStringContent(uint2str(lItr->second.getPortNumber()));
			ioStreamer.closeTag(); // Port
			ioStreamer.closeTag(); // Address
		}
	}

}




