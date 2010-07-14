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
 *  \file   Beagle/EC/FitnessSimple.hpp
 *  \brief  Definition of the class Beagle::EC::FitnessSimple.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_EC_FitnessSimple_hpp
#define Beagle_EC_FitnessSimple_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class FitnessSimple Beagle/EC/FitnessSimple.hpp "Beagle/EC/FitnessSimple.hpp"
 *  \brief Simple fitness class, made of a unique measure.
 *  \ingroup EC
 *  \ingroup FitStats
 */
class FitnessSimple : public Beagle::Fitness
{

public:

	//! FitnessSimple allocator type.
	typedef Beagle::AllocatorT<FitnessSimple,Beagle::Fitness::Alloc> Alloc;
	//! FitnessSimple handle type.
	typedef Beagle::PointerT<FitnessSimple,Beagle::Fitness::Handle> Handle;
	//! FitnessSimple bag type.
	typedef Beagle::ContainerT<FitnessSimple,Beagle::Fitness::Bag> Bag;

	FitnessSimple();
	explicit FitnessSimple(double inValue);
	virtual ~FitnessSimple()
	{ }

	virtual void                copy(const Beagle::Member& inOriginal, Beagle::System& ioSystem);
	virtual double              getDistance(const Beagle::Fitness& inRightFitness) const;
	virtual unsigned int        getNumberOfObjectives() const;
	virtual double              getObjective(unsigned int inObjIndex) const;
	virtual const std::string&  getType() const;
	virtual bool                isDominated(const Beagle::Fitness& inRightFitness) const;
	virtual bool                isEqual(const Beagle::Object& inRightObj) const;
	virtual bool                isLess(const Beagle::Object& inRightObj) const;
	virtual void                read(PACC::XML::ConstIterator inIter);
	virtual void                setValue(double inValue);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return fitness value.
	 *  \return Fitness value.
	 */
	inline double getValue() const
	{
		Beagle_StackTraceBeginM();
		return mValue;
		Beagle_StackTraceEndM("double FitnessSimple::getValue() const");
	}

protected:

	double mValue;  //!< Unique measure of the fitness.

};

}

}

#endif // Beagle_EC_FitnessSimple_hpp

