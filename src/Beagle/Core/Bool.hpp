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
 *  \file   Beagle/Core/Bool.hpp
 *  \brief  Definition of type Bool and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:41 $
 */

#ifndef Beagle_Core_Bool_hpp
#define Beagle_Core_Bool_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"


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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_Bool_hpp



