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
 *  \file   Beagle/Core/AllocatorT.hpp
 *  \brief  Definition of class template AllocatorT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_AllocatorT_hpp
#define Beagle_Core_AllocatorT_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/castObjectT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/InternalException.hpp"
#include "Beagle/Core/BadCastException.hpp"


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;

/*!
 *  \class AllocatorT Beagle/Core/AllocatorT.hpp "Beagle/Core/AllocatorT.hpp"
 *  \brief Templated class defining a T-type allocator.
 *  \param T Type of object allocated.
 *  \param BaseType Base type from which the actual allocator type is derived.
 *  \ingroup Core
 *  \ingroup Allocs
 */
template <class T, class BaseType>
class AllocatorT : public BaseType
{

public:

	//! AllocatorT allocator type.
	typedef AllocatorT<AllocatorT<T,BaseType>,typename BaseType::Alloc> Alloc;
	//! AllocatorT handle type.
	typedef PointerT<AllocatorT<T,BaseType>,typename BaseType::Handle> Handle;
	//! AllocatorT bag type.
	typedef ContainerT<AllocatorT<T,BaseType>,typename BaseType::Bag> Bag;

	AllocatorT()
	{ }
	virtual ~AllocatorT()
	{ }

	virtual Object* allocate() const;
	virtual Object* clone(const Object& inOrigObj) const;
	virtual void    copy(Object& outCopy, const Object& inOriginal) const;

};

}


/*!
 *  \brief  Allocate a new T-type object and the heap.
 *  \return Pointer to the allocated T-type object.
 */
template <class T, class BaseType>
Beagle::Object* Beagle::AllocatorT<T,BaseType>::allocate() const
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	T* lT = new T;
	if(!lT) throw Beagle_InternalExceptionM("Out of memory!");
	return lT;
#else // BEAGLE_NDEBUG
	return new T;
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Allocate a new T-type object on the heap that is a clone an existing object.
 *  \param  inOriginal Constant reference to the original T-type object to clone.
 *  \return Pointer to the allocated T-type object.
 *  \throw  BasCastException If the type of the parameter is not as specified.
 */
template <class T, class BaseType>
Beagle::Object* Beagle::AllocatorT<T,BaseType>::clone(const Beagle::Object& inOriginal) const
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	const T& lOrigT = castObjectT<const T&>(inOriginal);
	T* lCopy = new T(lOrigT);
	if(!lCopy) throw Beagle_InternalExceptionM("Out of memory!");
	return lCopy;
#else // BEAGLE_NDEBUG
	const T& lOrigT = castObjectT<const T&>(inOriginal);
	return new T(lOrigT);
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy a T-type object into another.
 *  \param outCopy Reference to the object that is a copy of the original.
 *  \param inOriginal Constant reference to the original object to copy.
 *  \throw BasCastException If the types of the parameters are not as specified.
 */
template <class T, class BaseType>
void Beagle::AllocatorT<T,BaseType>::copy(Beagle::Object& outCopy,
        const Beagle::Object& inOriginal) const
{
	Beagle_StackTraceBeginM();
	T& lCopyT = castObjectT<T&>(outCopy);
	const T& lOriginalT = castObjectT<const T&>(inOriginal);
	lCopyT = lOriginalT;
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_AllocatorT_hpp
