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
 *  \file   beagle/Distrib/Island/src/Communications.cpp
 *  \brief  Source code of class Communications.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct Communications component with name inName
 *  \param inName Name of the Communications component.
 */
Island::Communications::Communications(const std::string& inName) :
		Component(inName),
		mMessageQueueValid(false)
{ }



/*!
 *  \brief Add inRecipient to list of do-not-purge recipients.
 *  Recipients on this list will not have their messages purged during
 *  a call to getMessages().
 *  \param inRecipient Name of recipient to add to list.
 */
void Island::Communications::doNotPurge(const std::string& inRecipient)
{
	mDoNotPurgeList.push_back(inRecipient);
}


void Island::Communications::purgeMessages(Beagle::Context& ioContext,
        bool inPurgeEverything)
{
	unsigned int lNumPurged = 0;
	// Check if we need to get messages
	if (inPurgeEverything) {
		PACC::Threading::Thread::sleep(3.);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    "Getting messages for complete purge"
		);
		getMessageQueue(mMessageQueue);
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    std::string("There are now ")+uint2str(mMessageQueue.size())+" messages in the queue"
		);
		mMessageQueueValid = true;
	}

	// Loop through messages deleting those that aren't in the do-not-purge list
	MessageQueueType::iterator lMsgItr = mMessageQueue.begin();
	while (lMsgItr != mMessageQueue.end()) {
		// Search for recipient in do-not-purge list
		bool lPurge = true;
		if (!inPurgeEverything) {
			for(DoNotPurgeListType::iterator lPurgeItr = mDoNotPurgeList.begin();
			        lPurgeItr != mDoNotPurgeList.end();
			        ++lPurgeItr) {
				if ( *lPurgeItr == lMsgItr->first ) {
					// Do not purge
					lPurge = false;
					break;
				}
			}
		}
		// Purge message if required
		if (lPurge) {
			// Purge message
			Beagle_LogDetailedM(
			    ioContext.getSystem().getLogger(),
			    "communications", "Beagle::Communications",
			    std::string("Purging uncleared message: ")+
			    "Recipient: '"+lMsgItr->first+
			    "' Message: '"+lMsgItr->second+"'"
			);
			lMsgItr = mMessageQueue.erase( lMsgItr );
			++lNumPurged;
		} else {
			// Don't purge message
			++lMsgItr;
		}
	}
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "communications", "Beagle::Communications",
	    uint2str(lNumPurged)+(" messages were purged; ")+
	    int2str(mMessageQueue.size())+" messages remain"
	);
}



/*!
 *  \brief Clears current queue and replaces it with new messages
 *  \param ioContext Evolutionary context of the operation.
 */
void Island::Communications::getMessages(Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if (!mMessageQueue.empty()) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    std::string("WARNING: There are ")+uint2str(mMessageQueue.size())+
		    " messages that have not been cleared; these may be deleted"
		);
		purgeMessages(ioContext);
	}
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "communications", "Beagle::Communications",
	    "Getting messages"
	);
	getMessageQueue(mMessageQueue);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "communications", "Beagle::Communications",
	    std::string("There are now ")+uint2str(mMessageQueue.size())+" messages in the queue"
	);
	mMessageQueueValid = true;
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Find next message for given recipient.
 *  \param inRecipient The queue of messages will be search for messages for this recipient.
 *  \param outMessage The body of the message that was found if this method returned true.
 *  \param ioContext Evolutionary context of the operation.
 *  \return True if a message was found, false otherwise.
 *
 *  The oldest messages are returned first (i.e. this is a FIFO
 *  queue).
 *
 *  [Note for developers: This method could be made more efficient by
 *  moving from a queue to a map of lists.]
 */
bool Island::Communications::getNextMessage(const std::string& inRecipient,
        std::string& outMessage,
        Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Check message queue is valid
	if (!mMessageQueueValid) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    "WARNING: In Communications component: Message queue is not valid.  Consider either inserting the operator 'Beagle::CommunicationsOp' or calling Communications::getMessages()"
		);
		return false;
	}
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "communications", "Beagle::Communications",
	    std::string("Searching for message for recipient '")+inRecipient+"'"
	);
	if (mMessageQueue.empty()) {
		// No messages
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    "There are no messages in the queue"
		);
	} else {
		// Some messages
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    std::string("There are ")+uint2str(mMessageQueue.size())+" messages in the queue"
		);
	}
	// Look through queued messages for message for recipient
	for (MessageQueueType::iterator lItr = mMessageQueue.begin();
	        lItr != mMessageQueue.end();
	        ++lItr) {
		if (lItr->first == inRecipient) {
			// Found message
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "communications", "Beagle::Communications",
			    "Message found"
			);
			outMessage=lItr->second;
			mMessageQueue.erase(lItr);
			return true;
		}
	}
	// No messages for recipient were found
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "communications", "Beagle::Communications",
	    "No message found"
	);
	return false;
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Log all messages in the queue.
 *  \param ioContext Evolutionary context of the operation.
 *
 *  This method is for debugging purposes only.  Indeed, no log
 *  messages will even be seen unless the Logger is set to Debug mode
 *  (The exception is when the message queue is invalid).
 *
 *  \sa getMessages()
 */
void Island::Communications::logAllMessages(Beagle::Context& ioContext)
{
	// Check queue is valid
	if (!mMessageQueueValid) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    "WARNING: In Communications component: Message queue is not valid.  Consider either inserting the operator 'Beagle::CommunicationsOp' or calling Communications::getMessages()"
		);
		return;
	}
	// State that we're logging all queued messages
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "communications", "Beagle::Communications",
	    std::string("Logging all queued messages.  There are ")+
	    uint2str(mMessageQueue.size())+
	    " messages"
	);
	// Log the messages
	unsigned int lIndex=0;
	for (MessageQueueType::iterator lItr = mMessageQueue.begin();
	        lItr != mMessageQueue.end();
	        ++lItr) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "communications", "Beagle::Communications",
		    std::string("Message #")+uint2str(lIndex)+": "+
		    "Recipient: '"+lItr->first+
		    "' Message: '"+lItr->second+"'"
		);
		lIndex++;
	}
}



/*!
 *  \brief Read the Communications component from and XML source.
 *  Calls the readAddress() method.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioSystem Evolutionary system.
 */
void Island::Communications::readWithSystem(PACC::XML::ConstIterator inIter,
        Beagle::System& ioSystem)
{
	Beagle_LogDebugM(
	    ioSystem.getLogger(),
	    "communications", "Beagle::Communications",
	    "Reading configuration for Communications component"
	);

	if ((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName()))
		throw Beagle_IOExceptionNodeM(*inIter, std::string("tag <")+getName()+"> expected!");

	// Read addresses
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType()==PACC::XML::eData &&
		        lChild->getValue()=="Address") {
			readAddress( lChild, ioSystem );
		}
	}
}

