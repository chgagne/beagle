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
 *  \file   Beagle/IntVec/CrossoverTwoPointsOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverTwoPointsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_CrossoverTwoPointsOp_hpp
#define Beagle_IntVec_CrossoverTwoPointsOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{

namespace IntVec
{


/*!
 *  \class CrossoverTwoPointsOp Beagle/IntVec/CrossoverTwoPointsOp.hpp
 *    "Beagle/IntVec/CrossoverTwoPointsOp.hpp"
 *  \brief Integer-valued two points crossover operator class.
 *  \ingroup IntVecF
 */
class CrossoverTwoPointsOp : public EC::CrossoverTwoPointsOpT<IntegerVector>
{

public:

	//! IntVec::CrossoverTwoPointsOp allocator type.
	typedef AllocatorT<CrossoverTwoPointsOp,EC::CrossoverTwoPointsOpT<IntegerVector>::Alloc>
	Alloc;
	//! IntVec::CrossoverTwoPointsOp handle type.
	typedef PointerT<CrossoverTwoPointsOp,EC::CrossoverTwoPointsOpT<IntegerVector>::Handle>
	Handle;
	//! IntVec::CrossoverTwoPointsOp bag type.
	typedef ContainerT<CrossoverTwoPointsOp,EC::CrossoverTwoPointsOpT<IntegerVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct integer vector IntVec two points crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the integer vector IntVec two points crossover operator.
	 */
	explicit CrossoverTwoPointsOp(std::string inMatingPbName="intvec.cx2p.prob",
	                              std::string inName="IntVec-CrossoverTwoPointsOp") :
			EC::CrossoverTwoPointsOpT<IntegerVector>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverTwoPointsOp()
	{ }

};

}
}

#endif // Beagle_IntVec_CrossoverTwoPointsOp_hpp
