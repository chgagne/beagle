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
 *  \file   Beagle/Core/IsLessPointerPredicate.hpp
 *  \brief  Definition of class IsLessPointerPredicate.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_IsLessPointerPredicate_hpp
#define Beagle_Core_IsLessPointerPredicate_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"

namespace Beagle
{


/*!
 *  \class IsLessPointerPredicate Beagle/Core/IsLessPointerPredicate.hpp
 *    "Beagle/Core/IsLessPointerPredicate.hpp"
 *  \brief Binary predicate functor that test the ordering of two pointed Objects.
 *  \ingroup Core
 *  \ingroup Utils
 *  \par Note:
 *    The IsLessPointerPredicate is compliant with the STL BinaryPredicate functor.
 *    See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsLessPointerPredicate
{

public:
	IsLessPointerPredicate()
	{ }
	~IsLessPointerPredicate()
	{ }

	/*!
	 *  \brief Test the ordering of two pointed Objects.
	 *  \param inLeftPtr Pointer to the left object to compare.
	 *  \param inRightPtr Pointer to the right object to compare.
	 */
	inline bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const
	{
		Beagle_StackTraceBeginM();
		if( (!inLeftPtr) || (!inRightPtr) ) return false;
		return inLeftPtr->isLess(*inRightPtr);
		Beagle_StackTraceEndM("bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const");
	}

};

}

#endif // Beagle_Core_IsLessPointerPredicate_hpp
