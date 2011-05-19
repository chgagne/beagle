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
 *  \file   beagle/HPC/LoggerXMLD.cpp
 *  \brief  Implementation of class LoggerXMLD.
 *  \author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

/*!
 *  \brief Construct a new LoggerXMLD.
 */
HPC::LoggerXMLD::LoggerXMLD(void) :
		mStreamerConsoleTemp(mConsoleStreamTemp),
		mStreamerFileTemp(mFileStreamTemp),
		mThread(0)
{

}


/*!
 *  \brief Copy constructor of a LoggerXMLD is disallowed.
 */
HPC::LoggerXMLD::LoggerXMLD(const LoggerXMLD& inOrig) :
		LoggerXML(inOrig),
	mStreamerConsoleTemp(mConsoleStreamTemp),
	mStreamerFileTemp(mFileStreamTemp)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("LoggerXMLD", "LoggerXMLD", "LoggerXMLD");
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Destruct a LoggerXMLD.
 */
HPC::LoggerXMLD::~LoggerXMLD(void)
{
	Beagle_StackTraceBeginM();
	if(mNodeType != "Supervisor" ){
		if(mThread !=0){
			mThread->cancel();
			delete mThread;
		}
	}
	mMPI->barrier();
	if(mNodeType == "Supervisor"){
		if(mThread !=0){
//			mThread->cancel();
			delete mThread;
		}
		terminate();
	}
	else{
		*mFileLevel = 0;
		*mConsoleLevel = 0;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copying a LoggerXMLD is disallowed.
 */
void HPC::LoggerXMLD::operator=(const LoggerXMLD&)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("operator=", "LoggerXMLD", "LoggerXMLD");
	Beagle_StackTraceEndM();
}


/*!
*  \brief Initialize this logger.
 *  \param ioSystem Reference to the system to use for initialization.
 */
void HPC::LoggerXMLD::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	mMPI = castHandleT<MPICommunication>(ioSystem.getComponent("MPICommunication"));

	if(!mMPI->isInitialized()){
		mMPI->init(ioSystem);
		mMPI->setInitializedFlag(true);
	}

	mNodeType = mMPI->getNodeType();
	mNodeRank = mMPI->getNodeRank()->getWrappedValue();

	if(mNodeType == "Supervisor"){
		LoggerXML::init(ioSystem);
		if(mLogEvolver->getWrappedValue() || mLogEvaluator->getWrappedValue()){
			mThread = new SupervisorLoggingThread(this, mMPI);
			mThread->run();
		}
	}
	else{
		if((mNodeType == "Evolver"   && mLogEvolver->getWrappedValue()) ||
		   (mNodeType == "Evaluator" && mLogEvaluator->getWrappedValue())) {
			mThread = new ChildLoggingThread(this, mMPI);
			mThread->run();
		}
	}

	Beagle_StackTraceEndM();
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
void HPC::LoggerXMLD::outputMessage(const std::string& inMessage,
				    unsigned int inLevel,
				    const std::string& inType,
				    const std::string& inClass)
{
	Beagle_StackTraceBeginM();
	if(mNodeType == "Evolver" && !mLogEvolver->getWrappedValue()) return;
	if(mNodeType == "Evaluator" && !mLogEvaluator->getWrappedValue()) return;
	mStreamMutex.lock();
 	if(mNodeType == "Supervisor") LoggerXML::outputMessage(inMessage,inLevel,inType,inClass);
	else{
		// log to temp console stream.
		if(mConsoleLevel->getWrappedValue() >= inLevel) {
			mStreamerConsoleTemp.openTag("Log",false);
			if(mShowRank->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("nodeRank",mNodeRank);
			if(mShowNodeType->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("nodeType",mNodeType);
			if(mShowLevel->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("type", inType);
			if(mShowClass->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("class", inClass);
			mStreamerConsoleTemp.insertStringContent(inMessage);
			mStreamerConsoleTemp.closeTag();
		}

		// log to temp file stream.
		if(mFileLevel->getWrappedValue() >= inLevel && !mFileName->getWrappedValue().empty()) {
			mStreamerFileTemp.openTag("Log",false);
			if(mShowRank->getWrappedValue()) mStreamerFileTemp.insertAttribute("nodeRank",mNodeRank);
			if(mShowNodeType->getWrappedValue()) mStreamerFileTemp.insertAttribute("nodeType",mNodeType);
			if(mShowLevel->getWrappedValue()) mStreamerFileTemp.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue())  mStreamerFileTemp.insertAttribute("type",  inType);
			if(mShowClass->getWrappedValue()) mStreamerFileTemp.insertAttribute("class", inClass);
			mStreamerFileTemp.insertStringContent(inMessage);
			mStreamerFileTemp.closeTag();
		}
	}
	mStreamMutex.unlock();
	Beagle_StackTraceEndM();
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
void HPC::LoggerXMLD::outputObject(const Object& inObject,
				   unsigned int inLevel,
				   const std::string& inType,
				   const std::string& inClass)
{
	Beagle_StackTraceBeginM();
	if(mNodeType == "Evolver" && !mLogEvolver->getWrappedValue()) return;
	if(mNodeType == "Evaluator" && !mLogEvaluator->getWrappedValue()) return;
	mStreamMutex.lock();
 	if(mNodeType == "Supervisor") LoggerXML::outputObject(inObject,inLevel,inType,inClass);
	else{
		// log to console stream.
		if(mConsoleLevel->getWrappedValue() >= inLevel) {
			mStreamerConsoleTemp.openTag("Log",false);
			if(mShowRank->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("nodeRank",mNodeRank);
			if(mShowNodeType->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("nodeType",mNodeType);
			if(mShowLevel->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("type", inType);
			if(mShowClass->getWrappedValue()) mStreamerConsoleTemp.insertAttribute("class", inClass);
			inObject.write(mStreamerConsoleTemp);
			mStreamerConsoleTemp.closeTag();
		}
		// log to file stream.
		if(mFileLevel->getWrappedValue() >= inLevel && !mFileName->getWrappedValue().empty()) {
			mStreamerFileTemp.openTag("Log",false);
			if(mShowRank->getWrappedValue()) mStreamerFileTemp.insertAttribute("nodeRank",mNodeRank);
			if(mShowNodeType->getWrappedValue()) mStreamerFileTemp.insertAttribute("nodeType",mNodeType);
			if(mShowLevel->getWrappedValue()) mStreamerFileTemp.insertAttribute("level", uint2str(inLevel));
			if(mShowType->getWrappedValue())  mStreamerFileTemp.insertAttribute("type",  inType);
			if(mShowClass->getWrappedValue()) mStreamerFileTemp.insertAttribute("class", inClass);
			inObject.write(mStreamerFileTemp);
			mStreamerFileTemp.closeTag();
		}
	}
	mStreamMutex.unlock();
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Terminate XML logger operations.
 */
void HPC::LoggerXMLD::terminate(void)
{
	Beagle_StackTraceBeginM();
	mStreamMutex.lock();
 	if(mNodeType == "Supervisor") LoggerXML::terminate();
	mStreamMutex.unlock();
	Beagle_StackTraceEndM();
}

/*!
 *  \brief
 *  \param
 */
void HPC::LoggerXMLD::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	LoggerXML::registerParams(ioSystem);
	{
		Register::Description lDescription(
		  "Log evolver nodes evolution information.",
		  "Bool",
		  "1",
		  "Indicates whether evolver nodes should be included in logs."
		);
		mLogEvolver =
		castHandleT<Bool>(ioSystem.getRegister().insertEntry("hpc.log.evolver", new Bool(false), lDescription));
	}
	{
		Register::Description lDescription(
		  "Log evaluator nodes evaluation information.",
		  "Bool",
		  "1",
		  "Indicates whether evaluator nodes should be included in logs."
		);
		mLogEvaluator =
		castHandleT<Bool>(ioSystem.getRegister().insertEntry("hpc.log.evaluator", new Bool(false), lDescription));
	}
	{
		Register::Description lDescription(
		  "Log nodes rank.",
		  "Bool",
		  "1",
		  "Indicates whether node rank should be included in messages."
		);
		mShowRank =
		castHandleT<Bool>(ioSystem.getRegister().insertEntry("hpc.log.rank", new Bool(true), lDescription));
	}
	{
		Register::Description lDescription(
		  "Log nodes type.",
		  "Bool",
		  "1",
		  "Indicates whether node type should be included in messages."
		);
		mShowNodeType =
		castHandleT<Bool>(ioSystem.getRegister().insertEntry("hpc.log.type", new Bool(true), lDescription));
	}
	{
		Register::Description lDescription(
		  "Maximum number of seconds between two log sent to the supervisor node.",
		  "Double",
		  "0.5",
		  "If the buffer isn't full, the logging thread sleeps for this timeout before sending the buffer."
		);
		mTimeout =
		castHandleT<Double>(ioSystem.getRegister().insertEntry("hpc.logger.timeout", new Double(0.5), lDescription));
	}
	{
		Register::Description lDescription(
		  "Minimum number of characters that must be present in the buffer for it to be sent to the supervisor.",
		  "UInt",
		  "1",
		  "If the buffer isn't full, the logging thread sleeps for the time set by the timeout before sending the buffer."
		);
		mBufferSize =
		castHandleT<UInt>(ioSystem.getRegister().insertEntry("hpc.logger.buffersize", new UInt(256), lDescription));
	}
	Beagle_StackTraceEndM();
}

/*!
 *  \brief
 *  \param
 *  \param
 */
HPC::LoggerXMLD::SupervisorLoggingThread::SupervisorLoggingThread(LoggerXMLD* inLogger,
								  MPICommunication::Handle inMPI) :
								  mLogger(inLogger), mMPI(inMPI)
{

}

/*!
 *  \brief
 */
HPC::LoggerXMLD::SupervisorLoggingThread::~SupervisorLoggingThread()
{
/*	if(mReceiptConsole != 0){
		mMPI->cancel(mReceiptConsole);
	}
*/	wait();
}
/*!
 *  \brief
 *  \return
 */
unsigned int HPC::LoggerXMLD::SupervisorLoggingThread::computeActiveLoggingNodes() const
{
	unsigned int lNbActive = 0;
	if(mLogger->mLogEvolver->getWrappedValue())
		lNbActive += mMPI->getNbrOfType("Evolver");
	if(mLogger->mLogEvaluator->getWrappedValue())
		lNbActive += mMPI->getNbrOfType("Evaluator");
	return lNbActive;
}
/*!
 *  \brief
 */
void HPC::LoggerXMLD::SupervisorLoggingThread::main(void)
{
	unsigned int lEndCount = 0;
	const unsigned int lEndCountTotal = computeActiveLoggingNodes();
	mRequestBag.push_back(new MPICommunication::Request);
	mRequestBag.push_back(new MPICommunication::Request);
	mRequestBag.push_back(new MPICommunication::Request);
	bool lConsoleNew = true;
	bool lFileNew = true;
	bool lEndNew = true;
	while(!mCancel){
		if(lConsoleNew){
			mMPI->receiveNonBlocking(mRequestBag[0], "LogConsole", "Any");
			lConsoleNew = false;
		}
		if(lFileNew){
			mMPI->receiveNonBlocking(mRequestBag[1], "LogFile", "Any");
			lFileNew = false;
		}
		if(lEndNew){
			mMPI->receiveNonBlocking(mRequestBag[2], "LogEnd", "Any");
			lEndNew = false;
		}
		switch(mMPI->waitAny(mRequestBag)){
			case 0:{
				std::istringstream lISS(mRequestBag[0]->getMessage()->getWrappedValue());
				PACC::XML::Document lParser(lISS, "inString");
				mLogger->mStreamMutex.lock();
				for(PACC::XML::ConstIterator lIter=lParser.getFirstRoot(); lIter; ++lIter) {
					(*lIter).serialize(mLogger->mStreamerConsole,true);
				}
				mLogger->mStreamMutex.unlock();
				mRequestBag[0] = new MPICommunication::Request;
				lConsoleNew = true;
				break;
			}
			case 1:{
				std::istringstream lISS(mRequestBag[1]->getMessage()->getWrappedValue());
				PACC::XML::Document lParser(lISS, "inString");
				mLogger->mStreamMutex.lock();
				for(PACC::XML::ConstIterator lIter=lParser.getFirstRoot(); lIter; ++lIter) {
					(*lIter).serialize(mLogger->mStreamerFile,true);
				}
				mLogger->mStreamMutex.unlock();
				mRequestBag[1] = new MPICommunication::Request;
				lFileNew = true;
				break;
			}
			case 2:
				++lEndCount;
				if(lEndCount == lEndCountTotal)
					cancel();
				mRequestBag[1] = new MPICommunication::Request;
				lEndNew = true;
				break;
			default:
				continue;
				break;
		}

	}
}
/*!
 *  \brief
 *  \param
 *  \param
 */
HPC::LoggerXMLD::ChildLoggingThread::ChildLoggingThread(LoggerXMLD* inLogger,
							MPICommunication::Handle inMPI) :
							mLogger(inLogger), mMPI(inMPI)
{

}
/*!
 *  \brief
 */
HPC::LoggerXMLD::ChildLoggingThread::~ChildLoggingThread()
{
	wait();
	mMPI->send("", "LogEnd", "Supervisor");
}

/*!
 *  \brief
 */
void HPC::LoggerXMLD::ChildLoggingThread::main(void)
{
	while(!mCancel){
		// Check if the buffer contains enough characters. If not sleep for mTimeout seconds.
		if(mLogger->mConsoleStreamTemp.str().size() < mLogger->mBufferSize->getWrappedValue() ||
		   mLogger->mFileStreamTemp.str().size() < mLogger->mBufferSize->getWrappedValue())
			usleep(mLogger->mTimeout->getWrappedValue()*10e6);
		mLogger->mStreamMutex.lock();
		// Retrieve the console stream content.
		const std::string lConsoleString = mLogger->mConsoleStreamTemp.str();
		// Reset the stream.
		mLogger->mConsoleStreamTemp.str("");
		// Retrieve the console stream content.
		const std::string lFileString = mLogger->mFileStreamTemp.str();
		// Reset the stream.
		mLogger->mFileStreamTemp.str("");
		mLogger->mStreamMutex.unlock();
		if(lConsoleString.size() > 0) {
			// Send the string to the supervisor.
			mMPI->send(lConsoleString, "LogConsole", "Supervisor");
		}
		if(lFileString.size() > 0) {
			mMPI->send(lFileString, "LogFile", "Supervisor");
		}
	}
}
