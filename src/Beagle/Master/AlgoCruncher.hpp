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
 *  \file   beagle/Distrib/Master/AlgoCruncher.hpp
 *  \brief  Definition of the class Distrib::Master::AlgoCruncher.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:33 $
 */

#ifndef Beagle_Distrib_Master_AlgoCruncher_hpp
#define Beagle_Distrib_Master_AlgoCruncher_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Algorithm.hpp"


namespace Beagle
{

namespace Distrib
{

namespace Master
{


/*!
 *  \class AlgoCruncher beagle/AlgoCruncher.hpp "beagle/AlgoCruncher.hpp"
 *  \brief Algorithm for master-slave crunchers (clients evaluating fitnesses).
 *  \ingroup DBMS
 */
class AlgoCruncher : public Algorithm
{

public:

	//! AlgoCruncher allocator type.
	typedef AllocatorT< AlgoCruncher, Algorithm::Alloc > Alloc;
	//! AlgoCruncher handle type.
	typedef PointerT< AlgoCruncher, Algorithm::Handle > Handle;
	//! AlgoCruncher bag type.
	typedef ContainerT< AlgoCruncher, Algorithm::Bag > Bag;

	AlgoCruncher(void);

	virtual void configure(Evolver& ioEvolver, System& ioSystem);

};

}
}
}

#endif // Beagle_Distrib_Master_AlgoCruncher_hpp
