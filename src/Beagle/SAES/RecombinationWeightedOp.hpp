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
 *  \file   Beagle/SAES/RecombinationWeightedOp.hpp
 *  \brief  Definition of the class SAES::RecombinationWeightedOp.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_SAES_RecombinationWeightedOp_hpp
#define Beagle_SAES_RecombinationWeightedOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{
	
namespace SAES
{

/*!
 *  \class RecombinationWeightedOp Beagle/SAES/RecombinationWeightedOp.hpp
 *    "Beagle/SAES/RecombinationWeightedOp.hpp"
 *  \brief SA-ES recombination by weighted mean operator class.
 *  \ingroup SAESF
 */
class RecombinationWeightedOp : public EC::RecombinationOp
{

public:

	//! SAES::RecombinationWeightedOp allocator type.
	typedef AllocatorT<RecombinationWeightedOp,EC::RecombinationOp::Alloc>
	Alloc;
	//! SAES::RecombinationWeightedOp handle type.
	typedef PointerT<RecombinationWeightedOp,EC::RecombinationOp::Handle>
	Handle;
	//! SAES::RecombinationWeightedOp bag type.
	typedef ContainerT<RecombinationWeightedOp,EC::RecombinationOp::Bag>
	Bag;

	explicit RecombinationWeightedOp(std::string inRecombPbName="saes.recomb.prob",
	                                 std::string inNumberRecombName="saes.recomb.nbind",
	                                 std::string inName="SAES-RecombinationWeightedOp");
	virtual ~RecombinationWeightedOp()
	{ }

	virtual Individual::Handle recombine(Individual::Bag& inIndivPool, Context& ioContext);

};

}
}

#endif // Beagle_SAES_RecombinationWeightedOp_hpp
