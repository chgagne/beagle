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
 *  \file   beagle/Core/Allocator.cpp
 *  \brief  Source code of class Allocator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "beagle/Core.hpp"

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
	Beagle_StackTraceEndM("Beagle::Object* Beagle::Allocator::allocate() const");
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
  Beagle_StackTraceEndM("Beagle::Object* Beagle::Allocator::clone(const Beagle::Object&) const");
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
  Beagle_StackTraceEndM("void Beagle::Allocator::copy(Beagle::Object&, const Beagle::Object&) const");
}

