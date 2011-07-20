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
 *  \file   Beagle/Core/LoggerXML.hpp
 *  \brief  Definition of the class LoggerXML.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_LoggerXML_hpp
#define Beagle_Core_LoggerXML_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Logger.hpp"
#include "PACC/Threading.hpp"


namespace Beagle
{

/*!
 *  \class LoggerXML Beagle/Core/LoggerXML.hpp "Beagle/Core/LoggerXML.hpp"
 *  \brief XML message logger.
 *  \ingroup Core
 *
 *  This class formats log messages in XML.
 */
class LoggerXML : public Logger
{

public:

	//! LoggerXML allocator type.
	typedef AllocatorT<LoggerXML,Logger::Alloc> Alloc;
	//! LoggerXML handle type.
	typedef PointerT<LoggerXML,Logger::Handle> Handle;
	//! LoggerXML bag type.
	typedef ContainerT<LoggerXML,Logger::Bag> Bag;

	LoggerXML();
	virtual ~LoggerXML();

	virtual void init(System& ioSystem);
	virtual void logMessage(const std::string& inMessage,
	                        unsigned int inLevel,
	                        const std::string& inFile,
	                        const std::string& inFunction);
	virtual void logObject(const Object& inObject,
	                       unsigned int inLevel,
	                       const std::string& inFile,
	                       const std::string& inFunction);
	virtual void terminate();

protected:

	std::ofstream       mStream;           //!< Output stream for file log.
	PACC::XML::Streamer mStreamerConsole;  //!< Streamer used to output messages at standard output.
	PACC::XML::Streamer mStreamerFile;     //!< Streamer used to output messages in file.

	friend class AllocatorT<LoggerXML,Logger::Alloc>;  // Necessary to instanciate allocators
	LoggerXML(const LoggerXML&);                       // Disable copy constructor
	void operator=(const LoggerXML&);                  // Disable copy operator

};

}

#endif // Beagle_Core_LoggerXML_hpp

