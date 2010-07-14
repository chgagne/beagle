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
 *  \file   Beagle/EC/FitnessSimpleMin.hpp
 *  \brief  Definition of the class FitnessSimpleMin.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_EC_FitnessSimpleMin_hpp
#define Beagle_EC_FitnessSimpleMin_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/FitnessSimple.hpp"


namespace Beagle
{
	
namespace EC
{

/*!
 *  \class FitnessSimpleMin Beagle/EC/FitnessSimpleMin.hpp "Beagle/EC/FitnessSimpleMin.hpp"
 *  \brief Simple fitness class for minimization problems.
 *  \ingroup EC
 *  \ingroup FitStats
 */
class FitnessSimpleMin : public Beagle::EC::FitnessSimple
{

public:

	//! FitnessSimpleMin allocator type.
	typedef Beagle::AllocatorT<FitnessSimpleMin,Beagle::EC::FitnessSimple::Alloc> Alloc;
	//! FitnessSimpleMin handle type.
	typedef Beagle::PointerT<FitnessSimpleMin,Beagle::EC::FitnessSimple::Handle> Handle;
	//! FitnessSimpleMin bag type.
	typedef Beagle::ContainerT<FitnessSimpleMin,Beagle::EC::FitnessSimple::Bag> Bag;

	FitnessSimpleMin() { }
	explicit FitnessSimpleMin(double inFitness);
	virtual ~FitnessSimpleMin()
	{ }

	virtual void                copy(const Beagle::Member& inOriginal, Beagle::System& ioSystem);
	virtual const std::string&  getType() const;
	virtual bool                isDominated(const Beagle::Fitness& inRightFitness) const;
	virtual bool                isLess(const Beagle::Object& inRightObj) const;
	virtual void                setValue(double inFitness);

};

}

}

#endif // Beagle_EC_FitnessSimpleMin_hpp

