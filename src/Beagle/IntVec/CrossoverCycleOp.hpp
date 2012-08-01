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
 *  \file   Beagle/IntVec/CrossoverCycleOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverCycleOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_CrossoverCycleOp_hpp
#define Beagle_IntVec_CrossoverCycleOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{

namespace IntVec
{
	
/*!
 *  \class CrossoverCycleOp Beagle/IntVec/CrossoverCycleOp.hpp
 *    "Beagle/IntVec/CrossoverCycleOp.hpp"
 *  \brief Cycle crossover over indices permutation genotypes. 
 *  \ingroup IntVecF
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children that exchange the
 *	the values of the parents that are not contained in a cycle. It has been proposed 
 *	by Oliver, Smith and Holland in "A study of permutation crossover operators on the
 *	traveling salesman problem", 1987.
 *
 */
class CrossoverCycleOp : public EC::CrossoverOp {
	
public:
	//! IntVec::CrossoverCycleOp allocator type.
	typedef AllocatorT<CrossoverCycleOp,EC::CrossoverOp::Alloc> Alloc;
	//! IntVec::CrossoverCycleOp handle type.
	typedef PointerT<CrossoverCycleOp,EC::CrossoverOp::Handle> Handle;
	//! IntVec::CrossoverCycleOp bag type.
	typedef ContainerT<CrossoverCycleOp,EC::CrossoverOp::Bag> Bag;
	
	explicit CrossoverCycleOp(std::string inMatingPbName="intvec.cxc.prob",
							  std::string inName="IntVec-CrossoverCycleOp");
	virtual ~CrossoverCycleOp()
	{ }
	
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	virtual void registerParams(System& ioSystem);
					
};

}
}

#endif //Beagle_IntVec_CrossoverCycleOp_hpp
