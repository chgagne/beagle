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
 *  \file   Beagle/Core/ArrayT.hpp
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

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Exception.hpp"
#include "Beagle/Core/castObjectT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/IOException.hpp"

namespace Beagle
{


/*!
 *  \class ArrayT Beagle/Core/ArrayT.hpp "Beagle/Core/ArrayT.hpp"
 *  \brief Template of an ArrayT that adapt an array type (std::vector of the parametrized type)
*     to the Beagle Object interface.
 *  \param T The array-ed type.
 *  \ingroup Core
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_ArrayT_hpp



