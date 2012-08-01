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
 *  \file   Beagle/IntVec/CrossoverPMXOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverPMXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_CrossoverPMXOp_hpp
#define Beagle_IntVec_CrossoverPMXOp_hpp

#include <string>

#include <Beagle/Core.hpp>
#include <Beagle/EC.hpp>


namespace Beagle
{

namespace IntVec
{

/*!
 *  \class CrossoverPMXOp Beagle/IntVec/CrossoverPMXOp.hpp
 *    "Beagle/IntVec/CrossoverPMXOp.hpp"
 *  \brief Partially matched crossover over indices permutation genotypes.
 *  \ingroup IntVecF
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children by matching pairs
 *	of values in a certain range of the two parents and swaping the values of those
 *	indexes. It has been introduced by Goldberg and Lingel in "Alleles, loci, and
 *	the traveling salesman problem", 1985.
 *
 */
class CrossoverPMXOp : public EC::CrossoverOp {
	
public:
	
	//! IntVec::CrossoverPMXOp allocator type.
	typedef AllocatorT<CrossoverPMXOp,EC::CrossoverOp::Alloc> Alloc;
	//! IntVec::CrossoverPMXOp handle type.
	typedef PointerT<CrossoverPMXOp,EC::CrossoverOp::Handle> Handle;
	//! IntVec::CrossoverPMXOp bag type.
	typedef ContainerT<CrossoverPMXOp,EC::CrossoverOp::Bag> Bag;
	
	explicit CrossoverPMXOp(std::string inMatingPbName="intvec.cxpm.prob",
	                        std::string inName="IntVec-CrossoverPMXOp");
	virtual ~CrossoverPMXOp()
	{ }
	
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);
	virtual void registerParams(System& ioSystem);
	
};
	
}
}

#endif //Beagle_IntVec_CrossoverPMXOp_hpp
