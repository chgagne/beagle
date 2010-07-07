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
 *  \file   beagle/Core/BadCastException.hpp
 *  \brief  Definition of class BadCastException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_BadCastException_hpp
#define Beagle_Core_BadCastException_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/TargetedException.hpp"

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
 *  \class BadCastException beagle/Core/BadCastException.hpp "beagle/Core/BadCastException.hpp"
 *  \brief Beagle exception indicating that a bad cast (by a call to castObjectT) happen.
 *  \ingroup OOF
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
