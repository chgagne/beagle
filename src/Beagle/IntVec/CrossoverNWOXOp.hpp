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
 *  \file   Beagle/IntVec/CrossoverNWOXOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverNWOXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_CrossoverNWOXOp_hpp
#define Beagle_IntVec_CrossoverNWOXOp_hpp

#include <string>

#include <Beagle/Core.hpp>
#include <Beagle/EC.hpp>


namespace Beagle
{
	
namespace IntVec
{

/*!
 *  \class CrossoverNWOXOp Beagle/IntVec/CrossoverNWOXOp.hpp
 *    "Beagle/IntVec/CrossoverNWOXOp.hpp"
 *  \brief Non-wrapping order crossover over indices permutation genotypes.
 *  \ingroup IntVecF
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children by creating and
 *	filling holes in the two parents while keeping the absolute order of each gene.
 *	It has been introduced by Cicirello in "Non-Wrapping Order Crossover: An Order
 *	Preserving Crossover Operator that Respects Absolute Position", 2006.
 *
 */
class CrossoverNWOXOp : public EC::CrossoverOp {
public:
	//! IntVec::CrossoverNWOXOp allocator type.
	typedef AllocatorT<CrossoverNWOXOp,EC::CrossoverOp::Alloc> Alloc;
	//! IntVec::CrossoverNWOXOp handle type.
	typedef PointerT<CrossoverNWOXOp,EC::CrossoverOp::Handle> Handle;
	//! IntVec::CrossoverNWOXOp bag type.
	typedef ContainerT<CrossoverNWOXOp,EC::CrossoverOp::Bag> Bag;
	
	explicit CrossoverNWOXOp(std::string inMatingPbName="intvec.cxnwo.prob",
							 std::string inName="IntVec-CrossoverNWOXOp");
	virtual ~CrossoverNWOXOp()
	{ }
	
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	virtual void registerParams(System& ioSystem);
	
};

}
}

#endif //Beagle_IntVec_CrossoverNWOXOp_hpp
