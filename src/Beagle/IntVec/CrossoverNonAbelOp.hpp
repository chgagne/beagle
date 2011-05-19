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
 *  \file   beagle/GA/CrossoverNonAbelOp.hpp
 *  \brief  Definition of the class GA::CrossoverNonAbel.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_GA_CrossoverNonAbelOp_hpp
#define Beagle_GA_CrossoverNonAbelOp_hpp

#include <string>

#include <beagle/config.hpp>
#include <beagle/macros.hpp>
#include <beagle/Object.hpp>
#include <beagle/CrossoverOp.hpp>

namespace Beagle {
namespace GA {

/*!
 *  \class CrossoverNonAbelOp beagle/GA/CrossoverNonAbelOp.hpp
 *    "beagle/GA/CrossoverNonAbelOp.hpp"
 *  \brief 
 *  \ingroup GAIV
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children that use the non-Abel
 *	group theory to perform $P_1 \otimes P_2$ and $P_2 \otimes P_1$ to generate
 *	$C_1$ and $C_2$. It has been introduced by Bac and Perov in "New evolutionary 
 *	genetic algorithms for NP-complete combinatorial optimization problems", 1993.
 *
 */
class CrossoverNonAbelOp : public CrossoverOp {
public:
	//! GA::CrossoverNonAbel allocator type.
	typedef AllocatorT<CrossoverNonAbelOp,CrossoverOp::Alloc> Alloc;
	//! GA::CrossoverNonAbel handle type.
	typedef PointerT<CrossoverNonAbelOp,CrossoverOp::Handle> Handle;
	//! GA::CrossoverNonAbel bag type.
	typedef ContainerT<CrossoverNonAbelOp,CrossoverOp::Bag> Bag;
	
	explicit CrossoverNonAbelOp(std::string inMatingPbName="ga.cxna.prob",
								std::string inName="GA-CrossoverNonAbelOp");
	virtual ~CrossoverNonAbelOp()
	{ }
	
	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	
};

}
}

#endif
