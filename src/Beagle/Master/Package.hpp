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
 *  \file   beagle/Distrib/Master/Package.hpp
 *  \brief  Definition of the class Distrib::Master::Package.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:33 $
 */

#ifndef Beagle_Distrib_Master_Package_hpp
#define Beagle_Distrib_Master_Package_hpp

#include <map>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/System.hpp"
#include "Beagle/Core/Evolver.hpp"
#include "Beagle/Core/Package.hpp"


namespace Beagle
{

namespace Distrib
{

namespace Master
{

/*!
 *  \class Distrib::Master::Package beagle/Distrib/Master/Package.hpp
 *    "beagle/Distrib/Master/Package.hpp"
 *  \brief Package for adding basic objects related to master-slave distribution.
 *  \ingroup DBMS
 */
class Package : public PackageAbstract
{

public:

	//! Distrib::Master::Package allocator type.
	typedef AllocatorT< Package, PackageAbstract::Alloc > Alloc;
	//! Distrib::Master::Package handle type.
	typedef PointerT< Package, PackageAbstract::Handle > Handle;
	//! Distrib::Master::Package bag type.
	typedef ContainerT< Package, PackageAbstract::Bag > Bag;

	Package(void);

	virtual void                  configure(System& ioSystem);
	virtual PackageAbstract::Bag  listDependencies(void);

};

}
}
}

#endif // Beagle_Distrib_Master_Package_hpp
