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
 *  \file   Beagle/EC/RecombinationOp.hpp
 *  \brief  Definition of the class RecombinationOp.
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_RecombinationOp_hpp
#define Beagle_EC_RecombinationOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class RecombinationOp Beagle/EC/RecombinationOp.hpp "Beagle/EC/RecombinationOp.hpp"
 *  \brief Abstract recombination operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class RecombinationOp : public Beagle::BreederOp
{

public:

	//! RecombinationOp allocator type.
	typedef Beagle::AbstractAllocT<RecombinationOp,Beagle::BreederOp::Alloc> Alloc;
	//! RecombinationOp handle type.
	typedef Beagle::PointerT<RecombinationOp,Beagle::BreederOp::Handle> Handle;
	//! RecombinationOp bag type.
	typedef Beagle::ContainerT<RecombinationOp,Beagle::BreederOp::Bag> Bag;

	explicit RecombinationOp(std::string inRecombProbaName="ec.recomb.prob",
	                         std::string inNumberRecombName="ec.recomb.nbrind",
	                         std::string inName="RecombinationOp");
	virtual ~RecombinationOp()
	{ }

	/*!
	 *  \brief Recombine several individuals together.
	 *  \param inIndivPool Pool of individuals to recombine.
	 *  \param ioContext Evolutionary context.
	 *  \return Individual resulting of recombination.
	 */
	virtual Beagle::Individual::Handle recombine(Beagle::Individual::Bag& inIndivPool, Beagle::Context& ioContext) =0;

	virtual Beagle::Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	        Beagle::BreederNode::Handle inChild,
	        Beagle::Context& ioContext);
	virtual double             getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void               registerParams(Beagle::System& ioSystem);
	virtual void               operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void               readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Beagle::Double::Handle mRecombProba;       //!< Single individual recombination probability.
	std::string            mRecombProbaName;   //!< Single individual recombination probability parameter name.
	Beagle::UInt::Handle   mNumberRecomb;      //!< Number of individuals recombined.
	std::string            mNumberRecombName;  //!< Number of individuals recombined parameter name.

};

}

}

#endif // Beagle_EC_RecombinationOp_hpp
