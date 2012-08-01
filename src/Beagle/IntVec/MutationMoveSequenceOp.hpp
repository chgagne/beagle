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
 *  \file   Beagle/IntVec/MutationMoveSequenceOp.hpp
 *  \brief  Definition of the class IntVec::MutationMoveSequenceOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_MutationMoveSequenceOp_hpp
#define Beagle_IntVec_MutationMoveSequenceOp_hpp

#include <string>

#include "Beagle/IntVec.hpp"


namespace Beagle
{
namespace IntVec
{

/*!
 *  \class MutationMoveSequenceOp Beagle/IntVec/MutationMoveSequenceOp.hpp "Beagle/IntVec/MutationMoveSequenceOp.hpp"
 *  \brief IntVec individual move sequence mutation operator class.
 *  \ingroup IntVecF
 *
 *	This mutation select a select a sequence in the genotype and move it elsewhere
 *	using a copy of the standard rotation algorithm.
 */
class MutationMoveSequenceOp : public EC::MutationOp
{
	
public:
	
	//! IntVec::MutationMoveSequenceOp allocator type.
	typedef AllocatorT<MutationMoveSequenceOp,EC::MutationOp::Alloc>
		Alloc;
	//! IntVec::MutationMoveSequenceOp handle type.
	typedef PointerT<MutationMoveSequenceOp,EC::MutationOp::Handle>
		Handle;
	//! IntVec::MutationMoveSequenceOp bag type.
	typedef ContainerT<MutationMoveSequenceOp,EC::MutationOp::Bag>
		Bag;
	
	explicit MutationMoveSequenceOp(std::string inMutationPbName="intvec.mutmov.indpb",
									   std::string inName="IntVec-MutationMoveSequenceOp");
	virtual ~MutationMoveSequenceOp()
	{ }
	
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void registerParams(System& ioSystem);
		
};

}
}

#endif // Beagle_IntVec_MutationMoveSequenceOp_hpp
