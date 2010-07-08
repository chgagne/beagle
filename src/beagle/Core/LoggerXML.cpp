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
 *  \file   beagle/Core/LoggerXML.cpp
 *  \brief  Implementation of class LoggerXML.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.28 $
 *  $Date: 2007/09/27 14:58:02 $
 */

#include "beagle/Core.hpp"

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif //BEAGLE_HAVE_OPENMP

using namespace Beagle;

/*!
 *  \brief Construct a new LoggerXML.
 */
LoggerXML::LoggerXML(void) :
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
	Beagle_StackTraceEndM("LoggerXML::LoggerXML(const LoggerXML&)");
}


/*!
 *  \brief Destruct a LoggerXML.
 */
LoggerXML::~LoggerXML(void)
{
	Beagle_StackTraceBeginM();
	terminate();
	Beagle_StackTraceEndM("LoggerXML::~LoggerXML(void)");
}


/*!
 *  \brief Copying a LoggerXML is disallowed.
 */
void LoggerXML::operator=(const LoggerXML&)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("operator=", "LoggerXML", "LoggerXML");
	Beagle_StackTraceEndM("void LoggerXML::operator=(const LoggerXML&)");
}


/*!
*  \brief Initialize this logger.
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

	Beagle_StackTraceEndM("void LoggerXML::init(System&)");
}

/*!
 *  \brief Output message to console and file devices.
 *  \param inMessage Message to log.
 *  \param inLevel Log level of message.
 *  \param inType Type of message.
 *  \param inClass Class of message.
 *
 *  This method outputs messages in XML format.
 */
void LoggerXML::outputMessage(const std::string& inMessage, unsigned int inLevel, const std::string& inType, const std::string& inClass)
{
	Beagle_StackTraceBeginM();

	// log to console
	if(mConsoleLevel->getWrappedValue() >= inLevel) {
		#pragma omp critical (Beagle_Logger_Log_Console)
		{
			mStreamerConsole.openTag("Log", false);
			if(mShowLevel->getWrappedValue()) mStreamerConsole.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue())  mStreamerConsole.insertAttribute("type",  inType);
			if(mShowClass->getWrappedValue()) mStreamerConsole.insertAttribute("class", inClass);
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
			if(mShowType->getWrappedValue())  mStreamerFile.insertAttribute("type",  inType);
			if(mShowClass->getWrappedValue()) mStreamerFile.insertAttribute("class", inClass);
                        if(mShowTime->getWrappedValue())  mStreamerFile.insertAttribute("time", PACC::Date().get("%X"));
			mStreamerFile.insertStringContent(inMessage, false);
			mStreamerFile.closeTag();
		}
	}

	Beagle_StackTraceEndM("void LoggerXML::outputMessage(cons std::string&, unsigned int, const std::string&, const std::string&)");
}

/*!
 *  \brief Output serialized object to console and file devices.
 *  \param inObject Object to log.
 *  \param inLevel Log level of message.
 *  \param inType Type of message.
 *  \param inClass Class of message.
 *
 *  This method outputs the serialized objects in XML format.
 */
void LoggerXML::outputObject(const Object& inObject, unsigned int inLevel, const std::string& inType, const std::string& inClass)
{
	Beagle_StackTraceBeginM();

	// log to console
	if(mConsoleLevel->getWrappedValue() >= inLevel) {
		#pragma omp critical (Beagle_Logger_Log_Console)
		{
			mStreamerConsole.openTag("Log", true);
			if(mShowLevel->getWrappedValue()) mStreamerConsole.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue())  mStreamerConsole.insertAttribute("type",  inType);
			if(mShowClass->getWrappedValue()) mStreamerConsole.insertAttribute("class", inClass);
			if(mShowTime->getWrappedValue())  mStreamerConsole.insertAttribute("time", PACC::Date().get("%X"));
			inObject.write(mStreamerConsole, true);
			mStreamerConsole.closeTag();
		}
	}

	// log to file	
	if(mFileLevel->getWrappedValue() >= inLevel && !mFileName->getWrappedValue().empty()) {
		#pragma omp critical (Beagle_Logger_Log_File)
		{
			mStreamerFile.openTag("Log", true);
			if(mShowLevel->getWrappedValue()) mStreamerFile.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue())  mStreamerFile.insertAttribute("type",  inType);
			if(mShowClass->getWrappedValue()) mStreamerFile.insertAttribute("class", inClass);
            if(mShowTime->getWrappedValue())  mStreamerFile.insertAttribute("time", PACC::Date().get("%X"));
			inObject.write(mStreamerFile, true);
			mStreamerFile.closeTag();
		}
	}

	Beagle_StackTraceEndM("void LoggerXML::outputObject(const Object& inObject, unsigned int inLevel, const std::string& inType, const std::string& inClass)");
}

/*!
 *  \brief Terminate XML logger operations.
 */
void LoggerXML::terminate(void)
{
	Beagle_StackTraceBeginM();

	if((*this).isInitialized()){
		if((mConsoleLevel != NULL) && (mConsoleLevel->getWrappedValue() != 0)) {
			mStreamerConsole.closeAll();
		}

		if((mFileName != NULL) && (mFileName->getWrappedValue().empty() == false)) {
			mStreamerFile.closeAll();
			mStream.close();
		}
	}
	Beagle_StackTraceEndM("void LoggerXML::terminate(void)");
}

