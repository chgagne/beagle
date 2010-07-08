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
 *  \file   Beagle/Core/Char.hpp
 *  \brief  Definition of the type Char and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:41 $
 */

#ifndef Beagle_Core_Char_hpp
#define Beagle_Core_Char_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic char type.
 *  \ingroup Adapt
 */
typedef WrapperT< char >
Char;

}


/*!
 *  \brief     Test whether an char is less than another.
 *  \param     inLeftChar  Left char compared.
 *  \param     inRightChar Right char compared.
 *  \return    True if left char is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::Char.
 */
inline bool operator<(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.isLess(inRightChar);
	Beagle_StackTraceEndM("bool operator<(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief     Test whether an char is less than, or equal to another.
 *  \param     inLeftChar  Left char compared.
 *  \param     inRightChar Right char compared.
 *  \return    True if left char is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::Char.
 */
inline bool operator<=(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return ( inLeftChar.isLess(inRightChar) || inLeftChar.isEqual(inRightChar) );
	Beagle_StackTraceEndM("bool operator<=(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief     Test whether an char is more than another.
 *  \param     inLeftChar  Left char compared.
 *  \param     inRightChar Right char compared.
 *  \return    True if left char is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::Char.
 */
inline bool operator>(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inRightChar.isLess(inLeftChar);
	Beagle_StackTraceEndM("bool operator>(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief     Test whether an char is more than, or equal to another.
 *  \param     inLeftChar  Left char compared.
 *  \param     inRightChar Right char compared.
 *  \return    True if left char is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::Char.
 */
inline bool operator>=(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return ( inRightChar.isLess(inLeftChar) || inLeftChar.isEqual(inRightChar) );
	Beagle_StackTraceEndM("bool operator>=(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief     Test whether an char is equal to another.
 *  \param     inLeftChar  Left char compared.
 *  \param     inRightChar Right char compared.
 *  \return    True if left char is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Char.
 */
inline bool operator==(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.isEqual(inRightChar);
	Beagle_StackTraceEndM("bool operator==(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief     Test whether an char is not equal to another.
 *  \param     inLeftChar  Left char compared.
 *  \param     inRightChar Right char compared.
 *  \return    True if left char is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Char.
 */
inline bool operator!=(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return ( inLeftChar.isEqual(inRightChar) == false);
	Beagle_StackTraceEndM("bool operator!=(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Compare equality of a Char with a char.
 *  \param   inLeftChar Left value to compare.
 *  \param   inRightChar Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator==(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() == inRightChar;
	Beagle_StackTraceEndM("bool operator==(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Compare inequality of a Char with a char.
 *  \param   inLeftChar Left value to compare.
 *  \param   inRightChar Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator!=(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() != inRightChar;
	Beagle_StackTraceEndM("bool operator!=(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Test if a Char is < than a char.
 *  \param   inLeftChar Left value to compare.
 *  \param   inRightChar Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator<(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() < inRightChar;
	Beagle_StackTraceEndM("bool operator<(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Test if a Char is <= than a char.
 *  \param   inLeftChar Left value to compare.
 *  \param   inRightChar Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator<=(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() <= inRightChar;
	Beagle_StackTraceEndM("bool operator<=(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Test if a Char is > than a char.
 *  \param   inLeftChar Left value to compare.
 *  \param   inRightChar Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator>(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() > inRightChar;
	Beagle_StackTraceEndM("bool operator>(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Test if a Char is >= than a char.
 *  \param   inLeftChar Left value to compare.
 *  \param   inRightChar Right value to compare.
 *  \return  Result of the comparison.
 */
inline
bool operator>=(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() >= inRightChar;
	Beagle_StackTraceEndM("bool operator>=(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Increment a Char (prefix version).
 *  \param   inChar Char to increment.
 *  \return  Char incremented.
 */
inline
Beagle::Char& operator++(Beagle::Char& inChar)
{
	Beagle_StackTraceBeginM();
	inChar.getWrappedValue()++;
	return inChar;
	Beagle_StackTraceEndM("Char& operator++(Char& inChar)");
}


/*!
 *  \brief   Increment a Char (postfix version).
 *  \param   inChar Char to increment.
 *  \return  Char before being incremented.
 */
inline
Beagle::Char operator++(Beagle::Char& inChar, int)
{
	Beagle_StackTraceBeginM();
	Beagle::Char lChar = inChar;
	inChar.getWrappedValue()++;
	return lChar;
	Beagle_StackTraceEndM("Char operator++(Char& inChar, int)");
}


/*!
 *  \brief   Add two Char.
 *  \param   inLeftChar Left value to add.
 *  \param   inRightChar Right value to add.
 *  \return  Result of the addition.
 */
inline
Beagle::Char operator+(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() + inRightChar.getWrappedValue();
	Beagle_StackTraceEndM("Char operator+(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Add a Char with a char.
 *  \param   inLeftChar Left value to add.
 *  \param   inRightChar Right value to add.
 *  \return  Result of the addition.
 */
inline
Beagle::Char operator+(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() + inRightChar;
	Beagle_StackTraceEndM("Char operator+(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Add a Char to a Char.
 *  \param   inLeftChar Left value to which the right one is added.
 *  \param   inRightChar Value to add.
 *  \return  Result of the addition.
 */
inline
Beagle::Char& operator+=(Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() += inRightChar.getWrappedValue();
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator+=(Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Add a char to a Char.
 *  \param   inLeftChar Left value to which the right one is added.
 *  \param   inRightChar Value to add.
 *  \return  Result of the addition.
 */
inline
Beagle::Char& operator+=(Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() += inRightChar;
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator+=(Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Decrement a Char (prefix version).
 *  \param   inChar Char to decrement.
 *  \return  Char decremented.
 */
inline
Beagle::Char& operator--(Beagle::Char& inChar)
{
	Beagle_StackTraceBeginM();
	inChar.getWrappedValue()--;
	return inChar;
	Beagle_StackTraceEndM("Char& operator--(Char& inChar)");
}


/*!
 *  \brief   Decrement a Char (postfix version).
 *  \param   inChar Char to decrement.
 *  \return  Char before being decremented.
 */
inline
Beagle::Char operator--(Beagle::Char& inChar, int)
{
	Beagle_StackTraceBeginM();
	Beagle::Char lChar = inChar;
	inChar.getWrappedValue()--;
	return lChar;
	Beagle_StackTraceEndM("Char operator--(Char& inChar, int)");
}



/*!
 *  \brief   Subtract two Char.
 *  \param   inLeftChar Left value to subtract.
 *  \param   inRightChar Right value to subtract.
 *  \return  Result of the subtract.
 */
inline
Beagle::Char operator-(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() - inRightChar.getWrappedValue();
	Beagle_StackTraceEndM("Char operator-(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Subtract a char from a Char.
 *  \param   inLeftChar Left value to subtract.
 *  \param   inRightChar Right value to subtract.
 *  \return  Result of the subtract.
 */
inline
Beagle::Char operator-(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() - inRightChar;
	Beagle_StackTraceEndM("Char operator-(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Subtract a Char from a Char.
 *  \param   inLeftChar Left value from which the right char is subtracted.
 *  \param   inRightChar Value to subtract.
 *  \return  Result of the subtraction.
 */
inline
Beagle::Char& operator-=(Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() -= inRightChar.getWrappedValue();
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator-=(Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Subtract a char from a Char.
 *  \param   inLeftChar Left value from which the right char is subtracted.
 *  \param   inRightChar Value to subtract.
 *  \return  Result of the subtraction.
 */
inline
Beagle::Char& operator-=(Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() -= inRightChar;
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator-=(Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Multiply two Char.
 *  \param   inLeftChar Left value to multiply.
 *  \param   inRightChar Right value to multiply.
 *  \return  Result of the multiply.
 */
inline
Beagle::Char operator*(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() * inRightChar.getWrappedValue();
	Beagle_StackTraceEndM("Char operator*(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Multiply a Char with a char.
 *  \param   inLeftChar Left value to multiply.
 *  \param   inRightChar Right value to multiply.
 *  \return  Result of the multiply.
 */
inline
Beagle::Char operator*(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() * inRightChar;
	Beagle_StackTraceEndM("Char operator*(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Multiply a Char with a Char.
 *  \param   inLeftChar Left value to which the right char is multiplied.
 *  \param   inRightChar Right value to multiply.
 *  \return  Result of the multiplication.
 */
inline
Beagle::Char& operator*=(Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() *= inRightChar.getWrappedValue();
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator*=(Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Multiply a Char with a char.
 *  \param   inLeftChar Left value from which the right char is multiplied.
 *  \param   inRightChar Right value to multiply.
 *  \return  Result of the multiplication.
 */
inline
Beagle::Char& operator*=(Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() *= inRightChar;
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator*=(Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Divide two Char.
 *  \param   inLeftChar Left value to divide.
 *  \param   inRightChar Right value to divide.
 *  \return  Result of the division.
 */
inline
Beagle::Char operator/(const Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() / inRightChar.getWrappedValue();
	Beagle_StackTraceEndM("Char operator/(const Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Divide a Char with a char.
 *  \param   inLeftChar Left value to divide.
 *  \param   inRightChar Right value to divide.
 *  \return  Result of the division.
 */
inline
Beagle::Char operator/(const Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	return inLeftChar.getWrappedValue() / inRightChar;
	Beagle_StackTraceEndM("Char operator/(const Char& inLeftChar, char inRightChar)");
}


/*!
 *  \brief   Divide a Char with a Char.
 *  \param   inLeftChar Left value to which the right char is divided.
 *  \param   inRightChar Right value to divide.
 *  \return  Result of the division.
 */
inline
Beagle::Char& operator/=(Beagle::Char& inLeftChar, const Beagle::Char& inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() /= inRightChar.getWrappedValue();
	return inLeftChar;
	Beagle_StackTraceEndM("Char& operator/=(Char& inLeftChar, const Char& inRightChar)");
}


/*!
 *  \brief   Divide a Char with a char.
 *  \param   inLeftChar Left value from which the right char is divided.
 *  \param   inRightChar Right value to divide.
 *  \return  Result of the division.
 */
inline
Beagle::Char operator/=(Beagle::Char& inLeftChar, char inRightChar)
{
	Beagle_StackTraceBeginM();
	inLeftChar.getWrappedValue() /= inRightChar;
	return inLeftChar;
	Beagle_StackTraceEndM("Char operator/=(Char& inLeftChar, char inRightChar)");
}


#endif // Beagle_Core_Char_hpp



