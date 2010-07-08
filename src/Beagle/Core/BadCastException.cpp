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
 *  \file   Beagle/Core/BadCastException.cpp
 *  \brief  Implementation of class BadCastException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new BadCastException using data given.
 *  \param inMessage    Message related to the exception.
 *  \param inFileName   File name where the exception is detected (throwed).
 *  \param inLineNumber Line number where the exception is detected (throwed).
 */
BadCastException::BadCastException(std::string inMessage,
                                   std::string inFileName,
                                   unsigned int   inLineNumber) :
	TargetedException(inMessage,inFileName,inLineNumber)
{ }


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* BadCastException::getExceptionName() const throw()
{
	return "Beagle::BadCastException";
}

