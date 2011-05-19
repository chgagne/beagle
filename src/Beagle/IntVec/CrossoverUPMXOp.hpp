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
 *  \file   beagle/GA/CrossoverUPMXOp.hpp
 *  \brief  Definition of the class GA::CrossoverUPMXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_GA_CrossoverUPMXOp_hpp
#define Beagle_GA_CrossoverUPMXOp_hpp

#include <string>

#include <beagle/config.hpp>
#include <beagle/macros.hpp>
#include <beagle/Object.hpp>
#include <beagle/CrossoverOp.hpp>

namespace Beagle {
namespace GA {

/*!
 *  \class CrossoverUPMXOp beagle/GA/CrossoverUPMXOp.hpp
 *    "beagle/GA/CrossoverUPMXOp.hpp"
 *  \brief 
 *  \ingroup GAIV
 *	\warning This crossover operation may give unexpected results when not applied
 *	  on an indices permutation genotype.
 *
 *  This crossover operation consists in generating two children by matching pairs
 *	of values at the same index uniformly of the two parents and swaping the values
 *	of those indexes. It has been introduced by Cicirello and Smith in "Modeling GA
 *	performance for control parameter optimization", 2000.
 *
 */
class CrossoverUPMXOp : public CrossoverOp {
public:
	//! GA::CrossoverUPMXOp allocator type.
	typedef AllocatorT<CrossoverUPMXOp,CrossoverOp::Alloc> Alloc;
	//! GA::CrossoverUPMXOp handle type.
	typedef PointerT<CrossoverUPMXOp,CrossoverOp::Handle> Handle;
	//! GA::CrossoverUPMXOp bag type.
	typedef ContainerT<CrossoverUPMXOp,CrossoverOp::Bag> Bag;
	
	explicit CrossoverUPMXOp(std::string inMatingPbName="ga.cxupm.prob",
							 std::string inDistribPbName="ga.cxupm.distribprob",
							 std::string inName="GA-CrossoverUPMXOp");
	virtual ~CrossoverUPMXOp()
	{ }
	
	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
					  Individual& ioIndiv2, Context& ioContext2);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	
protected:
	Float::Handle mDistribProba;		//!< Distribution probability of the uniform partialy matched crossover.
	std::string   mDistribProbaName;	//!< Name used in the register for the distribution probability.
	
};
		
}
}

#endif //Beagle_GA_CrossoverUPMXOp_hpp
