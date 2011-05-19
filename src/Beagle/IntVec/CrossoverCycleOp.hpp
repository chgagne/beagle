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
 *  \file   beagle/GA/CrossoverCycleOp.hpp
 *  \brief  Definition of the class GA::CrossoverCycleOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_GA_CrossoverCycleOp_hpp
#define Beagle_GA_CrossoverCycleOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"

namespace Beagle {
namespace GA {

	
/*!
 *  \class CrossoverCycleOp beagle/GA/CrossoverCycleOp.hpp
 *    "beagle/GA/CrossoverCycleOp.hpp"
 *  \brief 
 *  \ingroup GAIV
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children that exchange the
 *	the values of the parents that are not contained in a cycle. It has been proposed 
 *	by Oliver, Smith and Hollad in "A study of permutation crossover operators on the
 *	traveling salesman problem", 1987.
 *
 */
class CrossoverCycleOp : public CrossoverOp {
public:
	//! GA::CrossoverCycleOp allocator type.
	typedef AllocatorT<CrossoverCycleOp,CrossoverOp::Alloc> Alloc;
	//! GA::CrossoverCycleOp handle type.
	typedef PointerT<CrossoverCycleOp,CrossoverOp::Handle> Handle;
	//! GA::CrossoverCycleOp bag type.
	typedef ContainerT<CrossoverCycleOp,CrossoverOp::Bag> Bag;
	
	explicit CrossoverCycleOp(std::string inMatingPbName="ga.cxc.prob",
							  std::string inName="GA-CrossoverCycleOp");
	virtual ~CrossoverCycleOp()
	{ }
	
	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
};

}
}

#endif //Beagle_GA_CrossoverCycleOp_hpp
