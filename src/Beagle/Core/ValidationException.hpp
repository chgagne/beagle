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
 *  \file   Beagle/Core/ValidationException.hpp
 *  \brief  Definition of class ValidationException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_ValidationException_hpp
#define Beagle_Core_ValidationException_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Exception.hpp"


/*!
 *  \def   Beagle_ValidateParameterM
 *  \brief Assert that a parameter is valid, following a given condition.
 *  \param COND Condition to test.
 *  \param PARAM String containing the name of the parameter.
 *  \param MESS String explaining the condition.
 *  \throw ValidationException When the condition is false.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_ValidateParameterM(COND,PARAM,MESS) \
  if( !(COND) ) throw Beagle::ValidationException( \
    std::string("The parameter ").append(PARAM).append(" is invalid (").append(MESS).append(")."))
#else  // BEAGLE_NDEBUG
#define Beagle_ValidateParameterM(COND,PARAM,MESS)
#endif // BEAGLE_NDEBUG


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class ValidationException Beagle/Core/ValidationException.hpp "Beagle/Core/ValidationException.hpp"
 *  \brief Beagle exception indicating that a parameter, or such, is invalid.
 *  \ingroup Core
 *  \ingroup Except
 */
class ValidationException : public Exception
{

public:

	//! ValidationException allocator type.
	typedef AllocatorT<ValidationException,Exception::Alloc> Alloc;
	//! ValidationException handle type.
	typedef PointerT<ValidationException,Exception::Handle> Handle;
	//! ValidationException bag type.
	typedef ContainerT<ValidationException,Exception::Bag> Bag;

	explicit ValidationException(std::string inMessage="");
	virtual ~ValidationException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}

#endif // Beagle_Core_ValidationException_hpp

