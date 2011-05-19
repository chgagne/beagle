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
 *  \file   Beagle/Core/NamedObject.cpp
 *  \brief  Source code of class NamedObject.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Constructor with arguments of a named object.
 *  \param inName Name of the object.
 */
NamedObject::NamedObject(const std::string& inName) :
	mName(inName)
{ }


/*!
 *  \brief Returns the name of the object
 */
const std::string& NamedObject::getName() const
{
	Beagle_StackTraceBeginM();
	return mName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Sets the name of the object
 *  \param inName Name of the object
 */
void NamedObject::setName(const std::string& inName)
{
	Beagle_StackTraceBeginM();
	mName = inName;
	Beagle_StackTraceEndM();
}

