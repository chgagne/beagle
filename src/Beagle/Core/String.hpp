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
 *  \file   beagle/Core/String.hpp
 *  \brief  Definition of the type String and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_String_hpp
#define Beagle_Core_String_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/WrapperT.hpp"


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
	Beagle_StackTraceEndM("bool operator<(const String& inLeftString, const String& inRightString)");
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
	Beagle_StackTraceEndM("bool operator<=(const String& inLeftString, const String& inRightString)");
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
	Beagle_StackTraceEndM("bool operator>(const String& inLeftString, const String& inRightString)");
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
	Beagle_StackTraceEndM("bool operator>=(const String& inLeftString, const String& inRightString)");
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
	Beagle_StackTraceEndM("bool operator==(const String& inLeftString, const String& inRightString)");
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
	Beagle_StackTraceEndM("bool operator!=(const String& inLeftString, const String& inRightString)");
}

#endif // Beagle_Core_String_hpp



