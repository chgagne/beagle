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
 *  \file   Beagle/EC/SelectWorstOp.hpp
 *  \brief  Definition of the class SelectWorstOp.
 *  \author Christian Gagne
 *  $Revision $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectWorstOp_hpp
#define Beagle_EC_SelectWorstOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectWorstOp Beagle/EC/SelectWorstOp.hpp "Beagle/EC/SelectWorstOp.hpp"
 *  \brief Select worst individual(s) of the population.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectWorstOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectWorstOp allocator type.
	typedef Beagle::AllocatorT<SelectWorstOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectWorstOp handle type.
	typedef Beagle::PointerT<SelectWorstOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectWorstOp bag type.
	typedef Beagle::ContainerT<SelectWorstOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectWorstOp(std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="SelectWorstOp");
	virtual ~SelectWorstOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);
	virtual void         selectManyIndividuals(unsigned int inN,
	        Beagle::Individual::Bag& ioPool,
	        Beagle::Context& ioContext,
	        std::vector<unsigned int>& outSelections);

};

}

}

#endif // Beagle_EC_SelectWorstOp_hpp
