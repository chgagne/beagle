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
 *  \file   Beagle/Core/Object.cpp
 *  \brief  Source code of class Object.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.22 $
 *  $Date: 2007/09/14 20:19:50 $
 */

#include "Beagle/Core.hpp"

#include <sstream>

using namespace Beagle;



/*!
 *  \brief Default constructor.
 */
Object::Object() :
	mRefCounter(0)
{ }


/*!
 *  \brief Copy constructor.
 */
Object::Object(const Object&) :
	mRefCounter(0)
{ }


/*!
 *  \brief Destructor.
 *  \throw Beagle::InternalException If reference counter is not equal to zero.
 */
Object::~Object()
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mRefCounter != 0) {
		std::string lMessage = "Reference counter of actual object is non-zero (value: ";
		lMessage += int2str(mRefCounter);
		lMessage += ")";
		throw Beagle_InternalExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy operator of Object.
 *  \return Actual object.
 */
Object& Object::operator=(const Object&)
{
	Beagle_StackTraceBeginM();
	return *this;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Returns the name of the object, in this case "Non-named Object".
 */
const std::string& Object::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string mName("Non-named Object");
	return mName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test if an object is equal to another.
 *  \return True if actual object is equal to right object, false if not.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
bool Object::isEqual(const Object&) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("isEqual","Object",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test if an object is less than another.
 *  \return True if actual object is less than right object, false if not.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
bool Object::isLess(const Object&) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("isLess","Object",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Extract Object from a XML tree.
 *  \throw Beagle::InternalException If the method is not overdefined in a subclass.
 */
void Object::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read", "Object", getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Transform an Object into a linear string (using write method).
 *  \param inIndent Whether serialized output should be indented.
 *  \param inIndentWidth Number of spaces for each indent. Supplying a value of zero
 *         will produce the data without indentation and without any newlines.
 *  \return String representing the Object.
 */
std::string Object::serialize(bool inIndent, unsigned int inIndentWidth) const
{
	Beagle_StackTraceBeginM();
	std::ostringstream lOSS;
	PACC::XML::Streamer lStreamer(lOSS, inIndentWidth);
	write(lStreamer, inIndent);
	std::string lStr = lOSS.str();
	return lStr;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert an Object into a XML streamer.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
void Object::write(PACC::XML::Streamer&, bool) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("write", "Object", getName());
	Beagle_StackTraceEndM();
}


