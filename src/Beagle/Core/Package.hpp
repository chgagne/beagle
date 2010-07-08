/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/Core/Package.hpp
 *  \brief  Definition of the class Package.
 *  \author Christian Gagne
 *  $Revision: 1.3 $
 *  $Date: 2007/09/12 14:23:47 $
 */

#ifndef Beagle_Core_Package_hpp
#define Beagle_Core_Package_hpp

#include <map>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/System.hpp"


namespace Beagle
{

/*!
 *  \class Package Beagle/Core/Package.hpp "Beagle/Core/Package.hpp"
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
