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
 *  \file   Beagle/Core/Float.hpp
 *  \brief  Definition of the type Float and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_Float_hpp
#define Beagle_Core_Float_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic float type.
 *  \ingroup Adapt
 */
typedef WrapperT< float >
Float;

/*!
 *  \brief Beagle array of the atomic float type.
 *  \ingroup Adapt
 */
typedef ArrayT< float >
FloatArray;

/*!
 *  \brief Evaluate absolute value of a Beagle::Float.
 *  \param inValue Float to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Adapt
 */
template <>
inline Float absolute(const Float& inValue)
{
	Beagle_StackTraceBeginM();
	return Float(std::fabs(inValue.getWrappedValue()));
	Beagle_StackTraceEndM();
}

}


/*!
 *  \brief  Test whether an float is less than another.
 *  \param  inLeftFloat  Left float compared.
 *  \param  inRightFloat Right float compared.
 *  \return True if left float is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::Float.
 */
inline bool operator<(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.isLess(inRightFloat);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an float is less than, or equal to another.
 *  \param  inLeftFloat  Left float compared.
 *  \param  inRightFloat Right float compared.
 *  \return True if left float is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::Float.
 */
inline bool operator<=(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return ( inLeftFloat.isLess(inRightFloat) || inLeftFloat.isEqual(inRightFloat) );
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an float is more than another.
 *  \param  inLeftFloat  Left float compared.
 *  \param  inRightFloat Right float compared.
 *  \return True if left float is more than the right one, false if not.
 *  \par Note:
 *     The operator is defined relatively to the function isLess of Beagle::Float.
 */
inline bool operator>(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inRightFloat.isLess(inLeftFloat);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an float is more than, or equal to another.
 *  \param  inLeftFloat  Left float compared.
 *  \param  inRightFloat Right float compared.
 *  \return True if left float is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::Float.
 */
inline bool operator>=(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return ( inRightFloat.isLess(inLeftFloat) || inLeftFloat.isEqual(inRightFloat) );
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an float is equal to another.
 *  \param  inLeftFloat  Left float compared.
 *  \param  inRightFloat Right float compared.
 *  \return True if left float is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Float.
 */
inline bool operator==(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.isEqual(inRightFloat);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an float is not equal to another.
 *  \param  inLeftFloat  Left float compared.
 *  \param  inRightFloat Right float compared.
 *  \return True if left float is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Float.
 */
inline bool operator!=(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return ( inLeftFloat.isEqual(inRightFloat) == false);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compare equality of a Float with a float.
 *  \param inLeftFloat Left value to compare.
 *  \param inRightFloat Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator==(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() == inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compare inequality of a Float with a float.
 *  \param inLeftFloat Left value to compare.
 *  \param inRightFloat Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator!=(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() != inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a Float is < than a float.
 *  \param inLeftFloat Left value to compare.
 *  \param inRightFloat Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() < inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a Float is <= than a float.
 *  \param inLeftFloat Left value to compare.
 *  \param inRightFloat Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<=(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() <= inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a Float is > than a float.
 *  \param inLeftFloat Left value to compare.
 *  \param inRightFloat Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() > inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a Float is >= than a float.
 *  \param inLeftFloat Left value to compare.
 *  \param inRightFloat Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>=(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() >= inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add two Float.
 *  \param inLeftFloat Left value to add.
 *  \param inRightFloat Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Float operator+(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() + inRightFloat.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a Float with a float.
 *  \param inLeftFloat Left value to add.
 *  \param inRightFloat Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Float operator+(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() + inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a Float to a Float.
 *  \param inLeftFloat Left value to which the right one is added.
 *  \param inRightFloat Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Float operator+=(Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() += inRightFloat.getWrappedValue();
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a float to a Float.
 *  \param inLeftFloat Left value to which the right one is added.
 *  \param inRightFloat Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Float operator+=(Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() += inRightFloat;
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract two Float.
 *  \param inLeftFloat Left value to subtract.
 *  \param inRightFloat Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::Float operator-(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() - inRightFloat.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract a float from a Float.
 *  \param inLeftFloat Left value to subtract.
 *  \param inRightFloat Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::Float operator-(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() - inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract a Float from a Float.
 *  \param inLeftFloat Left value from which the right float is subtracted.
 *  \param inRightFloat Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::Float operator-=(Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() -= inRightFloat.getWrappedValue();
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract a float from a Float.
 *  \param inLeftFloat Left value from which the right float is subtracted.
 *  \param inRightFloat Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::Float operator-=(Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() -= inRightFloat;
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply two Float.
 *  \param inLeftFloat Left value to multiply.
 *  \param inRightFloat Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::Float operator*(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() * inRightFloat.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply a Float with a float.
 *  \param inLeftFloat Left value to multiply.
 *  \param inRightFloat Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::Float operator*(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() * inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply a Float with a Float.
 *  \param inLeftFloat Left value to which the right float is multiplied.
 *  \param inRightFloat Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::Float operator*=(Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() *= inRightFloat.getWrappedValue();
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply a Float with a float.
 *  \param inLeftFloat Left value from which the right float is multiplied.
 *  \param inRightFloat Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::Float operator*=(Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() *= inRightFloat;
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide two Float.
 *  \param inLeftFloat Left value to divide.
 *  \param inRightFloat Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Float operator/(const Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() / inRightFloat.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide a Float with a float.
 *  \param inLeftFloat Left value to divide.
 *  \param inRightFloat Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Float operator/(const Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	return inLeftFloat.getWrappedValue() / inRightFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide a Float with a Float.
 *  \param inLeftFloat Left value to which the right float is divided.
 *  \param inRightFloat Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Float operator/=(Beagle::Float& inLeftFloat, const Beagle::Float& inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() /= inRightFloat.getWrappedValue();
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide a Float with a float.
 *  \param inLeftFloat Left value from which the right float is divided.
 *  \param inRightFloat Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Float operator/=(Beagle::Float& inLeftFloat, float inRightFloat)
{
	Beagle_StackTraceBeginM();
	inLeftFloat.getWrappedValue() /= inRightFloat;
	return inLeftFloat;
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_Float_hpp



