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
 *  \file   beagle/Core/Package.hpp
 *  \brief  Definition of the class Package.
 *  \author Christian Gagne
 *  $Revision: 1.3 $
 *  $Date: 2007/09/12 14:23:47 $
 */

#ifndef Beagle_Core_Package_hpp
#define Beagle_Core_Package_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/System.hpp"


namespace Beagle
{

/*!
 *  \class Package beagle/Core/Package.hpp "beagle/Core/Package.hpp"
 *  \brief Abstract package class, used to setup a system.
 *  \ingroup Core
 */
class Package : public NamedObject
{

public:

	//! Package allocator type.
	typedef AbstractAllocT< Package, NamedObject::Alloc > Alloc;
	//! Package handle type.
	typedef PointerT< Package, NamedObject::Handle > Handle;
	//! Package bag type.
	typedef ContainerT< Package, NamedObject::Bag > Bag;

	explicit Package(std::string inName="UnnamedPackage");
	virtual ~Package()
	{ }

	/*!
	 *  \brief Configure system by adding necessary objects into it.
	 *  \param ioSystem System to configure.
	 */
	virtual void configure(System& ioSystem) =0;

	/*!
	 *  \brief Obtain list of package on which current package depends.
	 *  \return Bag with the packages instanciation on which current package depends.
	 */
	virtual ContainerT< Package, NamedObject::Bag > listDependencies(void) =0;

};

}

#endif // Beagle_Core_Package_hpp
