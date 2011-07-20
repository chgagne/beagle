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
#include <sstream>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/String.hpp"
#include "Beagle/Core/Bool.hpp"

#include "PACC/Util/Date.hpp"


/*!
 *  \def Beagle_LogM
 *  \brief Log given message for a given log level.
 *  \param ioLogger Logger to log message into.
 *  \param inLevel inLogLevel of message.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogM(ioLogger, inLevel, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,inLevel,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}

/*!
 *  \def Beagle_AddToLogBufferM
 *  \brief Log given message for a given log level.
 *  \param ioLogger Logger to log message into.
 *  \param inLevel inLogLevel of message.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_AddToLogBufferM(ioLogger, inLevel, inMessage) \
{ioLogger.addToBuffer(inLevel,inMessage,__FILE__,__PRETTY_FUNCTION__);}

/*!
 *  \def Beagle_LogBasicM
 *  \brief Log given message if log level is equal to or more than "Basic (1)".
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogBasicM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eBasic,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}

/*!
 *  \def Beagle_LogStatsM
 *  \brief Log given message if log level is equal to or more than "Stats (2)".
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogStatsM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eStats,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}

/*!
 *  \def Beagle_LogInfoM
 *  \brief Log given message if log level is equal to or more than "Info (3)".
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogInfoM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eInfo,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}

/*!
 *  \def Beagle_LogDetailedM
 *  \brief Log given message if log level is equal to or more than "Detailed (4)".
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogDetailedM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eDetailed,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}

/*!
 *  \def Beagle_LogTraceM
 *  \brief Log given message if log level is equal to or more than "Trace (5)".
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogTraceM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eTrace,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}

/*!
 *  \def Beagle_LogVerboseM
 *  \brief Log given message if log level is equal to or more than "Verbose (6)".
 *    Executed only when the framework is compiled with optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_LogVerboseM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eVerbose,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}
#else  // BEAGLE_NDEBUG
#define Beagle_LogVerboseM(ioLogger, inLog)
#endif // BEAGLE_NDEBUG

/*!
 *  \def Beagle_LogDebugM
 *  \brief Log given message if log level is equal to or more than "Debug (7)".
 *    Executed only when the framework is compiled with full debug mode activated,
 *    and optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inLog Message to log.
 *  \ingroup Sys
 */
#if defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogDebugM(ioLogger, inLog) \
{Beagle::Logger::Entry lEntry(ioLogger,Beagle::Logger::eDebug,__FILE__,__PRETTY_FUNCTION__); lEntry << inLog;}
#else  // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogDebugM(ioLogger, inLog)
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
 *  Virtual pure methods \c Logger::logMessage and \c Logger::logObject
 *  should be overridden in a derived class to physically write messages in a
 *  specific format. Also, method Logger::terminate must be overridden to
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

	// Forward declaration of class Logger::Entry
	class Entry;


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

	/*!
	 *  \brief Log message to console and file devices.
	 *  \param inMessage Message to log.
	 *  \param inLevel Log level of message.
	 *  \param inFile Source file where message is produced.
	 *  \param inFunction Function name where message is produced.
	 *
	 *  This method must be overridden in a subclass for writing the message in a
	 *  specific format.
	 */
	virtual void logMessage(const std::string& inMessage,
	                        unsigned int inLevel,
	                        const std::string& inFile,
	                        const std::string& inFunction) =0;

	/*!
	 *  \brief Log object to console and file devices.
	 *  \param inObject Object to log.
	 *  \param inLevel Log level of message.
	 *  \param inFile Source file where message is produced.
	 *  \param inFunction Function name where message is produced.
	 *
	 *  This method must be overridden in a subclass for serializing the object
	 *  in a specific format.
	 */	
	virtual void logObject(const Object& inObject,
	                       unsigned int inLevel,
	                       const std::string& inFile,
	                       const std::string& inFunction) =0;

	/*!
	 *  \brief Terminate logging process.
	 *
	 *  This method must overridden in a subclass for terminating the logging
	 *  process. For instance, it should close any openned files.
	 */
	virtual void terminate() =0;

	/*!
	 *  \return Log level used for the console.
	 */
	inline unsigned int getConsoleLevel() const
	{
		Beagle_StackTraceBeginM();
		return mConsoleLevel->getWrappedValue();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Log level used for the file.
	 */
	inline unsigned int getFileLevel() const
	{
		Beagle_StackTraceBeginM();
		return mFileLevel->getWrappedValue();
		Beagle_StackTraceEndM();
	}

            void addToBuffer(std::string inMessage,
			                 unsigned int inLevel,
			                 std::string inFile,
			                 std::string inFunction);
	        void copyBuffer(const Logger& inLogger);
 	        void logCurrentTime(unsigned int inLevel);
	virtual void init(System& ioSystem);
	virtual void registerParams(System& ioSystem);

protected:

	struct Message {
		std::string  mMessage;
		unsigned int mLevel;
		std::string  mFile;
		std::string  mFunction;

		Message(const std::string& inMessage,
		        unsigned int       inLevel,
		        const std::string& inFile,
		        const std::string& inFunction
		       ) :
			mMessage(inMessage),
			mLevel(inLevel),
			mFile(inFile),
			mFunction(inFunction)
		{ }

	};

	UInt::Handle       mConsoleLevel;     //!< Console log level.
	UInt::Handle       mFileLevel;        //!< File log level.
	String::Handle     mFileName;         //!< File name.
	Bool::Handle       mShowLevel;        //!< Show log level in messages.
	Bool::Handle       mShowFile;         //!< Show source filename of messages.
	Bool::Handle       mShowFunction;     //!< Show source function that produce messages.
	Bool::Handle       mShowTime;         //!< Show messages log time.
	std::list<Message> mBuffer;           //!< Messages buffer.

};

}


