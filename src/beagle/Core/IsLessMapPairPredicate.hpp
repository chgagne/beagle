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
 *  \file   beagle/Core/IsLessMapPairPredicate.hpp
 *  \brief  Definition of class IsLessMapPredicate.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_IsLessMapPairPredicate_hpp
#define Beagle_Core_IsLessMapPairPredicate_hpp

#include <string>
#include <utility>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"

namespace Beagle
{


/*!
 *  \class IsLessMapPairPredicate beagle/Core/IsLessMapPairPredicate.hpp
 *    "beagle/Core/IsLessMapPairPredicate.hpp"
 *  \brief Binary predicate functor testing the ordering of two
 *    std::pair<string,Object::Handle>.
 *  \ingroup OOF
 *  \ingroup Utils
 *  \par Note:
 *    The IsLessMapPairPredicate is compliant with the STL BinaryPredicate functor.
 *    See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsLessMapPairPredicate
{

public:
	IsLessMapPairPredicate()
	{ }
	~IsLessMapPairPredicate()
	{ }

	/*!
	 *  \brief Test the ordering of two std::pair<string,Object::Handle>.
	 *  \param inLeftPair Left map pair to compare.
	 *  \param inRightPair Right map pair to compare.
	 */
	inline bool operator()(const std::pair<std::string,Object::Handle>& inLeftPair,
	                       const std::pair<std::string,Object::Handle>& inRightPair) const
	{
		Beagle_StackTraceBeginM();
		if( (inLeftPair.first < inRightPair.first) == false ) return false;
		if( (!inLeftPair.second) || (!inRightPair.second) ) return false;
		return inLeftPair.second->isLess(*inRightPair.second);
		Beagle_StackTraceEndM("bool operator()(const std::pair<std::string,Object::Handle>& inLeftPair, const std::pair<std::string,Object::Handle>& inRightPair) const");
	}

};

}

#endif // Beagle_Core_IsLessMapPairPredicate_hpp
