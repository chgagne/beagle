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
 *  \file   Beagle/EC/AlgoGenerational.hpp
 *  \brief  Definition of the class AlgoGenerational.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_EC_AlgoGenerational_hpp
#define Beagle_EC_AlgoGenerational_hpp

#include <map>
#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class AlgoGenerational Beagle/EC/AlgoGenerational.hpp "Beagle/EC/AlgoGenerational.hpp"
 *  \brief Generational algorithm class, rely on GenerationalOp replacement strategy.
 *  \ingroup EC
 */
class AlgoGenerational : public Beagle::Algorithm
{

public:

	//! AlgoGenerational allocator type.
	typedef Beagle::AllocatorT< AlgoGenerational, Beagle::Algorithm::Alloc > Alloc;
	//! AlgoGenerational handle type.
	typedef Beagle::PointerT< AlgoGenerational, Beagle::Algorithm::Handle > Handle;
	//! AlgoGenerational bag type.
	typedef Beagle::ContainerT< AlgoGenerational, Beagle::Algorithm::Bag > Bag;

	AlgoGenerational();
	virtual ~AlgoGenerational()
	{ }

	virtual void configure(Beagle::Evolver& ioEvolver, Beagle::System& ioSystem);

};

}

}

#endif // Beagle_EC_AlgoGenerational_hpp
