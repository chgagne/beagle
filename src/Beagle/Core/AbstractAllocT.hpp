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
 *  \file   Beagle/Core/AbstractAllocT.hpp
 *  \brief  Definition of abstract templated class AbstractAllocT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_AbstractAllocT_hpp
#define Beagle_Core_AbstractAllocT_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/AllocatorT.hpp"


namespace Beagle
{

/*!
 *  \class AbstractAllocT Beagle/Core/AbstractAllocT.hpp "Beagle/Core/AbstractAllocT.hpp"
 *  \brief Abstract templated allocator class.
 *  \param T Type (abtsract) of object allocated.
 *  \param BaseType Base type from which the actual allocator type is derived.
 *  \ingroup Core
 *  \ingroup Allocs
 *  \par Note:
 *    This class is usually used to define the Alloc type of an abstract Beagle class.
 */
template <class T, class BaseType>
class AbstractAllocT : public BaseType
{

public:

	//! AbstractAllocT allocator type.
	typedef AbstractAllocT<AbstractAllocT<T,BaseType>,typename BaseType::Alloc> Alloc;
	//! AbstractAllocT handle type.
	typedef PointerT<AbstractAllocT<T,BaseType>,typename BaseType::Handle> Handle;
	//! AbstractAllocT bag type.
	typedef ContainerT<AbstractAllocT<T,BaseType>,typename BaseType::Bag> Bag;

	AbstractAllocT()
	{ }
	virtual ~AbstractAllocT()
	{ }

	/*!
	 *  \brief  Allocate a new object on the heap.
	 *  \return Pointer to the allocated object.
	 */
	virtual Object* allocate() const =0;

	/*!
	 *  \brief  Allocate a new object on the heap that is a clone of an existing object.
	 *  \param  inOriginal Constant reference to the original object to clone.
	 *  \return Pointer to the allocated object.
	 */
	virtual Object* clone(const Object& inOriginal) const =0;

	/*!
	 *  \brief Copy an object to another.
	 *  \param outCopy Constant reference to the original object to copy.
	 *  \param inOriginal  Reference to the object that is a copy of the original.
	 */
	virtual void copy(Object& outCopy, const Object& inOriginal) const =0;

};

}

#endif // Beagle_Core_AbstractAllocT_hpp
