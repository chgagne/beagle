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

#ifndef Beagle_Core_RandomizerMulti_hpp
#define Beagle_Core_RandomizerMulti_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ULong.hpp"
#include "Beagle/Core/Randomizer.hpp"

/*!
 *  \file   Beagle/Core/RandomizerMulti.hpp
 *  \brief  Definition of the class RandomizerMulti.
 *	\author Felix-Antoine Fortin
 *	\author Francois-Michel De Rainville
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

namespace Beagle
{

/*!
 *  \class Randomizer Beagle/Core/RandomizerMulti.hpp "Beagle/Core/RandomizerMulti.hpp"
 *  \brief Multiple random number generator class.
 *  \ingroup Core
 *  \ingroup Sys
 */
class RandomizerMulti : public Beagle::Component
{
public:
	//! RandomizerMulti allocator type.
	typedef AllocatorT<RandomizerMulti, Component::Alloc>
	Alloc;
	//! RandomizerMulti handle type.
	typedef PointerT<RandomizerMulti, Component::Handle>
	Handle;
	//! RandomizerMulti bag type.
	typedef ContainerT<RandomizerMulti, Component::Bag>
	Bag;

	RandomizerMulti(Randomizer::Bag::Handle inRandomizers = 0);
	virtual ~RandomizerMulti() {}

	/*!
	 *  \brief Return a handle to the Randomizer associated to the index \c inIndex.
	 *  \param inIndex Index of the Randomizer.
	 */
	inline Randomizer::Handle operator[](unsigned int inIndex) {
		return (*mRandomizers)[inIndex];
	}

	/*!
	 *  \brief Return a const handle to the Randomizer associated to the index \c inIndex.
	 *  \param inIndex Index of the Randomizer.
	 */
	inline const Randomizer::Handle operator[](unsigned int inIndex) const {
		return (*mRandomizers)[inIndex];
	}

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
	ULongArray::Handle mRegisteredSeed;  	//!< Registered seeds of the random number generators.
	Randomizer::Bag::Handle mRandomizers;   //!< Bag of random number generators, one per thread.
};

}

#endif // Beagle_Core_RandomizerMulti
