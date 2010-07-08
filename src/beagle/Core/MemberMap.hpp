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
 *  \file   beagle/Core/MemberMap.hpp
 *  \brief  Definition of class MemberMap.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_MemberMap_hpp
#define Beagle_Core_MemberMap_hpp

#include <string>
#include <map>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Map.hpp"
#include "beagle/Core/Member.hpp"


namespace Beagle
{


/*!
 *  \class MemberMap beagle/Core/MemberMap.hpp "beagle/Core/MemberMap.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    Member::Handle instances.
 *  \ingroup ECF
 *  \ingroup Sys
 */
class MemberMap : public Map
{

public:

	//! MemberMap allocator type.
	typedef AllocatorT<MemberMap,Map::Alloc> Alloc;
	//! MemberMap handle type.
	typedef PointerT<MemberMap,Map::Handle> Handle;
	//! MemberMap bag type.
	typedef ContainerT<MemberMap,Map::Bag> Bag;

	MemberMap()
	{ }
	virtual ~MemberMap()
	{ }

	/*!
	 *  \brief Get member from the map.
	 *  \param inKey Key associated to the member.
	 *  \return Smart pointer to the member, a NULL pointer if the member isn't in the map.
	 */
	inline Member::Handle& operator[](std::string inKey)
	{
		Beagle_StackTraceBeginM();
		return castHandleT<Member>(Map::operator[](inKey));
		Beagle_StackTraceEndM("Member::Handle& MemberMap::operator[](std::string inKey)");
	}

};

}

#endif // Beagle_Core_MemberMap_hpp

