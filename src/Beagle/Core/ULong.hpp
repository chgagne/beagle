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
 *  \file   Beagle/Core/ULong.hpp
 *  \brief  Definition of the type ULong and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_ULong_hpp
#define Beagle_Core_ULong_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic unsigned long type.
 *  \ingroup Adapt
 */
typedef WrapperT< unsigned long >
ULong;

/*!
 *  \brief Beagle array of the atomic unsigned long type.
 *  \ingroup Adapt
 */
typedef ArrayT< unsigned long >
ULongArray;

}


/*!
 *  \brief  Test whether an unsigned long is less than another.
 *  \param  inLeftULong  Left unsigned long compared.
 *  \param  inRightULong Right unsigned long compared.
 *  \return True if left unsigned long is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::ULong.
 */
inline bool operator<(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.isLess(inRightULong);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an unsigned long is less than, or equal to another.
 *  \param  inLeftULong  Left unsigned long compared.
 *  \param  inRightULong Right unsigned long compared.
 *  \return True if left unsigned long is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::ULong.
 */
inline bool operator<=(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return ( inLeftULong.isLess(inRightULong) || inLeftULong.isEqual(inRightULong) );
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an unsigned long is more than another.
 *  \param  inLeftULong  Left unsigned long compared.
 *  \param  inRightULong Right unsigned long compared.
 *  \return True if left unsigned long is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::ULong.
 */
inline bool operator>(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inRightULong.isLess(inLeftULong);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an unsigned long is more than, or equal to another.
 *  \param  inLeftULong  Left unsigned long compared.
 *  \param  inRightULong Right unsigned long compared.
 *  \return True if left unsigned long is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::ULong.
 */
inline bool operator>=(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return ( inRightULong.isLess(inLeftULong) || inLeftULong.isEqual(inRightULong) );
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an unsigned long is equal to another.
 *  \param  inLeftULong  Left unsigned long compared.
 *  \param  inRightULong Right unsigned long compared.
 *  \return True if left unsigned long is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::ULong.
 */
inline bool operator==(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.isEqual(inRightULong);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test whether an unsigned long is not equal to another.
 *  \param  inLeftULong  Left unsigned long compared.
 *  \param  inRightULong Right unsigned long compared.
 *  \return True if left unsigned long is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::ULong.
 */
inline bool operator!=(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return ( inLeftULong.isEqual(inRightULong) == false);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compare equality of a ULong with a unsigned long.
 *  \param inLeftULong Left value to compare.
 *  \param inRightULong Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator==(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() == inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compare inequality of a ULong with a unsigned long.
 *  \param inLeftULong Left value to compare.
 *  \param inRightULong Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator!=(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() != inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a ULong is < than a unsigned long.
 *  \param inLeftULong Left value to compare.
 *  \param inRightULong Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() < inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a ULong is <= than a unsigned long.
 *  \param inLeftULong Left value to compare.
 *  \param inRightULong Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<=(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() <= inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a ULong is > than a unsigned long.
 *  \param inLeftULong Left value to compare.
 *  \param inRightULong Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() > inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test if a ULong is >= than a unsigned long.
 *  \param inLeftULong Left value to compare.
 *  \param inRightULong Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>=(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() >= inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Increment a ULong (prefix version).
 *  \param inULong ULong to increment.
 *  \return ULong incremented.
 */
inline
Beagle::ULong& operator++(Beagle::ULong& inULong)
{
	Beagle_StackTraceBeginM();
	inULong.getWrappedValue()++;
	return inULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Increment a ULong (postfix version).
 *  \param inULong ULong to increment.
 *  \return ULong before being incremented.
 */
inline
Beagle::ULong operator++(Beagle::ULong& inULong, int)
{
	Beagle_StackTraceBeginM();
	Beagle::ULong lULong = inULong;
	inULong.getWrappedValue()++;
	return lULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add two ULong.
 *  \param inLeftULong Left value to add.
 *  \param inRightULong Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::ULong operator+(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() + inRightULong.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a ULong with a unsigned long.
 *  \param inLeftULong Left value to add.
 *  \param inRightULong Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::ULong operator+(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() + inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a ULong to a ULong.
 *  \param inLeftULong Left value to which the right one is added.
 *  \param inRightULong Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::ULong& operator+=(Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() += inRightULong.getWrappedValue();
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a unsigned long to a ULong.
 *  \param inLeftULong Left value to which the right one is added.
 *  \param inRightULong Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::ULong& operator+=(Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() += inRightULong;
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Decrement a ULong (prefix version).
 *  \param inULong ULong to decrement.
 *  \return ULong decremented.
 */
inline
Beagle::ULong& operator--(Beagle::ULong& inULong)
{
	Beagle_StackTraceBeginM();
	inULong.getWrappedValue()--;
	return inULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Decrement a ULong (postfix version).
 *  \param inULong ULong to decrement.
 *  \return ULong before being decremented.
 */
inline
Beagle::ULong operator--(Beagle::ULong& inULong, int)
{
	Beagle_StackTraceBeginM();
	Beagle::ULong lULong = inULong;
	inULong.getWrappedValue()--;
	return lULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract two ULong.
 *  \param inLeftULong Left value to subtract.
 *  \param inRightULong Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::ULong operator-(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() - inRightULong.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract a unsigned long from a ULong.
 *  \param inLeftULong Left value to subtract.
 *  \param inRightULong Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::ULong operator-(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() - inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract a ULong from a ULong.
 *  \param inLeftULong Left value from which the right unsigned long is subtracted.
 *  \param inRightULong Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::ULong& operator-=(Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() -= inRightULong.getWrappedValue();
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Subtract a unsigned long from a ULong.
 *  \param inLeftULong Left value from which the right unsigned long is subtracted.
 *  \param inRightULong Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::ULong& operator-=(Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() -= inRightULong;
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply two ULong.
 *  \param inLeftULong Left value to multiply.
 *  \param inRightULong Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::ULong operator*(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() * inRightULong.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply a ULong with a unsigned long.
 *  \param inLeftULong Left value to multiply.
 *  \param inRightULong Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::ULong operator*(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() * inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply a ULong with a ULong.
 *  \param inLeftULong Left value to which the right unsigned long is multiplied.
 *  \param inRightULong Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::ULong& operator*=(Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() *= inRightULong.getWrappedValue();
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Multiply a ULong with a unsigned long.
 *  \param inLeftULong Left value from which the right unsigned long is multiplied.
 *  \param inRightULong Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::ULong& operator*=(Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() *= inRightULong;
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide two ULong.
 *  \param inLeftULong Left value to divide.
 *  \param inRightULong Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::ULong operator/(const Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() / inRightULong.getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide a ULong with a unsigned long.
 *  \param inLeftULong Left value to divide.
 *  \param inRightULong Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::ULong operator/(const Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	return inLeftULong.getWrappedValue() / inRightULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide a ULong with a ULong.
 *  \param inLeftULong Left value to which the right unsigned long is divided.
 *  \param inRightULong Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::ULong& operator/=(Beagle::ULong& inLeftULong, const Beagle::ULong& inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() /= inRightULong.getWrappedValue();
	return inLeftULong;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Divide a ULong with a unsigned long.
 *  \param inLeftULong Left value from which the right unsigned long is divided.
 *  \param inRightULong Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::ULong operator/=(Beagle::ULong& inLeftULong, unsigned long inRightULong)
{
	Beagle_StackTraceBeginM();
	inLeftULong.getWrappedValue() /= inRightULong;
	return inLeftULong;
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_ULong_hpp



