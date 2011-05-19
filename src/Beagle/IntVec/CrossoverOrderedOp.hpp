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
 *  \file   beagle/GA/CrossoverOrderedOp.hpp
 *  \brief  Definition of the class GA::CrossoverOrderedOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_GA_CrossoverOrderedOp_hpp
#define Beagle_GA_CrossoverOrderedOp_hpp

#include <string>

#include <beagle/config.hpp>
#include <beagle/macros.hpp>
#include <beagle/Object.hpp>
#include <beagle/CrossoverOp.hpp>

namespace Beagle {
namespace GA {

/*!
 *  \class CrossoverOrderedOp beagle/GA/CrossoverOrderedOp.hpp
 *    "beagle/GA/CrossoverOrderedOp.hpp"
 *  \brief 
 *  \ingroup GAIV
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children by creating and
 *	filling holes in the two parents while every non-hole positions to the left.
 *	It has been introduced by Goldberg in "Genetic algorithms in search, 
 *	optimization and machine learning", 1989.
 *
 */
class CrossoverOrderedOp : public CrossoverOp {
public:
	//! GA::CrossoverOrderedOp allocator type.
	typedef AllocatorT<CrossoverOrderedOp,CrossoverOp::Alloc> Alloc;
	//! GA::CrossoverOrderedOp handle type.
	typedef PointerT<CrossoverOrderedOp,CrossoverOp::Handle> Handle;
	//! GA::CrossoverOrderedOp bag type.
	typedef ContainerT<CrossoverOrderedOp,CrossoverOp::Bag> Bag;
	
	explicit CrossoverOrderedOp(std::string inMatingPbName="ga.cxo.prob",
								std::string inName="GA-CrossoverOrderedOp");
	virtual ~CrossoverOrderedOp()
	{ }
	
	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	
};

}
}

#endif //Beagle_GA_CrossoverOrderedOp_hpp
