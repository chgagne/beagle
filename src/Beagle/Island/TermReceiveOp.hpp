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
 *  \file   beagle/TermReceiveOp.hpp
 *  \brief  Definition of the class TermReceiveOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_TermReceiveOp_hpp
#define Beagle_Distrib_Island_TermReceiveOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Context.hpp"
#include "beagle/System.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class TermReceiveOp beagle/TermReceiveOp.hpp "beagle/TermReceiveOp.hpp"
 *  \brief Termination via Communications termination operator.
 *  \ingroup DBIM
 *
 *  A "Terminate" signal is sent by TermSendOp.
 */
class TermReceiveOp : public TerminationOp
{

public:

	//! TermReceiveOp allocator type
	typedef AllocatorT<TermReceiveOp,TerminationOp::Alloc>
	Alloc;
	//! TermReceiveOp handle type.
	typedef PointerT<TermReceiveOp,TerminationOp::Handle>
	Handle;
	//! TermReceiveOp bag type.
	typedef ContainerT<TermReceiveOp,TerminationOp::Bag>
	Bag;

	explicit TermReceiveOp(const std::string& inCommsName=std::string(),
	                       const std::string& inName="DBIM-TermReceiveOp");
	virtual ~TermReceiveOp()
	{ }

	virtual bool terminate(const Deme& inDeme, Context& ioContext);

protected:

	std::string mCommsName; //!< Name of Communications component

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_TermReceiveOp_hpp
