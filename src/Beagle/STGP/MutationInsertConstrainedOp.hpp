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
 *  \file   beagle/GP/MutationInsertConstrainedOp.hpp
 *  \brief  Definition of the class GP::MutationInsertConstrainedOp.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:35 $
 *
 *  This operator mutate a GP tree by inserting a new branch at a random position in a tree,
 *  using the original subtree at this position as one argument, and if necessary randomly
 *  selecting terminal primitives to complete the arguments of the inserted node.
 */

#ifndef Beagle_GP_MutationInsertConstrainedOp_hpp
#define Beagle_GP_MutationInsertConstrainedOp_hpp

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
#include "beagle/GP/MutationInsertOp.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class MutationInsertConstrainedOp beagle/GP/MutationInsertConstrainedOp.hpp "beagle/GP/MutationInsertConstrainedOp.hpp"
 *  \brief Constrained GP insert mutation operator class, insert a branch in the tree.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationInsertConstrainedOp : public Beagle::GP::MutationInsertOp
{

public:

	//! GP::MutationInsertConstrainedOp allocator type.
	typedef AllocatorT<MutationInsertConstrainedOp,Beagle::GP::MutationInsertOp::Alloc>
	Alloc;
	//! GP::MutationInsertConstrainedOp handle type.
	typedef PointerT<MutationInsertConstrainedOp,Beagle::GP::MutationInsertOp::Handle>
	Handle;
	//! GP::MutationInsertConstrainedOp bag type.
	typedef ContainerT<MutationInsertConstrainedOp,Beagle::GP::MutationInsertOp::Bag>
	Bag;

	explicit MutationInsertConstrainedOp(std::string inMutationPbName="gp.mutinsert.indpb",
	                                     std::string inName="GP-MutationInsertConstrainedOp");
	virtual ~MutationInsertConstrainedOp()
	{ }

	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

};

}
}

#endif // Beagle_GP_MutationInsertConstrainedOp_hpp
