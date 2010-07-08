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
 *  \file   Beagle/Core/OperatorMap.hpp
 *  \brief  Definition of class OperatorMap.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_OperatorMap_hpp
#define Beagle_Core_OperatorMap_hpp

#include <string>
#include <map>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/Operator.hpp"


namespace Beagle
{


/*!
 *  \class OperatorMap Beagle/Core/OperatorMap.hpp "Beagle/Core/OperatorMap.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    Operator::Handle instances.
 *  \ingroup Core
 *  \ingroup Op
 */
class OperatorMap : public Map
{

public:

	//! OperatorMap allocator type.
	typedef AllocatorT<OperatorMap,Map::Alloc> Alloc;
	//! OperatorMap handle type.
	typedef PointerT<OperatorMap,Map::Handle> Handle;
	//! OperatorMap bag type.
	typedef ContainerT<OperatorMap,Map::Bag> Bag;

	OperatorMap()
	{ }
	virtual ~OperatorMap()
	{ }

	/*!
	 *  \brief Get operator from the map.
	 *  \param inKey Key associated to the operator.
	 *  \return Smart pointer to the operator.
	 */
	inline Operator::Handle& operator[](std::string inKey)
	{
		Beagle_StackTraceBeginM();
		return castHandleT<Operator>(Map::operator[](inKey));
		Beagle_StackTraceEndM("Operator::Handle& OperatorMap::operator[](std::string inKey)");
	}

};

}

#endif // Beagle_Core_OperatorMap_hpp


