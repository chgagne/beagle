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
 *  \file   beagle/Core/WrapperT.hpp
 *  \brief  Definition and implementation of template WrapperT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:45 $
 */

/*!
 *  \defgroup Adapt Object Wrappers
 *  \ingroup OOF
 *  \brief Object wrapper and predefined adapted types, part of the Object Oriented Foundations.
 *
 *  Given all this functionalities, the predominance of the object abstraction in Open BEAGLE's
 *  design is evident. But, if a user has some elaborated classes, or if a user wants to use other 
 *  libraries in conjunction of Open BEAGLE, it might be restrictive that everything must inherit from 
 *  the superclass Beagle::Object. The user might not want to redefine everything or to use multiple
 *  inheritance to created hybrid classes. To enhance the user experience, a class template that would
 *  keep the use of Open BEAGLE simple is defined: the object wrapper. The wrapper is a simple adapter 
 *  of any type to the Open BEAGLE Beagle::Object interface. The concept of object wrapper is based 
 *  upon the \e Adapter design pattern.
 *
 *  The wrapper is defined in the C++ template Beagle::WrapperT. The template maps the Beagle::Object 
 *  interface to the usual methods of the wrapped type. The template got some casting operators to
 *  indifferently use the wrapper as the wrapped type. Furthermore, some types that are wrappers of 
 *  the C++ fundamental types are defined as standard types of Open BEAGLE.
 *
 */


#ifndef Beagle_Core_WrapperT_hpp
#define Beagle_Core_WrapperT_hpp

#include <functional>
#include <string>

#include "PACC/XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Exception.hpp"
#include "beagle/Core/castObjectT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/IOException.hpp"

namespace Beagle
{


/*!
 *  \class WrapperT beagle/Core/WrapperT.hpp "beagle/Core/WrapperT.hpp"
 *  \brief Template of an Wrapper that adapt a type to the Beagle Object interface.
 *  \param T The type wrapped.
 *  \ingroup OOF
 *  \ingroup Adapt
 *
 *  A type that is wrapped must have the following operator predefined:
 *  \li bool operator<(const T&, const T&); to compare strict weak ordering two wrapped values.
 *  \li bool operator==(const T&, const T&); to compare equality of two wrapped values.
 *  \li std::ostream& operator<<(std::ostream&, const T&); to output value wrapped.
 *  \li std::istream& operator>>(std::istream&, T&); to input value wrapped.
 */
template <class T>
class WrapperT : public Object
{

public:

	//! WrapperT allocator type.
	typedef AllocatorT< WrapperT<T>, Object::Alloc >
	Alloc;
	//! WrapperT handle type.
	typedef PointerT< WrapperT<T>, Object::Handle >
	Handle;
	//! WrapperT bag type.
	typedef ContainerT< WrapperT<T>, Object::Bag >
	Bag;

	WrapperT()
	{ }
	WrapperT(const T& inWrappedValue);
	virtual ~WrapperT()
	{ }

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent) const;

	/*!
	 *  \brief Cast the actual wrapper type into the wrapped type.
	 *  \return A reference to the wrapped value.
	 */
	inline operator T() const
	{
		Beagle_StackTraceBeginM();
		return mWrappedValue;
		Beagle_StackTraceEndM("WrapperT<T>::operator T() const");
	}

	/*!
	 *  \brief Get the wrapped type value.
	 *  \return A reference to the wrapped value.
	 */
	inline T& getWrappedValue()
	{
		Beagle_StackTraceBeginM();
		return mWrappedValue;
		Beagle_StackTraceEndM("T& WrapperT<T>::getWrappedValue()");
	}

	/*!
	 *  \brief Get the wrapped type value.
	 *  \return A constant reference to the wrapped value.
	 */
	inline const T& getWrappedValue() const
	{
		Beagle_StackTraceBeginM();
		return mWrappedValue;
		Beagle_StackTraceEndM("const T& WrapperT<T>::getWrappedValue() const");
	}


	/*!
	 *  \brief Set the wrapped type value.
	 *  \param inNewValue Set the wrapped value.
	 */
	inline void setWrappedValue(const T& inNewValue)
	{
		Beagle_StackTraceBeginM();
		mWrappedValue = inNewValue;
		Beagle_StackTraceEndM("void WrapperT<T>::setWrappedValue(const T& inNewValue)");
	}

private:
	T mWrappedValue;   //!< Wrapped type value.

};

}


/*!
 *  \brief Constructor a wrapper with the value given.
 *  \param inWrappedValue Value of the wrapped type instance.
 */
template <class T>
Beagle::WrapperT<T>::WrapperT(const T& inWrappedValue) :
		mWrappedValue(inWrappedValue)
{ }


/*!
 *  \brief Compare the equality of two wrapper of a given type, using EqualCompType.
 *  \param inRightObj Object reference to the right object to compare.
 *  \return True if wrappers are equal, false if not.
 */
template <class T>
bool Beagle::WrapperT<T>::isEqual(const Beagle::Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const WrapperT<T>& lRightWrapper = castObjectT<const WrapperT<T>&>(inRightObj);
	return mWrappedValue == lRightWrapper.mWrappedValue;
	Beagle_StackTraceEndM("bool WrapperT<T>::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Get the strict weak order of two wrapper of a given type, using LessCompType.
 *  \param inRightObj Object reference to the right object from which we get the order.
 *  \return True if left (this) wrapper is less than the right one, false if not.
 */
template <class T>
bool Beagle::WrapperT<T>::isLess(const Beagle::Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const WrapperT<T>& lRightWrapper = castObjectT<const WrapperT<T>&>(inRightObj);
	return mWrappedValue < lRightWrapper.mWrappedValue;
	Beagle_StackTraceEndM("bool WrapperT<T>::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a wrapper from a XML iterator.
 *  \param inIter XML iterator from which the wrapper is read.
 *  \throw Beagle::IOException If a problem occured during the operation (usually bad format).
 */
template <class T>
void Beagle::WrapperT<T>::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if(!inIter) mWrappedValue = T();
	else {
		if(inIter->getType() != PACC::XML::eString)
			throw Beagle_IOExceptionNodeM(*inIter, "value of wrapper not found");
		if(inIter->getValue().empty()) mWrappedValue = T();
		else {
			std::istringstream lISS(inIter->getValue());
			lISS >> mWrappedValue;
		}
	}
	Beagle_StackTraceEndM("void WrapperT<T>::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write a wrapper into a XML streamer.
 *  \param ioStreamer XML streamer into which the wrapper is written.
 *  \param inIndent Whether output should be indented.
 */
template <class T>
void Beagle::WrapperT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mWrappedValue << std::flush;
	ioStreamer.insertStringContent(lOSS.str(), false);
	Beagle_StackTraceEndM("void WrapperT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


#endif // Beagle_Core_WrapperT_hpp



