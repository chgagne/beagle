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
 *  \file   Beagle/Core/Pointer.hpp
 *  \brief  Definition of class Pointer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_Pointer_hpp
#define Beagle_Core_Pointer_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AssertException.hpp"


namespace Beagle
{

/*!
 *  \class Pointer Beagle/Core/Pointer.hpp "Beagle/Core/Pointer.hpp"
 *  \brief Basic class describing a smart pointer to an object.
 *  \ingroup Core
 *  \ingroup ObjPt
 */
class Pointer
{

public:

	/*!
	 *  \brief Construct a NULL pointing smart pointer.
	 */
	inline Pointer() :
		mObjectPointer(NULL)
	{ }

	/*!
	 *  \brief Construct a smart pointer that refer to the object pointed given.
	 *  \param inObjPtr Pointer to the object to be referenced.
	 */
	inline Pointer(const Object* inObjPtr) :
		mObjectPointer( (inObjPtr!=NULL) ? (const_cast<Object*>(inObjPtr)->refer()) : NULL )
	{ }

	/*!
	 *  \brief Copy construct a smart pointer.
	 *  \param inPointer Smart pointer to copy.
	 */
	inline Pointer(const Pointer& inPointer) :
		mObjectPointer( (inPointer.getPointer()!=NULL) ?
		                (const_cast<Beagle::Object*>(inPointer.getPointer())->refer()) : NULL )
	{ }

	/*!
	 *  \brief Destruct a smart pointer by unrefering the pointed object.
	 */
	inline ~Pointer() {
		Beagle_StackTraceBeginM();
		if(mObjectPointer!=NULL) mObjectPointer->unrefer();
		mObjectPointer = NULL;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Affect the actual pointer to an object pointer.
	 *  \param inObjPtr Pointer to the object to refer.
	 *  \return Actual smart pointer.
	 */
	inline Pointer& operator=(const Object* inObjPtr) {
		Beagle_StackTraceBeginM();
		if(mObjectPointer==inObjPtr) return *this;
		if(mObjectPointer!=NULL) mObjectPointer->unrefer();
		if(inObjPtr) mObjectPointer = const_cast<Object*>(inObjPtr)->refer();
		else mObjectPointer = NULL;
		return *this;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Affect the actual pointer to another smart pointer.
	 *  \param inPointer Smart pointer to copy.
	 *  \return Actual smart pointer.
	 */
	inline Pointer& operator=(const Pointer& inPointer) {
		Beagle_StackTraceBeginM();
		if(mObjectPointer==inPointer.getPointer()) return *this;
		if(mObjectPointer) mObjectPointer->unrefer();
		if(inPointer.getPointer()!=NULL)
			mObjectPointer = const_cast<Object*>(inPointer.getPointer())->refer();
		else mObjectPointer = NULL;
		return *this;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get reference the object pointed.
	 *  \return Reference to the object pointed.
	 *  \throw Beagle::AssertException If the pointer to the object is NULL.
	 */
	inline Object& operator*() {
		Beagle_StackTraceBeginM();
		Beagle_NonNullPointerAssertM(mObjectPointer);
		return *mObjectPointer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get constant reference the object pointed.
	 *  \return Constant reference to the object pointed.
	 *  \throw Beagle::AssertException If the pointer to the object is NULL.
	 */
	inline const Object& operator*() const {
		Beagle_StackTraceBeginM();
		Beagle_NonNullPointerAssertM(mObjectPointer);
		return *mObjectPointer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Get reference the object pointed.
	 *  \return Pointer to the object pointed.
	 *  \throw  Beagle::AssertException If the pointer to the object is NULL.
	 */
	inline Object* operator->() {
		Beagle_StackTraceBeginM();
		Beagle_NonNullPointerAssertM(mObjectPointer);
		return mObjectPointer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Get constant reference the object pointed.
	 *  \return Constant pointer to the object pointed.
	 *  \throw  Beagle::AssertException If the pointer to the object is NULL.
	 */
	inline const Object* operator->() const {
		Beagle_StackTraceBeginM();
		Beagle_NonNullPointerAssertM(mObjectPointer);
		return mObjectPointer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Test whether the pointer is NULL or not.
	 *  \return True if the pointer is NULL, false if it is not.
	 */
	inline bool operator!() const {
		Beagle_StackTraceBeginM();
		return (!mObjectPointer);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare a smart pointer to an object pointer.
	 *  \param inObjPtr Right hand side object pointer to compare.
	 *  \return True if they both refer to the same object (or are both NULL), false if not.
	 */
	inline bool operator==(const Object* inObjPtr) const {
		Beagle_StackTraceBeginM();
		return (mObjectPointer == inObjPtr);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare two smart pointers.
	 *  \param inPointer Right hand side smart pointer to compare.
	 *  \return True if they both refer to the same object (or are both NULL), false if not.
	 */
	inline bool operator==(const Pointer& inPointer) const {
		Beagle_StackTraceBeginM();
		return (mObjectPointer == inPointer.getPointer());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare a smart pointer to an object pointer.
	 *  \param inObjPtr Right hand side object pointer to compare.
	 *  \return False if they both refer to the same object (or are both NULL), true if not.
	 */
	inline bool operator!=(const Object* inObjPtr) const {
		Beagle_StackTraceBeginM();
		return (mObjectPointer != inObjPtr);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Compare two smart pointers.
	 *  \param inPointer Right hand side smart pointer to compare.
	 *  \return False if they both refer to the same object (or are both NULL), true if not.
	 */
	inline bool operator!=(const Pointer& inPointer) const {
		Beagle_StackTraceBeginM();
		return (mObjectPointer != inPointer.getPointer());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return pointer to the referenced object.
	 *  \return C++ pointer to the object.
	 */
	inline Object* getPointer() {
		Beagle_StackTraceBeginM();
		return mObjectPointer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return constant pointer to the referenced object.
	 *  \return Constant C++ pointer to the object.
	 */
	inline const Object* getPointer() const {
		Beagle_StackTraceBeginM();
		return mObjectPointer;
		Beagle_StackTraceEndM();
	}

private:

	Object* mObjectPointer;  //!< Pointer to the object refered.

};

}

#endif // Beagle_Core_Pointer_hpp
