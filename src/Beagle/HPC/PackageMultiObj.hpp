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
 *  \file   beagle/HPC/PackageMultiObj.hpp
 *  \brief  Definition of the class HPC::PackageMultiObj.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_HPC_PackageMultiObj_hpp
#define Beagle_HPC_PackageMultiObj_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/System.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/Package.hpp"

namespace Beagle
{

namespace HPC
{

/*!
 *  \class Beagle::HPC::PackageMultiObj beagle/HPC/PackageMultiObj.hpp "beagle/HPC/PackageMultiObj.hpp"
 *  \brief Package for adding objects related to Multi-objectives distrib EA.
 *  \ingroup MultiObj
 *  \ingroup HPC
 */
class PackageMultiObj : public Package
{

public:

	//! HPC::PackageMultiObj allocator type.
	typedef AllocatorT< PackageMultiObj, Package::Alloc > Alloc;
	//! HPC::PackageMultiObj handle type.
	typedef PointerT< PackageMultiObj, Package::Handle > Handle;
	//! HPC::PackageMultiObj bag type.
	typedef ContainerT< PackageMultiObj, Package::Bag > Bag;

	explicit PackageMultiObj();

	virtual void          configure(System& ioSystem);
	virtual Package::Bag  listDependencies(void);
};

}
}

#endif // Beagle_HPC_PackageMultiObj_hpp
