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
	inline Operator::Handle& operator[](std::string inKey) {
		Beagle_StackTraceBeginM();
		return castHandleT<Operator>(Map::operator[](inKey));
		Beagle_StackTraceEndM();
	}

};

}

#endif // Beagle_Core_OperatorMap_hpp


