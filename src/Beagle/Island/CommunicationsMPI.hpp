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
 *  \file   beagle/CommunicationsMPI.hpp
 *  \brief  Definition of the class CommunicationsMPI.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_CommunicationsMPIOp_hpp
#define Beagle_Distrib_Island_CommunicationsMPIOp_hpp

#include "beagle/Distrib/Island/Communications.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class CommunicationsMPI beagle/CommunicationsMPI.hpp "beagle/CommunicationsMPI.hpp"
 *  \brief Concrete class for Communications system-component.  Implements MPI communications.
 *  \ingroup DBIM
 */
class CommunicationsMPI : public Communications
{

public:

	//! CommunicationsMPI allocator type.
	typedef AllocatorT< CommunicationsMPI, Communications::Alloc > Alloc;
	//! CommunicationsMPI allocator type.
	typedef PointerT  < CommunicationsMPI, Communications::Handle > Handle;
	//! CommunicationsMPI allocator type.
	typedef ContainerT< CommunicationsMPI, Communications::Bag > Bag;
	//! Enumeration for MPI's tags.
	enum { eLengths, eRecipient, eMessage };


	explicit CommunicationsMPI(const std::string inName="DBIM-CommunicationsMPI");
	virtual ~CommunicationsMPI();

	virtual void initialize(System &ioSystem, int& ioArgc, char**& ioArgv);
	virtual void init(System &ioSystem);
	virtual void readAddress(PACC::XML::ConstIterator inIter,
	                         Beagle::System& ioSystem);

	void addAddress(const std::string& inName,
	                const int inAddress );
	void sendMessage(const std::string& inAddress,
	                 const std::string& inRecipient,
	                 const std::string& inMessage,
	                 Beagle::Context& ioContext);

private:

	//! Typedef for a map that maps names to rank.
	typedef std::map< std::string, int > AddressToMPIMapType;

	virtual MessageQueueType getMessageQueue();
	bool             getMPIAddress(const std::string& inAddress,
	                               int& outAddress);

	//! Map to map names to MPI addresses.
	AddressToMPIMapType mAddressToMPIMap;
	//! Receiving thread
	CommunicationsMPIReceiver* mReceiver;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_CommunicationsMPIOp_hpp
