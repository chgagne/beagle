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
 *  \file   beagle/Core/LoggerXML.hpp
 *  \brief  Definition of the class LoggerXML.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_LoggerXML_hpp
#define Beagle_Core_LoggerXML_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Logger.hpp"
#include "PACC/Threading.hpp"

namespace Beagle
{

/*!
 *  \class LoggerXML beagle/Core/LoggerXML.hpp "beagle/Core/LoggerXML.hpp"
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

	LoggerXML(void);
	virtual ~LoggerXML(void);

	virtual void outputMessage(const std::string& inMessage, unsigned int inLevel, const std::string& inType, const std::string& inClass);
	virtual void outputObject(const Object& inObject, unsigned int inLevel, const std::string& inType, const std::string& inClass);
	virtual void init(System& ioSystem);
	virtual void terminate(void);

protected:

	std::ofstream       mStream;           //!< Output stream for file log.
	PACC::XML::Streamer mStreamerConsole;  //!< Streamer used to output messages at standard output.
	PACC::XML::Streamer mStreamerFile;     //!< Streamer used to output messages in file.

	friend class AllocatorT<LoggerXML,Logger::Alloc>;  // Necessary to instanticate allocators
	LoggerXML(const LoggerXML&);                       // Disable copy constructor
	void operator=(const LoggerXML&);                  // Disable copy operator

};

}

#endif // Beagle_Core_LoggerXML_hpp

