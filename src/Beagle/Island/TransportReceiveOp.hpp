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
 *  \file   beagle/TransportReceiveOp.hpp
 *  \brief  Definition of the class TransportReceiveOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_TransportReceiveOp_hpp
#define Beagle_Distrib_Island_TransportReceiveOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class TransportReceiveOp beagle/TransportReceiveOp.hpp "beagle/TransportReceiveOp.hpp"
 *  \brief Operator to receive transported individuals via Communications component.
 *  \ingroup DBIM
 */
class TransportReceiveOp : public Operator
{

public:

	//! TransportReceiveOp allocator type.
	typedef AllocatorT<TransportReceiveOp,Operator::Alloc>
	Alloc;
	//! TransportReceiveOp handle type.
	typedef PointerT<TransportReceiveOp,Operator::Handle>
	Handle;
	//! TransportReceiveOp bag type.
	typedef ContainerT<TransportReceiveOp,Operator::Bag>
	Bag;

	explicit TransportReceiveOp(const std::string& inCommsName=std::string(),
	                            const std::string& inTransCompName="DBIM-TransportComponent",
	                            const std::string& inName="DBIM-TransportReceiveOp");
	explicit TransportReceiveOp(SelectionOp::Handle inReplacementOp,
	                            const std::string& inCommsName=std::string(),
	                            const std::string& inTransCompName="DBIM-TransportComponent",
	                            const std::string& inName="DBIM-TransportReceiveOp");
	virtual ~TransportReceiveOp();

	virtual void init(System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme &ioDeme, Context &ioContext);
	virtual void receive(Individual::Bag& outIndividuals, Context &ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write (PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

protected:

	//! Replacment selection operator
	SelectionOp::Handle mReplacementOp;
	//! Maximum proportion of the population to replace.
	Double::Handle mMaxReplace;
	//! Name of Communications component.
	std::string mCommsName;
	//! Generations between transportations
	UInt::Handle mTransportInterval;
	//! Name of transport component
	std::string mTransCompName;
	//! Flag for use of replacement selection operator.
	Bool::Handle mUseReplaceOp;
	//! Flag to indicate if sent individuals should be replaced first.
	Bool::Handle mReplaceSent;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_TransportReceiveOp_hpp
