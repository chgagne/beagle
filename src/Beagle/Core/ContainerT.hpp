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
 *  \file   Beagle/Core/ContainerT.hpp
 *  \brief  Definition of template ContainerT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/09/12 14:23:47 $
 */

#ifndef Beagle_Core_ContainerT_hpp
#define Beagle_Core_ContainerT_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/RunTimeException.hpp"


namespace Beagle
{

/*!
 *  \class ContainerT Beagle/Core/ContainerT.hpp "Beagle/Core/ContainerT.hpp"
 *  \brief Templated class defining a T-type container, inheriting from BaseType container.
 *  \param T Type of the values contained.
 *  \param BaseType Base type from which the actual uniform bag type is derived.
 *  \ingroup Core
 *  \ingroup DataStruct
 */
template <class T, class BaseType>
class ContainerT : public BaseType
{

public:

	//! ContainerT allocator type.
	typedef AllocatorT< ContainerT<T,BaseType>, typename BaseType::Alloc > Alloc;
	//! ContainerT handle type.
	typedef PointerT< ContainerT<T,BaseType>, typename BaseType::Handle > Handle;
	//! ContainerT bag type.
	typedef ContainerT< ContainerT<T,BaseType>, typename BaseType::Bag > Bag;

	explicit ContainerT(unsigned int inN=0, typename T::Handle inModel=NULL);
	virtual ~ContainerT()
	{ }

	/*!
	 *  \brief  Access to a constant handle to the Nth element of the ContainerT.
	 *  \param  inN Indice of the constant handle (T::Handle) to get.
	 *  \return Constant handle (T::Handle) at the indice N.
	 */
	inline const typename T::Handle& operator[](unsigned int inN) const {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
		return castHandleT<T>(std::vector<Pointer>::operator[](inN));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Acces to an handle the Nth element of the ContainerT.
	 *  \param  inN Indice of the handle (T::Handle) to get.
	 *  \return Pointer (T::Handle) at the indice N.
	 */
	inline typename T::Handle& operator[](unsigned int inN) {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
		return castHandleT<T>(std::vector<Pointer>::operator[](inN));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Access to a handle to the Nth element of the ContainerT.
	 *  \param  inN Index of the handle (T::Handle) to get.
	 *  \return Pointer (T::Handle) at the index N.
	 */
	inline typename T::Handle& at(unsigned int inN) {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
		return castHandleT<T>(std::vector<Pointer>::operator[](inN));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Access to a handle to the Nth element of the ContainerT.
	 *  \param  inN Index of the handle (T::Handle) to get.
	 *  \return const Pointer (T::Handle) at the index N.
	 */
	inline const typename T::Handle& at(unsigned int inN) const {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
		return castHandleT<T>(std::vector<Pointer>::operator[](inN));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Access to a handle to the last element of the ContainerT.
	 *  \return Pointer (T::Handle) at the index N.
	 */
	inline typename T::Handle& back() {
		Beagle_StackTraceBeginM();
		if(BaseType::empty()) {
			std::ostringstream lOSS;
			lOSS << "Container named '" << BaseType::getName();
			lOSS << "' is empty; you've called the back() method but there is nothing to return.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		return castHandleT<T>(std::vector<Pointer>::back());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Access to a handle to the last element of the ContainerT.
	 *  \return Pointer (T::Handle) at the index N.
	 */
	inline const typename T::Handle& back() const {
		Beagle_StackTraceBeginM();
		if(BaseType::empty()) {
			std::ostringstream lOSS;
			lOSS << "Container named '" << BaseType::getName();
			lOSS << "' is empty; you've called the back() method but there is nothing to return.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		return castHandleT<T>(std::vector<Pointer>::back());
		Beagle_StackTraceEndM();
	}


};

}


/*!
 *  \brief Construct a container.
 *  \param inN Initial size of the container.
 *  \param inModel Initial value of the composing elements.
 */
template <class T, class BaseType>
Beagle::ContainerT<T,BaseType>::ContainerT(unsigned int inN, typename T::Handle inModel) :
	BaseType(inN, inModel)
{ }


#endif // Beagle_Core_ContainerT_hpp

