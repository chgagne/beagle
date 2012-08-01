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
 *  \file   Beagle/IntVec/CrossoverUPMXOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverUPMXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_IntVec_CrossoverUPMXOp_hpp
#define Beagle_IntVec_CrossoverUPMXOp_hpp

#include <string>

#include <Beagle/Core.hpp>
#include <Beagle/EC.hpp>


namespace Beagle {
namespace IntVec {

/*!
 *  \class CrossoverUPMXOp Beagle/IntVec/CrossoverUPMXOp.hpp
 *    "Beagle/IntVec/CrossoverUPMXOp.hpp"
 *  \brief Uniform partially matched crossover for indices (integer vector) genotypes.
 *  \ingroup IntVecF
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children by matching pairs
 *	of values at the same index uniformly of the two parents and swaping the values
 *	of those indexes. It has been introduced by Cicirello and Smith in "Modeling GA
 *	performance for control parameter optimization", 2000.
 *
 */
class CrossoverUPMXOp : public EC::CrossoverOp {
	
public:
	//! IntVec::CrossoverUPMXOp allocator type.
	typedef AllocatorT<CrossoverUPMXOp,EC::CrossoverOp::Alloc> Alloc;
	//! IntVec::CrossoverUPMXOp handle type.
	typedef PointerT<CrossoverUPMXOp,EC::CrossoverOp::Handle> Handle;
	//! IntVec::CrossoverUPMXOp bag type.
	typedef ContainerT<CrossoverUPMXOp,EC::CrossoverOp::Bag> Bag;
	
	explicit CrossoverUPMXOp(std::string inMatingPbName="intvec.cxupm.prob",
							 std::string inDistribPbName="intvec.cxupm.distribprob",
							 std::string inName="IntVec-CrossoverUPMXOp");
	virtual ~CrossoverUPMXOp()
	{ }
	
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	
protected:
	Double::Handle mDistribProba;		//!< Distribution probability of the uniform partialy matched crossover.
	std::string    mDistribProbaName;	//!< Name used in the register for the distribution probability.
	
};
		
}
}

#endif //Beagle_IntVec_CrossoverUPMXOp_hpp
