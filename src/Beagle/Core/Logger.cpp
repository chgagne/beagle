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
 *  \brief Add a message in the log buffer, required to log before the logger gets initialized.
 *  \param inMessage Message to log.
 *  \param inLevel Log level used for message.
 *  \param inFile Filename where message is created.
 *  \param inFunction Function name of message.
 */
void Logger::addToBuffer(std::string inMessage,
                         unsigned int inLevel,
                         std::string inFile,
                         std::string inFunction)
{
	Beagle_StackTraceBeginM();
	if(isInitialized()) throw Beagle_InternalExceptionM("Cannot add to log buffer once the logger is initialized!");
	mBuffer.push_back(Logger::Message(inMessage,inLevel,inFile,inFunction));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy messages buffer.
 *  \param inLogger Logger containing message buffer to copy.
 */
void Logger::copyBuffer(const Logger& inLogger) {
	Beagle_StackTraceBeginM();
	mBuffer = inLogger.mBuffer;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this logger.
 *  \param ioSystem System reference.
 */
void Logger::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Set initialized flag
	setInitializedFlag(true);

	// Log Beagle version and current log levels
	logMessage(std::string("Open BEAGLE, version ")+BEAGLE_VERSION, eBasic, __FILE__, __PRETTY_FUNCTION__);
	if(mConsoleLevel->getWrappedValue() == Logger::eNothing) {
		logMessage("Console logging is disabled", eBasic, __FILE__, __PRETTY_FUNCTION__);
	} else {
		logMessage(std::string("Setting console log level ")+mConsoleLevel->serialize(), eBasic, __FILE__, __PRETTY_FUNCTION__);
	}

	if(mFileName->getWrappedValue().empty()) {
		logMessage("File logging is disabled", eBasic, __FILE__, __PRETTY_FUNCTION__);
	} else {
		logMessage(std::string("Setting file log level ")+mFileLevel->serialize(), eBasic, __FILE__, __PRETTY_FUNCTION__);
		logMessage(std::string("Logging to file '")+mFileName->serialize()+std::string("'"), eBasic, __FILE__, __PRETTY_FUNCTION__);
	}

	// output buffered messages
	for(std::list<Message>::const_iterator lIter = mBuffer.begin(); lIter != mBuffer.end(); ++lIter) {
		logMessage(lIter->mMessage, lIter->mLevel, lIter->mFile, lIter->mFunction);
	}
	mBuffer.clear();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Log current time if log level is equal to than the level associated.
 *  \param inLevel Log level of the message.
 */
void Logger::logCurrentTime(unsigned int inLevel)
{
	Beagle_StackTraceBeginM();
	std::string lMessage = std::string("Current date and time: ")+PACC::Date().get("%X %d %b %Y");
	logMessage(lMessage, inLevel, __FILE__, __PRETTY_FUNCTION__);
	Beagle_StackTraceEndM();
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

	lDescription = Register::Description("Show source filename of messages in logs",
	                                     "Bool",
	                                     "0",
	                                     "Indicates whether source filename of messages should be included in messages."
	                                    );
	mShowFile = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.file", new Bool(false), lDescription));

	lDescription = Register::Description("Show function name information in logs",
	                                     "Bool",
	                                     "0",
	                                     "Indicates whether source function name of messages should be included in messages."
	                                    );
	mShowFunction = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.function", new Bool(false), lDescription));

	lDescription = Register::Description("Show message's time in logs",
	                                     "Bool",
	                                     "0",
	                                     "Indicates whether message's time should be included in messages."
	                                    );
	mShowTime = castHandleT<Bool>(ioSystem.getRegister().insertEntry("lg.show.time", new Bool(false), lDescription));

	Beagle_StackTraceEndM();
}
