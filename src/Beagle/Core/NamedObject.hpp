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
 *  \file   Beagle/Core/NamedObject.hpp
 *  \brief  Class NamedObject declaration.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */


#ifndef Beagle_Core_NamedObject_hpp
#define Beagle_Core_NamedObject_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"


namespace Beagle
{

/*!
 *  \class NamedObject Beagle/Core/NamedObject.hpp "Beagle/Core/NamedObject.hpp"
 *  \brief An Object with a name.
 *  \ingroup Core
 *  \author Matthew Walker
 *  \author Christian Gagne
 */
class NamedObject : public Object
{

public:

	//! NamedObject allocator type.
	typedef AllocatorT<NamedObject,Object::Alloc>
	Alloc;
	//! NamedObject handle type.
	typedef PointerT<NamedObject,Object::Handle>
	Handle;
	//! NamedObject bag type.
	typedef ContainerT<NamedObject,Object::Bag>
	Bag;

	explicit NamedObject(const std::string& inName="Unnamed Object");
	virtual ~NamedObject()
	{ }

	virtual const std::string& getName() const;
	virtual void               setName(const std::string& inName);

private:

	std::string mName;  //!< Name of Object

};

}

#endif // Beagle_Core_NamedObject_hpp
