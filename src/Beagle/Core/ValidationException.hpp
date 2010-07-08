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
 *  \file   beagle/Core/ValidationException.hpp
 *  \brief  Definition of class ValidationException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_ValidationException_hpp
#define Beagle_Core_ValidationException_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Exception.hpp"


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
 *  \class ValidationException beagle/Core/ValidationException.hpp "beagle/Core/ValidationException.hpp"
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

