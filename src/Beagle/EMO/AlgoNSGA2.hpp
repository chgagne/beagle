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
 *  \file   beagle/AlgoNSGA2.hpp
 *  \brief  Definition of the class AlgoNSGA2.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_AlgoNSGA2_hpp
#define Beagle_AlgoNSGA2_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Algorithm.hpp"


namespace Beagle
{

/*!
 *  \class AlgoNSGA2 beagle/AlgoNSGA2.hpp "beagle/AlgoNSGA2.hpp"
 *  \brief NSGA-II algorithm class, for multi-objective optimization.
 *  \ingroup ECF
 *  \ingroup MultiObj
 */
class AlgoNSGA2 : public Algorithm
{

public:

	//! AlgoNSGA2 allocator type.
	typedef AllocatorT< AlgoNSGA2, Algorithm::Alloc > Alloc;
	//! AlgoNSGA2 handle type.
	typedef PointerT< AlgoNSGA2, Algorithm::Handle > Handle;
	//! AlgoNSGA2 bag type.
	typedef ContainerT< AlgoNSGA2, Algorithm::Bag > Bag;

	AlgoNSGA2(void);
	virtual ~AlgoNSGA2()
	{ }

	virtual void configure(Evolver& ioEvolver, System& ioSystem);

};

}

#endif // Beagle_AlgoNSGA2_hpp
