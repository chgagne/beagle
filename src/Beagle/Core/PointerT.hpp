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
 *  \file   Beagle/Core/PointerT.hpp
 *  \brief  Definition of class template PointerT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_PointerT_hpp
#define Beagle_Core_PointerT_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"


namespace Beagle
{

/*!
 *  \class PointerT Beagle/Core/PointerT.hpp "Beagle/Core/PointerT.hpp"
 *  \brief Templated class defining a T-type smart pointer.
 *  \param T Type pointed.
 *  \param BaseType Pointer type from which the actual type is derived.
 *  \ingroup Core
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
	inline PointerT<T,BaseType>& operator=(const T* inObjPtr) {
		Beagle_StackTraceBeginM();
		BaseType::operator=(inObjPtr);
		return *this;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Affect the actual pointer to another smart pointer.
	 *  \param inPointerT Smart pointer to copy.
	 *  \return Actual smart pointer.
	 */
	inline PointerT<T,BaseType>& operator=(const PointerT<T,BaseType>& inPointerT) {
		Beagle_StackTraceBeginM();
		BaseType::operator=(inPointerT);
		return *this;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get reference the T-type object pointed.
	 *  \return Reference to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline T& operator*() {
		Beagle_StackTraceBeginM();
		return static_cast<T&>(Beagle::Pointer::operator*());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get constant reference the T-type object pointed.
	 *  \return Constant reference to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline const T& operator*() const {
		Beagle_StackTraceBeginM();
		return static_cast<const T&>(Beagle::Pointer::operator*());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get reference the T-type object pointed.
	 *  \return Pointer to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline T* operator->() {
		Beagle_StackTraceBeginM();
		return static_cast<T*>(Beagle::Pointer::operator->());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get constant reference the T-type object pointed.
	 *  \return Constant pointer to the T-type object pointed.
	 *  \throw Beagle::AssertException If the pointer to the T-type object is NULL.
	 */
	inline const T* operator->() const {
		Beagle_StackTraceBeginM();
		return static_cast<const T*>(Beagle::Pointer::operator->());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare a smart pointer to a T-type object pointer.
	 *  \param inObjPtr Object pointer to compare.
	 *  \return True if they both refer to the same T-type object (or are both NULL), false if not.
	 */
	inline bool operator==(const T* inObjPtr) const {
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator==(inObjPtr);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Compare two T-type smart pointers.
	 *  \param inPointerT Smart pointer to compare.
	 *  \return True if they both refer to the same T-type object (or are both NULL), false if not.
	 */
	inline bool operator==(const PointerT<T,BaseType>& inPointerT) const {
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator==(inPointerT);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare a T-type smart pointer to an T-type object pointer.
	 *  \param inObjPtr Object pointer to compare.
	 *  \return False if they both refer to the same T-type object (or are both NULL), true if not.
	 */
	inline bool operator!=(const T* inObjPtr) const {
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator!=(inObjPtr);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare two T-type smart pointers.
	 *  \param inPointerT Smart pointer to compare.
	 *  \return False if they both refer to the same T-type object (or are both NULL), true if not.
	 */
	inline bool operator!=(const PointerT<T,BaseType>& inPointerT) const {
		Beagle_StackTraceBeginM();
		return Beagle::Pointer::operator!=(inPointerT);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Return T-type pointer to the referenced object.
	 *  \return C++ pointer to the T-type object.
	 */
	inline T* getPointer() {
		Beagle_StackTraceBeginM();
		return static_cast<T*>(Beagle::Pointer::getPointer());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Return constant T-type pointer to the referenced object.
	 *  \return Constant C++ pointer to the T-type object.
	 */
	inline const T* getPointer() const {
		Beagle_StackTraceBeginM();
		return static_cast<const T*>(Beagle::Pointer::getPointer());
		Beagle_StackTraceEndM();
	}

};

}

#endif // Beagle_Core_PointerT_hpp


