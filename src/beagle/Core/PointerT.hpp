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
 *  \file   beagle/Core/PointerT.hpp
 *  \brief  Definition of class template PointerT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_PointerT_hpp
#define Beagle_Core_PointerT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"


namespace Beagle
{

/*!
 *  \class PointerT beagle/Core/PointerT.hpp "beagle/Core/PointerT.hpp"
 *  \brief Templated class defining a T-type smart pointer.
 *  \param T Type pointed.
 *  \param BaseType Pointer type from which the actual type is derived.
 *  \ingroup OOF
 *  \ingroup ObjPt
 */
template <class T, class BaseType>
class PointerT : public BaseType
{

public:

	/*!
	 *  \brief Construct a NULL pointing smart pointer.
	 */
	inline PointerT()
	{ }

	/*!
	 *  \brief Construct a smart pointer that refer to the T-type object pointed given.
	 *  \param inObjPtr Pointer to the T-type object to be referenced.
	 */
	inline PointerT(const T* inObjPtr) :
			BaseType(inObjPtr)
	{ }

	/*!
	 *  \brief Copy construct a smart pointer.
	 *  \param inPointerT  Smart pointer to copy.
	 */
	inline PointerT(const PointerT<T,BaseType>& inPointerT) :
			BaseType(inPointerT)
	{ }

	/*!
	 *  \brief Destructor.
	 */
	inline ~PointerT()
	{ }

	/*!
	 *  \brief Affect the actual pointer to an object pointer.
	 *  \param inObjPtr Pointer to the object to refer.
	 *  \return Actual smart pointer.
	 */
	inline PointerT<T,BaseType>& operator=(const T* inObjPtr)
	{
		Beagle_StackTraceBeginM();
		BaseType::operator=(inObjPtr);
		return *this;
		Beagle_StackTraceEndM("PointerT<T,BaseType>& PointerT<T,BaseType>::operator=(const T* inObjPtr)");
	}

	/*!
	 *  \brief Affect the actual pointer to another smart pointer.
	 *  \param inPointerT Smart pointer to copy.
	 *  \return Actual smart pointer.
	 */
	inline PointerT<T,BaseType>& operator=(const PointerT<T,BaseType>& inPointerT)
	{
		Beagle_StackTraceBeginM();
		BaseType::operator=(inPointerT);
		return *this;
		Beagle_StackTraceEndM("PointerT<T,BaseType>& PointerT<T,BaseType>::operator=(const PointerT<T,BaseType>& inPointerT)");
	}

	/*!
	 *  \brief Get reference the T-type object pointed.
	 *  \return Reference to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline T& operator*()
	{
		Beagle_StackTraceBeginM();
		return static_cast<T&>(Beagle::Pointer::operator*());
		Beagle_StackTraceEndM("T& PointerT<T,BaseType>::operator*()");
	}

	/*!
	 *  \brief Get constant reference the T-type object pointed.
	 *  \return Constant reference to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline const T& operator*() const
	{
		Beagle_StackTraceBeginM();
		return static_cast<const T&>(Beagle::Pointer::operator*());
		Beagle_StackTraceEndM("const T& PointerT<T,BaseType>::operator*() const");
	}

	/*!
	 *  \brief Get reference the T-type object pointed.
	 *  \return Pointer to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline T* operator->()
	{
		Beagle_StackTraceBeginM();
		return static_cast<T*>(Beagle::Pointer::operator->());
		Beagle_StackTraceEndM("T* PointerT<T,BaseType>::operator->()");
	}

	/*!
	 *  \brief Get constant reference the T-type object pointed.
	 *  \return Constant pointer to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline const T* operator->() const
	{
		Beagle_StackTraceBeginM();
		return static_cast<const T*>(Beagle::Pointer::operator->());
		Beagle_StackTraceEndM("const T* PointerT<T,BaseType>::operator->() const");
	}

	/*!
	 *  \brief Compare a smart pointer to a T-type object pointer.
	 *  \param inObjPtr Object pointer to compare.
	 *  \return True if they both refer to the same T-type object (or are both NULL), false if not.
	 */
	inline bool operator==(const T* inObjPtr) const
	{
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator==(inObjPtr);
		Beagle_StackTraceEndM("bool PointerT<T,BaseType>::operator==(const T* inObjPtr) const");
	}

	/*!
	 *  \brief  Compare two T-type smart pointers.
	 *  \param inPointerT Smart pointer to compare.
	 *  \return True if they both refer to the same T-type object (or are both NULL), false if not.
	 */
	inline bool operator==(const PointerT<T,BaseType>& inPointerT) const
	{
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator==(inPointerT);
		Beagle_StackTraceEndM("bool PointerT<T,BaseType>::operator==(const PointerT<T,BaseType>& inPointerT) const");
	}

	/*!
	 *  \brief Compare a T-type smart pointer to an T-type object pointer.
	 *  \param inObjPtr Object pointer to compare.
	 *  \return False if they both refer to the same T-type object (or are both NULL), true if not.
	 */
	inline bool operator!=(const T* inObjPtr) const
	{
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator!=(inObjPtr);
		Beagle_StackTraceEndM("bool PointerT<T,BaseType>::operator!=(const T* inObjPtr) const");
	}

	/*!
	 *  \brief Compare two T-type smart pointers.
	 *  \param inPointerT Smart pointer to compare.
	 *  \return False if they both refer to the same T-type object (or are both NULL), true if not.
	 */
	inline bool operator!=(const PointerT<T,BaseType>& inPointerT) const
	{
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator!=(inPointerT);
		Beagle_StackTraceEndM("bool PointerT<T,BaseType>::operator!=(const PointerT<T,BaseType>& inPointerT) const");
	}

	/*!
	 *  \brief  Return T-type pointer to the referenced object.
	 *  \return C++ pointer to the T-type object.
	 */
	inline T* getPointer()
	{
		Beagle_StackTraceBeginM();
		return static_cast<T*>(Beagle::Pointer::getPointer());
		Beagle_StackTraceEndM("T* PointerT<T,BaseType>::getPointer()");
	}

	/*!
	 *  \brief  Return constant T-type pointer to the referenced object.
	 *  \return Constant C++ pointer to the T-type object.
	 */
	inline const T* getPointer() const
	{
		Beagle_StackTraceBeginM();
		return static_cast<const T*>(Beagle::Pointer::getPointer());
		Beagle_StackTraceEndM("const T* PointerT<T,BaseType>::getPointer() const");
	}

};

}

#endif // Beagle_Core_PointerT_hpp


