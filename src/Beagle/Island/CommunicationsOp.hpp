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
 *  \file   beagle/CommunicationsOp.hpp
 *  \brief  Definition of the class CommunicationsOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_CommunicationsOp_hpp
#define Beagle_Distrib_Island_CommunicationsOp_hpp

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
 *  \class CommunicationsOp beagle/CommunicationsOp.hpp "beagle/CommunicationsOp.hpp"
 *  \brief Class to obtain messages from Communications component.
 *  \ingroup DBIM
 */
class CommunicationsOp : public Beagle::Operator
{

public:

	//! CommunicationsOp allocator type.
	typedef AllocatorT< CommunicationsOp, Operator::Alloc > Alloc;
	//! CommunicationsOp handle type.
	typedef PointerT  < CommunicationsOp, Operator::Handle > Handle;
	//! CommunicationsOp bag type.
	typedef ContainerT< CommunicationsOp, Operator::Bag > Bag;

	CommunicationsOp(const std::string inCommsName=std::string(),
	                 const std::string& inName="DBIM-CommunicationsOp");
	virtual ~CommunicationsOp()
	{}

	virtual void operate(Deme& ioDeme, Context& ioContext);

protected:

	//! Name of Communications component.
	std::string mCommsName;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_CommunicationsOp_hpp
