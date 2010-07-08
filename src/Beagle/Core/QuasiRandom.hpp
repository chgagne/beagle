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
 *  \file   beagle/Core/QuasiRandom.hpp
 *  \brief  Definition of the class QuasiRandom.
 *  \author Christian Gagne
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_QuasiRandom_hpp
#define Beagle_Core_QuasiRandom_hpp

#include "PACC/Util.hpp"
#include "PACC/Math.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Component.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/ULong.hpp"


namespace Beagle
{

/*!
 *  \class QuasiRandom beagle/Core/QuasiRandom.hpp "beagle/Core/QuasiRandom.hpp"
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
