/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
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
