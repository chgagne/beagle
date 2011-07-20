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
 *  \file   Beagle/Core/LoggerXML.cpp
 *  \brief  Implementation of class LoggerXML.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.28 $
 *  $Date: 2007/09/27 14:58:02 $
 */

#include "Beagle/Core.hpp"

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif //BEAGLE_HAVE_OPENMP

using namespace Beagle;

/*!
 *  \brief Construct a new LoggerXML.
 */
LoggerXML::LoggerXML() :
	mStreamerConsole(std::cout),
	mStreamerFile(mStream)
{}


/*!
 *  \brief Copy constructor of a LoggerXML is disallowed.
 */
LoggerXML::LoggerXML(const LoggerXML& inOrig) :
	Logger(inOrig),
	mStreamerConsole(std::cout),
	mStreamerFile(mStream)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("LoggerXML", "LoggerXML", "LoggerXML");
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Destruct a LoggerXML.
 */
LoggerXML::~LoggerXML()
{
	Beagle_StackTraceBeginM();
	terminate();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copying a LoggerXML is disallowed.
 */
void LoggerXML::operator=(const LoggerXML&)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("operator=", "LoggerXML", "LoggerXML");
	Beagle_StackTraceEndM();
}


/*!
*   \brief Initialize this logger.
 *  \param ioSystem Reference to the system to use for initialization.
 */
void LoggerXML::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if(mConsoleLevel->getWrappedValue() != Logger::eNothing) {
		mStreamerConsole.insertHeader();
		mStreamerConsole.openTag("Beagle");
		mStreamerConsole.insertAttribute("version", BEAGLE_VERSION);
		mStreamerConsole.openTag("Logger");
	}

	if(!mFileName->getWrappedValue().empty()) {
		// remove previous backup
		std::string lFilenameBak = mFileName->getWrappedValue() + "~";
		std::remove(lFilenameBak.c_str());
		std::rename(mFileName->getWrappedValue().c_str(), lFilenameBak.c_str());
		// open log file
		mStream.open(mFileName->getWrappedValue().c_str());
		if(!mStream.good())
			throw Beagle_RunTimeExceptionM(std::string("Cannot open log file ")+mFileName->getWrappedValue());
		mStreamerFile.insertHeader();
		mStreamerFile.openTag("Beagle");
		mStreamerFile.insertAttribute("version", BEAGLE_VERSION);
		mStreamerFile.openTag("Logger");
	}

	Logger::init(ioSystem);

	Beagle_StackTraceEndM();
}

/*!
 *  \brief Log message to console and file devices.
 *  \param inMessage Message to log.
 *  \param inLevel Log level of message.
 *  \param inFile Source file of message.
 *  \param inFunction Source function of message.
 *
 *  This method outputs messages in XML format.
 */
void LoggerXML::logMessage(const std::string& inMessage,
                           unsigned int inLevel,
                           const std::string& inFile,
                           const std::string& inFunction)
{
	Beagle_StackTraceBeginM();

	if(isInitialized() == false) {
		std::ostringstream lOSS;
		lOSS << "Logger is not initialized, therefore logging is impossible. Logging before the initialization phase ";
		lOSS << "should be done through the log buffer, for instance by using macro Beagle_AddToLogBufferM.";
		throw Beagle_InternalExceptionM(lOSS.str());
	}

	// log to console
	if(mConsoleLevel->getWrappedValue() >= inLevel) {
#pragma omp critical (Beagle_Logger_Log_Console)
		{
			mStreamerConsole.openTag("Log", false);
			if(mShowLevel->getWrappedValue()) mStreamerConsole.insertAttribute("level", uint2str(inLevel));
			if(mShowFile->getWrappedValue())  mStreamerConsole.insertAttribute("file",  inFile);
			if(mShowFunction->getWrappedValue()) mStreamerConsole.insertAttribute("function", inFunction);
			if(mShowTime->getWrappedValue())  mStreamerConsole.insertAttribute("time", PACC::Date().get("%X"));
			mStreamerConsole.insertStringContent(inMessage, false);
			mStreamerConsole.closeTag();
		}
	}

	// log to file
	if(mFileLevel->getWrappedValue() >= inLevel && !mFileName->getWrappedValue().empty()) {
#pragma omp critical (Beagle_Logger_Log_File)
		{
			mStreamerFile.openTag("Log", false);
			if(mShowLevel->getWrappedValue()) mStreamerFile.insertAttribute("level", uint2str(inLevel));
			if(mShowFile->getWrappedValue())  mStreamerFile.insertAttribute("file",  inFile);
			if(mShowFunction->getWrappedValue()) mStreamerFile.insertAttribute("function", inFunction);
			if(mShowTime->getWrappedValue())  mStreamerFile.insertAttribute("time", PACC::Date().get("%X"));
			mStreamerFile.insertStringContent(inMessage, false);
			mStreamerFile.closeTag();
		}
	}

	Beagle_StackTraceEndM();
}

