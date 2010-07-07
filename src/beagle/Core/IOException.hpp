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
 *  \file   beagle/Core/IOException.hpp
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

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/TargetedException.hpp"


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
 *  \class IOException beagle/Core/IOException.hpp "beagle/Core/IOException.hpp"
 *  \brief Beagle exception indicating that a parameter, or such, is invalid.
 *  \ingroup OOF
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

