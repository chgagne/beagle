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
 *  \file   Beagle/Core/IsEqualPointerPredicate.hpp
 *  \brief  Definition of class IsEqualPointerPredicate.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_IsEqualPointerPredicate_hpp
#define Beagle_Core_IsEqualPointerPredicate_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"

namespace Beagle
{


/*!
 *  \class IsEqualPointerPredicate Beagle/Core/IsEqualPointerPredicate.hpp
 *    "Beagle/Core/IsEqualPointerPredicate.hpp"
 *  \brief Binary predicate functor that test if two pointed Objects are equals.
 *  \ingroup Core
 *  \ingroup Utils
 *  \par Note:
 *    The IsEqualPointerPredicate is compliant with the STL BinaryPredicate functor.
 *    See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsEqualPointerPredicate
{

public:
	IsEqualPointerPredicate()
	{ }
	~IsEqualPointerPredicate()
	{ }

	/*!
	 *  \brief Test if the Objects pointed are equal.
	 *  \param inLeftPtr Pointer to the left object to compare.
	 *  \param inRightPtr Pointer to the right object to compare.
	 */
	inline bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const {
		Beagle_StackTraceBeginM();
		if( (!inLeftPtr) && (!inRightPtr) ) return true;
		else if( (!inLeftPtr) || (!inRightPtr) ) return false;
		return inLeftPtr->isEqual(*inRightPtr);
		Beagle_StackTraceEndM();
	}

};

}

#endif // Beagle_Core_IsEqualPointerPredicate_hpp
