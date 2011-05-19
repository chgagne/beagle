/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
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
