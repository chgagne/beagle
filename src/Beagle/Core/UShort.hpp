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
 *  \file   Beagle/Core/UShort.hpp
 *  \brief  Definition of the type UShort and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_UShort_hpp
#define Beagle_Core_UShort_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"


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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_UShort_hpp



