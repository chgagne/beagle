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
 *  \file   Beagle/EC/AlgoSequential.hpp
 *  \brief  Definition of the class AlgoSequential.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_EC_AlgoSequential_hpp
#define Beagle_EC_AlgoSequential_hpp

#include <map>
#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class AlgoSequential Beagle/EC/AlgoSequential.hpp "Beagle/EC/AlgoSequential.hpp"
 *  \brief Basic algorithm class, list operations in sequences (implicit generational).
 *  \ingroup EC
 */
class AlgoSequential : public Beagle::Algorithm
{

public:

	//! AlgoSequential allocator type.
	typedef Beagle::AllocatorT< AlgoSequential, Beagle::Algorithm::Alloc > Alloc;
	//! AlgoSequential handle type.
	typedef Beagle::PointerT< AlgoSequential, Beagle::Algorithm::Handle > Handle;
	//! AlgoSequential bag type.
	typedef Beagle::ContainerT< AlgoSequential, Beagle::Algorithm::Bag > Bag;

	AlgoSequential(void);
	virtual ~AlgoSequential()
	{ }

	virtual void configure(Beagle::Evolver& ioEvolver, Beagle::System& ioSystem);

};

}

}

#endif // Beagle_EC_AlgoSequential_hpp
