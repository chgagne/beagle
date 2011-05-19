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
 *  \file   beagle/GA/MutationMoveSequenceOp.hpp
 *  \brief  Definition of the class GA::MutationMoveSequenceOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_GA_MutationMoveSequenceOp_hpp
#define Beagle_GA_MutationMoveSequenceOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationMoveSequenceOp beagle/GA/MutationMoveSequenceOp.hpp "beagle/GA/MutationMoveSequenceOp.hpp"
 *  \brief GA individual move sequence mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 *
 *	This mutation select a select a sequence in the genotype and move it elsewhere
 *	using a copy of the standard rotation algorithm.
 */
class MutationMoveSequenceOp : public Beagle::MutationOp
{
	
public:
	
	//! GA::MutationMoveSequenceOp allocator type.
	typedef AllocatorT<MutationMoveSequenceOp,Beagle::MutationOp::Alloc>
		Alloc;
	//! GA::MutationMoveSequenceOp handle type.
	typedef PointerT<MutationMoveSequenceOp,Beagle::MutationOp::Handle>
		Handle;
	//! GA::MutationMoveSequenceOp bag type.
	typedef ContainerT<MutationMoveSequenceOp,Beagle::MutationOp::Bag>
		Bag;
	
	explicit MutationMoveSequenceOp(std::string inMutationPbName="ga.mutmov.indpb",
									   std::string inName="GA-MutationMoveSequenceOp");
	virtual ~MutationMoveSequenceOp()
	{ }
	
	virtual void registerParams(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	
};

}
}

#endif // Beagle_GA_MutationMoveSequenceOp_hpp
