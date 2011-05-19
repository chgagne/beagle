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
 *  \file   beagle/TransportSendOp.hpp
 *  \brief  Definition of the class TransportSendOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_TransportSendOp_hpp
#define Beagle_Distrib_Island_TransportSendOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Float.hpp"
#include "beagle/Distrib/Island/SendOp.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class TransportSendOp beagle/TransportSendOp.hpp "beagle/TransportSendOp.hpp"
 *  \brief Operator to transport individuals via Communications component.
 *  \ingroup DBIM
 */
class TransportSendOp : public SendOp
{

public:

	//! TransportSendOp allocator type.
	typedef AllocatorT<TransportSendOp,SendOp::Alloc>
	Alloc;
	//! TransportSendOp handle type.
	typedef PointerT<TransportSendOp,SendOp::Handle>
	Handle;
	//! TransportSendOp bag type.
	typedef ContainerT<TransportSendOp,SendOp::Bag>
	Bag;

	explicit TransportSendOp(const std::string& inCommsName=std::string(),
	                         const std::string& inTransCompName="DBIM-TransportComponent",
	                         const std::string& inDefaultRecipient="DBIM-TransportReceiveOp",
	                         const std::string& inName="DBIM-TransportSendOp");
	explicit TransportSendOp(SelectionOp::Handle inSelectionOp,
	                         const std::string& inCommsName=std::string(),
	                         const std::string& inTransCompName="DBIM-TransportComponent",
	                         const std::string& inDefaultRecipient="DBIM-TransportReceiveOp",
	                         const std::string& inName="DBIM-TransportSendOp");
	virtual ~TransportSendOp();

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme &ioDeme, Context &ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void send(const Individual::Bag& inIndividuals, Context &ioContext);
	virtual void write (PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

protected:

	//! Selection operator used to select individuals to send
	SelectionOp::Handle mSelectionOp;
	//! Proportion of the population to send
	Float::Handle mProportionToSend;
	//! Generations between transportations
	UInt::Handle mTransportInterval;
	//! Name of transport component
	std::string mTransCompName;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_TransportSendOp_hpp

