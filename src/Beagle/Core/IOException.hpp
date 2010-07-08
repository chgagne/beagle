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
 *  \file   Beagle/Core/IOException.hpp
 *  \brief  Definition of class IOException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_Core_IOException_hpp
#define Beagle_Core_IOException_hpp

#include <string>

#include "PACC/XML.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/TargetedException.hpp"


/*!
 *  \def   Beagle_IOExceptionMessageM
 *  \brief Construct a Beagle::IOException using the message given.
 *  \param MESS Exception message.
 *  \ingroup Except
 */
#define Beagle_IOExceptionMessageM(MESS) \
  Beagle::IOException((MESS),__FILE__,__LINE__)


/*!
 *  \def   Beagle_IOExceptionNodeM
 *  \brief Construct a Beagle::IOException using the message given.
 *  \param XMLNODE XML node for which the exception occured.
 *  \param MESS Exception message.
 *  \ingroup Except
 */
#define Beagle_IOExceptionNodeM(XMLNODE,MESS) \
  Beagle::IOException((XMLNODE),(MESS),__FILE__,__LINE__)


namespace Beagle
{

// Forward declarations
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;


/*!
 *  \class IOException Beagle/Core/IOException.hpp "Beagle/Core/IOException.hpp"
 *  \brief Beagle exception indicating that a parameter, or such, is invalid.
 *  \ingroup Core
 *  \ingroup Except
 *  \ingroup IO
 */
class IOException : public TargetedException
{

public:

	//! IOException allocator type.
	typedef AllocatorT<IOException,TargetedException::Alloc> Alloc;
	//! IOException handle type.
	typedef PointerT<IOException,TargetedException::Handle> Handle;
	//! IOException bag type.
	typedef ContainerT<IOException,TargetedException::Bag> Bag;

	explicit IOException(std::string  inMessage="",
	                     std::string  inFileName="",
	                     unsigned int inLineNumber=0);
	explicit IOException(const PACC::XML::Node& inIter,
	                     std::string  inMessage="",
	                     std::string  inFileName="",
	                     unsigned int inLineNumber=0);
	virtual ~IOException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}

#endif // Beagle_Core_IOException_hpp

