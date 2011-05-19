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
 *  \file   Beagle/Core/TargetedException.hpp
 *  \brief  Definition of class TargetedException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_TargetedException_hpp
#define Beagle_Core_TargetedException_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Exception.hpp"


namespace Beagle
{

// Forward declaration.
template <class T, class BaseType> class AbstractAllocT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class TargetedException Beagle/Core/TargetedException.hpp "Beagle/Core/TargetedException.hpp"
 *  \brief Targeted Beagle exception that indicate a precise line number in a source file.
 *  \ingroup Core
 *  \ingroup Except
 */
class TargetedException : public Exception
{

public:

	//! TargetedException allocator type.
	typedef AbstractAllocT<TargetedException,Exception::Alloc> Alloc;
	//! TargetedException handle type.
	typedef PointerT<TargetedException,Exception::Handle> Handle;
	//! TargetedException bag type.
	typedef ContainerT<TargetedException,Exception::Bag> Bag;

	explicit TargetedException(std::string  inMessage="",
	                           std::string  inFileName="",
	                           unsigned int inLineNumber=0);
	virtual ~TargetedException() throw()
	{ }

	/*!
	 *  \brief  Getting the actual name of a Beagle exception.
	 *  \return Exception name.
	 */
	virtual const char* getExceptionName() const throw() =0;

	virtual void explain(std::ostream& ioES=std::cerr) throw();

	/*!
	 *  \brief  Getting the file name where the exception is detected (throwed).
	 *  \return File name where the exception is detected (throwed).
	 */
	inline const std::string& getFileName() const throw() {
		return mFileName;
	}

	/*!
	 *  \brief  Getting the line number in the file where the exception is detected (throwed).
	 *  \return Line number in the file where the exception is detected (throwed).
	 */
	inline unsigned int getLineNumber() const throw() {
		return mLineNumber;
	}

	/*!
	 *  \brief Set the file name where the exception is detected (throwed).
	 *  \param inFileName File name where the exception is detected.
	 */
	inline void setFileName(const std::string& inFileName) {
		Beagle_StackTraceBeginM();
		mFileName = inFileName;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the line number in the file where the exception is detected (throwed).
	 *  \param inLineNumber Line number in the file where the exception is detected.
	 */
	inline void setLineNumber(unsigned int inLineNumber) {
		Beagle_StackTraceBeginM();
		mLineNumber = inLineNumber;
		Beagle_StackTraceEndM();
	}

private:

	std::string  mFileName;    //!< Name of the file where the error is targeted.
	unsigned int mLineNumber;  //!< Line number in the file where the error is targeted.

};

}


#endif // Beagle_Core_ObjectException_hpp


