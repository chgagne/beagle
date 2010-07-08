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
 *  \file   Beagle/Core/UInt.hpp
 *  \brief  Definition of the type UInt and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_UInt_hpp
#define Beagle_Core_UInt_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic unsigned int type.
 *  \ingroup Adapt
 */
typedef WrapperT< unsigned int >
UInt;

/*!
 *  \brief Beagle array of the atomic unsigned int type.
 *  \ingroup Adapt
 */
typedef ArrayT< unsigned int >
UIntArray;

}


/*!
 *  \brief  Test whether an unsigned int is less than another.
 *  \param  inLeftUInt  Left unsigned int compared.
 *  \param  inRightUInt Right unsigned int compared.
 *  \return True if left unsigned int is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::UInt.
 */
inline bool operator<(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.isLess(inRightUInt);
	Beagle_StackTraceEndM("bool operator<(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief  Test whether an unsigned int is less than, or equal to another.
 *  \param  inLeftUInt  Left unsigned int compared.
 *  \param  inRightUInt Right unsigned int compared.
 *  \return True if left unsigned int is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::UInt.
 */
inline bool operator<=(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return ( inLeftUInt.isLess(inRightUInt) || inLeftUInt.isEqual(inRightUInt) );
	Beagle_StackTraceEndM("bool operator<=(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief  Test whether an unsigned int is more than another.
 *  \param  inLeftUInt  Left unsigned int compared.
 *  \param  inRightUInt Right unsigned int compared.
 *  \return True if left unsigned int is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::UInt.
 */
inline bool operator>(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inRightUInt.isLess(inLeftUInt);
	Beagle_StackTraceEndM("bool operator>(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief  Test whether an unsigned int is more than, or equal to another.
 *  \param  inLeftUInt  Left unsigned int compared.
 *  \param  inRightUInt Right unsigned int compared.
 *  \return True if left unsigned int is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::UInt.
 */
inline bool operator>=(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return ( inRightUInt.isLess(inLeftUInt) || inLeftUInt.isEqual(inRightUInt) );
	Beagle_StackTraceEndM("bool operator>=(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief  Test whether an unsigned int is equal to another.
 *  \param  inLeftUInt  Left unsigned int compared.
 *  \param  inRightUInt Right unsigned int compared.
 *  \return True if left unsigned int is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::UInt.
 */
inline bool operator==(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.isEqual(inRightUInt);
	Beagle_StackTraceEndM("bool operator==(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief  Test whether an unsigned int is not equal to another.
 *  \param  inLeftUInt  Left unsigned int compared.
 *  \param  inRightUInt Right unsigned int compared.
 *  \return True if left unsigned int is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::UInt.
 */
inline bool operator!=(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return ( inLeftUInt.isEqual(inRightUInt) == false);
	Beagle_StackTraceEndM("bool operator!=(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Compare equality of a UInt with a unsigned int.
 *  \param inLeftUInt Left value to compare.
 *  \param inRightUInt Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator==(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() == inRightUInt;
	Beagle_StackTraceEndM("bool operator==(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Compare inequality of a UInt with a unsigned int.
 *  \param inLeftUInt Left value to compare.
 *  \param inRightUInt Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator!=(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() != inRightUInt;
	Beagle_StackTraceEndM("bool operator!=(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Test if a UInt is < than a unsigned int.
 *  \param inLeftUInt Left value to compare.
 *  \param inRightUInt Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() < inRightUInt;
	Beagle_StackTraceEndM("bool operator<(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Test if a UInt is <= than a unsigned int.
 *  \param inLeftUInt Left value to compare.
 *  \param inRightUInt Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<=(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() <= inRightUInt;
	Beagle_StackTraceEndM("bool operator<=(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Test if a UInt is > than a unsigned int.
 *  \param inLeftUInt Left value to compare.
 *  \param inRightUInt Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() > inRightUInt;
	Beagle_StackTraceEndM("bool operator>(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Test if a UInt is >= than a unsigned int.
 *  \param inLeftUInt Left value to compare.
 *  \param inRightUInt Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>=(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() >= inRightUInt;
	Beagle_StackTraceEndM("bool operator>=(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Increment a UInt (prefix version).
 *  \param inUInt UInt to increment.
 *  \return UInt incremented.
 */
inline
Beagle::UInt& operator++(Beagle::UInt& inUInt)
{
	Beagle_StackTraceBeginM();
	inUInt.getWrappedValue()++;
	return inUInt;
	Beagle_StackTraceEndM("UInt& operator++(UInt& inUInt)");
}


/*!
 *  \brief Increment a UInt (postfix version).
 *  \param inUInt UInt to increment.
 *  \return UInt before being incremented.
 */
inline
Beagle::UInt operator++(Beagle::UInt& inUInt, int)
{
	Beagle_StackTraceBeginM();
	Beagle::UInt lUInt = inUInt;
	inUInt.getWrappedValue()++;
	return lUInt;
	Beagle_StackTraceEndM("UInt operator++(UInt& inUInt, int)");
}


/*!
 *  \brief Add two UInt.
 *  \param inLeftUInt Left value to add.
 *  \param inRightUInt Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UInt operator+(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() + inRightUInt.getWrappedValue();
	Beagle_StackTraceEndM("UInt operator+(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Add a UInt with a unsigned int.
 *  \param inLeftUInt Left value to add.
 *  \param inRightUInt Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UInt operator+(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() + inRightUInt;
	Beagle_StackTraceEndM("UInt operator+(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Add a UInt to a UInt.
 *  \param inLeftUInt Left value to which the right one is added.
 *  \param inRightUInt Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UInt& operator+=(Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() += inRightUInt.getWrappedValue();
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator+=(UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Add a unsigned int to a UInt.
 *  \param inLeftUInt Left value to which the right one is added.
 *  \param inRightUInt Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UInt& operator+=(Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() += inRightUInt;
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator+=(UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Decrement a UInt (prefix version).
 *  \param inUInt UInt to decrement.
 *  \return UInt decremented.
 */
inline
Beagle::UInt& operator--(Beagle::UInt& inUInt)
{
	Beagle_StackTraceBeginM();
	inUInt.getWrappedValue()--;
	return inUInt;
	Beagle_StackTraceEndM("UInt& operator--(UInt& inUInt)");
}


/*!
 *  \brief Decrement a UInt (postfix version).
 *  \param inUInt UInt to decrement.
 *  \return UInt before being decremented.
 */
inline
Beagle::UInt operator--(Beagle::UInt& inUInt, int)
{
	Beagle_StackTraceBeginM();
	Beagle::UInt lUInt = inUInt;
	inUInt.getWrappedValue()--;
	return lUInt;
	Beagle_StackTraceEndM("UInt operator--(UInt& inUInt, int)");
}


/*!
 *  \brief Subtract two UInt.
 *  \param inLeftUInt Left value to subtract.
 *  \param inRightUInt Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::UInt operator-(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() - inRightUInt.getWrappedValue();
	Beagle_StackTraceEndM("UInt operator-(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Subtract a unsigned int from a UInt.
 *  \param inLeftUInt Left value to subtract.
 *  \param inRightUInt Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::UInt operator-(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() - inRightUInt;
	Beagle_StackTraceEndM("UInt operator-(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Subtract a UInt from a UInt.
 *  \param inLeftUInt Left value from which the right unsigned int is subtracted.
 *  \param inRightUInt Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::UInt& operator-=(Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() -= inRightUInt.getWrappedValue();
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator-=(UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Subtract a unsigned int from a UInt.
 *  \param inLeftUInt Left value from which the right unsigned int is subtracted.
 *  \param inRightUInt Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::UInt& operator-=(Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() -= inRightUInt;
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator-=(UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Multiply two UInt.
 *  \param inLeftUInt Left value to multiply.
 *  \param inRightUInt Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::UInt operator*(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() * inRightUInt.getWrappedValue();
	Beagle_StackTraceEndM("UInt operator*(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Multiply a UInt with a unsigned int.
 *  \param inLeftUInt Left value to multiply.
 *  \param inRightUInt Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::UInt operator*(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() * inRightUInt;
	Beagle_StackTraceEndM("UInt operator*(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Multiply a UInt with a UInt.
 *  \param inLeftUInt Left value to which the right unsigned int is multiplied.
 *  \param inRightUInt Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::UInt& operator*=(Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() *= inRightUInt.getWrappedValue();
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator*=(UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Multiply a UInt with a unsigned int.
 *  \param inLeftUInt Left value from which the right unsigned int is multiplied.
 *  \param inRightUInt Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::UInt& operator*=(Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() *= inRightUInt;
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator*=(UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Divide two UInt.
 *  \param inLeftUInt Left value to divide.
 *  \param inRightUInt Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UInt operator/(const Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() / inRightUInt.getWrappedValue();
	Beagle_StackTraceEndM("UInt operator/(const UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Divide a UInt with a unsigned int.
 *  \param inLeftUInt Left value to divide.
 *  \param inRightUInt Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UInt operator/(const Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	return inLeftUInt.getWrappedValue() / inRightUInt;
	Beagle_StackTraceEndM("UInt operator/(const UInt& inLeftUInt, unsigned int inRightUInt)");
}


/*!
 *  \brief Divide a UInt with a UInt.
 *  \param inLeftUInt Left value to which the right unsigned int is divided.
 *  \param inRightUInt Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UInt& operator/=(Beagle::UInt& inLeftUInt, const Beagle::UInt& inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() /= inRightUInt.getWrappedValue();
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt& operator/=(UInt& inLeftUInt, const UInt& inRightUInt)");
}


/*!
 *  \brief Divide a UInt with a unsigned int.
 *  \param inLeftUInt Left value from which the right unsigned int is divided.
 *  \param inRightUInt Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UInt operator/=(Beagle::UInt& inLeftUInt, unsigned int inRightUInt)
{
	Beagle_StackTraceBeginM();
	inLeftUInt.getWrappedValue() /= inRightUInt;
	return inLeftUInt;
	Beagle_StackTraceEndM("UInt operator/=(UInt& inLeftUInt, unsigned int inRightUInt)");
}


#endif // Beagle_Core_UInt_hpp



