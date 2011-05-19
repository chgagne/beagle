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
 *  \file   beagle/GA/RecombinationFltVecOp.hpp
 *  \brief  Definition of the class GA::RecombinationFltVecOp.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_RecombinationFltVecOp_hpp
#define Beagle_GA_RecombinationFltVecOp_hpp

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
 *  \class RecombinationFltVecOp beagle/GA/RecombinationFltVecOp.hpp
 *    "beagle/GA/RecombinationFltVecOp.hpp"
 *  \brief Real-valued GA recombination by averaging operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class RecombinationFltVecOp : public RecombinationOp
{

public:

	//! GA::RecombinationFltVecOp allocator type.
	typedef AllocatorT<RecombinationFltVecOp,RecombinationOp::Alloc>
	Alloc;
	//! GA::RecombinationFltVecOp handle type.
	typedef PointerT<RecombinationFltVecOp,RecombinationOp::Handle>
	Handle;
	//! GA::RecombinationFltVecOp bag type.
	typedef ContainerT<RecombinationFltVecOp,RecombinationOp::Bag>
	Bag;

	explicit RecombinationFltVecOp(std::string inRecombPbName="ga.rec.prob",
	                               std::string inNumberRecombName="ga.rec.nbrind",
	                               std::string inName="GA-RecombinationFltVecOp");
	virtual ~RecombinationFltVecOp()
	{ }

	virtual Individual::Handle recombine(Individual::Bag& inIndivPool, Context& ioContext);

};

}
}

#endif // Beagle_GA_RecombinationFltVecOp_hpp
