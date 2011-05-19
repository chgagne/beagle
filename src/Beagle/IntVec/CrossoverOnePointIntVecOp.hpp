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
 *  \file   beagle/GA/CrossoverOnePointIntVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverOnePointIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverOnePointIntVecOp_hpp
#define Beagle_GA_CrossoverOnePointIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverOnePointOpT.hpp"
#include "beagle/GA/IntegerVector.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverOnePointIntVecOp beagle/GA/CrossoverOnePointIntVecOp.hpp
 *    "beagle/GA/CrossoverOnePointIntVecOp.hpp"
 *  \brief Integer-valued GA one point crossover operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class CrossoverOnePointIntVecOp : public GA::CrossoverOnePointOpT<GA::IntegerVector>
{

public:

	//! GA::CrossoverOnePointIntVecOp allocator type.
	typedef AllocatorT<CrossoverOnePointIntVecOp,GA::CrossoverOnePointOpT<GA::IntegerVector>::Alloc>
	Alloc;
	//! GA::CrossoverOnePointIntVecOp handle type.
	typedef PointerT<CrossoverOnePointIntVecOp,GA::CrossoverOnePointOpT<GA::IntegerVector>::Handle>
	Handle;
	//! GA::CrossoverOnePointIntVecOp bag type.
	typedef ContainerT<CrossoverOnePointIntVecOp,GA::CrossoverOnePointOpT<GA::IntegerVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct integer vector GA one point crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the integer vector GA one point crossover operator.
	 */
	explicit CrossoverOnePointIntVecOp(std::string inMatingPbName="ga.cx1p.prob",
	                                   std::string inName="GA-CrossoverOnePointIntVecOp") :
			GA::CrossoverOnePointOpT<GA::IntegerVector>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverOnePointIntVecOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverOnePointIntVecOp_hpp
