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
 *  \file   beagle/GA/CrossoverNWOXOp.hpp
 *  \brief  Definition of the class GA::CrossoverNWOXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_GA_CrossoverNWOXOp_hpp
#define Beagle_GA_CrossoverNWOXOp_hpp

#include <string>

#include <beagle/config.hpp>
#include <beagle/macros.hpp>
#include <beagle/Object.hpp>
#include <beagle/CrossoverOp.hpp>

namespace Beagle {
namespace GA {

/*!
 *  \class CrossoverNWOXOp beagle/GA/CrossoverNWOXOp.hpp
 *    "beagle/GA/CrossoverNWOXOp.hpp"
 *  \brief 
 *  \ingroup GAIV
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children by creating and
 *	filling holes in the two parents while keeping the absolute order of each gene.
 *	It has been introduced by Cicirello in "Non-Wrapping Order Crossover: An Order
 *	Preserving Crossover Operator that Respects Absolute Position", 2006.
 *
 */
class CrossoverNWOXOp : public CrossoverOp {
public:
	//! GA::CrossoverNWOXOp allocator type.
	typedef AllocatorT<CrossoverNWOXOp,CrossoverOp::Alloc> Alloc;
	//! GA::CrossoverNWOXOp handle type.
	typedef PointerT<CrossoverNWOXOp,CrossoverOp::Handle> Handle;
	//! GA::CrossoverNWOXOp bag type.
	typedef ContainerT<CrossoverNWOXOp,CrossoverOp::Bag> Bag;
	
	explicit CrossoverNWOXOp(std::string inMatingPbName="ga.cxnwo.prob",
							 std::string inName="GA-CrossoverNWOXOp");
	virtual ~CrossoverNWOXOp()
	{ }
	
	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	
};

}
}

#endif //Beagle_GA_CrossoverNWOXOp_hpp
