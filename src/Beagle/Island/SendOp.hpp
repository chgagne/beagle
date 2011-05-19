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
*  \file   beagle/SendOp.hpp
*  \brief  Definition of the class SendOp.
*  \author Matthew Walker <m.g.walker@massey.ac.nz>
*  $Revision: 1.6 $
*  $Date: 2007/08/10 20:32:20 $
*/

#ifndef Beagle_Distrib_Island_SendOp_hpp
#define Beagle_Distrib_Island_SendOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class SendOp beagle/SendOp.hpp "beagle/SendOp.hpp"
 *  \brief Abstract base class for operators that send messages via a
 *    Communications system-component.
 *  \ingroup DBIM
 */
class SendOp : public Operator
{

public:

	//! SendOp allocator type.
	typedef AllocatorT<SendOp,Operator::Alloc>
	Alloc;
	//! SendOp handle type.
	typedef PointerT<SendOp,Operator::Handle>
	Handle;
	//! SendOp bag type.
	typedef ContainerT<SendOp,Operator::Bag>
	Bag;

	explicit SendOp(const std::string& inCommsName=std::string(),
	                const std::string& inDefaultRecipient=std::string(),
	                const std::string& inName="DBIM-SendOp");
	virtual ~SendOp();

	virtual void addAddress(const std::string& inAddress,const std::string& inRecipient);
	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void sendToAll(const std::string& inMessage, Context &ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	//! Typedef of vector of pairs of addresses and recipients to send to
	typedef std::vector< std::pair<std::string, std::string> > AddressesType;

	virtual void writeAddresses(PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

	//! Name of Communications component
	std::string mCommsName;
	//! Addresses to send to
	AddressesType mAddresses;
	//! Default recipient class name
	std::string mDefaultRecipient;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_SendOp_hpp
