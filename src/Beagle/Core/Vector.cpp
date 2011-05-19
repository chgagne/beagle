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
 *  \file   Beagle/Core/Vector.cpp
 *  \brief  Source code of class Vector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a Beagle Vector.
 *  \param inSize Size of the vector.
 *  \param inValue Default value of vector elements.
 */
Vector::Vector(unsigned int inSize, double inValue) :
	Beagle::Object(),
	PACC::Vector(inSize, inValue)
{ }


/*!
 *  \brief Construct a Beagle vector from a PACC matrix.
 *  \param inMatrix PACC matrix model.
 */
Vector::Vector(const PACC::Matrix& inMatrix) :
	Beagle::Object(),
	PACC::Vector(inMatrix)
{ }


/*!
 *  \brief Construct a Beagle vector from a PACC vector.
 *  \param inVector PACC vector model.
 */
Vector::Vector(const PACC::Vector& inVector) :
	Beagle::Object(),
	PACC::Vector(inVector)
{ }


/*!
 *  \brief Construct a Beagle vector from a Beagle matrix.
 *  \param inMatrix Beagle matrix model.
 */
Vector::Vector(const Beagle::Matrix& inMatrix) :
	Beagle::Object(),
	PACC::Vector(inMatrix)
{ }


/*!
 *  \brief Operator=, copy a PACC matrix into a Beagle vector.
 *  \param inMatrix PACC matrix to copy.
 *  \return Reference to actual vector.
 */
Vector& Vector::operator=(const PACC::Matrix& inMatrix)
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inMatrix.getCols() <= 1);
	PACC::Matrix::operator=(inMatrix);
	return *this;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Operator=, copy a PACC vector into a Beagle vector.
 *  \param inVector PACC vector to copy.
 *  \return Reference to actual vector.
 */
Vector& Vector::operator=(const PACC::Vector& inVector)
{
	Beagle_StackTraceBeginM();
	PACC::Vector::operator=(inVector);
	return *this;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Operator=, copy a Beagle matrix into a Beagle vector.
 *  \param inMatrix Beagle matrix to copy.
 *  \return Reference to actual vector.
 */
Vector& Vector::operator=(const Beagle::Matrix& inMatrix)
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inMatrix.getCols() <= 1);
	PACC::Matrix::operator=(inMatrix);
	return *this;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read vector from a XML subtree.
 *  \param inIter XML iterator to read the vector from.
 */
void Vector::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if(inIter->getType()!=PACC::XML::eString)
		throw Beagle_IOExceptionNodeM(*inIter,"expected string to read vector!");
	clear();
	std::istringstream lISS(inIter->getValue());
	while(lISS.good()) {
		double lValue;
		lISS >> lValue;
		resize(size()+1);
		(*this)[size()-1] = lValue;
		if(lISS.good()==false) break;
		int lDelim=lISS.get();
		if((lISS.good()==false) || (lDelim==-1)) break;
	}
	Beagle_StackTraceEndM();
}

/*!
*  \brief Transform a Vector into a linear string.
 *  \param inIndent Whether serialized output should be indented.
 *  \param inIndentWidth Number of spaces for each indent. Supplying a value of zero
 *         will produce the data without indentation and without any newlines.
 *  \return String representing the Object.
 */
std::string Vector::serialize(bool inIndent, unsigned int inIndentWidth) const
{
	Beagle_StackTraceBeginM();
	return Object::serialize(inIndent, inIndentWidth);
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Write a matrix into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether output should be indented.
 */
void Vector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertStringContent(PACC::Matrix::serialize());
	Beagle_StackTraceEndM();
}
