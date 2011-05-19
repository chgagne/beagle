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
 *  \file   Beagle/Core/Randomizer.hpp
 *  \brief  Definition of the class Randomizer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_Randomizer_hpp
#define Beagle_Core_Randomizer_hpp

#include "PACC/Util.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ULong.hpp"

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif //BEAGLE_HAVE_OPENMP

namespace Beagle
{

// Forward declaration
class System;

/*!
 *  \class Randomizer Beagle/Randomizer.hpp "Beagle/Randomizer.hpp"
 *  \brief Random number generator class.
 *  \note The operator() allow compliance with the STL random number generator interface.
 *  \ingroup Core
 *  \ingroup Sys
 */
class Randomizer : public Component, public PACC::Randomizer
{

public:

	//! Randomizer allocator type.
	typedef AllocatorT<Randomizer,Component::Alloc>
	Alloc;
	//! Randomizer handle type.
	typedef PointerT<Randomizer,Component::Handle>
	Handle;
	//! Randomizer bag type.
	typedef ContainerT<Randomizer,Component::Bag>
	Bag;

	Randomizer(unsigned long inSeed=0);
	virtual ~Randomizer()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Generate a floating-point number following a Gaussian distribution.
	 *  \param inMean Mean of the Gaussain distribution.
	 *  \param inStdDev Standard-error of the Gaussian distribution.
	 */
	inline double rollGaussian(double inMean=0.0, double inStdDev=1.0) {
		Beagle_StackTraceBeginM();
		Beagle_AssertM(inStdDev >= 0.0);
		return randNorm(inMean, inStdDev);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Generate an positive integer following an uniform discrete distribution.
	 *  \param inLower Lower bound of the distribution.
	 *  \param inUpper Upper bound of the distribution.
	 *  \return Random positive integer generated in [inLower, inUpper].
	 */
	inline unsigned long rollInteger(unsigned long inLower=0, unsigned long inUpper=ULONG_MAX) {
		Beagle_StackTraceBeginM();
		Beagle_AssertM(inLower <= inUpper);
		return randInt(inUpper-inLower)+inLower;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Generate a number following a uniform distribution in the interval [inLower,inUpper).
	 *  \param inLower Lower bound of the uniform distribution.
	 *  \param inUpper Upper bound of the uniform distribution.
	 *  \return Random number generated in [inLower,inUpper).
	 */
	inline double rollUniform(double inLower=0.0, double inUpper=1.0) {
		Beagle_StackTraceBeginM();
		Beagle_AssertM(inLower <= inUpper);
		return (randExc() * (inUpper-inLower)) + inLower;
		Beagle_StackTraceEndM();
	}

protected:

#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
	ULongArray::Handle mRegisteredSeed;
#else
	ULong::Handle mRegisteredSeed;  //!< Registered seed of the random number generator.
#endif
	unsigned long mSeed;            //!< Seed used to initialize the random number generator.
};

}

#endif // Beagle_Core_Randomizer_hpp
