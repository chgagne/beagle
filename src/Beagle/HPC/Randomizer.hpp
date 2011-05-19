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
 *  \file   beagle/HPC/Randomizer.hpp
 *  \brief  Definition of the class Randomizer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_HPC_Randomizer_hpp
#define Beagle_HPC_Randomizer_hpp

#include "beagle/Beagle.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Component.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/ULong.hpp"


namespace Beagle
{
namespace HPC
{

/*!
 *  \class Randomizer beagle/HPC/Randomizer.hpp "beagle/HPC/Randomizer.hpp"
 *  \brief Random number generator class.
 *  \note The operator() allow compliance with the STL random number generator interface.
 *  \ingroup ECF
 *  \ingroup Sys
 *  \ingroup HPC
 *
 */
class Randomizer : public Beagle::Randomizer
{

public:

	//! Randomizer allocator type.
	typedef AllocatorT<HPC::Randomizer,Beagle::Randomizer::Alloc> Alloc;
	//! Randomizer handle type.
	typedef PointerT<HPC::Randomizer,Beagle::Randomizer::Handle> Handle;
	//! Randomizer bag type.
	typedef ContainerT<HPC::Randomizer,Beagle::Randomizer::Bag> Bag;

	Randomizer(unsigned long inSeed=0);
	virtual ~Randomizer() { }

	virtual void init(System& ioSystem);

};

}
}

#endif // Beagle_HPC_Randomizer_hpp
