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
 *  \file   beagle/GP/MaxTimeExecutionException.hpp
 *  \brief  Definition of class GP::MaxTimeExecutionException.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MaxTimeExecutionException_hpp
#define Beagle_GP_MaxTimeExecutionException_hpp

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
 *  \class MaxTimeExecutionException beagle/GP/MaxTimeExecutionException.hpp 
 *     "beagle/GP/MaxTimeExecutionException.hpp"
 *  \brief Throwed when the maximum time allowed to an individual execution is overpassed.
 *  \ingroup GPF
 *  \ingroup Except
 */
class MaxTimeExecutionException : public ResourcesExceededExecutionException
{

public:

	//! MaxTimeExecutionException allocator type.
	typedef AllocatorT<MaxTimeExecutionException,ResourcesExceededExecutionException::Alloc>
	Alloc;
	//! MaxTimeExecutionException handle type.
	typedef PointerT<MaxTimeExecutionException,ResourcesExceededExecutionException::Handle>
	Handle;
	//! MaxTimeExecutionException bag type.
	typedef ContainerT<MaxTimeExecutionException,ResourcesExceededExecutionException::Bag>
	Bag;

	explicit MaxTimeExecutionException(std::string inMessage="",
	                                   double inTimeElapsed=0.0,
	                                   double inMaxTimeAllowed=0.0);
	virtual ~MaxTimeExecutionException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

	/*!
	 *  \return Time elapsed (in seconds) before the exception is thrown.
	 */
	inline double getTimeElapsed() const
	{
		Beagle_StackTraceBeginM();
		return mTimeElapsed;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Maximum execution time allowed (in seconds).
	 */
	inline double getMaxTimeAllowed() const
	{
		Beagle_StackTraceBeginM();
		return mMaxTimeAllowed;
		Beagle_StackTraceEndM();
	}

protected:

	double mTimeElapsed;     //!< Time elapsed in individual execution before throwing the exception.
	double mMaxTimeAllowed;  //!< Maximum execution time allowed.

};

}
}

#endif // Beagle_GP_MaxTimeExecutionException_hpp

