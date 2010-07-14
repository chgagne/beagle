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
 *  \file   Beagle/EC/SelectFirstOp.hpp
 *  \brief  Definition of the class SelectFirstOp.
 *  \author Christian Gagne
 *  $Revision $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectFirstOp_hpp
#define Beagle_EC_SelectFirstOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectFirstOp Beagle/EC/SelectFirstOp.hpp "Beagle/EC/SelectFirstOp.hpp"
 *  \brief Select first individual(s) in a population using given order.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectFirstOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectFirstOp allocator type.
	typedef Beagle::AllocatorT<SelectFirstOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectFirstOp handle type.
	typedef Beagle::PointerT<SelectFirstOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectFirstOp bag type.
	typedef Beagle::ContainerT<SelectFirstOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectFirstOp(std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="SelectFirstOp");
	virtual ~SelectFirstOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);
	virtual void         selectManyIndividuals(unsigned int inN,
	        Beagle::Individual::Bag& ioPool,
	        Beagle::Context& ioContext,
	        std::vector<unsigned int>& outSelections);
};

}

}

#endif // Beagle_EC_SelectFirstOp_hpp
