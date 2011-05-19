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
 *  \file   beagle/Distrib/Island/src/SendOp.cpp
 *  \brief  Source code of class SendOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct SendOp.
 *  \param inCommsName Name of the Communications component.
 *  \param inName Name of this operator.
 */
Island::SendOp::SendOp(const std::string& inCommsName,
                       const std::string& inDefaultRecipient,
                       const std::string& inName) :
		Operator(inName),
		mCommsName(inCommsName),
		mDefaultRecipient(inDefaultRecipient)
{}


/*!
 *  \brief SendOp destructor.
 */
Island::SendOp::~SendOp()
{}


/*!
 *  \brief Add address to list of addresses to send to.
 *  \param inAddress Name of address to send to.
 *  \param inRecipient Name of recipient at the receiving end.
 */
void Island::SendOp::addAddress(const std::string& inAddress, const std::string& inRecipient)
{
	mAddresses.push_back( make_pair( inAddress, inRecipient ) );
}



/*!
 *  \brief Initialize SendOp.
 *  \param ioSystem Evolutionary System.
 *
 *  This method ensures that the named Communication component
 *  actually exists.  If the Logger is set to debug mode, all stored
 *  addresses and their recipients will be logged.
 */
void Island::SendOp::init(System& ioSystem)
{
#ifdef BEAGLE_FULL_DEBUG
	// Log addresses to send to
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::SendOp",
	    getName()+" has "+uint2str(mAddresses.size())+
	    " address/recipient pairs"
	);
	unsigned int lIndex=0;
	for (AddressesType::iterator lItr = mAddresses.begin();
	        lItr != mAddresses.end(); ++lItr) {
		Beagle_LogDebugM(
		    ioSystem.getLogger(),
		    "communications", "Beagle::SendOp",
		    uint2ordinal(++lIndex)+" address: '"+lItr->first+
		    "' recipient: '"+lItr->second+"'"
		);
	}
	// Log name of Communications component
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::SendOp",
	    std::string("Name of Communications component is '")+mCommsName+"'"
	);
#endif // BEAGLE_FULL_DEBUG
	if(mCommsName.empty()) {
		mCommsName = ioSystem.getFactory().getConceptTypeName("DBIM-Communications");
	}
	// Check that named Comms component exists
	ioSystem.getComponent(mCommsName); // throws exception if it doesn't exist
}


/*!
 *  \brief Logs a warning if address list is empty.  Logs a warning is
 *  specified address is unknow.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void Island::SendOp::operate(Deme& ioDeme, Context& ioContext)
{
	if (mAddresses.empty()) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::SendOp",
		    std::string("WARNING: In SendOp::operate(): There are no addresses specified for operator '")+
		    getName()+"'"
		);
	}

	// Get Comms component
	System& lSystem = ioContext.getSystem();
	Communications::Handle lComms
	= castHandleT<Communications>(lSystem.getComponent( mCommsName ));

	// Loop through addresses checking they're known
	for (AddressesType::iterator lItr = mAddresses.begin();
	        lItr != mAddresses.end();
	        ++lItr) {
		if (!(lComms->haveAddress(lItr->first))) {
			// Address not known; warning.
			Beagle_LogBasicM(
			    ioContext.getSystem().getLogger(),
			    "communications", "Beagle::SendOp",
			    std::string("WARNING: In SendOp::operate(): Address '")+lItr->first+"' for operator '"+
			    getName()+"' is not known by communications component named '"+mCommsName+"'"
			);
		}
	}
}


/*!
 *  \brief Reads addresses from XML source.
 *  \param inIter XML source.
 *  \param ioSystem Evolutionary system.
 */
void Island::SendOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Operator::readWithSystem(inIter, ioSystem);
	// Clear any current addresses
	mAddresses.clear();

	// Read Communications component's name
	if (inIter->isDefined("commsName")) {
		mCommsName = inIter->getAttribute("commsName");
	}

	// Read addresses
	for (PACC::XML::ConstIterator lItr = inIter->getFirstChild();
	        lItr;
	        lItr = lItr->getNextSibling() ) {
		// Read addresses
		if (lItr->getValue() == "SendTo") {
			// Get 'address'
			std::string lAddress;
			if (lItr->isDefined("address")) {
				lAddress = lItr->getAttribute("address");
			} else {
				// 'name' attribute missing
				throw Beagle_RunTimeExceptionM
				(std::string("Couldn't find 'address' attribute of 'SendTo' tag for '")+
				 getName()+"' operator");
			}
			if (lItr->isDefined("recipient")) {
				addAddress( lAddress, lItr->getAttribute("recipient") );
			} else {
				addAddress( lAddress, mDefaultRecipient );
			}
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Sends a message via Communications system-component to all
 *  stored addresses.
 *  \param inMessage Message to send.
 *  \param ioContext Evolutionary context.
 */
void Island::SendOp::sendToAll(const std::string& inMessage, Context &ioContext)
{
	Beagle_StackTraceBeginM();
	// Check there is at least one address to send to
	if (mAddresses.empty()) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::SendOp",
		    std::string("WARNING: In SendOp::sendToAll(): There are no addresses specified for operator '")+
		    getName()+"'"
		);
		return;
	}

	// Get Comms component
	System& lSystem = ioContext.getSystem();
	Communications::Handle lComms
	= castHandleT<Communications>(lSystem.getComponent( mCommsName ));

	// Send message to every address
	for (AddressesType::iterator lItr = mAddresses.begin();
	        lItr != mAddresses.end();
	        ++lItr) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::SendOp",
		    std::string("Sending to '")+lItr->first+"', recipient '"+lItr->second+"'"
		);
		lComms->sendMessage(lItr->first,  // address
		                    lItr->second, // recipient
		                    inMessage,    // message
		                    ioContext);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a send operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 *
 *  Writes "comms" attribute that declares the name of the
 *  communications component to use.
 */
void Island::SendOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(),inIndent);
	ioStreamer.insertAttribute("comms",mCommsName);
	writeAddresses(ioStreamer,inIndent);
	ioStreamer.closeTag(); // getName()
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Writes the addresses that send operator will send to.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 *
 *  A warning comment will be written if no addresses have been specified.
 */
void Island::SendOp::writeAddresses(PACC::XML::Streamer &ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if (mAddresses.empty()) {
		ioStreamer.insertComment("Warning!  No addresses have been specified for sending");
		return;
	}
	for (AddressesType::const_iterator lItr = mAddresses.begin();
	        lItr != mAddresses.end();
	        ++lItr) {
		ioStreamer.openTag("SendTo", inIndent);
		ioStreamer.insertAttribute("address",lItr->first);
		ioStreamer.insertAttribute("recipient",lItr->second);
		ioStreamer.closeTag(); // SendTo
	}
	Beagle_StackTraceEndM();
}
