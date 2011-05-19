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
 *  \file   Beagle/EC/SelectionOp.hpp
 *  \brief  Definition of the class SelectionOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_SelectionOp_hpp
#define Beagle_EC_SelectionOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectionOp Beagle/EC/SelectionOp.hpp "Beagle/EC/SelectionOp.hpp"
 *  \brief Abstract selection operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectionOp : public Beagle::BreederOp
{

public:

	//! SelectionOp allocator type.
	typedef Beagle::AbstractAllocT<SelectionOp,Beagle::BreederOp::Alloc> Alloc;
	//! SelectionOp handle type.
	typedef Beagle::PointerT<SelectionOp,Beagle::BreederOp::Handle> Handle;
	//! SelectionOp bag type.
	typedef Beagle::ContainerT<SelectionOp,Beagle::BreederOp::Bag> Bag;

	explicit SelectionOp(std::string inReproProbaName="ec.repro.prob",
	                     std::string inName="EC-SelectionOp");
	virtual ~SelectionOp()
	{ }

	/*!
	 *  \brief Select an individual in a pool.
	 *  \param ioPool Pool in which the individual is choosen.
	 *  \param ioContext Evolutionary context.
	 *  \return Index of the choosen individual in the pool.
	 */
	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool,
	        Beagle::Context& ioContext) = 0;

	virtual void selectManyIndividuals(unsigned int inN,
	                                   Beagle::Individual::Bag& ioPool,
	                                   Beagle::Context& ioContext,
	                                   std::vector<unsigned int>& outSelections);

	void convertToList(unsigned int inN, std::vector<unsigned int>& ioSelections);

	virtual Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	                                 Beagle::BreederNode::Handle inChild,
	                                 Beagle::Context& ioContext);
	virtual double             getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void               registerParams(Beagle::System& ioSystem);
	virtual void               operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void               readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Beagle::Double::Handle  mReproductionProba;    //!< Reproduction probability.
	std::string             mReproProbaName;       //!< Reproduction probability name.

};

}

}

#endif // Beagle_EC_SelectionOp_hpp
