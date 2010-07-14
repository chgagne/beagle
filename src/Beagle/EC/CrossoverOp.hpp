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
 *  \file   Beagle/EC/CrossoverOp.hpp
 *  \brief  Definition of the class CrossoverOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_EC_CrossoverOp_hpp
#define Beagle_EC_CrossoverOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class CrossoverOp Beagle/EC/CrossoverOp.hpp "Beagle/EC/CrossoverOp.hpp"
 *  \brief Abstract crossover operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class CrossoverOp : public Beagle::BreederOp
{

public:

	//! CrossoverOp allocator type.
	typedef Beagle::AbstractAllocT<CrossoverOp,Beagle::BreederOp::Alloc>
	Alloc;
	//! CrossoverOp handle type.
	typedef Beagle::PointerT<CrossoverOp,Beagle::BreederOp::Handle>
	Handle;
	//! CrossoverOp bag type.
	typedef Beagle::ContainerT<CrossoverOp,Beagle::BreederOp::Bag>
	Bag;

	explicit CrossoverOp(std::string inMatingPbName="ec.cx.prob",
	                     std::string inName="CrossoverOp");
	virtual ~CrossoverOp()
	{ }

	/*!
	 *  \brief Mate two individuals for a crossover.
	 *  \param ioIndiv1   First individual to mate.
	 *  \param ioContext1 Evolutionary context of the first individual.
	 *  \param ioIndiv2   Second individual to mate.
	 *  \param ioContext2 Evolutionary context of the second individual.
	 *  \return True if the individuals are effectively mated, false if not.
	 */
	virtual bool mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
	                  Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2) = 0;

	virtual Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	                                 Beagle::BreederNode::Handle inChild,
	                                 Beagle::Context& ioContext);
	virtual double             getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void               registerParams(Beagle::System& ioSystem);
	virtual void               operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void               readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Beagle::Double::Handle  mMatingProba;      //!< Single individual mating probability
	std::string             mMatingProbaName;  //!< Single individual mating probability parameter name

};

}

}

#endif // Beagle_EC_CrossoverOp_hpp
