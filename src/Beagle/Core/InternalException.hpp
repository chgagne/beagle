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
 *  \file   Beagle/Core/InternalException.hpp
 *  \brief  Definition of class InternalException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_Core_InternalException_hpp
#define Beagle_Core_InternalException_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/TargetedException.hpp"


/*!
 *  \def   Beagle_UndefinedMethodInternalExceptionM
 *  \brief Construct a Beagle::InternalException using the message given.
 *  \param METHOD Name of the method undefined.
 *  \param OBJECT Name of the object where the method is declared but undefined.
 *  \param NAME   Name of the instance that generated this exception.
 *  \ingroup Except
 */
#define Beagle_UndefinedMethodInternalExceptionM(METHOD,OBJECT,NAME) \
  Beagle::InternalException( \
    std::string("Method '").append(METHOD).append("' of class '").append(OBJECT).append( \
      "' is called but is not properly overdefined in subclass with name '").append(NAME).append("'."), \
    __FILE__,__LINE__)


/*!
 *  \def   Beagle_InternalExceptionM
 *  \brief Construct a Beagle::InternalException using the message given.
 *  \param MESS Exception message.
 *  \ingroup Except
 */
#define Beagle_InternalExceptionM(MESS) \
  Beagle::InternalException(MESS,__FILE__,__LINE__)


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class InternalException Beagle/Core/InternalException.hpp "Beagle/Core/InternalException.hpp"
 *  \brief Indication of an internal Beagle exception.
 *  \ingroup Core
 *  \ingroup Except
 */
class InternalException : public TargetedException
{

public:

	//! InternalException allocator type.
	typedef AllocatorT<InternalException,TargetedException::Alloc> Alloc;
	//! InternalException handle type.
	typedef PointerT<InternalException,TargetedException::Handle> Handle;
	//! InternalException bag type.
	typedef ContainerT<InternalException,TargetedException::Bag> Bag;

	explicit InternalException(std::string  inMessage="",
	                           std::string  inFileName="",
	                           unsigned int inLineNumber=0);
	virtual ~InternalException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}

#endif // Beagle_Core_InternalException_hpp
