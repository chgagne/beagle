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
 *  \file   beagle/Core/ArrayT.hpp
 *  \brief  Definition and implementation of template WrapperT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:11 $
 */


#ifndef Beagle_Core_ArrayT_hpp
#define Beagle_Core_ArrayT_hpp

#include <vector>

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
 *  \class ArrayT beagle/Core/ArrayT.hpp "beagle/Core/ArrayT.hpp"
 *  \brief Template of an ArrayT that adapt an array type (std::vector of the parametrized type) 
*     to the Beagle Object interface.
 *  \param T The array-ed type.
 *  \ingroup OOF
 *  \ingroup Adapt
 *
 *  The array-ed type that is wrapped must have the following operator predefined:
 *  \li bool operator<(const T&, const T&); to compare strict weak ordering two arrays.
 *  \li bool operator==(const T&, const T&); to compare equality of two arrays.
 *  \li std::ostream& operator<<(std::ostream&, const T&); to output the array.
 *  \li std::istream& operator>>(std::istream&, T&); to input value the array.
 */
template <class T>
class ArrayT : public Object,
			public std::vector<T>
{

public:

	//! ArrayT allocator type.
	typedef AllocatorT< ArrayT<T>, Object::Alloc >
	Alloc;
	//! ArrayT handle type.
	typedef PointerT< ArrayT<T>, Object::Handle >
	Handle;
	//! ArrayT bag type.
	typedef ContainerT< ArrayT<T>, Object::Bag >
	Bag;

	explicit ArrayT(unsigned int inSize=0);
	explicit ArrayT(unsigned int inSize, const T& inModel);
	virtual ~ArrayT()
	{ }

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}


/*!
 *  \brief Constructor an array of the given size.
 *  \param inSize Size of the array.
 */
template <class T>
Beagle::ArrayT<T>::ArrayT(unsigned int inSize) :
		std::vector<T>(inSize)
{ }


/*!
 *  \brief Constructor an array of the given size and model.
 *  \param inSize Size of the array.
 *  \param inModel Model to use to build the array.
 */
template <class T>
Beagle::ArrayT<T>::ArrayT(unsigned int inSize, const T& inModel) :
		std::vector<T>(inSize, inModel)
{ }


/*!
 *  \brief Compare the equality of two array of a given type.
 *  \param inRightObj Object reference to the right object to compare.
 *  \return True if arrays are equal, false if not.
 */
template <class T>
bool Beagle::ArrayT<T>::isEqual(const Beagle::Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Beagle::ArrayT<T>& lLeftArray  = castObjectT<const Beagle::ArrayT<T>&>(*this);
	const std::vector<T>& lLeftVector    = lLeftArray;
	const Beagle::ArrayT<T>& lRightArray = castObjectT<const Beagle::ArrayT<T>&>(inRightObj);
	const std::vector<T>& lRightVector   = lRightArray;
	return (lLeftVector == lRightVector);
	Beagle_StackTraceEndM("bool ArrayT<T>::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Get the strict weak order of two wrapper of a given type.
 *  \param inRightObj Object reference to the right object from which we get the order.
 *  \return True if left (this) array is less than the right one, false if not.
 */
template <class T>
bool Beagle::ArrayT<T>::isLess(const Beagle::Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Beagle::ArrayT<T>& lLeftArray  = castObjectT<const Beagle::ArrayT<T>&>(*this);
	const std::vector<T>& lLeftVector    = lLeftArray;
	const Beagle::ArrayT<T>& lRightArray = castObjectT<const Beagle::ArrayT<T>&>(inRightObj);
	const std::vector<T>& lRightVector   = lRightArray;
	return (lLeftVector < lRightVector);
	Beagle_StackTraceEndM("bool ArrayT<T>::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a array from a XML iterator.
 *  \param inIter XML iterator from which the array is read.
 *  \throw Beagle::IOException If a problem occured during the operation (usually bad format).
 */
template <class T>
void Beagle::ArrayT<T>::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if(inIter->getType()!=PACC::XML::eString)
		throw Beagle_IOExceptionNodeM(*inIter,"expected string to read array!");
	std::vector<T>::clear();
	std::istringstream lISS(inIter->getValue());
	while(lISS.good()) {
		T lValue;
		lISS >> lValue;
		std::vector<T>::push_back(lValue);
		if(lISS.good()==false) break;
		int lDelim=lISS.get();
		if((lISS.good()==false) || (lDelim==-1)) break;
	}
	Beagle_StackTraceEndM("void ArrayT<T>::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write a array into a XML streamer.
 *  \param ioStreamer XML streamer into which the array is written.
 *  \param inIndent Whether output should be indented.
 */
template <class T>
void Beagle::ArrayT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	std::ostringstream lOSS;
	for(unsigned int i=0; i<std::vector<T>::size(); ++i) {
		lOSS << (*this)[i];
		if(i != (std::vector<T>::size()-1)) lOSS << ',';
	}
	ioStreamer.insertStringContent(lOSS.str());
	Beagle_StackTraceEndM("void ArrayT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


#endif // Beagle_Core_ArrayT_hpp



