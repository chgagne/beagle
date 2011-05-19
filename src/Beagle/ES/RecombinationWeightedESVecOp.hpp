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
 *  \file   beagle/GA/RecombinationWeightedESVecOp.hpp
 *  \brief  Definition of the class GA::RecombinationWeightedESVecOp.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_RecombinationWeightedESVecOp_hpp
#define Beagle_GA_RecombinationWeightedESVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/RecombinationOp.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class RecombinationWeightedESVecOp beagle/GA/RecombinationWeightedESVecOp.hpp
 *    "beagle/GA/RecombinationWeightedESVecOp.hpp"
 *  \brief SA-ES recombination by weighted mean operator class.
 *  \ingroup GAF
 *  \ingroup GAES
 */
class RecombinationWeightedESVecOp : public RecombinationOp
{

public:

	//! GA::RecombinationWeightedESVecOp allocator type.
	typedef AllocatorT<RecombinationWeightedESVecOp,RecombinationOp::Alloc>
	Alloc;
	//! GA::RecombinationWeightedESVecOp handle type.
	typedef PointerT<RecombinationWeightedESVecOp,RecombinationOp::Handle>
	Handle;
	//! GA::RecombinationWeightedESVecOp bag type.
	typedef ContainerT<RecombinationWeightedESVecOp,RecombinationOp::Bag>
	Bag;

	explicit RecombinationWeightedESVecOp(std::string inRecombPbName="es.rec.prob",
	                                      std::string inNumberRecombName="es.rec.nbrind",
	                                      std::string inName="GA-RecombinationWeightedESVecOp");
	virtual ~RecombinationWeightedESVecOp()
	{ }

	virtual Individual::Handle recombine(Individual::Bag& inIndivPool, Context& ioContext);

};

}
}

#endif // Beagle_GA_RecombinationWeightedESVecOp_hpp
