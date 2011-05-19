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
 *  \file   Beagle/EC/InvalidateFitnessOp.hpp
 *  \brief  Definition of class InvalidateFitnessOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_InvalidateFitnessOp_hpp
#define Beagle_EC_InvalidateFitnessOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class InvalidateFitnessOp Beagle/EC/InvalidateFitnessOp.hpp "Beagle/EC/InvalidateFitnessOp.hpp"
 *  \brief Invalidate individual's fitness operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class InvalidateFitnessOp : public Beagle::BreederOp
{

public:

	//! InvalidateFitnessOp allocator type.
	typedef Beagle::AllocatorT<InvalidateFitnessOp,Beagle::BreederOp::Alloc> Alloc;
	//! InvalidateFitnessOp handle type.
	typedef Beagle::PointerT<InvalidateFitnessOp,Beagle::BreederOp::Handle> Handle;
	//! InvalidateFitnessOp bag type.
	typedef Beagle::ContainerT<InvalidateFitnessOp,Beagle::BreederOp::Bag> Bag;

	explicit InvalidateFitnessOp(std::string inName="InvalidateFitnessOp");
	virtual ~InvalidateFitnessOp()
	{ }

	virtual Beagle::Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	        Beagle::BreederNode::Handle inChild,
	        Beagle::Context& ioContext);
	virtual double             getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void               operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

};

}

}

#endif // Beagle_EC_InvalidateFitnessOp_hpp
