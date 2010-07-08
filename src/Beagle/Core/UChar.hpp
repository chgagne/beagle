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
 *  \file   Beagle/Core/UChar.hpp
 *  \brief  Definition of the type UChar and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_UChar_hpp
#define Beagle_Core_UChar_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic unsigned char type.
 *  \ingroup Adapt
 */
typedef WrapperT< unsigned char >
UChar;

}


/*!
 *  \brief  Test whether an unsigned char is less than another.
 *  \param  inLeftUChar  Left unsigned char compared.
 *  \param  inRightUChar Right unsigned char compared.
 *  \return True if left unsigned char is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::UChar.
 */
inline bool operator<(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.isLess(inRightUChar);
	Beagle_StackTraceEndM("bool operator<(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief  Test whether an unsigned char is less than, or equal to another.
 *  \param  inLeftUChar  Left unsigned char compared.
 *  \param  inRightUChar Right unsigned char compared.
 *  \return True if left unsigned char is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::UChar.
 */
inline bool operator<=(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return ( inLeftUChar.isLess(inRightUChar) || inLeftUChar.isEqual(inRightUChar) );
	Beagle_StackTraceEndM("bool operator<=(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief  Test whether an unsigned char is more than another.
 *  \param  inLeftUChar  Left unsigned char compared.
 *  \param  inRightUChar Right unsigned char compared.
 *  \return True if left unsigned char is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::UChar.
 */
inline bool operator>(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inRightUChar.isLess(inLeftUChar);
	Beagle_StackTraceEndM("bool operator>(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief  Test whether an unsigned char is more than, or equal to another.
 *  \param  inLeftUChar  Left unsigned char compared.
 *  \param  inRightUChar Right unsigned char compared.
 *  \return True if left unsigned char is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::UChar.
 */
inline bool operator>=(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return ( inRightUChar.isLess(inLeftUChar) || inLeftUChar.isEqual(inRightUChar) );
	Beagle_StackTraceEndM("bool operator>=(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief  Test whether an unsigned char is equal to another.
 *  \param  inLeftUChar  Left unsigned char compared.
 *  \param  inRightUChar Right unsigned char compared.
 *  \return True if left unsigned char is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::UChar.
 */
inline bool operator==(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.isEqual(inRightUChar);
	Beagle_StackTraceEndM("bool operator==(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief  Test whether an unsigned char is not equal to another.
 *  \param  inLeftUChar  Left unsigned char compared.
 *  \param  inRightUChar Right unsigned char compared.
 *  \return True if left unsigned char is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::UChar.
 */
inline bool operator!=(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return ( inLeftUChar.isEqual(inRightUChar) == false);
	Beagle_StackTraceEndM("bool operator!=(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Compare equality of a UChar with a unsigned char.
 *  \param inLeftUChar Left value to compare.
 *  \param inRightUChar Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator==(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() == inRightUChar;
	Beagle_StackTraceEndM("bool operator==(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Compare inequality of a UChar with a unsigned char.
 *  \param inLeftUChar Left value to compare.
 *  \param inRightUChar Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator!=(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() != inRightUChar;
	Beagle_StackTraceEndM("bool operator!=(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Test if a UChar is < than a unsigned char.
 *  \param inLeftUChar Left value to compare.
 *  \param inRightUChar Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() < inRightUChar;
	Beagle_StackTraceEndM("bool operator<(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Test if a UChar is <= than a unsigned char.
 *  \param inLeftUChar Left value to compare.
 *  \param inRightUChar Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<=(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() <= inRightUChar;
	Beagle_StackTraceEndM("bool operator<=(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Test if a UChar is > than a unsigned char.
 *  \param inLeftUChar Left value to compare.
 *  \param inRightUChar Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() > inRightUChar;
	Beagle_StackTraceEndM("bool operator>(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Test if a UChar is >= than a unsigned char.
 *  \param inLeftUChar Left value to compare.
 *  \param inRightUChar Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>=(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() >= inRightUChar;
	Beagle_StackTraceEndM("bool operator>=(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Increment a UChar (prefix version).
 *  \param inUChar UChar to increment.
 *  \return UChar incremented.
 */
inline
Beagle::UChar& operator++(Beagle::UChar& inUChar)
{
	Beagle_StackTraceBeginM();
	inUChar.getWrappedValue()++;
	return inUChar;
	Beagle_StackTraceEndM("UChar& operator++(UChar& inUChar)");
}


/*!
 *  \brief Increment a UChar (postfix version).
 *  \param inUChar UChar to increment.
 *  \return UChar before being incremented.
 */
inline
Beagle::UChar operator++(Beagle::UChar& inUChar, int)
{
	Beagle_StackTraceBeginM();
	Beagle::UChar lUChar = inUChar;
	inUChar.getWrappedValue()++;
	return lUChar;
	Beagle_StackTraceEndM("UChar operator++(UChar& inUChar, int)");
}


/*!
 *  \brief Add two UChar.
 *  \param inLeftUChar Left value to add.
 *  \param inRightUChar Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UChar operator+(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() + inRightUChar.getWrappedValue();
	Beagle_StackTraceEndM("UChar operator+(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Add a UChar with a unsigned char.
 *  \param inLeftUChar Left value to add.
 *  \param inRightUChar Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UChar operator+(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() + inRightUChar;
	Beagle_StackTraceEndM("UChar operator+(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Add a UChar to a UChar.
 *  \param inLeftUChar Left value to which the right one is added.
 *  \param inRightUChar Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UChar& operator+=(Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() += inRightUChar.getWrappedValue();
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator+=(UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Add a unsigned char to a UChar.
 *  \param inLeftUChar Left value to which the right one is added.
 *  \param inRightUChar Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UChar& operator+=(Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() += inRightUChar;
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator+=(UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Decrement a UChar (prefix version).
 *  \param inUChar UChar to decrement.
 *  \return UChar decremented.
 */
inline
Beagle::UChar& operator--(Beagle::UChar& inUChar)
{
	Beagle_StackTraceBeginM();
	inUChar.getWrappedValue()--;
	return inUChar;
	Beagle_StackTraceEndM("UChar& operator--(UChar& inUChar)");
}


/*!
 *  \brief Decrement a UChar (postfix version).
 *  \param inUChar UChar to decrement.
 *  \return UChar before being decremented.
 */
inline
Beagle::UChar operator--(Beagle::UChar& inUChar, int)
{
	Beagle_StackTraceBeginM();
	Beagle::UChar lUChar = inUChar;
	inUChar.getWrappedValue()--;
	return lUChar;
	Beagle_StackTraceEndM("UChar operator--(UChar& inUChar, int)");
}



/*!
 *  \brief Subtract two UChar.
 *  \param inLeftUChar Left value to subtract.
 *  \param inRightUChar Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::UChar operator-(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() - inRightUChar.getWrappedValue();
	Beagle_StackTraceEndM("UChar operator-(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Subtract a unsigned char from a UChar.
 *  \param inLeftUChar Left value to subtract.
 *  \param inRightUChar Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::UChar operator-(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() - inRightUChar;
	Beagle_StackTraceEndM("UChar operator-(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Subtract a UChar from a UChar.
 *  \param inLeftUChar Left value from which the right unsigned char is subtracted.
 *  \param inRightUChar Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::UChar& operator-=(Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() -= inRightUChar.getWrappedValue();
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator-=(UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Subtract a unsigned char from a UChar.
 *  \param inLeftUChar Left value from which the right unsigned char is subtracted.
 *  \param inRightUChar Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::UChar& operator-=(Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() -= inRightUChar;
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator-=(UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Multiply two UChar.
 *  \param inLeftUChar Left value to multiply.
 *  \param inRightUChar Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::UChar operator*(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() * inRightUChar.getWrappedValue();
	Beagle_StackTraceEndM("UChar operator*(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Multiply a UChar with a unsigned char.
 *  \param inLeftUChar Left value to multiply.
 *  \param inRightUChar Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::UChar operator*(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() * inRightUChar;
	Beagle_StackTraceEndM("UChar operator*(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Multiply a UChar with a UChar.
 *  \param inLeftUChar Left value to which the right unsigned char is multiplied.
 *  \param inRightUChar Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::UChar& operator*=(Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() *= inRightUChar.getWrappedValue();
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator*=(UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Multiply a UChar with a unsigned char.
 *  \param inLeftUChar Left value from which the right unsigned char is multiplied.
 *  \param inRightUChar Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::UChar& operator*=(Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() *= inRightUChar;
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator*=(UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Divide two UChar.
 *  \param inLeftUChar Left value to divide.
 *  \param inRightUChar Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UChar operator/(const Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() / inRightUChar.getWrappedValue();
	Beagle_StackTraceEndM("UChar operator/(const UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Divide a UChar with a unsigned char.
 *  \param inLeftUChar Left value to divide.
 *  \param inRightUChar Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UChar operator/(const Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	return inLeftUChar.getWrappedValue() / inRightUChar;
	Beagle_StackTraceEndM("UChar operator/(const UChar& inLeftUChar, unsigned char inRightUChar)");
}


/*!
 *  \brief Divide a UChar with a UChar.
 *  \param inLeftUChar Left value to which the right unsigned char is divided.
 *  \param inRightUChar Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UChar& operator/=(Beagle::UChar& inLeftUChar, const Beagle::UChar& inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() /= inRightUChar.getWrappedValue();
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar& operator/=(UChar& inLeftUChar, const UChar& inRightUChar)");
}


/*!
 *  \brief Divide a UChar with a unsigned char.
 *  \param inLeftUChar Left value from which the right unsigned char is divided.
 *  \param inRightUChar Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UChar operator/=(Beagle::UChar& inLeftUChar, unsigned char inRightUChar)
{
	Beagle_StackTraceBeginM();
	inLeftUChar.getWrappedValue() /= inRightUChar;
	return inLeftUChar;
	Beagle_StackTraceEndM("UChar operator/=(UChar& inLeftUChar, unsigned char inRightUChar)");
}


#endif // Beagle_Core_UChar_hpp