/*!
 *  \brief Log serialized object to console and file devices.
 *  \param inObject Object to log.
 *  \param inLevel Log level of message.
 *  \param inFile Source filename of message.
 *  \param inFunction Source function name of message.
 *
 *  This method outputs the serialized objects in XML format.
 */
void LoggerXML::logObject(const Object& inObject, unsigned int inLevel, const std::string& inFile, const std::string& inFunction)
{
	Beagle_StackTraceBeginM();

	if(isInitialized() == false) {
		std::ostringstream lOSS;
		lOSS << "Logger is not initialized, therefore logging is impossible. Logging before the initialization phase ";
		lOSS << "should be done through the log buffer, for instance by using macro Beagle_AddToLogBufferM.";
		throw Beagle_InternalExceptionM(lOSS.str());
	}

	// Log to console
	if(mConsoleLevel->getWrappedValue() >= inLevel) {
#pragma omp critical (Beagle_Logger_Log_Console)
		{
			mStreamerConsole.openTag("Log", true);
			if(mShowLevel->getWrappedValue()) mStreamerConsole.insertAttribute("level", uint2str(inLevel));
			if(mShowFile->getWrappedValue())  mStreamerConsole.insertAttribute("file",  inFile);
			if(mShowFunction->getWrappedValue()) mStreamerConsole.insertAttribute("function", inFunction);
			if(mShowTime->getWrappedValue())  mStreamerConsole.insertAttribute("time", PACC::Date().get("%X"));
			inObject.write(mStreamerConsole, true);
			mStreamerConsole.closeTag();
		}
	}

	// Log to file
	if(mFileLevel->getWrappedValue() >= inLevel && !mFileName->getWrappedValue().empty()) {
#pragma omp critical (Beagle_Logger_Log_File)
		{
			mStreamerFile.openTag("Log", true);
			if(mShowLevel->getWrappedValue()) mStreamerFile.insertAttribute("level", uint2str(inLevel));
			if(mShowFile->getWrappedValue())  mStreamerFile.insertAttribute("file",  inFile);
			if(mShowFunction->getWrappedValue()) mStreamerFile.insertAttribute("function", inFunction);
			if(mShowTime->getWrappedValue())  mStreamerFile.insertAttribute("time", PACC::Date().get("%X"));
			inObject.write(mStreamerFile, true);
			mStreamerFile.closeTag();
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Terminate XML logger operations.
 */
void LoggerXML::terminate()
{
	Beagle_StackTraceBeginM();

	if(isInitialized()) {
		if((mConsoleLevel != NULL) && (mConsoleLevel->getWrappedValue() != 0)) {
			mStreamerConsole.closeAll();
		}

		if((mFileName != NULL) && (mFileName->getWrappedValue().empty() == false)) {
			mStreamerFile.closeAll();
			mStream.close();
		}
	}
	Beagle_StackTraceEndM();
}

