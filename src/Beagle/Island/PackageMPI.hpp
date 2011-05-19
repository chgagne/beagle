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
 *  \file   beagle/Distrib/Island/PackageMPI.hpp
 *  \brief  Definition of the class Distrib::Island::PackageMPI.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:31 $
 */

#ifndef Beagle_Distrib_Island_PackageMPI_hpp
#define Beagle_Distrib_Island_PackageMPI_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/System.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/Package.hpp"


namespace Beagle
{

namespace Distrib
{

namespace Island
{

/*!
 *  \class Distrib::Island::PackageMPI beagle/Distrib/Island/PackageMPI.hpp
 *    "beagle/Distrib/Island/PackageMPI.hpp"
 *  \brief Package for adding objects related to island-model MPI distribution.
 *  \ingroup DBIM
 */
class PackageMPI : public Beagle::Package
{

public:

	//! Distrib::Island::PackageMPI allocator type.
	typedef AllocatorT< PackageMPI, Package::Alloc > Alloc;
	//! Distrib::Island::PackageMPI handle type.
	typedef PointerT< PackageMPI, Package::Handle > Handle;
	//! Distrib::Island::PackageMPI bag type.
	typedef ContainerT< PackageMPI, Package::Bag > Bag;

	PackageMPI(void);

	virtual void                  configure(System& ioSystem);
	virtual Beagle::Package::Bag  listDependencies(void);

};

}
}
}

#endif // Beagle_Distrib_Island_PackageMPI_hpp
