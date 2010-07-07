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
 *  \file   beagle/Core/Bool.hpp
 *  \brief  Definition of type Bool and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:41 $
 */

#ifndef Beagle_Core_Bool_hpp
#define Beagle_Core_Bool_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/ArrayT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic bool type.
 *  \ingroup Adapt
 */
typedef WrapperT< bool >
Bool;

/*!
 *  \brief Beagle array of the atomic bool type.
 *  \ingroup Adapt
 */
typedef ArrayT< bool >
BoolArray;

}


/*!
 *  \brief     Test whether an bool is equal to another.
 *  \param     inLeftBool  Left bool compared.
 *  \param     inRightBool Right bool compared.
 *  \return    True if left bool is equal to the right one, false if not.
 *  \par Note:
 *     The operator is defined relatively to the function isEqual of Beagle::Bool.
 */
inline bool operator==(const Beagle::Bool& inLeftBool, const Beagle::Bool& inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.isEqual(inRightBool);
	Beagle_StackTraceEndM("bool operator==(const Bool& inLeftBool, const Bool& inRightBool)");
}


/*!
 *  \brief     Test whether an bool is not equal to another.
 *  \param     inLeftBool  Left bool compared.
 *  \param     inRightBool Right bool compared.
 *  \return    True if left bool is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Bool.
 */
inline bool operator!=(const Beagle::Bool& inLeftBool, const Beagle::Bool& inRightBool)
{
	Beagle_StackTraceBeginM();
	return ( inLeftBool.isEqual(inRightBool) == false);
	Beagle_StackTraceEndM("bool operator!=(const Bool& inLeftBool, const Bool& inRightBool)");
}


/*!
 *  \brief   Negate a Bool.
 *  \param   inBool Bool to negate.
 *  \return  Result of the negation.
 */
inline
Beagle::Bool operator!(const Beagle::Bool& inBool)
{
	Beagle_StackTraceBeginM();
	return !(inBool.getWrappedValue());
	Beagle_StackTraceEndM("Bool operator!(const Bool& inBool)");
}


/*!
 *  \brief   Compare equality of a Bool with a bool.
 *  \param   inLeftBool Left value to compare.
 *  \param   inRightBool Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator==(const Beagle::Bool& inLeftBool, bool inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.getWrappedValue() == inRightBool;
	Beagle_StackTraceEndM("bool operator==(const Bool& inLeftBool, bool inRightBool)");
}


/*!
 *  \brief   Compare inequality of a Bool with a bool.
 *  \param   inLeftBool Left value to compare.
 *  \param   inRightBool Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator!=(const Beagle::Bool& inLeftBool, bool inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.getWrappedValue() != inRightBool;
	Beagle_StackTraceEndM("bool operator!=(const Bool& inLeftBool, bool inRightBool)");
}


/*!
 *  \brief   Logical AND of a Bool with a Bool.
 *  \param   inLeftBool Left value.
 *  \param   inRightBool Right value.
 *  \return  Result of the logical AND.
 */
inline
Beagle::Bool operator&&(const Beagle::Bool& inLeftBool,
                        const Beagle::Bool& inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.getWrappedValue() && inRightBool.getWrappedValue();
	Beagle_StackTraceEndM("Bool operator&&(const Bool& inLeftBool, const Bool& inRightBool)");
}


/*!
 *  \brief   Logical AND of a Bool with a bool.
 *  \param   inLeftBool Left value.
 *  \param   inRightBool Right value.
 *  \return  Result of the logical AND.
 */
inline
Beagle::Bool operator&&(const Beagle::Bool& inLeftBool, bool inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.getWrappedValue() && inRightBool;
	Beagle_StackTraceEndM("Bool operator&&(const Bool& inLeftBool, bool inRightBool)");
}


/*!
 *  \brief   Logical OR of a Bool with a Bool.
 *  \param   inLeftBool Left value.
 *  \param   inRightBool Right value.
 *  \return  Result of the logical OR.
 */
inline
Beagle::Bool operator||(const Beagle::Bool& inLeftBool,
                        const Beagle::Bool& inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.getWrappedValue() || inRightBool.getWrappedValue();
	Beagle_StackTraceEndM("Bool operator||(const Bool& inLeftBool, const Bool& inRightBool)");
}


/*!
 *  \brief   Logical OR of a Bool with a bool.
 *  \param   inLeftBool Left value.
 *  \param   inRightBool Right value.
 *  \return  Result of the logical OR.
 */
inline
Beagle::Bool operator||(const Beagle::Bool& inLeftBool, bool inRightBool)
{
	Beagle_StackTraceBeginM();
	return inLeftBool.getWrappedValue() || inRightBool;
	Beagle_StackTraceEndM("Bool operator||(const Bool& inLeftBool, bool inRightBool)");
}


#endif // Beagle_Core_Bool_hpp



