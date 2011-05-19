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
 *  \file   beagle/CommunicationsTCPIP.hpp
 *  \brief  Definition of the class CommunicationsTCPIP.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_CommunicationsTCPIPOp_hpp
#define Beagle_Distrib_Island_CommunicationsTCPIPOp_hpp

#include "PACC/Socket.hpp"
#include "beagle/Distrib/Island/Communications.hpp"
#include "beagle/Distrib/Island/CommunicationsTCPIPReceiver.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class CommunicationsTCPIP beagle/CommunicationsTCPIP.hpp "beagle/CommunicationsTCPIP.hpp"
 *  \brief Concrete class for Communications system-component. Implements TCP/IP communications.
 *  \ingroup DBIM
 */
class CommunicationsTCPIP : public Communications
{

public:

	//! CommunicationsTCPIP allocator type.
	typedef AllocatorT< CommunicationsTCPIP, Communications::Alloc > Alloc;
	//! CommunicationsTCPIP allocator type.
	typedef PointerT  < CommunicationsTCPIP, Communications::Handle > Handle;
	//! CommunicationsTCPIP allocator type.
	typedef ContainerT< CommunicationsTCPIP, Communications::Bag > Bag;


	explicit CommunicationsTCPIP(unsigned int inPortNumber=(unsigned int)-1,
	                             const std::string inName="DBIM-CommunicationsTCPIP");
	virtual ~CommunicationsTCPIP();

	virtual void init(System &ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter,
	                            Beagle::System& ioSystem);
	virtual void readAddress(PACC::XML::ConstIterator inIter,
	                         Beagle::System& ioSystem);
	virtual void writeContent (PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

	void addAddress(const std::string& inName,
	                const PACC::Socket::Address& inAddress );
	virtual bool haveAddress(const std::string& inAddress);
	void sendMessage(const std::string& inAddress,
	                 const std::string& inRecipient,
	                 const std::string& inMessage,
	                 Beagle::Context& ioContext);

private:

	//! Typedef for a map that maps names to TCP/IP addresses
	typedef std::map< std::string, PACC::Socket::Address > AddressToTCPIPMapType;

	virtual void getMessageQueue(MessageQueueType& ioQueue);
	bool getTCPIPAddress(const std::string& inAddress,
	                     PACC::Socket::Address& outAddress);

	//! Map to map names to TCP/IP addresses
	AddressToTCPIPMapType mAddressToTCPIPMap;
	//! Port number on which to listen for messages
	unsigned int mPortNumber;
	//! Pointer to the message-receiving thread
	CommunicationsTCPIPReceiver* mReceiver;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_CommunicationsTCPIPOp_hpp
