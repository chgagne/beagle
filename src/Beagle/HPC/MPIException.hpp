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
 *  \file   beagle/HPC/MPIException.hpp
 *  \brief  Definition of class MPIException.
 *  \author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision:  $
 *  $Date: $
 */

#ifndef Beagle_HPC_MPIException_hpp
#define Beagle_HPC_MPIException_hpp

#include <mpi.h>
#include "beagle/TargetedException.hpp"

/*!
 *  \def   Beagle_MPIExceptionM
 *  \brief Construct a Beagle::MPIException using the MPI::Exception that has been caught.
 *  \param MESS Exception message.
 *  \ingroup Except
 */
#define Beagle_MPIExceptionM(MESS) \
  Beagle::HPC::MPIException((MESS),__FILE__,__LINE__)

#ifndef BEAGLE_NDEBUG
#define Beagle_HPC_StackTraceEndM(NAME) \
} catch (MPI::Exception& inException) { \
    throw Beagle_MPIExceptionM(inException.Get_error_string()); \
} catch (Beagle::Exception& inException) { \
    inException.pushStackTrace(NAME,__FILE__,__LINE__); \
    throw; \
}
#else // BEAGLE_NDEBUG
#define Beagle_HPC_StackTraceEndM(NAME)
#endif // BEAGLE_NDEBUG


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

namespace HPC
{

/*!
 *  \class MPIException beagle/HPC/MPIException.hpp "beagle/HPC/MPIException.hpp"
 *  \brief Run time checking exception.
 *  \ingroup OOF
 *  \ingroup Except
 *  \ingroup HPC
 */
class MPIException : public TargetedException
{

public:

	//! MPIException allocator type.
	typedef AllocatorT<MPIException,TargetedException::Alloc> Alloc;
	//! MPIException handle type.
	typedef PointerT<MPIException,TargetedException::Handle> Handle;
	//! MPIException bag type.
	typedef ContainerT<MPIException,TargetedException::Bag> Bag;

	explicit MPIException(std::string inMessage="",
	                          std::string inFileName="",
	                          unsigned int inLineNumber=0);
	virtual ~MPIException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};
}
}


#endif // Beagle_HPC_MPIException_hpp

