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
 *  \file   Beagle/Core/ComponentMap.hpp
 *  \brief  Definition of class ComponentMap.
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_ComponentMap_hpp
#define Beagle_Core_ComponentMap_hpp

#include <string>
#include <map>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/Component.hpp"


namespace Beagle
{


/*!
 *  \class ComponentMap Beagle/Core/ComponentMap.hpp "Beagle/Core/ComponentMap.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    Component::Handle instances.
 *  \ingroup Core
 *  \ingroup Sys
 */
class ComponentMap : public Map
{

public:

	//! ComponentMap allocator type.
	typedef AllocatorT<ComponentMap,Map::Alloc>
	Alloc;
	//! ComponentMap handle type.
	typedef PointerT<ComponentMap,Map::Handle>
	Handle;
	//! ComponentMap bag type.
	typedef ContainerT<ComponentMap,Map::Bag>
	Bag;

	ComponentMap()
	{ }
	virtual ~ComponentMap()
	{ }

	/*!
	 *  \brief Get component from the map.
	 *  \param inKey Key associated to the component.
	 *  \return Smart pointer to the component, a NULL pointer if the component isn't in the map.
	 */
	inline Component::Handle& operator[](std::string inKey) {
		Beagle_StackTraceBeginM();
		return castHandleT<Component>(Map::operator[](inKey));
		Beagle_StackTraceEndM();
	}

};

}

#endif // Beagle_Core_ComponentMap_hpp

