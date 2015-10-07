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
 *  \file   beagle/GP/InitHalfConstrainedOp.hpp
 *  \brief  Definition of the class GP::InitHalfConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_InitHalfConstrainedOp_hpp
#define Beagle_GP_InitHalfConstrainedOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/GP/InitFullConstrainedOp.hpp"
#include "beagle/GP/InitGrowConstrainedOp.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class InitHalfConstrainedOp
 *    beagle/GP/InitHalfConstrainedOp.hpp "beagle/GP/InitHalfConstrainedOp.hpp"
 *  \brief GP trees "half-and-half" initialization operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 *  \ingroup GPPop
 */
class InitHalfConstrainedOp : public GP::InitializationOp
{

public:

	//! GP::InitHalfConstrainedOp allocator type.
	typedef AllocatorT<InitHalfConstrainedOp,InitializationOp::Alloc>
	Alloc;
	//! GP::InitHalfConstrainedOp handle type.
	typedef PointerT<InitHalfConstrainedOp,InitializationOp::Handle>
	Handle;
	//! GP::InitGrowOp bag type.
	typedef ContainerT<InitHalfConstrainedOp,InitializationOp::Bag>
	Bag;

	explicit InitHalfConstrainedOp(std::string inReproProbaName="ec.repro.prob",
	                               std::string inName="GP-InitHalfConstrainedOp");
	virtual ~InitHalfConstrainedOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual unsigned int initTree(GP::Tree &outTree,
	                              unsigned int inMinDepth,
	                              unsigned int inMaxDepth,
	                              GP::Context &ioContext) const;

protected:

	InitFullConstrainedOp mInitFullOp;  //!< Full initialization operator.
	InitGrowConstrainedOp mInitGrowOp;  //!< Grow initialization operator.

};

}
}

#endif // Beagle_GP_InitHalfConstrainedOp_hpp
