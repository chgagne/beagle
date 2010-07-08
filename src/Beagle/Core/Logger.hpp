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
 *  \file   Beagle/Core/Logger.hpp
 *  \brief  Definition of the class Logger.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2007/08/29 17:38:31 $
 */

#ifndef Beagle_Core_Logger_hpp
#define Beagle_Core_Logger_hpp

#include <fstream>
#include <string>
#include <list>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/String.hpp"
#include "Beagle/Core/Bool.hpp"

#include "PACC/Util/Date.hpp"

/*!
 *  \def Beagle_LogBasicM
 *  \brief Log given message if log level is equal to or more than "Basic (1)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogBasicM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eBasic, inType, inClass)

/*!
 *  \def Beagle_LogStatsM
 *  \brief Log given message if log level is equal to or more than "Stats (2)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogStatsM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eStats, inType, inClass)

/*!
 *  \def Beagle_LogInfoM
 *  \brief Log given message if log level is equal to or more than "Info (3)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogInfoM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eInfo, inType, inClass)

/*!
 *  \def Beagle_LogDetailedM
 *  \brief Log given message if log level is equal to or more than "Detailed (4)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogDetailedM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eDetailed, inType, inClass)

/*!
 *  \def Beagle_LogTraceM
 *  \brief Log given message if log level is equal to or more than "Trace (5)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogTraceM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eTrace, inType, inClass)

/*!
 *  \def Beagle_LogVerboseM
 *  \brief Log given message if log level is equal to or more than "Verbose (6)".
 *    Executed only when the framework is compiled with optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_LogVerboseM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eVerbose, inType, inClass)
#else  // BEAGLE_NDEBUG
#define Beagle_LogVerboseM(ioLogger,inType, inClass, inMessage)
#endif // BEAGLE_NDEBUG

/*!
 *  \def Beagle_LogDebugM
 *  \brief Log given message if log level is equal to or more than "Debug (7)".
 *    Executed only when the framework is compiled with full debug mode activated,
 *    and optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#if defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogDebugM(ioLogger, inType, inClass, inMessage) \
ioLogger.log(inMessage, Beagle::Logger::eDebug, inType, inClass)
#else  // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogDebugM(ioLogger, inType, inClass, inObject)
#endif // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)

/*!
 *  \def Beagle_LogObjectM
 *  \brief Log given Beagle object using the given log level.
 *  \param ioLogger Logger to log message into.
 *  \param inLogLevel Log level of the message
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inObject Beagle object to log.
 *  \ingroup Sys
 */
#define Beagle_LogObjectM(ioLogger, inLogLevel, inType, inClass, inObject) \
ioLogger.log((const Beagle::Object&)inObject, inLogLevel, inType, inClass)

/*!
 *  \def Beagle_LogObjectDebugM
 *  \brief Log given Beagle object using log level #7.
 *    Executed only when the framework is compiled with full debug mode activated,
 *    and optimisation turned off.
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inObject Beagle object to log.
 *  \ingroup Sys
 */
#if defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogObjectDebugM(ioLogger, inType, inClass, inObject) \
ioLogger.log((const Beagle::Object&)inObject, Beagle::Logger::eDebug, inType, inClass)
#else  // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogObjectDebugM(ioLogger, inType, inClass, inObject)
#endif // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)

namespace Beagle
{

// Forward declaration
class System;

/*!
 *  \class Logger Beagle/Core/Logger.hpp "Beagle/Core/Logger.hpp"
 *  \brief Message logger.
 *  \ingroup Core
 *  \ingroup Sys
 *
 *  This abstract class is for logging messages that summarize the internal 
 *  processes of Beagle. Two output devices are supported: console and log
 *  file. For each device, a distinct user modifiable log level is 
 *  implemented through the Register. Parameters \c lg.console.level and
 *  \c lg.file.level define the log levels associated with the console and 
 *  file devices, respectively. Messages are written to the output device 
 *  only if their log level is equal or high than the log level of the device.
 *
 *  Virtual pure methods \c Logger::outputMessage and \c Logger::outputObject 
 *  should be overridden in a derived class to physically write messages in a 
 *  specific format. Also, method Logger::terminate must be orridden to 
 *  terminate the logging process. For an example, see LoggerXML.
 *
 *  The derived class should override method \c Logger::registerParams if it 
 *  needs to add any parameters in the Register. But this method should always
 *  call the method of its base class first. It should also override method
 *  method \c Logger::init to initialize the physical device. This method
 *  should itself call the method of its base class after it has finished
 *  making its own initializations. 
 */
class Logger : public Component
{

public:

