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
 *  \file   beagle/GA/CrossoverTwoPointsIntVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverTwoPointsIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverTwoPointsIntVecOp_hpp
#define Beagle_GA_CrossoverTwoPointsIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverTwoPointsOpT.hpp"
#include "beagle/GA/IntegerVector.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverTwoPointsIntVecOp beagle/GA/CrossoverTwoPointsIntVecOp.hpp
 *    "beagle/GA/CrossoverTwoPointsIntVecOp.hpp"
 *  \brief Integer-valued GA two points crossover operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class CrossoverTwoPointsIntVecOp : public GA::CrossoverTwoPointsOpT<GA::IntegerVector>
{

public:

	//! GA::CrossoverTwoPointsIntVecOp allocator type.
	typedef AllocatorT<CrossoverTwoPointsIntVecOp,GA::CrossoverTwoPointsOpT<GA::IntegerVector>::Alloc>
	Alloc;
	//! GA::CrossoverTwoPointsIntVecOp handle type.
	typedef PointerT<CrossoverTwoPointsIntVecOp,GA::CrossoverTwoPointsOpT<GA::IntegerVector>::Handle>
	Handle;
	//! GA::CrossoverTwoPointsIntVecOp bag type.
	typedef ContainerT<CrossoverTwoPointsIntVecOp,GA::CrossoverTwoPointsOpT<GA::IntegerVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct integer vector GA two points crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the integer vector GA two points crossover operator.
	 */
	explicit CrossoverTwoPointsIntVecOp(std::string inMatingPbName="ga.cx2p.prob",
	                                    std::string inName="GA-CrossoverTwoPointsIntVecOp") :
			GA::CrossoverTwoPointsOpT<GA::IntegerVector>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverTwoPointsIntVecOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverTwoPointsIntVecOp_hpp
