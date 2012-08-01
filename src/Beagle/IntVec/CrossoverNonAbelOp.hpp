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
 *  \file   Beagle/IntVec/CrossoverNonAbelOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverNonAbel.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_CrossoverNonAbelOp_hpp
#define Beagle_IntVec_CrossoverNonAbelOp_hpp

#include <string>

#include <Beagle/Core.hpp>
#include <Beagle/EC.hpp>

namespace Beagle {

namespace IntVec {

/*!
 *  \class CrossoverNonAbelOp Beagle/IntVec/CrossoverNonAbelOp.hpp
 *    "Beagle/IntVec/CrossoverNonAbelOp.hpp"
 *  \brief Non-Abel crossover over indices permutation genotypes.
 *  \ingroup IntVecF
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children that use the non-Abel
 *	group theory to perform $P_1 \otimes P_2$ and $P_2 \otimes P_1$ to generate
 *	$C_1$ and $C_2$. It has been introduced by Bac and Perov in "New evolutionary 
 *	genetic algorithms for NP-complete combinatorial optimization problems", 1993.
 *
 */
class CrossoverNonAbelOp : public EC::CrossoverOp {
public:
	//! IntVec::CrossoverNonAbel allocator type.
	typedef AllocatorT<CrossoverNonAbelOp,EC::CrossoverOp::Alloc> Alloc;
	//! IntVec::CrossoverNonAbel handle type.
	typedef PointerT<CrossoverNonAbelOp,EC::CrossoverOp::Handle> Handle;
	//! IntVec::CrossoverNonAbel bag type.
	typedef ContainerT<CrossoverNonAbelOp,EC::CrossoverOp::Bag> Bag;
	
	explicit CrossoverNonAbelOp(std::string inMatingPbName="intvec.cxna.prob",
								std::string inName="IntVec-CrossoverNonAbelOp");
	virtual ~CrossoverNonAbelOp()
	{ }
	
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	virtual void registerParams(System& ioSystem);
	
};

}
}

#endif // Beagle_IntVec_CrossoverNonAbelOp_hpp
