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
 *  \file   Beagle/Core/Container.hpp
 *  \brief  Container class for implementing the Bag data type.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:11 $
 */

/*!
 *  \defgroup DataStruct Data Structures
 *  \ingroup Core
 *  \brief Object oriented data structures, part of the Core library.
 *
 *  The Open BEAGLE container is a random access table of heterogeneous objects. It is implemented as
 *  a dynamic array of specified smart pointers. Any objects that is a specialization of the contained
 *  type could be put in a container. The constraints are the same as those of the smart pointers: the
 *  objects must be allocated on the heap (via the \c new operator) and these objects must not be
 *  unallocated explicitly (i.e. \c delete of the objects are forbidden). Our container is
 *  implemented as a Beagle object that is also a STL vector of object handles.
 *  \code
 *  class Container : public Object, public std::vector<Object::Handle> { ... };
 *  \endcode
 *  Built this way, our container could be manipulated by the STL generic algorithms. It is also possible
 *  to make container of containers, since a container is an object that can be smart pointed.
 *
 *  As with the smart pointers and allocators, every Open BEAGLE type has a nested type that define a
 *  container of the specified type, the nested type named \c Bag. For example, the type that is a
 *  container of floating-point values, Beagle::Float, is the type Beagle::Float::Bag. The nested type
 *  is declared in classes as usual for the nested types \c Handle and \c Alloc, using a \c typedef
 *  to a template, in this case to template class Beagle::ContainerT.
 *  \code
 *  class MyClass : public SuperClass {
 *  public:
 *    typedef AllocatorT<MyClass,SuperClass::Alloc> Alloc;
 *    typedef PointerT<MyClass,SuperClass::Handle> Handle;
 *    typedef ContainerT<MyClass,SuperClass::Bag> Bag;
 *  };
 *  \endcode
 *
 */


#ifndef Beagle_Core_Container_hpp
#define Beagle_Core_Container_hpp

#include <vector>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/AssertException.hpp"


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class ContainerT;
template <class T, class BaseType> class PointerT;

/*!
 *  \class Container Beagle/Core/Container.hpp "Beagle/Core/Container.hpp"
 *  \brief Basic container data structure, implemented as a vector of smart pointers.
 *  \ingroup Core
 *  \ingroup DataStruct
 */
class Container : public Object, public std::vector<Object::Handle>
{

public:

	//! Container's allocator type.
	typedef AllocatorT<Container,Object::Alloc> Alloc;
	//! Container's handle type.
	typedef PointerT<Container,Object::Handle> Handle;
	//! Container's bag type.
	typedef ContainerT<Container,Object::Bag> Bag;

	explicit Container(unsigned int inN=0, Object::Handle inModel=NULL);
	virtual ~Container()
	{ }

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief  Standard const operator for accessing elements by index.
	 *  \param  inN Index of element (first is 0).
	 *  \return Smart pointer to element.
	 */
	inline const Object::Handle& operator[](unsigned int inN) const {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,size()-1);
		return std::vector<Pointer>::operator[](inN);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Standard  operator for accessing elements by index.
	 *  \param inN Index of element (first is 0).
	 *  \return Smart pointer to element.
	 */
	inline Object::Handle& operator[](unsigned int inN) {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,size()-1);
		return std::vector<Pointer>::operator[](inN);
		Beagle_StackTraceEndM();
	}

};

}

#endif // Beagle_Core_Container_hpp
