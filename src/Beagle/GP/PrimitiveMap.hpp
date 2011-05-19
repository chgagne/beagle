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
 *  \file   beagle/GP/PrimitiveMap.hpp
 *  \brief  Definition of class GP::PrimitiveMap.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_PrimitiveMap_hpp
#define Beagle_GP_PrimitiveMap_hpp

#include <string>
#include <map>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Map.hpp"
#include "beagle/GP/Primitive.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class PrimitiveMap beagle/GP/PrimitiveMap.hpp "beagle/GP/PrimitiveMap.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    GP::Primitive::Handle instances.
 *  \ingroup GPF
 *  \ingroup Primit
 */
class PrimitiveMap : public Map
{

public:

	//! PrimitiveMap allocator type.
	typedef AllocatorT<GP::PrimitiveMap,Map::Alloc>
	Alloc;
	//! PrimitiveMap handle type.
	typedef PointerT<GP::PrimitiveMap,Map::Handle>
	Handle;
	//! PrimitiveMap bag type.
	typedef ContainerT<GP::PrimitiveMap,Map::Bag>
	Bag;

	PrimitiveMap()
	{ }
	virtual ~PrimitiveMap()
	{ }

	/*!
	 *  \brief Get primitive from the map.
	 *  \param inKey Key associated to the primitive.
	 *  \return Smart pointer to the primitive.
	 */
	inline GP::Primitive::Handle& operator[](std::string inKey)
	{
		Beagle_StackTraceBeginM();
		return castHandleT<GP::Primitive>(Map::operator[](inKey));
		Beagle_StackTraceEndM();
	}

};

}
}

#endif // Beagle_GP_PrimitiveMap_hpp
