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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_Double_hpp



