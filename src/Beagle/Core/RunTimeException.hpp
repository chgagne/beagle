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
 *  \file   Beagle/Core/RunTimeException.hpp
 *  \brief  Definition of class RunTimeException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_RunTimeException_hpp
#define Beagle_Core_RunTimeException_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Exception.hpp"
#include "Beagle/Core/TargetedException.hpp"

/*!
 *  \def   Beagle_RunTimeExceptionM
 *  \brief Construct a Beagle::RunTimeException using the message given.
 *  \param MESS Exception message.
 *  \ingroup Except
 */
#define Beagle_RunTimeExceptionM(MESS) \
  Beagle::RunTimeException((MESS),__FILE__,__LINE__)


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;


/*!
 *  \class RunTimeException Beagle/Core/RunTimeException.hpp "Beagle/Core/RunTimeException.hpp"
 *  \brief Run time checking exception.
 *  \ingroup Core
 *  \ingroup Except
 */
class RunTimeException : public TargetedException
{

public:

	//! RunTimeException allocator type.
	typedef AllocatorT<RunTimeException,TargetedException::Alloc> Alloc;
	//! RunTimeException handle type.
	typedef PointerT<RunTimeException,TargetedException::Handle> Handle;
	//! RunTimeException bag type.
	typedef ContainerT<RunTimeException,TargetedException::Bag> Bag;

	explicit RunTimeException(std::string inMessage="",
	                          std::string inFileName="",
	                          unsigned int inLineNumber=0);
	virtual ~RunTimeException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}


#endif // Beagle_Core_RunTimeException_hpp

