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
 *  \file   Beagle/Core/BadCastException.hpp
 *  \brief  Definition of class BadCastException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_BadCastException_hpp
#define Beagle_Core_BadCastException_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/TargetedException.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


/*!
 *  \def   Beagle_BadCastExceptionM
 *  \brief Construct a Beagle::BadCastException.
 *  \param MESS Message explaining the bad cast problem.
 *  \ingroup Except
 */
#define Beagle_BadCastExceptionM(MESS) \
  Beagle::BadCastException(MESS,__FILE__,__LINE__)


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class BadCastException Beagle/Core/BadCastException.hpp "Beagle/Core/BadCastException.hpp"
 *  \brief Beagle exception indicating that a bad cast (by a call to castObjectT) happen.
 *  \ingroup Core
 *  \ingroup Except
 *  \ingroup ObjPt
 */
class BadCastException : public TargetedException
{

public:

	//! BadCastException allocator type.
	typedef AllocatorT<BadCastException,TargetedException::Alloc>
	Alloc;
	//! BadCastException handle type.
	typedef PointerT<BadCastException,TargetedException::Handle>
	Handle;
	//! BadCastException bag type.
	typedef ContainerT<BadCastException,TargetedException::Bag>
	Bag;

	explicit BadCastException(std::string  inMessage="",
	                          std::string  inFileName="",
	                          unsigned int inLineNumber=0);
	virtual ~BadCastException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}

#endif // Beagle_Core_BadCastException_hpp
