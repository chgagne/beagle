/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2009 by Christian Gagne and Marc Parizeau
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

#ifndef Beagle_Core_RandomizerMulti_hpp
#define Beagle_Core_RandomizerMulti_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Component.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/ULong.hpp"
#include "beagle/Core/Randomizer.hpp"

/*!
 *  \file   beagle/Core/RandomizerMulti.hpp
 *  \brief  Definition of the class RandomizerMulti.
 *	\author Felix-Antoine Fortin
 *	\author Francois-Michel De Rainville
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

namespace Beagle {

/*!
 *  \class Randomizer beagle/Core/RandomizerMulti.hpp "beagle/Core/RandomizerMulti.hpp"
 *  \brief Multiple random number generator class.
 *  \ingroup ECF
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
	inline Randomizer::Handle operator[](unsigned int inIndex)
	{
		return (*mRandomizers)[inIndex];
	}
	
	/*!
	 *  \brief Return a const handle to the Randomizer associated to the index \c inIndex.
	 *  \param inIndex Index of the Randomizer.
	 */
	inline const Randomizer::Handle operator[](unsigned int inIndex) const
	{
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
