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
 *  \file   Beagle/Core/IndividualBag.hpp
 *  \brief  Definition of the class IndividualBag.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.11 $
 *  $Date: 2008/02/29 14:12:43 $
 */


#ifndef Beagle_Core_IndividualBag_hpp
#define Beagle_Core_IndividualBag_hpp

#include <string>
#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Individual.hpp"

namespace Beagle
{


/*!
 *  \class IndividualBag Beagle/Core/IndividualBag.hpp "Beagle/Core/IndividualBag.hpp"
 *  \brief Container of individuals.
 *  \ingroup Core
 *  \ingroup Pop
 */
class IndividualBag : public ContainerT<Individual,ContainerT<Genotype,Member::Bag>::Bag>
{

public:

	//! IndividualBag's allocator type.
	typedef AllocatorT<IndividualBag,ContainerT<Individual,ContainerT<Genotype,Member::Bag>::Bag>::Alloc> Alloc;
	//! IndividualBag's handle type.
	typedef PointerT<IndividualBag,ContainerT<Individual,ContainerT<Genotype,Member::Bag>::Bag>::Handle> Handle;
	//! IndividualBag's bag type.
	typedef ContainerT<IndividualBag,ContainerT<Individual,ContainerT<Genotype,Member::Bag>::Bag>::Bag> Bag;

	explicit IndividualBag(unsigned int inN=0, Individual::Handle inModel=NULL);
	virtual ~IndividualBag()
	{ }

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual bool readFromFile(std::string inFileName, System& ioSystem, std::string inBagTag="Deme");
	virtual void readIndividuals(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_IndividualBag_hpp
