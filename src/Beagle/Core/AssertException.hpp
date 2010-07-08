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
 *  \file   Beagle/Core/AssertException.hpp
 *  \brief  Definition of class AssertException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:41 $
 */

#ifndef Beagle_Core_AssertException_hpp
#define Beagle_Core_AssertException_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/TargetedException.hpp"


/*!
 *  \def   Beagle_AssertM
 *  \brief Assert that the condition is true. If not, throw a Beagle::AssertException.
 *  \param COND Condition to test, to assert.
 *  \throw Beagle::AssertException The condition is false.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_AssertM(COND) \
  if( !(COND) ) throw Beagle::AssertException("Assertion failed.",__FILE__,__LINE__)
#else // BEAGLE_NDEBUG
#define Beagle_AssertM(COND)
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_NonNullPointerAssertM
 *  \brief Assert that the pointer is non null. If not, throw a Beagle::AssertException.
 *  \param PTR Pointer to test.
 *  \throw Beagle::AssertException If the pointer is NULL.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_NonNullPointerAssertM(PTR) \
  if( !(PTR) ) throw Beagle::AssertException("Pointer asserted is NULL.",__FILE__,__LINE__)
#else  // BEAGLE_NDEBUG
#define Beagle_NonNullPointerAssertM(PTR)
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_BoundCheckAssertM
 *  \brief Assert the bounds of an index.
 *  \param INDEX Index value to test.
 *  \param LOWBOUND Lower bound.
 *  \param UPBOUND Upper bound.
 *  \throw Beagle::AssertException If the value is out of bound.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_BoundCheckAssertM(INDEX,LOWBOUND,UPBOUND) \
  if( (INDEX) < (LOWBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is less than the lower bound accepted (" << LOWBOUND << ")."; \
    throw Beagle::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  } \
  if( (INDEX) > (UPBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is more than the upper bound accepted (" << UPBOUND << ")."; \
    throw Beagle::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  }
#else  // BEAGLE_NDEBUG
#define Beagle_BoundCheckAssertM(INDEX,LOWBOUND,UPBOUND)
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_UpperBoundCheckAssertM
 *  \brief Assert the upper bound of an index value.
 *  \param INDEX Index value to test.
 *  \param UPBOUND Upper bound.
 *  \throw Beagle::AssertException If the value is out of bound.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_UpperBoundCheckAssertM(INDEX,UPBOUND) \
  if( (INDEX) > (UPBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is more than the upper bound accepted (" << UPBOUND << ")."; \
    throw Beagle::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  }
#else  // BEAGLE_NDEBUG
#define Beagle_UpperBoundCheckAssertM(INDEX,UPBOUND)
#endif // BEAGLE_NDEBUG


namespace Beagle
{

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class AssertException Beagle/Core/AssertException.hpp "Beagle/Core/AssertException.hpp"
 *  \brief Beagle exception indicating an badly asserted condition.
 *  \ingroup Core
 *  \ingroup Except
 */
class AssertException : public TargetedException
{

public:

	//! AssertException allocator type.
	typedef AllocatorT<AssertException,TargetedException::Alloc>
	Alloc;
	//! AssertException handle type.
	typedef PointerT<AssertException,TargetedException::Handle>
	Handle;
	//! AssertException bag type.
	typedef ContainerT<AssertException,TargetedException::Bag>
	Bag;

	explicit AssertException(std::string  inMessage="",
	                         std::string  inFileName="",
	                         unsigned int inLineNumber=0);
	virtual ~AssertException() throw()
	{ }

	virtual const char* getExceptionName() const throw();

};

}

#endif // Beagle_Core_AssertException_hpp



