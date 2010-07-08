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
 *  \file   Beagle/Core/Package.hpp
 *  \brief  Definition of the class PackageBase.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_PackageBase_hpp
#define Beagle_Core_PackageBase_hpp

#include <map>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/System.hpp"
#include "Beagle/Core/Evolver.hpp"
#include "Beagle/Core/Package.hpp"


namespace Beagle
{

/*!
 *  \class PackageBase Beagle/Core/PackageBase.hpp "Beagle/Core/PackageBase.hpp"
 *  \brief Package class for adding basic objects to the system.
 *  \ingroup Core
 */
class PackageBase : public Package
{

public:

	//! PackageBase allocator type.
	typedef AllocatorT< PackageBase, Package::Alloc > Alloc;
	//! PackageBase handle type.
	typedef PointerT< PackageBase, Package::Handle > Handle;
	//! PackageBase bag type.
	typedef ContainerT< PackageBase, Package::Bag > Bag;

	PackageBase(void);
	virtual ~PackageBase()
	{ }

	virtual void         configure(System& ioSystem);
	virtual Package::Bag listDependencies(void);

};

}

#endif // Beagle_Core_PackageBase_hpp