// Forward declarations
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, bool inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, short inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, unsigned short inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, int inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, unsigned int inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, long inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, unsigned long inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, float inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, double inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, const char* inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, std::string inValue);
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, const Beagle::Object& inObject);


namespace Beagle {

/*!
 *  \brief Internal logger class, for management and printing of log messages.
 */
class Logger::Entry : public std::ostringstream {
	
public:
	
	/*!
	 *  \brief Construct a Logger::Entry object.
	 *  \param inLogger Logger in which logging is done.
	 *  \param inLevel Level of logging.
	 *  \param inFile Name of file in which log is produced.
	 *  \param inFunction Name of function in which log is produced.
	 */
	Entry(Logger& inLogger, Logger::LogLevel inLevel, std::string inFile, std::string inFunction) :
		mLogger(inLogger),
		mLevel(inLevel),
		mFile(inFile),
		mFunction(inFunction),
		mObject(NULL),
		mSkip((inLogger.getConsoleLevel() < inLevel) && (inLogger.getFileLevel() < inLevel))
	{ }
	
	/*!
	 *  \brief Close a log line by emptying it.
	 */
	~Entry() {
		Beagle_StackTraceBeginM();
		if(mSkip) return;
		if(mObject == NULL) mLogger.logMessage(str(), mLevel, mFile, mFunction);
		else mLogger.logObject(*mObject, mLevel, mFile, mFunction);
		Beagle_StackTraceEndM();	
	}

	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, bool inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, short inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, unsigned short inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, int inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, unsigned int inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, long inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, unsigned long inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, float inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, double inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, const char* inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, std::string inValue);
	friend Beagle::Logger::Entry& ::operator<<(Beagle::Logger::Entry& ioEntry, const Beagle::Object& inObject);

private:
	
	Logger&                mLogger;    //!< Logger in which logging is made.
	Logger::LogLevel       mLevel;     //!< Level used for the message.
	std::string            mFile;      //!< File in which message is produced.
	std::string            mFunction;  //!< Exact function in which message is produced.
	Beagle::Object const*  mObject;    //!< Object logged.
	bool                   mSkip;      //!< Whether we should not log anything.

};

}


/*!
 *  \brief Insert a bool in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, bool inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a short in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, short inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert an unsigned short in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, unsigned short inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert an int in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, int inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert an unsigned int in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, unsigned int inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a long in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, long inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert an unsigned long in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, unsigned long inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a float in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, float inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a double in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, double inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a char* in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, const char* inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a std::string in the log line.
 *  \param ioEntry Log line used.
 *  \param inValue Value to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, std::string inValue)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	std::ostringstream& lOSS = ioEntry;
	if(ioEntry.mObject != NULL) {
		lOSS << ioEntry.mObject->serialize();
		ioEntry.mObject = NULL;
	}
	lOSS << inValue;
	return ioEntry;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a Beagle::Object in the log line.
 *  \param ioEntry Log line used.
 *  \param inObject Beagle Object to log.
 *  \return A reference to log line objet.
 */
inline Beagle::Logger::Entry& operator<<(Beagle::Logger::Entry& ioEntry, const Beagle::Object& inObject)
{
	Beagle_StackTraceBeginM();
	if(ioEntry.mSkip) return ioEntry;
	if(ioEntry.str().empty()) ioEntry.mObject = &inObject;
	else {
		std::ostringstream& lOSS = ioEntry;
		lOSS << inObject.serialize();
	}
	return ioEntry;
	Beagle_StackTraceEndM();
}


#endif // Beagle_Core_Logger_hpp
