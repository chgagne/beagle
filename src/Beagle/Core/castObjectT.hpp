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
 *  \file   Beagle/Core/castObjectT.hpp
 *  \brief  Implementation of castObjectT templated functions.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#ifndef Beagle_Core_castObjectT_hpp
#define Beagle_Core_castObjectT_hpp

#include <typeinfo>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/BadCastException.hpp"


namespace Beagle
{


/*!
 *  \brief   Cast a Beagle::Object pointer into a CastType pointer.
 *  \param   inObject Object pointer to be casted.
 *  \return  Casted pointer to the object.
 *  \relates Beagle::Object
 *  \ingroup ObjPt
 *  \warning If RTTI is enabled and the typecast is invalid, the function return a NULL pointer.
 */
template <class CastPointerType>
inline CastPointerType castObjectT(Beagle::Object* inObject)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_NDEBUG
	return static_cast<CastPointerType>(inObject);
#else // BEAGLE_NDEBUG
#ifdef BEAGLE_HAVE_DYNAMIC_CAST
	return dynamic_cast<CastPointerType>(inObject);
#else // BEAGLE_HAVE_DYNAMIC_CAST
	return static_cast<CastPointerType>(inObject);
#endif // BEAGLE_HAVE_DYNAMIC_CAST
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief   Cast a Beagle::Object constant pointer into a CastType constant pointer.
 *  \param   inObject Object constant pointer to be casted.
 *  \return  Casted constant pointer to the object.
 *  \relates Beagle::Object
 *  \ingroup ObjPt
 *  \warning If RTTI is enabled and the typecast is invalid, the function return a NULL pointer.
 */
template <class CastPointerType>
inline CastPointerType castObjectT(const Beagle::Object* inObject)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_NDEBUG
	return static_cast<CastPointerType>(inObject);
#else // BEAGLE_NDEBUG
#ifdef BEAGLE_HAVE_DYNAMIC_CAST
	return dynamic_cast<CastPointerType>(inObject);
#else // BEAGLE_HAVE_DYNAMIC_CAST
	return static_cast<CastPointerType>(inObject);
#endif // BEAGLE_HAVE_DYNAMIC_CAST
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief   Cast a Beagle::Object reference into a CastType reference.
 *  \param   inObject Object reference to be casted.
 *  \return  Casted reference to the object.
 *  \throw   Beagle::BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates Beagle::Object
 *  \ingroup ObjPt
 */
template <class CastRefType>
inline CastRefType castObjectT(Beagle::Object& inObject)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_NDEBUG
	return static_cast<CastRefType>(inObject);
#else // BEAGLE_NDEBUG
#ifdef BEAGLE_HAVE_DYNAMIC_CAST
	try {
		return dynamic_cast<CastRefType>(inObject);
	} catch(...) {
		std::string lMessage("Unable to cast object reference into desired type! ");
		lMessage += "Type of the object to cast: ";
		lMessage += typeid(inObject).name();
		lMessage += ", \t";
		lMessage += "Desired type cast: ";
		lMessage += typeid(CastRefType).name();
		lMessage += ".";
		throw Beagle_BadCastExceptionM(lMessage);
	}
	return static_cast<CastRefType>(inObject);
#else // BEAGLE_HAVE_DYNAMIC_CAST
	return static_cast<CastRefType>(inObject);
#endif // BEAGLE_HAVE_DYNAMIC_CAST
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief   Cast a Beagle::Object constant reference into a CastType constant reference.
 *  \param   inObject Object constant reference to be casted.
 *  \return  Casted constant reference to the object.
 *  \throw   Beagle::BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates Beagle::Object
 *  \ingroup ObjPt
 */
template <class CastConstRefType>
inline CastConstRefType castObjectT(const Beagle::Object& inObject)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_NDEBUG
	return static_cast<CastConstRefType>(inObject);
#else // BEAGLE_NDEBUG
#ifdef BEAGLE_HAVE_DYNAMIC_CAST
	try {
		return dynamic_cast<CastConstRefType>(inObject);
	} catch(...) {
		std::string lMessage("Unable to cast object reference into desired type! ");
		lMessage += "Type of the object to cast: ";
		lMessage += typeid(inObject).name();
		lMessage += ", \t";
		lMessage += "Desired type cast: ";
		lMessage += typeid(CastConstRefType).name();
		lMessage += ".";
		throw Beagle_BadCastExceptionM(lMessage);
	}
	return static_cast<CastConstRefType>(inObject);
#else // BEAGLE_HAVE_DYNAMIC_CAST
	return static_cast<CastConstRefType>(inObject);
#endif // BEAGLE_HAVE_DYNAMIC_CAST
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief   Cast a Beagle::Object smart pointer into a CastType handle type.
 *  \param   inObject Object smart pointer to be casted.
 *  \return  Casted smart pointer to the object.
 *  \throw   Beagle::BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates Beagle::Object
 *  \ingroup ObjPt
 */
template <class CastType>
inline typename CastType::Handle& castHandleT(Beagle::Object::Handle& inObject)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_NDEBUG
	return static_cast<typename CastType::Handle&>(inObject);
#else // BEAGLE_NDEBUG
#ifdef BEAGLE_HAVE_DYNAMIC_CAST
	if(inObject.getPointer()) {
		if( dynamic_cast<CastType*>(inObject.getPointer()) == NULL ) {
			std::string lMessage("Unable to cast object handle into desired type! ");
			lMessage += "Type of the object to cast: ";
			lMessage += typeid(*inObject).name();
			lMessage += ", \t";
			lMessage += "Desired type cast: ";
			lMessage += typeid(CastType).name();
			lMessage += ".";
			throw Beagle_BadCastExceptionM(lMessage);
		}
	}
	return static_cast<typename CastType::Handle&>(inObject);
#else // BEAGLE_HAVE_DYNAMIC_CAST
	return static_cast<typename CastType::Handle&>(inObject);
#endif // BEAGLE_HAVE_DYNAMIC_CAST
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}


/*!
 *  \brief   Cast a constant Beagle::Object smart pointer into a constant CastType handle type.
 *  \param   inObject Constant Object smart pointer to be casted.
 *  \return  Casted constant smart pointer to the object.
 *  \throw   Beagle::BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates Beagle::Object
 *  \ingroup ObjPt
 */
template <class CastType>
inline const typename CastType::Handle& castHandleT(const Beagle::Object::Handle& inObject)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_NDEBUG
	return static_cast<const typename CastType::Handle&>(inObject);
#else // BEAGLE_NDEBUG
#ifdef BEAGLE_HAVE_DYNAMIC_CAST
	if(inObject.getPointer()) {
		if( dynamic_cast<const CastType*>(inObject.getPointer()) == NULL ) {
			std::string lMessage("Unable to cast object handle into desired type! ");
			lMessage += "Type of the object to cast: ";
			lMessage += typeid(*inObject).name();
			lMessage += ", \t";
			lMessage += "Desired type cast: ";
			lMessage += typeid(CastType).name();
			lMessage += ".";
			throw Beagle_BadCastExceptionM(lMessage);
		}
	}
	return static_cast<const typename CastType::Handle&>(inObject);
#else // BEAGLE_HAVE_DYNAMIC_CAST
	return static_cast<const typename CastType::Handle&>(inObject);
#endif // BEAGLE_HAVE_DYNAMIC_CAST
#endif // BEAGLE_NDEBUG
	Beagle_StackTraceEndM();
}

}

#endif // Beagle_Core_castObjectT_hpp
