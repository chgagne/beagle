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
 *  \file   beagle/GP/MaxNodesExecutionException.hpp
 *  \brief  Definition of class GP::MaxNodesExecutionException.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MaxNodesExecutionException_hpp
#define Beagle_GP_MaxNodesExecutionException_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Exception.hpp"
#include "beagle/GP/ExecutionException.hpp"
#include "beagle/GP/ResourcesExceededExecutionException.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class MaxNodesExecutionException beagle/GP/MaxNodesExecutionException.hpp 
 *     "beagle/GP/MaxNodesExecutionException.hpp"
 *  \brief Throwed when the maximum number of GP nodes in an individual execution is overpassed.
 *  \ingroup GPF
 *  \ingroup Except
 */
class MaxNodesExecutionException : public ResourcesExceededExecutionException
{

public:

	//! MaxNodesExecutionException allocator type.
	typedef AllocatorT<MaxNodesExecutionException,ResourcesExceededExecutionException::Alloc>
	Alloc;
	//! MaxNodesExecutionException handle type.
	typedef PointerT<MaxNodesExecutionException,ResourcesExceededExecutionException::Handle>
	Handle;
	//! MaxNodesExecutionException bag type.
	typedef ContainerT<MaxNodesExecutionException,ResourcesExceededExecutionException::Bag>
	Bag;

	explicit MaxNodesExecutionException(std::string inMessage="",
	                                    unsigned int inNodesExecuted=0,
	                                    unsigned int inMaxNodesAllowed=0);
	virtual ~MaxNodesExecutionException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

	/*!
	 *  \return Number of nodes executed before the exception is thrown.
	 */
	inline unsigned int getNodesExecuted() const
	{
		Beagle_StackTraceBeginM();
		return mNodesExecuted;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Maximum number of nodes execution allowed.
	 */
	inline unsigned int getMaxNodesAllowed() const
	{
		Beagle_StackTraceBeginM();
		return mMaxNodesAllowed;
		Beagle_StackTraceEndM();
	}

protected:

	unsigned int mNodesExecuted;    //!< Number of nodes executed before the exception is thrown.
	unsigned int mMaxNodesAllowed;  //!< Maximum number of nodes execution allowed.

};

}
}

#endif // Beagle_GP_MaxNodesExecutionException_hpp

