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
 *  \file   Beagle/EC/SelectRouletteOp.hpp
 *  \brief  Definition of the class SelectRouletteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_SelectRouletteOp_hpp
#define Beagle_EC_SelectRouletteOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectRouletteOp Beagle/EC/SelectRouletteOp.hpp "Beagle/EC/SelectRouletteOp.hpp"
 *  \brief Proportionnal roulette selection operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \warning To use roulette selection, every individuals composing the population must have
 *    non-negative fitness value. Roulette selection is valid only with FitnessSimple object
 *    and derived (FitnessSimpleMin, FitnessKoza).
 */
class SelectRouletteOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectRouletteOp allocator type.
	typedef Beagle::AllocatorT<SelectRouletteOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectRouletteOp handle type.
	typedef Beagle::PointerT<SelectRouletteOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectRouletteOp bag type.
	typedef Beagle::ContainerT<SelectRouletteOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectRouletteOp(std::string inReproProbaName="ec.repro.prob",
	                          std::string inName="SelectRouletteOp");
	virtual ~SelectRouletteOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);
	virtual void         selectManyIndividuals(unsigned int inN,
	        Beagle::Individual::Bag& ioPool,
	        Beagle::Context& ioContext,
	        std::vector<unsigned int>& outSelections);

};

}

}

#endif // Beagle_EC_SelectRouletteOp_hpp
