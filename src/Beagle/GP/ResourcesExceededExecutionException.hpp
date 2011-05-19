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
 *  \file   beagle/GP/ResourcesExceededExecutionException.hpp
 *  \brief  Definition of class GP::ResourcesExceededExecutionException.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_ResourcesExceededExecutionException_hpp
#define Beagle_GP_ResourcesExceededExecutionException_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Exception.hpp"
#include "beagle/GP/ExecutionException.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class ResourcesExceededExecutionException beagle/GP/ResourcesExceededExecutionException.hpp 
 *     "beagle/GP/ResourcesExceededExecutionException.hpp"
 *  \brief Basic GP exception related to resources management in individuals execution.
 *  \ingroup GPF
 *  \ingroup Except
 */
class ResourcesExceededExecutionException : public ExecutionException
{

public:

	//! ResourcesExceededExecutionException allocator type.
	typedef AllocatorT<ResourcesExceededExecutionException,ExecutionException::Alloc>
	Alloc;
	//! ResourcesExceededExecutionException handle type.
	typedef PointerT<ResourcesExceededExecutionException,ExecutionException::Handle>
	Handle;
	//! ResourcesExceededExecutionException bag type.
	typedef ContainerT<ResourcesExceededExecutionException,ExecutionException::Bag>
	Bag;

	explicit ResourcesExceededExecutionException(std::string inMessage="");
	virtual ~ResourcesExceededExecutionException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}
}

#endif // Beagle_GP_ResourcesExceededExecutionException_hpp

