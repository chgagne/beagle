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
 *  \file   beagle/Core/UShort.hpp
 *  \brief  Definition of the type UShort and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_UShort_hpp
#define Beagle_Core_UShort_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/ArrayT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic unsigned short type.
 *  \ingroup Adapt
 */
typedef WrapperT< unsigned short >
UShort;

/*!
 *  \brief Beagle array of the atomic unsigned short type.
 *  \ingroup Adapt
 */
typedef ArrayT< unsigned short >
UShortArray;

}


/*!
 *  \brief  Test whether an unsigned short is less than another.
 *  \param  inLeftUShort  Left unsigned short compared.
 *  \param  inRightUShort Right unsigned short compared.
 *  \return True if left unsigned short is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::UShort.
 */
inline bool operator<(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.isLess(inRightUShort);
	Beagle_StackTraceEndM("bool operator<(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief  Test whether an unsigned short is less than, or equal to another.
 *  \param  inLeftUShort  Left unsigned short compared.
 *  \param  inRightUShort Right unsigned short compared.
 *  \return True if left unsigned short is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::UShort.
 */
inline bool operator<=(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return ( inLeftUShort.isLess(inRightUShort) || inLeftUShort.isEqual(inRightUShort) );
	Beagle_StackTraceEndM("bool operator<=(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief  Test whether an unsigned short is more than another.
 *  \param  inLeftUShort  Left unsigned short compared.
 *  \param  inRightUShort Right unsigned short compared.
 *  \return True if left unsigned short is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::UShort.
 */
inline bool operator>(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inRightUShort.isLess(inLeftUShort);
	Beagle_StackTraceEndM("bool operator>(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief  Test whether an unsigned short is more than, or equal to another.
 *  \param  inLeftUShort  Left unsigned short compared.
 *  \param  inRightUShort Right unsigned short compared.
 *  \return True if left unsigned short is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::UShort.
 */
inline bool operator>=(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return ( inRightUShort.isLess(inLeftUShort) || inLeftUShort.isEqual(inRightUShort) );
	Beagle_StackTraceEndM("bool operator>=(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief  Test whether an unsigned short is equal to another.
 *  \param  inLeftUShort  Left unsigned short compared.
 *  \param  inRightUShort Right unsigned short compared.
 *  \return True if left unsigned short is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::UShort.
 */
inline bool operator==(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.isEqual(inRightUShort);
	Beagle_StackTraceEndM("bool operator==(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief  Test whether an unsigned short is not equal to another.
 *  \param  inLeftUShort  Left unsigned short compared.
 *  \param  inRightUShort Right unsigned short compared.
 *  \return True if left unsigned short is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::UShort.
 */
inline bool operator!=(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return ( inLeftUShort.isEqual(inRightUShort) == false);
	Beagle_StackTraceEndM("bool operator!=(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Compare equality of a UShort with a unsigned short.
 *  \param inLeftUShort Left value to compare.
 *  \param inRightUShort Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator==(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() == inRightUShort;
	Beagle_StackTraceEndM("bool operator==(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Compare inequality of a UShort with a unsigned short.
 *  \param inLeftUShort Left value to compare.
 *  \param inRightUShort Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator!=(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() != inRightUShort;
	Beagle_StackTraceEndM("bool operator!=(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Test if a UShort is < than a unsigned short.
 *  \param inLeftUShort Left value to compare.
 *  \param inRightUShort Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() < inRightUShort;
	Beagle_StackTraceEndM("bool operator<(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Test if a UShort is <= than a unsigned short.
 *  \param inLeftUShort Left value to compare.
 *  \param inRightUShort Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<=(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() <= inRightUShort;
	Beagle_StackTraceEndM("bool operator<=(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Test if a UShort is > than a unsigned short.
 *  \param inLeftUShort Left value to compare.
 *  \param inRightUShort Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() > inRightUShort;
	Beagle_StackTraceEndM("bool operator>(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Test if a UShort is >= than a unsigned short.
 *  \param inLeftUShort Left value to compare.
 *  \param inRightUShort Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>=(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() >= inRightUShort;
	Beagle_StackTraceEndM("bool operator>=(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Increment a UShort (prefix version).
 *  \param inUShort UShort to increment.
 *  \return UShort incremented.
 */
inline
Beagle::UShort& operator++(Beagle::UShort& inUShort)
{
	Beagle_StackTraceBeginM();
	inUShort.getWrappedValue()++;
	return inUShort;
	Beagle_StackTraceEndM("UShort& operator++(UShort& inUShort)");
}


/*!
 *  \brief Increment a UShort (postfix version).
 *  \param inUShort UShort to increment.
 *  \return UShort before being incremented.
 */
inline
Beagle::UShort operator++(Beagle::UShort& inUShort, int)
{
	Beagle_StackTraceBeginM();
	Beagle::UShort lUShort = inUShort;
	inUShort.getWrappedValue()++;
	return lUShort;
	Beagle_StackTraceEndM("UShort operator++(UShort& inUShort, int)");
}


/*!
 *  \brief Add two UShort.
 *  \param inLeftUShort Left value to add.
 *  \param inRightUShort Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UShort operator+(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() + inRightUShort.getWrappedValue();
	Beagle_StackTraceEndM("UShort operator+(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Add a UShort with a unsigned short.
 *  \param inLeftUShort Left value to add.
 *  \param inRightUShort Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UShort operator+(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() + inRightUShort;
	Beagle_StackTraceEndM("UShort operator+(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Add a UShort to a UShort.
 *  \param inLeftUShort Left value to which the right one is added.
 *  \param inRightUShort Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UShort& operator+=(Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() += inRightUShort.getWrappedValue();
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator+=(UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Add a unsigned short to a UShort.
 *  \param inLeftUShort Left value to which the right one is added.
 *  \param inRightUShort Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::UShort& operator+=(Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() += inRightUShort;
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator+=(UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Decrement a UShort (prefix version).
 *  \param inUShort UShort to decrement.
 *  \return UShort decremented.
 */
inline
Beagle::UShort& operator--(Beagle::UShort& inUShort)
{
	Beagle_StackTraceBeginM();
	inUShort.getWrappedValue()--;
	return inUShort;
	Beagle_StackTraceEndM("UShort& operator--(UShort& inUShort)");
}


/*!
 *  \brief Decrement a UShort (postfix version).
 *  \param inUShort UShort to decrement.
 *  \return UShort before being decremented.
 */
inline
Beagle::UShort operator--(Beagle::UShort& inUShort, int)
{
	Beagle_StackTraceBeginM();
	Beagle::UShort lUShort = inUShort;
	inUShort.getWrappedValue()--;
	return lUShort;
	Beagle_StackTraceEndM("UShort operator--(UShort& inUShort, int)");
}



/*!
 *  \brief Subtract two UShort.
 *  \param inLeftUShort Left value to subtract.
 *  \param inRightUShort Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::UShort operator-(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() - inRightUShort.getWrappedValue();
	Beagle_StackTraceEndM("UShort operator-(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Subtract a unsigned short from a UShort.
 *  \param inLeftUShort Left value to subtract.
 *  \param inRightUShort Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::UShort operator-(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() - inRightUShort;
	Beagle_StackTraceEndM("UShort operator-(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Subtract a UShort from a UShort.
 *  \param inLeftUShort Left value from which the right unsigned short is subtracted.
 *  \param inRightUShort Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::UShort& operator-=(Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() -= inRightUShort.getWrappedValue();
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator-=(UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Subtract a unsigned short from a UShort.
 *  \param inLeftUShort Left value from which the right unsigned short is subtracted.
 *  \param inRightUShort Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::UShort& operator-=(Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() -= inRightUShort;
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator-=(UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Multiply two UShort.
 *  \param inLeftUShort Left value to multiply.
 *  \param inRightUShort Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::UShort operator*(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() * inRightUShort.getWrappedValue();
	Beagle_StackTraceEndM("UShort operator*(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Multiply a UShort with a unsigned short.
 *  \param inLeftUShort Left value to multiply.
 *  \param inRightUShort Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::UShort operator*(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() * inRightUShort;
	Beagle_StackTraceEndM("UShort operator*(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Multiply a UShort with a UShort.
 *  \param inLeftUShort Left value to which the right unsigned short is multiplied.
 *  \param inRightUShort Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::UShort& operator*=(Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() *= inRightUShort.getWrappedValue();
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator*=(UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Multiply a UShort with a unsigned short.
 *  \param inLeftUShort Left value from which the right unsigned short is multiplied.
 *  \param inRightUShort Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::UShort& operator*=(Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() *= inRightUShort;
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator*=(UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Divide two UShort.
 *  \param inLeftUShort Left value to divide.
 *  \param inRightUShort Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UShort operator/(const Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() / inRightUShort.getWrappedValue();
	Beagle_StackTraceEndM("UShort operator/(const UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Divide a UShort with a unsigned short.
 *  \param inLeftUShort Left value to divide.
 *  \param inRightUShort Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UShort operator/(const Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	return inLeftUShort.getWrappedValue() / inRightUShort;
	Beagle_StackTraceEndM("UShort operator/(const UShort& inLeftUShort, unsigned short inRightUShort)");
}


/*!
 *  \brief Divide a UShort with a UShort.
 *  \param inLeftUShort Left value to which the right unsigned short is divided.
 *  \param inRightUShort Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UShort& operator/=(Beagle::UShort& inLeftUShort, const Beagle::UShort& inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() /= inRightUShort.getWrappedValue();
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort& operator/=(UShort& inLeftUShort, const UShort& inRightUShort)");
}


/*!
 *  \brief Divide a UShort with a unsigned short.
 *  \param inLeftUShort Left value from which the right unsigned short is divided.
 *  \param inRightUShort Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::UShort operator/=(Beagle::UShort& inLeftUShort, unsigned short inRightUShort)
{
	Beagle_StackTraceBeginM();
	inLeftUShort.getWrappedValue() /= inRightUShort;
	return inLeftUShort;
	Beagle_StackTraceEndM("UShort operator/=(UShort& inLeftUShort, unsigned short inRightUShort)");
}


#endif // Beagle_Core_UShort_hpp



