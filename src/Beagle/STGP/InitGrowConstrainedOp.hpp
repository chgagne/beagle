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
 *  \file   beagle/GP/InitGrowConstrainedOp.hpp
 *  \brief  Definition of the class GP::InitGrowConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_InitGrowConstrainedOp_hpp
#define Beagle_GP_InitGrowConstrainedOp_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/EC/InitializationOp.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/Bool.hpp"
#include "Beagle/GP/Tree.hpp"
#include "Beagle/GP/InitGrowOp.hpp"


namespace Beagle
{
namespace STGP
{

/*!
 *  \class InitGrowConstrainedOp
 *    beagle/GP/InitGrowConstrainedOp.hpp "beagle/GP/InitGrowConstrainedOp.hpp"
 *  \brief Constrained GP trees "grow" initialization operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 *  \ingroup GPPop
 */
class InitGrowConstrainedOp : public GP::InitGrowOp
{

public:

	//! GP::InitGrowConstrainedOp allocator type.
	typedef AllocatorT<InitGrowConstrainedOp,GP::InitGrowOp::Alloc>
	Alloc;
	//! GP::InitGrowConstrainedOp handle type.
	typedef PointerT<InitGrowConstrainedOp,GP::InitGrowOp::Handle>
	Handle;
	//! GP::InitGrowConstrainedOp bag type.
	typedef ContainerT<InitGrowConstrainedOp,GP::InitGrowOp::Bag>
	Bag;

	explicit InitGrowConstrainedOp(std::string inReproProbaName="ec.repro.prob",
	                               std::string inName="GP-InitGrowConstrainedOp");
	virtual ~InitGrowConstrainedOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual unsigned int initTree(GP::Tree& outTree,
	                              unsigned int inMinDepth,
	                              unsigned int inMaxDepth,
	                              GP::Context& ioContext) const;

protected:

	unsigned int initConstrainedSubTreeGrow(GP::Tree& ioTree,
	                                        unsigned int inMinDepth,
	                                        unsigned int inMaxDepth,
	                                        GP::Context& ioContext) const;

	UInt::Handle mNumberAttempts;   //!< Number of attempts in each initialization.
};

}
}

#endif // Beagle_GP_InitGrowConstrainedOp_hpp
