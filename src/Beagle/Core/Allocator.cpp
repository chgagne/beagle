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
 *  \file   Beagle/Core/Allocator.cpp
 *  \brief  Source code of class Allocator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief  Allocate a new object on the heap.
 *  \return Pointer to the allocated object.
 */
Beagle::Object* Beagle::Allocator::allocate() const
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	Object* lObject = new Object;
	if(!lObject) throw Beagle_InternalExceptionM("Out-of-memory!");
	return lObject;
#else // BEAGLE_NDEBUG
	return new Object;
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Allocate a new object on the heap that is a clone of an existing object.
 *  \param  inOriginal Constant reference to the original object to clone.
 *  \return Pointer to the allocated object.
 */
Beagle::Object* Beagle::Allocator::clone(const Beagle::Object& inOriginal) const
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	Object* lObject = new Object(inOriginal);
	if(!lObject) throw Beagle_InternalExceptionM("Out-of-memory!");
	return lObject;
#else // BEAGLE_NDEBUG
	return new Object(inOriginal);
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy an object to another.
 *  \param outCopy Constant reference to the original object to copy.
 *  \param inOriginal  Reference to the object that is a copy of the original.
 */
void Beagle::Allocator::copy(Beagle::Object& outCopy,
                             const Beagle::Object& inOriginal) const
{
	Beagle_StackTraceBeginM();
	outCopy = inOriginal;
	Beagle_StackTraceEndM();
}

