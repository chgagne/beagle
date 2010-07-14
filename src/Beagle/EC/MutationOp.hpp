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
 *  \file   Beagle/EC/MutationOp.hpp
 *  \brief  Definition of the class MutationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_EC_MutationOp_hpp
#define Beagle_EC_MutationOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{
	
namespace EC
{

/*!
 *  \class MutationOp Beagle/EC/MutationOp.hpp "Beagle/EC/MutationOp.hpp"
 *  \brief Abstract mutation operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class MutationOp : public Beagle::BreederOp
{

public:

	//! MutationOp allocator type.
	typedef Beagle::AbstractAllocT<MutationOp,Beagle::BreederOp::Alloc>
	Alloc;
	//! MutationOp handle type.
	typedef Beagle::PointerT<MutationOp,Beagle::BreederOp::Handle>
	Handle;
	//! MutationOp bag type.
	typedef Beagle::ContainerT<MutationOp,Beagle::BreederOp::Bag>
	Bag;

	explicit MutationOp(std::string inMutationPbName="ec.mut.prob",
	                    std::string inName="MutationOp");
	virtual ~MutationOp()
	{ }

	/*!
	 *  \brief Mutate an individual.
	 *  \param ioIndividual Individual to mutate.
	 *  \param ioContext Context of the evolution.
	 *  \return True if the individual is effectively mutated, false if not.
	 */
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext) = 0;

	virtual Beagle::Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	                                         Beagle::BreederNode::Handle inChild,
	                                         Beagle::Context& ioContext);
	virtual double             getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void               registerParams(Beagle::System& ioSystem);
	virtual void               operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void               readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
	Beagle::Double::Handle mMutationProba;   //!< Individual mutation probability.
	std::string            mMutationPbName;  //!< Mutation probability parameter name.

};

}

}

#endif // Beagle_EC_MutationOp_hpp
