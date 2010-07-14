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
 *  \file   Beagle/EC/InitializationOp.hpp
 *  \brief  Definition of the class InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_EC_InitializationOp_hpp
#define Beagle_EC_InitializationOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class InitializationOp Beagle/EC/InitializationOp.hpp "Beagle/EC/InitializationOp.hpp"
 *  \brief Abstract initialization operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup Pop
 */
class InitializationOp : public Beagle::BreederOp
{

public:

	//! InitializationOp allocator type.
	typedef Beagle::AbstractAllocT<InitializationOp,Beagle::BreederOp::Alloc>
	Alloc;
	//! InitializationOp handle type.
	typedef Beagle::PointerT<InitializationOp,Beagle::BreederOp::Handle>
	Handle;
	//! InitializationOp  bag type.
	typedef Beagle::ContainerT<InitializationOp,Beagle::BreederOp::Bag>
	Bag;

	explicit InitializationOp(std::string inReproProbaName="ec.repro.prob",
	                          std::string inName="InitializationOp");
	virtual ~InitializationOp()
	{ }

	/*!
	 *  \brief Initialize an individual.
	 *  \param outIndividual Individual to initialize.
	 *  \param ioContext Evolution context.
	 */
	virtual void initIndividual(Beagle::Individual& outIndividual, Beagle::Context& ioContext) = 0;

	virtual Beagle::Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	        Beagle::BreederNode::Handle inChild,
	        Beagle::Context& ioContext);
	virtual double       getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void         registerParams(Beagle::System& ioSystem);
	virtual void         operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual unsigned int readSeeds(std::string inFileName, Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void         readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void         writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Beagle::Double::Handle    mReproductionProba;    //!< Reproduction probability.
	std::string               mReproProbaName;       //!< Reproduction probability name.
	Beagle::UIntArray::Handle mPopSize;              //!< Population size for the evolution.
	Beagle::String::Handle    mSeedsFile;            //!< File to use to seed population.

};

}

}

#endif // Beagle_EC_InitializationOp_hpp
