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
 *  \file   Beagle/Core/String.hpp
 *  \brief  Definition of the type String and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_String_hpp
#define Beagle_Core_String_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the STL string type.
 *  \ingroup Adapt
 */
typedef WrapperT< std::string >
String;

}


/*!
 *  \brief  Test whether an string is less than another.
 *  \param  inLeftString  Left string compared.
 *  \param  inRightString Right string compared.
 *  \return True if left string is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::String.
 */
inline bool operator<(const Beagle::String& inLeftString, const Beagle::String& inRightString)
{
	Beagle_StackTraceBeginM();
	return inLeftString.isLess(inRightString);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an string is less than, or equal to another.
 *  \param  inLeftString  Left string compared.
 *  \param  inRightString Right string compared.
 *  \return True if left string is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::String.
 */
inline bool operator<=(const Beagle::String& inLeftString, const Beagle::String& inRightString)
{
	Beagle_StackTraceBeginM();
	return ( inLeftString.isLess(inRightString) || inLeftString.isEqual(inRightString) );
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an string is more than another.
 *  \param  inLeftString  Left string compared.
 *  \param  inRightString Right string compared.
 *  \return True if left string is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::String.
 */
inline bool operator>(const Beagle::String& inLeftString, const Beagle::String& inRightString)
{
	Beagle_StackTraceBeginM();
	return inRightString.isLess(inLeftString);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an string is more than, or equal to another.
 *  \param  inLeftString  Left string compared.
 *  \param  inRightString Right string compared.
 *  \return True if left string is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::String.
 */
inline bool operator>=(const Beagle::String& inLeftString, const Beagle::String& inRightString)
{
	Beagle_StackTraceBeginM();
	return ( inRightString.isLess(inLeftString) || inLeftString.isEqual(inRightString) );
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an string is equal to another.
 *  \param  inLeftString  Left string compared.
 *  \param  inRightString Right string compared.
 *  \return True if left string is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::String.
 */
inline bool operator==(const Beagle::String& inLeftString, const Beagle::String& inRightString)
{
	Beagle_StackTraceBeginM();
	return inLeftString.isEqual(inRightString);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an string is not equal to another.
 *  \param  inLeftString  Left string compared.
 *  \param  inRightString Right string compared.
 *  \return True if left string is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::String.
 */
inline bool operator!=(const Beagle::String& inLeftString, const Beagle::String& inRightString)
{
	Beagle_StackTraceBeginM();
	return ( inLeftString.isEqual(inRightString) == false);
	Beagle_StackTraceEndM();
}

#endif // Beagle_Core_String_hpp



