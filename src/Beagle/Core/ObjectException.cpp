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
 *  \file   Beagle/Core/ObjectException.cpp
 *  \brief  Implementation of class ObjectException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/Core.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace Beagle;


/*!
 *  \brief Construct the ObjectException using given values.
 *  \param inObject     Reference to the object related to the exception.
 *  \param inMessage    Message explaining the exception.
 *  \param inFileName   Name of the file where the error state had been detected.
 *  \param inLineNumber Line number in the filename.
 */
ObjectException::ObjectException(const Object&   inObject,
                                 std::string  inMessage,
                                 std::string  inFileName,
                                 unsigned int    inLineNumber) :
	TargetedException(inMessage,inFileName,inLineNumber)
{
	Beagle_StackTraceBeginM();
	setObjectState(inObject);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Explain the exception throwed in the stream given.
 *  \param ioES Output C++ stream where the explanation about the exception is inserted.
 */
void ObjectException::explain(std::ostream& ioES) throw()
{
	TargetedException::explain(ioES);
	ioES << "Object name: " << mObjectName << std::endl;
	ioES << "Object RTTI name: " << mObjectTypeName << std::endl;
	ioES << "Object reference counter value: " << mObjectRefCounter << std::endl;
	ioES << "Serialized Object: " << std::endl << mSerializedObject << std::endl << std::flush;
}


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* ObjectException::getExceptionName() const throw()
{
	return "Beagle::ObjectException";
}


/*!
 *  \brief Set values of the ObjectException using given object reference.
 *  \param inObject Reference to object related to the exception.
 */
void ObjectException::setObjectState(const Object& inObject)
{
	Beagle_StackTraceBeginM();
	mObjectRefCounter = inObject.getRefCounter();
	mObjectName = inObject.getName();
#ifdef BEAGLE_HAVE_RTTI
	try {
		mObjectTypeName = typeid(inObject).name();
	} catch(...) {
		mObjectTypeName = "<Failed>";
	}
#else // BEAGLE_HAVE_RTTI
	mObjectTypeName = "<RTTI disabled>";
#endif // BEAGLE_HAVE_RTTI
	try {
		mSerializedObject = inObject.serialize();
	} catch(...) {
		mSerializedObject = "<Failed>";
	}
	Beagle_StackTraceEndM();
}

