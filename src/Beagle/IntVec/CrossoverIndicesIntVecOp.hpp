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
 *  \file   beagle/GA/CrossoverIndicesIntVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverIndicesIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverIndicesIntVecOp_hpp
#define Beagle_GA_CrossoverIndicesIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverIndicesIntVecOp beagle/GA/CrossoverIndicesIntVecOp.hpp
 *    "beagle/GA/CrossoverIndicesIntVecOp.hpp"
 *  \brief Indices integer vector GA crossover operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class CrossoverIndicesIntVecOp : public CrossoverOp
{

public:

	//! GA::CrossoverIndicesIntVecOp allocator type.
	typedef AllocatorT<CrossoverIndicesIntVecOp,CrossoverOp::Alloc>
	Alloc;
	//! GA::CrossoverIndicesIntVecOp handle type.
	typedef PointerT<CrossoverIndicesIntVecOp,CrossoverOp::Handle>
	Handle;
	//! GA::CrossoverIndicesIntVecOp bag type.
	typedef ContainerT<CrossoverIndicesIntVecOp,CrossoverOp::Bag>
	Bag;

	explicit CrossoverIndicesIntVecOp(std::string inMatingPbName="ga.cxind.prob",
	                                  std::string inName="GA-CrossoverIndicesIntVecOp");
	virtual ~CrossoverIndicesIntVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);

};

}
}

#endif // Beagle_GA_CrossoverIndicesIntVecOp_hpp
