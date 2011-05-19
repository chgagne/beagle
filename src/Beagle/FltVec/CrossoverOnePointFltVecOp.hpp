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
 *  \file   beagle/GA/CrossoverOnePointFltVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverOnePointFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverOnePointFltVecOp_hpp
#define Beagle_GA_CrossoverOnePointFltVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverOnePointOpT.hpp"
#include "beagle/GA/FloatVector.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverOnePointFltVecOp beagle/GA/CrossoverOnePointFltVecOp.hpp
 *    "beagle/GA/CrossoverOnePointFltVecOp.hpp"
 *  \brief Real-valued GA one point crossover operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class CrossoverOnePointFltVecOp : public GA::CrossoverOnePointOpT<GA::FloatVector>
{

public:

	//! GA::CrossoverOnePointFltVecOp allocator type.
	typedef AllocatorT<CrossoverOnePointFltVecOp,GA::CrossoverOnePointOpT<GA::FloatVector>::Alloc>
	Alloc;
	//! GA::CrossoverOnePointFltVecOp handle type.
	typedef PointerT<CrossoverOnePointFltVecOp,GA::CrossoverOnePointOpT<GA::FloatVector>::Handle>
	Handle;
	//! GA::CrossoverOnePointFltVecOp bag type.
	typedef ContainerT<CrossoverOnePointFltVecOp,GA::CrossoverOnePointOpT<GA::FloatVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct float vector GA one point crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the float vector GA one point crossover operator.
	 */
	explicit CrossoverOnePointFltVecOp(std::string inMatingPbName="ga.cx1p.prob",
	                                   std::string inName="GA-CrossoverOnePointFltVecOp") :
			GA::CrossoverOnePointOpT<GA::FloatVector>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverOnePointFltVecOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverOnePointFltVecOp_hpp
