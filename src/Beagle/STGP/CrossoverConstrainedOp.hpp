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
 *  \file   beagle/GP/CrossoverConstrainedOp.hpp
 *  \brief  Definition of the class GP::CrossoverConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_CrossoverConstrainedOp_hpp
#define Beagle_GP_CrossoverConstrainedOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Tree.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/CrossoverOp.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

namespace Beagle
{
namespace GP
{

/*!
 *  \class CrossoverConstrainedOp
 *      beagle/GP/CrossoverConstrainedOp.hpp "beagle/GP/CrossoverConstrainedOp.hpp"
 *  \brief GP crossover for constrained trees operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class CrossoverConstrainedOp : public Beagle::GP::CrossoverOp
{

public:

	//! GP::CrossoverConstrainedOp allocator type.
	typedef AllocatorT<CrossoverConstrainedOp,Beagle::GP::CrossoverOp::Alloc>
	Alloc;
	//! GP::CrossoverConstrainedOp handle type.
	typedef PointerT<CrossoverConstrainedOp,Beagle::GP::CrossoverOp::Handle>
	Handle;
	//! GP::CrossoverConstrainedOp bag type.
	typedef ContainerT<CrossoverConstrainedOp,Beagle::GP::CrossoverOp::Bag>
	Bag;

	explicit CrossoverConstrainedOp(std::string inMatingPbName="gp.cx.indpb",
	                                std::string inDistribPbName="gp.cx.distrpb",
	                                std::string inName="GP-CrossoverConstrainedOp");
	virtual ~CrossoverConstrainedOp()
	{ }

	virtual bool mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
	                  Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2);

protected:


	unsigned int buildRoulette(RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
	                           bool inSelectABranch,
	                           unsigned int inMaxSubTreeDepth,
	                           unsigned int inMaxSubTreeSize,
	                           unsigned int inActualIndex,
	                           GP::Tree& inTree,
	                           GP::Context& ioContext) const;

	bool selectNodeToMate(unsigned int& outSelectTreeIndex,
	                      unsigned int& outSelectNodeIndex,
	                      bool inSelectABranch,
	                      unsigned int inPrimitSetIndex,
	                      unsigned int inMaxSubTreeDepth,
	                      unsigned int inMaxSubTreeSize,
	                      GP::Individual& inIndividual,
	                      GP::Context& ioContext) const;

#ifdef BEAGLE_HAVE_RTTI
	unsigned int buildRouletteWithType(RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
	                                   bool inSelectABranch,
	                                   const std::type_info* inNodeReturnType,
	                                   unsigned int inMaxSubTreeDepth,
	                                   unsigned int inMaxSubTreeSize,
	                                   unsigned int inActualIndex,
	                                   GP::Tree& inTree,
	                                   GP::Context& ioContext) const;

	bool selectNodeToMateWithType(unsigned int& outSelectTreeIndex,
	                              unsigned int& outSelectNodeIndex,
	                              bool inSelectABranch,
	                              const std::type_info* inNodeReturnType,
	                              unsigned int inPrimitSetIndex,
	                              unsigned int inMaxSubTreeDepth,
	                              unsigned int inMaxSubTreeSize,
	                              GP::Individual& inIndividual,
	                              GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

};

}
}

#endif // Beagle_GP_CrossoverConstrainedOp_hpp
