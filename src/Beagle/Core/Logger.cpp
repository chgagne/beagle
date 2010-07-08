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
 *  \file   Beagle/Core/Logger.cpp
 *  \brief  Implementation of class Logger.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Initialize this logger.
 *  \param ioSystem System reference.
 */
void Logger::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// set initialized flag
	setInitializedFlag(true);

	// log Beagle version and current log levels
	log(std::string("Open BEAGLE, version ")+BEAGLE_VERSION, eBasic, "logger", "Beagle::Logger");
	if(mConsoleLevel->getWrappedValue() == Logger::eNothing) {
		log("Console logging is disabled", eBasic, "logger", "Beagle::Logger");
	}
	else {
	  log(std::string("Setting console log level ")+mConsoleLevel->serialize(), eBasic, "logger", "Beagle::Logger");
	}

	if(mFileName->getWrappedValue().empty()) {
		log("File logging is disabled", eBasic, "logger", "Beagle::Logger");
	}
	else {
		log(std::string("Setting file log level ")+mFileLevel->serialize(), eBasic, "logger", "Beagle::Logger");
		log(std::string("Logging to file '")+mFileName->serialize()+std::string("'"), eBasic, "logger", "Beagle::LoggerXML");
	}

	// output buffered messages
	for(std::list<Message>::const_iterator lIter = mBuffer.begin(); lIter != mBuffer.end(); ++lIter) {
		outputMessage(lIter->mMessage, lIter->mLevel, lIter->mType, lIter->mClass);
	}
	mBuffer.clear();

	Beagle_StackTraceEndM("void Logger::init(System&)");
}

/*!
 *  \brief Log string message to console and file.
 *  \param inMessage Message to log.
 *  \param inLevel Log level of message.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message.
 *
 *  Message is written only if its log level is equal or higher than the log 
 *  level of the device.
 */
void Logger::log(const std::string& inMessage, unsigned int inLevel, const std::string& inType, const std::string& inClass)
{
	Beagle_StackTraceBeginM();

	if(isInitialized()) {
		outputMessage(inMessage, inLevel, inType, inClass);
	} else {
		mBuffer.push_back(Message(inMessage, inLevel, inType, inClass));
	}

	Beagle_StackTraceEndM("void Logger::log(const std::string&, unsigned int, const std::string&, const std::string&)");
}

/*!
 *  \brief Log object to console or file.
 *  \param inObject Object to log.
 *  \param inLevel Log level of message.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message.
 *
 *  Message is written only if its log level is equal or higher than the log 
 *  level of the device.
 */
void Logger::log(const Object& inObject, unsigned int inLevel, const std::string& inType, const std::string& inClass)
{
	Beagle_StackTraceBeginM();

	if(isInitialized()) {
		outputObject(inObject, inLevel, inType, inClass);
	} else {
		throw Beagle_RunTimeExceptionM("Cannot log an object before logger has been initialized!");
	}

	Beagle_StackTraceEndM("void Logger::log(const Object&, unsigned int, const std::string&, const std::string&)");
}

/*!
 *  \brief Log current time if log level is equal to than the level associated.
 *  \param inLevel Log level of the message.
 */
void Logger::logCurrentTime(unsigned int inLevel)
{
	Beagle_StackTraceBeginM();

	std::string lMessage = std::string("Current date and time: ")+PACC::Date().get("%X %d %b %Y");
	log(lMessage, inLevel, "logger", "Beagle::Logger");

	Beagle_StackTraceEndM("void Logger::logCurrentTime(unsigned int)");
}

/*!
 *  \brief register the parameters of this logger.
 *  \param ioSystem Reference to the system to use for initialization.
 *
 * This method registers the following parameters:
 *   - lg.console.level
 *   - lg.file.level
 *   - lg.file.name
 *   - lg.show.level
 *   - lg.show.type
 *   - lg.show.class
 *   - lg.show.time
 */
void Logger::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Register::Description lDescription;

	lDescription = Register::Description("Console log level",
	                                     "UInt",
	                                     "2",
	                                     "Log level used for console output. Available levels are: (0) no log, (1) basic logs, (2) stats, (3) general informations, (4) details on operations, (5) trace of the algorithms, (6) verbose, (7) debug (enabled only in full debug mode)."
	                                    );
	mConsoleLevel = castHandleT<UInt>(ioSystem.getRegister().insertEntry("lg.console.level", new UInt(eStats), lDescription));

	lDescription = Register::Description("File log level",
	                                     "UInt",
	                                     "3",
	                                     "Log level used for file output. Available levels are: (0) no log, (1) basic logs, (2) stats, (3) general informations, (4) details on operations, (5) trace of the algorithms, (6) verbose, (7) debug (enabled only in full debug mode)."
	                                    );
	mFileLevel = castHandleT<UInt>(ioSystem.getRegister().insertEntry("lg.file.level", new UInt(eInfo), lDescription));

	lDescription = Register::Description("Log filename",
	                                     "String",
	                                     "'beagle.log'",
	                                     "Filename of the file log. An empty string means no output file."
	                                    );
	mFileName = castHandleT<String>(ioSystem.getRegister().insertEntry("lg.file.name", new String("beagle.log"), lDescription));

	lDescription = Register::Description("Show level information in logs",
	                                     "Bool",
	                                     "0",
	                                     "Indicates whether log levels should be included in messages."
	                                    );
	mShowLevel = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.level", new Bool(false), lDescription));

	lDescription = Register::Description("Show message type information in logs",
	                                     "Bool",
	                                     "0",
	                                     "Indicates whether message types should be included in messages."
	                                    );
	mShowType = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.type", new Bool(false), lDescription));

	lDescription = Register::Description("Show class information in logs",
	                                     "Bool",
	                                     "0",
	                                     "Indicates whether class names should be included in messages."
	                                    );
	mShowClass = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.class", new Bool(false), lDescription));

        lDescription = Register::Description("Show message's time in logs",
                                             "Bool",
                                             "0",
                                             "Indicates whether message's time should be included in messages."
                                            );
        mShowTime = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.time", new Bool(false), lDescription));

	Beagle_StackTraceEndM("void Logger::registerParams(System&)");
}
