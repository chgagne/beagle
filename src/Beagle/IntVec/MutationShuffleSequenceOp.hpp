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
 *  \file   Beagle/IntVec/MutationShuffleSequenceOp.hpp
 *  \brief  Definition of the class IntVec::MutationReverseSequenceOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_MutationShuffleSequenceOp_hpp
#define Beagle_IntVec_MutationShuffleSequenceOp_hpp

#include "Beagle/IntVec.hpp"
#include <string>


namespace Beagle
{
namespace IntVec
{

/*!
 *  \class MutationShuffleSequenceOp Beagle/IntVec/MutationShuffleSequenceOp.hpp "Beagle/IntVec/MutationShuffleSequenceOp.hpp"
 *  \brief Indices permutation individual shuffle sequence mutation operator class.
 *  \ingroup IntVecF
 *
 *	This mutation select a sequence in the genotype and shuffle the values in it.
 */
class MutationShuffleSequenceOp : public EC::MutationOp
{
	
public:
	
	//! IntVec::MutationShuffleSequenceOp allocator type.
	typedef AllocatorT<MutationShuffleSequenceOp,EC::MutationOp::Alloc> Alloc;
	//! IntVec::MutationShuffleSequenceOp handle type.
	typedef PointerT<MutationShuffleSequenceOp,EC::MutationOp::Handle> Handle;
	//! IntVec::MutationShuffleSequenceOp bag type.
	typedef ContainerT<MutationShuffleSequenceOp,EC::MutationOp::Bag> Bag;
	
	explicit MutationShuffleSequenceOp(std::string inMutationPbName="intvec.mutshf.indpb",
	                                   std::string inName="IntVec-MutationShuffleSequenceOp");
	virtual ~MutationShuffleSequenceOp()
	{ }
	
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void registerParams(System& ioSystem);
	
};

}
}

#endif // Beagle_IntVec_MutationShuffleSequenceOp_hpp
