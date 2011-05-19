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
 *  \file   beagle/Communications.hpp
 *  \brief  Definition of the class Communications.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_Communications_hpp
#define Beagle_Distrib_Island_Communications_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Component.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class Communications beagle/Communications.hpp "beagle/Communications.hpp"
 *  \brief Base class for communications system-component.
 *  \ingroup DBIM
 *
 *  The Communications classes are used to send and receive messages
 *  to and from Open BEAGLE programs.  They may also be used to
 *  communicate with other programs.
 *
 *  Messages consist of two parts: the recipient and the body of the
 *  message.  When receiving messages (using the
 *  Beagle::Communications::getNextMessage() method) the recipient must be
 *  specified.  This ensures that messages are given to the
 *  appropriate object within the receiving program.  The receiving
 *  object may then process the body of the message as it wishes.
 *  Thus, when sending messages (using the
 *  Beagle::Communications::sendMessage() method) it is very important to
 *  specify the appropriate recipient of the receiving program.
 *
 *  Target addresses are made known to the Communications class
 *  through the parameters file.  Each address is given a name to
 *  identify it.  These names are used as the first argument to the
 *  Beagle::Communications::sendMessage() method.
 *  This is a pure-virtual class (which means it cannot be
 *  instantiated).  For a class that can be instantiated, you might
 *  like to consider Beagle::CommunicationsTCPIP.
 *
 *  \sa Beagle::CommunicationsOp
 */
class Communications : public Beagle::Component
{

public:

	//! Communications allocator type.
	typedef AbstractAllocT< Communications, Component::Alloc > Alloc;
	//! Communications handle type.
	typedef PointerT  < Communications, Component::Handle > Handle;
	//! Communications bag type.
	typedef ContainerT< Communications, Component::Bag > Bag;

	// Constructor, destructor, and initialization
	explicit Communications(const std::string& inName="DBIM-Communications");
	virtual ~Communications()
	{ }

	/*!
	 *  \brief Read an address from an XML source.
	 *  \param inIter The XML source.
	 *  \param ioSystem Evolutionary system.
	 */
	virtual void readAddress(PACC::XML::ConstIterator inIter,
	                         Beagle::System& ioSystem) =0;

	/*!
	 *  \brief Returns true if address is known.
	 *  \param inAddress Address to check for.
	 */
	virtual bool haveAddress(const std::string& inAddress) =0;

	/*!
	 *  \brief Send message to inAddress
	 *  \param inAddress The name of the receiver.
	 *  \param inRecipient The name of the recipient within the receiver.
	 *  \param inMessage The body of the message.
	 *  \param ioContext Evolutionary context of the operation.
	 */
	virtual void sendMessage(const std::string& inAddress,
	                         const std::string& inRecipient,
	                         const std::string& inMessage,
	                         Beagle::Context& ioContext) =0;

	void doNotPurge(const std::string& inRecipient);
	void purgeMessages(Beagle::Context& ioContext,
	                   bool inPurgeEverything=false);
	void getMessages(Beagle::Context& ioContext);
	bool getNextMessage(const std::string& inRecipient,
	                    std::string& outMessage,
	                    Beagle::Context& ioContext);
	void logAllMessages(Beagle::Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter,
	                            Beagle::System& ioSystem);

protected:

	//! Typedef for a message (pair of strings; the first string is the
	//! recipient, the second is the message itself)
	typedef std::pair< std::string, std::string > MessageType;
	//! Typedef for a queue of messages
	typedef std::vector< MessageType > MessageQueueType;
	//! Typedef for list of recipients whose messages should not be purged
	typedef std::vector<std::string> DoNotPurgeListType;

	/*!
	 *  \brief Get message queue.
	 *  \param ioQueue Message queue.
	 *
	 *  Received messages are queued during normal operation
	 *  (e.g. during evolution, and during evaluation).  When messages
	 *  are to be evaluated, the queue is obtained (see
	 *  Beagle::CommunicationsOp) and the messages are requested via
	 *  Communications::getNextMessage().  If the messages are not
	 *  requested, then they are cleared at the next call of
	 *  Communications::getMessages().
	 */
	virtual void getMessageQueue(MessageQueueType& ioQueue) =0;

	//! The queue of messages
	MessageQueueType mMessageQueue;
	//! Flag to ensure getMessages() is called before getNextMessage()
	bool mMessageQueueValid;
	//! List of recipients whose messages should not be purged
	DoNotPurgeListType mDoNotPurgeList;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_Communications_hpp
