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
 *  \file   beagle/GP/InitGrowOp.hpp
 *  \brief  Definition of the class GP::InitGrowOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_InitGrowOp_hpp
#define Beagle_GP_InitGrowOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Tree.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class InitGrowOp beagle/GP/InitGrowOp.hpp "beagle/GP/InitGrowOp.hpp"
 *  \brief GP trees "grow" initialization operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 *  \ingroup GPPop
 */
class InitGrowOp : public GP::InitializationOp
{

public:

	//! GP::InitGrowOp allocator type.
	typedef AllocatorT<InitGrowOp,InitializationOp::Alloc>
	Alloc;
	//! GP::InitGrowOp handle type.
	typedef PointerT<InitGrowOp,InitializationOp::Handle>
	Handle;
	//! GP::InitGrowOp bag type.
	typedef ContainerT<InitGrowOp,InitializationOp::Bag>
	Bag;

	explicit InitGrowOp(std::string inReproProbaName="ec.repro.prob",
	                    std::string inName="GP-InitGrowOp");
	virtual ~InitGrowOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual unsigned int initTree(GP::Tree &outTree,
	                              unsigned int inMinDepth,
	                              unsigned int inMaxDepth,
	                              GP::Context &ioContext) const;

protected:

	unsigned int initSubTreeGrow(GP::Tree& ioTree,
	                             unsigned int inMinDepth,
	                             unsigned int inMaxDepth,
	                             GP::Context& ioContext) const;

	Bool::Handle mKozaGrow;         //!< Flag to force 'grow' as defined by Koza in GPI (and used in GPII).

};

}
}

#endif // Beagle_GP_InitGrowOp_hpp