	/*!
	 *  \brief Available log levels (a level imply all previous levels).
	 */
	enum LogLevel {
	    eNothing=0,   //!< Nothing.
	    eBasic=1,     //!< Essential informations.
	    eStats=2,     //!< eBasic    + evolution statistics.
	    eInfo=3,      //!< eStats    + general informations.
	    eDetailed=4,  //!< eInfo     + detailed operations.
	    eTrace=5,     //!< eDetailed + trace of algorithms.
	    eVerbose=6,   //!< eTrace    + details on everything (disabled in optimization mode).
	    eDebug=7      //!< Debug (enabled only in full debug mode).
	};

	//! Logger allocator type.
	typedef AbstractAllocT<Logger,Component::Alloc> Alloc;
	//! Logger handle type.
	typedef PointerT<Logger,Component::Handle> Handle;
	//! Logger bag type.
	typedef ContainerT<Logger,Component::Bag> Bag;

	//! Construct default logger.
	Logger(void) : Component("Logger")
	{ }
	virtual ~Logger()
	{ }

	inline void copyBuffer(const Logger& inLogger) { mBuffer = inLogger.mBuffer; }

	virtual void registerParams(System& ioSystem);

	void log(const std::string& inMessage, unsigned int inLevel, const std::string& inType="",
	         const std::string& inClass="");
	void log(const Object& inObject, unsigned int inLevel, const std::string& inType="",
	         const std::string& inClass="");

	void logCurrentTime(unsigned int inLevel);

	/*!
	 *  \brief Output message to console and file devices.
	 *  \param inMessage Message to log.
	 *  \param inLevel Log level of message.
	 *  \param inType Type of message.
	 *  \param inClass Class of message.
	 *
	 *  This method must be overridden in a subclass for writing the message in a 
	 *  specific format.
	 */
	virtual void outputMessage(const std::string& inMessage, unsigned int inLevel, const std::string& inType, const std::string& inClass) = 0;

	/*!
	 *  \brief Output serialized object to console and file devices.
	 *  \param inObject Object to log.
	 *  \param inLevel Log level of message.
	 *  \param inType Type of message.
	 *  \param inClass Class of message.
	 *
	 *  This method must be overridden in a subclass for serializing the object 
	 *  in a specific format.
	 */
	virtual void outputObject(const Object& inObject, unsigned int inLevel, const std::string& inType, const std::string& inClass) = 0;

	virtual void init(System& ioSystem);

	/*!
	 *  \brief Terminate logging process.
	 *
	 *  This method must overridden in a subclass for terminating the logging
	 *  process. For instance, it should close any openned files.
	 */
	virtual void terminate(void) = 0;

protected:

	struct Message
	{
		std::string  mMessage;
		unsigned int mLevel;
		std::string  mType;
		std::string  mClass;

		Message(const std::string& inMessage,
		        unsigned int       inLevel,
		        const std::string& inType,
		        const std::string& inClass
		       ) :
				mMessage(inMessage),
				mLevel(inLevel),
				mType(inType),
				mClass(inClass)
		{ }

	};

	UInt::Handle       mConsoleLevel;     //!< Console log level.
	UInt::Handle       mFileLevel;        //!< File log level.
	String::Handle     mFileName;         //!< File name.
	Bool::Handle       mShowLevel;        //!< Show log level in messages.
	Bool::Handle       mShowType;         //!< Show type of messages.
	Bool::Handle       mShowClass;        //!< Show class that produce messages.
	Bool::Handle       mShowTime;         //!< Show messages log time.
	std::list<Message> mBuffer;           //!< Messages buffer.

};

}

#endif // Beagle_Core_Logger_hpp
