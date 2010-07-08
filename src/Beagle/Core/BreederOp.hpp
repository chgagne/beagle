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
 *  \file   Beagle/Core/BreederOp.hpp
 *  \brief  Definition of the class BreederOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_BreederOp_hpp
#define Beagle_Core_BreederOp_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/AbstractAllocT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Operator.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/BreederNode.hpp"
#include "Beagle/Core/Double.hpp"


namespace Beagle
{

/*!
 *  \class BreederOp Beagle/Core/BreederOp.hpp "Beagle/Core/BreederOp.hpp"
 *  \brief Breeder operator abstract class.
 *  \ingroup Core
 *  \ingroup Op
 *
 *  A breeder is an operator that can process individuals in an evolution at the individual level.
 *  Breeders are hierachically organized into trees. A breeder receive a breeding pool to process,
 *  and must return from it one bred individual.
 */
class BreederOp : public Operator
{

public:

	//! BreederOp allocator type.
	typedef AbstractAllocT<BreederOp,Operator::Alloc>
	Alloc;
	//! BreederOp handle type.
	typedef PointerT<BreederOp,Operator::Handle>
	Handle;
	//! BreederOp bag type.
	typedef ContainerT<BreederOp,Operator::Bag>
	Bag;

	BreederOp(std::string inName="Unnamed");
	virtual ~BreederOp()
	{ }

	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Apply the breeding operation on a breeding pool, returning a bred individual.
	 *  \param inBreedingPool Breeding pool to use for the breeding operation.
	 *  \param inChild Node handle associated to child node in the breeder tree.
	 *  \param ioContext Evolutionary context of the breeding operation.
	 *  \return Bred individual.
	 */
	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext) = 0;

	/*!
	 *  \return Return selection probability of breeder operator.
	 *  \param inChild Child node in the breeder tree.
	 */
	virtual double getBreedingProba(BreederNode::Handle inChild) = 0;

	/*!
	 *  \brief Apply the operation on a deme in the given context.
	 *  \param ioDeme Reference to the deme on which the operation takes place.
	 *  \param ioContext Evolutionary context of the operation.
	 */
	virtual void operate(Deme& ioDeme, Context& ioContext) = 0;

};

}


#endif // Beagle_Core_BreederOp_hpp

