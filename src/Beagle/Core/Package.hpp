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
#include "Beagle/Core/PackageAbstract.hpp"


namespace Beagle
{

/*!
 *  \class Package Beagle/Core/Package.hpp "Beagle/Core/Package.hpp"
 *  \brief Package class for adding basic objects to the system.
 *  \ingroup Core
 */
class Package : public PackageAbstract
{

public:

	//! Package allocator type.
	typedef AllocatorT< Package,PackageAbstract::Alloc > Alloc;
	//! Package handle type.
	typedef PointerT< Package,PackageAbstract::Handle > Handle;
	//! Package bag type.
	typedef ContainerT< Package,PackageAbstract::Bag > Bag;

	Package();
	virtual ~Package()
	{ }

	virtual void                 configure(System& ioSystem);
	virtual PackageAbstract::Bag listDependencies(void);

};

}

#endif // Beagle_Core_Package_hpp
