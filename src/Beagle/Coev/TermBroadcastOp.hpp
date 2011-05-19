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
 *  \file   beagle/Coev/TermBroadcastOp.hpp
 *  \brief  Definition of the class TermBroadcastOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_TermBroadcastOp_hpp
#define Beagle_TermBroadcastOp_hpp

#include "PACC/Threading.hpp"

#include "beagle/Beagle.hpp"


namespace Beagle
{
namespace Coev
{

/*!
 *  \class TermBroadcastOp beagle/Coev/TermBroadcastOp.hpp "beagle/Coev/TermBroadcastOp.hpp"
 *  \brief Broadcast termination criterion amoung coevolution threads operator class.
 *  \ingroup COEVF
 */
class TermBroadcastOp : public TerminationOp
{

public:

	//! TermBroadcastOp allocator type
	typedef AllocatorT<TermBroadcastOp,TerminationOp::Alloc>
	Alloc;
	//! TermBroadcastOp handle type.
	typedef PointerT<TermBroadcastOp,TerminationOp::Handle>
	Handle;
	//! TermBroadcastOp bag type.
	typedef ContainerT<TermBroadcastOp,TerminationOp::Bag>
	Bag;

	explicit TermBroadcastOp(std::string inName="Coev-TermBroadcastOp");
	virtual ~TermBroadcastOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool terminate(const Deme& inDeme, Context& ioContext);

protected:

	static PACC::Threading::Condition smCondition;       //!< Condition for synchronizing threads termination.
	static unsigned int               smTrigger;         //!< Actual number of threads locked on condition.
	static std::set<void*>            smSysPtrsInvolved; //!< Pointers to systems involved in co-evolution.
	static unsigned int               smNbrThreads;      //!< Number of threads involved in co-evolution.
	static bool                       smTermReached;     //!< Stopping criterion reached.

};

}
}

#endif // Beagle_TerminationCheckOp_hpp
