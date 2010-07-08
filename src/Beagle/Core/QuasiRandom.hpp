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
 *  \file   Beagle/Core/QuasiRandom.hpp
 *  \brief  Definition of the class QuasiRandom.
 *  \author Christian Gagne
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_QuasiRandom_hpp
#define Beagle_Core_QuasiRandom_hpp

#include "PACC/Util.hpp"
#include "PACC/Math.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ULong.hpp"


namespace Beagle
{

/*!
 *  \class QuasiRandom Beagle/Core/QuasiRandom.hpp "Beagle/Core/QuasiRandom.hpp"
 *  \brief Generator of low-discrepancy sequences.
 *  \ingroup Core
 *  \ingroup Sys
 */
class QuasiRandom : public Component, public PACC::QRandSequencer
{

public:

	//! QuasiRandom allocator type.
	typedef AllocatorT<QuasiRandom,Component::Alloc> Alloc;
	//! QuasiRandom handle type.
	typedef PointerT<QuasiRandom,Component::Handle> Handle;
	//! QuasiRandom bag type.
	typedef ContainerT<QuasiRandom,Component::Bag> Bag;

	QuasiRandom(unsigned int inDimensionality=0, PACC::Randomizer& inRand=PACC::rand);
	virtual ~QuasiRandom()
	{ }

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_QuasiRandom_hpp
