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
 *  \file   beagle/GP/MutationShrinkOp.hpp
 *  \brief  Definition of the class GP::MutationShrinkOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MutationShrinkOp_hpp
#define Beagle_GP_MutationShrinkOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Individual.hpp"

namespace Beagle
{
namespace GP
{

/*!
 *  \class MutationShrinkOp beagle/GP/MutationShrinkOp.hpp "beagle/GP/MutationShrinkOp.hpp"
 *  \brief GP Shrink mutation operator class.
 *
 *  This operator shrinks the individuals that are subjected to it.
 *  The operator randomly chooses a branch in the individual and
 *  replaces it with one of the branch's arguments (also randomly
 *  chosen).
 *
 *  For example, let's start with the tree 
 *  \code <ADD><X/><Y/></ADD> \endcode
 *  If node 1 is selected, and node 1's second argument is
 *  selected, then the tree becomes just 
 *  \code <Y/> \endcode
 *
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationShrinkOp : public Beagle::MutationOp
{

public:

	//! GP::MutationShrinkOp allocator type.
	typedef AllocatorT<MutationShrinkOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GP::MutationShrinkOp handle type.
	typedef PointerT<MutationShrinkOp,Beagle::MutationOp::Handle>
	Handle;
	//! GP::MutationShrinkOp bag type.
	typedef ContainerT<MutationShrinkOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationShrinkOp(std::string inMutationPbName="gp.mutshrink.indpb",
	                          std::string inName="GP-MutationShrinkOp");
	virtual ~MutationShrinkOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

};

}
}

#endif // Beagle_GP_MutationShrinkOp_hpp
