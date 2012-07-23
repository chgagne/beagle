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
 *  \file   Beagle/IntVec/CrossoverIndicesOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverIndicesOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_CrossoverIndicesOp_hpp
#define Beagle_IntVec_CrossoverIndicesOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{
	
namespace IntVec
{

/*!
 *  \class CrossoverIndicesOp Beagle/IntVec/CrossoverIndicesOp.hpp
 *    "Beagle/IntVec/CrossoverIndicesOp.hpp"
 *  \brief Indices integer vector crossover operator class.
 *  \ingroup IntVecF
 */
class CrossoverIndicesOp : public EC::CrossoverOp
{

public:

	//! IntVec::CrossoverIndicesOp allocator type.
	typedef AllocatorT<CrossoverIndicesOp,EC::CrossoverOp::Alloc>
	Alloc;
	//! IntVec::CrossoverIndicesOp handle type.
	typedef PointerT<CrossoverIndicesOp,EC::CrossoverOp::Handle>
	Handle;
	//! IntVec::CrossoverIndicesOp bag type.
	typedef ContainerT<CrossoverIndicesOp,EC::CrossoverOp::Bag>
	Bag;

	explicit CrossoverIndicesOp(std::string inMatingPbName="intvec.cxind.prob",
	                            std::string inName="IntVec-CrossoverIndicesOp");
	virtual ~CrossoverIndicesOp()
	{ }

	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);
	virtual void registerParams(System& ioSystem);
	
};

}
}

#endif // Beagle_IntVec_CrossoverIndicesOp_hpp
