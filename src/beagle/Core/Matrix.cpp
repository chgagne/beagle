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
 *  \file   beagle/Core/Matrix.cpp
 *  \brief  Source code of class Matrix.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.26 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "beagle/Core.hpp"

#include <cfloat>
#include <climits>

using namespace Beagle;


/*!
 *  \brief Construct a Beagle Matrix.
 *  \param inRows Number of rows in the matrix.
 *  \param inCols Number of colums in the matrix.
 *  \param inValue Value of matrix elements.
 */
Matrix::Matrix(unsigned int inRows,
               unsigned int inCols,
               double inValue) :
		Beagle::Object(),
		PACC::Matrix(inRows,inCols,inValue)
{ }


/*!
 *  \brief Construct a Beagle matrix from a PACC matrix.
 *  \param inMatrix PACC matrix model.
 */
Matrix::Matrix(const PACC::Matrix& inMatrix) :
		Beagle::Object(),
		PACC::Matrix(inMatrix)
{ }


/*!
 *  \brief Construct a Beagle matrix from a PACC vector.
 *  \param inVector PACC vector model.
 */
Matrix::Matrix(const PACC::Vector& inVector) :
		Beagle::Object(),
		PACC::Matrix(inVector)
{ }


/*!
 *  \brief Construct a Beagle matrix from a Beagle vector.
 *  \param inVector Beagle vector model.
 */
Matrix::Matrix(const Beagle::Vector& inVector) :
		Beagle::Object(),
		PACC::Matrix(inVector)
{ }


/*!
 *  \brief Operator=, copy a PACC matrix into a Beagle matrix.
 *  \param inMatrix PACC matrix to copy.
 *  \return Reference to actual matrix.
 */
Matrix& Matrix::operator=(const PACC::Matrix& inMatrix)
{
	Beagle_StackTraceBeginM();
	PACC::Matrix::operator=(inMatrix);
	return *this;
	Beagle_StackTraceEndM("Matrix& Matrix::operator=(const PACC::Matrix& inMatrix)");
}


/*!
 *  \brief Operator=, copy a PACC vector into a Beagle matrix.
 *  \param inVector PACC vector to copy.
 *  \return Reference to actual matrix.
 */
Matrix& Matrix::operator=(const PACC::Vector& inVector)
{
	Beagle_StackTraceBeginM();
	PACC::Matrix::operator=(inVector);
	return *this;
	Beagle_StackTraceEndM("Matrix& Matrix::operator=(const PACC::Vector& inVector)");
}


/*!
 *  \brief Operator=, copy a Beagle vector into a Beagle matrix.
 *  \param inVector Beagle vector to copy.
 *  \return Reference to actual matrix.
 */
Matrix& Matrix::operator=(const Beagle::Vector& inVector)
{
	Beagle_StackTraceBeginM();
	PACC::Matrix::operator=(inVector);
	return *this;
	Beagle_StackTraceEndM("Matrix& Matrix::operator=(const Beagle::Vector& inVector)");
}


/*!
 *  \brief Read matrix from a XML subtree.
 *  \param inIter XML iterator to read the matrix from.
 */
void Matrix::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	PACC::Matrix::read(inIter);
	Beagle_StackTraceEndM("void Matrix::read(PACC::XML::ConstIterator inIter)");
}

/*!
*  \brief Transform a Matrix into a linear string.
 *  \param inIndent Whether serialized output should be indented.
 *  \param inIndentWidth Number of spaces for each indent. Supplying a value of zero
 *         will produce the data without indentation and without any newlines.
 *  \return String representing the Object.
 */
std::string Matrix::serialize(bool inIndent, unsigned int inIndentWidth) const
{
	Beagle_StackTraceBeginM();
	return Object::serialize(inIndent, inIndentWidth);
	Beagle_StackTraceEndM("std::string Matrix::serialize(bool inIndent, unsigned int inIndentWidth) const");
}

/*!
 *  \brief Write a matrix into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether output should be indented.
 */
void Matrix::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertStringContent(PACC::Matrix::serialize());
	Beagle_StackTraceEndM("void Matrix::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

