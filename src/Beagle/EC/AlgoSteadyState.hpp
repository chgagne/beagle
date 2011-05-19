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
 *  \file   Beagle/EC/AlgoSteadyState.hpp
 *  \brief  Definition of the class AlgoSteadyState.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_EC_AlgoSteadyState_hpp
#define Beagle_EC_AlgoSteadyState_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{


/*!
 *  \class AlgoSteadyState Beagle/EC/AlgoSteadyState.hpp "Beagle/EC/AlgoSteadyState.hpp"
 *  \brief Steady-state algorithm class, rely on SteadyStateOp replacement strategy.
 *  \ingroup EC
 */
class AlgoSteadyState : public Beagle::Algorithm
{

public:

	//! AlgoSteadyState allocator type.
	typedef Beagle::AllocatorT< AlgoSteadyState, Beagle::Algorithm::Alloc > Alloc;
	//! AlgoSteadyState handle type.
	typedef Beagle::PointerT< AlgoSteadyState, Beagle::Algorithm::Handle > Handle;
	//! AlgoSteadyState bag type.
	typedef Beagle::ContainerT< AlgoSteadyState, Beagle::Algorithm::Bag > Bag;

	AlgoSteadyState();
	virtual ~AlgoSteadyState()
	{ }

	virtual void configure(Beagle::Evolver& ioEvolver, Beagle::System& ioSystem);

};

}

}

#endif // Beagle_EC_AlgoSteadyState_hpp
