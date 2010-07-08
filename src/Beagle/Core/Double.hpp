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
 *  \file   Beagle/Core/Double.hpp
 *  \brief  Definition of the type Double and related operators.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_Double_hpp
#define Beagle_Core_Double_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"


namespace Beagle
{

/*!
 *  \brief Beagle wrapper of the atomic double type.
 *  \ingroup Adapt
 */
typedef WrapperT< double >
Double;

/*!
 *  \brief Beagle array of the atomic double type.
 *  \ingroup Adapt
 */
typedef ArrayT< double >
DoubleArray;

/*!
 *  \brief Evaluate absolute value of a Beagle::Double.
 *  \param inValue Double to evaluate the absolute value.
 *  \return Absolute value of the input.
 *  \ingroup Adapt
 */
template <>
inline Double absolute(const Double& inValue)
{
	Beagle_StackTraceBeginM();
	return Double(std::fabs(inValue.getWrappedValue()));
	Beagle_StackTraceEndM("Double absolute(const Double& inValue)");
}

}


/*!
 *  \brief  Test whether an double is less than another.
 *  \param  inLeftDouble  Left double compared.
 *  \param  inRightDouble Right double compared.
 *  \return True if left double is less than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::Double.
 */
inline bool operator<(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.isLess(inRightDouble);
	Beagle_StackTraceEndM("bool operator<(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief  Test whether an double is less than, or equal to another.
 *  \param  inLeftDouble  Left double compared.
 *  \param  inRightDouble Right double compared.
 *  \return True if left double is less than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::Double.
 */
inline bool operator<=(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return ( inLeftDouble.isLess(inRightDouble) || inLeftDouble.isEqual(inRightDouble) );
	Beagle_StackTraceEndM("bool operator<=(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief  Test whether an double is more than another.
 *  \param  inLeftDouble  Left double compared.
 *  \param  inRightDouble Right double compared.
 *  \return True if left double is more than the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isLess of Beagle::Double.
 */
inline bool operator>(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inRightDouble.isLess(inLeftDouble);
	Beagle_StackTraceEndM("bool operator>(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief  Test whether an double is more than, or equal to another.
 *  \param  inLeftDouble  Left double compared.
 *  \param  inRightDouble Right double compared.
 *  \return True if left double is more than, or equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the functions isLess and isEqual of Beagle::Double.
 */
inline bool operator>=(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return ( inRightDouble.isLess(inLeftDouble) || inLeftDouble.isEqual(inRightDouble) );
	Beagle_StackTraceEndM("bool operator>=(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief  Test whether an double is equal to another.
 *  \param  inLeftDouble  Left double compared.
 *  \param  inRightDouble Right double compared.
 *  \return True if left double is equal to the right one, false if not.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Double.
 */
inline bool operator==(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.isEqual(inRightDouble);
	Beagle_StackTraceEndM("bool operator==(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief  Test whether an double is not equal to another.
 *  \param  inLeftDouble  Left double compared.
 *  \param  inRightDouble Right double compared.
 *  \return True if left double is not equal to the right one, false if it is.
 *  \par Note:
 *    The operator is defined relatively to the function isEqual of Beagle::Double.
 */
inline bool operator!=(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return ( inLeftDouble.isEqual(inRightDouble) == false);
	Beagle_StackTraceEndM("bool operator!=(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Compare equality of a Double with a double.
 *  \param inLeftDouble Left value to compare.
 *  \param inRightDouble Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator==(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() == inRightDouble;
	Beagle_StackTraceEndM("bool operator==(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Compare inequality of a Double with a double.
 *  \param inLeftDouble Left value to compare.
 *  \param inRightDouble Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator!=(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() != inRightDouble;
	Beagle_StackTraceEndM("bool operator!=(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Test if a Double is < than a double.
 *  \param inLeftDouble Left value to compare.
 *  \param inRightDouble Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() < inRightDouble;
	Beagle_StackTraceEndM("ool operator<(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Test if a Double is <= than a double.
 *  \param inLeftDouble Left value to compare.
 *  \param inRightDouble Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator<=(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() <= inRightDouble;
	Beagle_StackTraceEndM("bool operator<=(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Test if a Double is > than a double.
 *  \param inLeftDouble Left value to compare.
 *  \param inRightDouble Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() > inRightDouble;
	Beagle_StackTraceEndM("bool operator>(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Test if a Double is >= than a double.
 *  \param inLeftDouble Left value to compare.
 *  \param inRightDouble Right value to compare.
 *  \return Result of the comparison.
 */
inline
bool operator>=(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() >= inRightDouble;
	Beagle_StackTraceEndM("bool operator>=(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Add two Double.
 *  \param inLeftDouble Left value to add.
 *  \param inRightDouble Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Double operator+(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() + inRightDouble.getWrappedValue();
	Beagle_StackTraceEndM("Double operator+(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Add a Double with a double.
 *  \param inLeftDouble Left value to add.
 *  \param inRightDouble Right value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Double operator+(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() + inRightDouble;
	Beagle_StackTraceEndM("Double operator+(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Add a Double to a Double.
 *  \param inLeftDouble Left value to which the right one is added.
 *  \param inRightDouble Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Double operator+=(Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() += inRightDouble.getWrappedValue();
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator+=(Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Add a double to a Double.
 *  \param inLeftDouble Left value to which the right one is added.
 *  \param inRightDouble Value to add.
 *  \return Result of the addition.
 */
inline
Beagle::Double operator+=(Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() += inRightDouble;
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator+=(Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Subtract two Double.
 *  \param inLeftDouble Left value to subtract.
 *  \param inRightDouble Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::Double operator-(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() - inRightDouble.getWrappedValue();
	Beagle_StackTraceEndM("Double operator-(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Subtract a double from a Double.
 *  \param inLeftDouble Left value to subtract.
 *  \param inRightDouble Right value to subtract.
 *  \return Result of the subtract.
 */
inline
Beagle::Double operator-(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() - inRightDouble;
	Beagle_StackTraceEndM("Double operator-(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Subtract a Double from a Double.
 *  \param inLeftDouble Left value from which the right double is subtracted.
 *  \param inRightDouble Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::Double operator-=(Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() -= inRightDouble.getWrappedValue();
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator-=(Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Subtract a double from a Double.
 *  \param inLeftDouble Left value from which the right double is subtracted.
 *  \param inRightDouble Value to subtract.
 *  \return Result of the subtraction.
 */
inline
Beagle::Double operator-=(Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() -= inRightDouble;
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator-=(Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Multiply two Double.
 *  \param inLeftDouble Left value to multiply.
 *  \param inRightDouble Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::Double operator*(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() * inRightDouble.getWrappedValue();
	Beagle_StackTraceEndM("Double operator*(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Multiply a Double with a double.
 *  \param inLeftDouble Left value to multiply.
 *  \param inRightDouble Right value to multiply.
 *  \return Result of the multiply.
 */
inline
Beagle::Double operator*(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() * inRightDouble;
	Beagle_StackTraceEndM("Double operator*(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Multiply a Double with a Double.
 *  \param inLeftDouble Left value to which the right double is multiplied.
 *  \param inRightDouble Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::Double operator*=(Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() *= inRightDouble.getWrappedValue();
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator*=(Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Multiply a Double with a double.
 *  \param inLeftDouble Left value from which the right double is multiplied.
 *  \param inRightDouble Right value to multiply.
 *  \return Result of the multiplication.
 */
inline
Beagle::Double operator*=(Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() *= inRightDouble;
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator*=(Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Divide two Double.
 *  \param inLeftDouble Left value to divide.
 *  \param inRightDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Double operator/(const Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() / inRightDouble.getWrappedValue();
	Beagle_StackTraceEndM("Double operator/(const Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Divide a Double with a double.
 *  \param inLeftDouble Left value to divide.
 *  \param inRightDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Double operator/(const Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	return inLeftDouble.getWrappedValue() / inRightDouble;
	Beagle_StackTraceEndM("Double operator/(const Double& inLeftDouble, double inRightDouble)");
}


/*!
 *  \brief Divide a Double with a Double.
 *  \param inLeftDouble Left value to which the right double is divided.
 *  \param inRightDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Double operator/=(Beagle::Double& inLeftDouble, const Beagle::Double& inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() /= inRightDouble.getWrappedValue();
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator/=(Double& inLeftDouble, const Double& inRightDouble)");
}


/*!
 *  \brief Divide a Double with a double.
 *  \param inLeftDouble Left value from which the right double is divided.
 *  \param inRightDouble Right value to divide.
 *  \return Result of the division.
 */
inline
Beagle::Double operator/=(Beagle::Double& inLeftDouble, double inRightDouble)
{
	Beagle_StackTraceBeginM();
	inLeftDouble.getWrappedValue() /= inRightDouble;
	return inLeftDouble;
	Beagle_StackTraceEndM("Double operator/=(Double& inLeftDouble, double inRightDouble)");
}


#endif // Beagle_Core_Double_hpp



