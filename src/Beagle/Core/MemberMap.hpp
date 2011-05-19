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
 *  \file   Beagle/Core/MemberMap.hpp
 *  \brief  Definition of class MemberMap.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_MemberMap_hpp
#define Beagle_Core_MemberMap_hpp

#include <string>
#include <map>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/Member.hpp"


namespace Beagle
{


/*!
 *  \class MemberMap Beagle/Core/MemberMap.hpp "Beagle/Core/MemberMap.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    Member::Handle instances.
 *  \ingroup Core
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
	inline Member::Handle& operator[](std::string inKey) {
		Beagle_StackTraceBeginM();
		return castHandleT<Member>(Map::operator[](inKey));
		Beagle_StackTraceEndM();
	}

};

}

#endif // Beagle_Core_MemberMap_hpp

