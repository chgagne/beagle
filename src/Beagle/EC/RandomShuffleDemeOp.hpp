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
 *  \file   Beagle/EC/RandomShuffleDemeOp.hpp
 *  \brief  Definition of the class RandomShuffleDemeOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_RandomShuffleDemeOp_hpp
#define Beagle_EC_RandomShuffleDemeOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class RandomShuffleDemeOp Beagle/EC/RandomShuffleDemeOp.hpp "Beagle/EC/RandomShuffleDemeOp.hpp"
 *  \brief Random shuffle a deme operator class.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  Random shuffle order of individuals in the actual deme.
 */
class RandomShuffleDemeOp : public Beagle::Operator
{

public:

	//! RandomShuffleDemeOp allocator type
	typedef Beagle::AllocatorT<RandomShuffleDemeOp,Beagle::Operator::Alloc> Alloc;
	//! RandomShuffleDemeOp handle type.
	typedef Beagle::PointerT<RandomShuffleDemeOp,Beagle::Operator::Handle> Handle;
	//! RandomShuffleDemeOp bag type.
	typedef Beagle::ContainerT<RandomShuffleDemeOp,Beagle::Operator::Bag> Bag;

	explicit RandomShuffleDemeOp(std::string inName="RandomShuffleDemeOp");
	virtual ~RandomShuffleDemeOp()
	{ }

	virtual void operate(Deme& ioDeme, Context& ioContext);

};

}

}

#endif // Beagle_EC_RandomShuffleDemeOp_hpp
