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
 *  \file   beagle/HPC/LoggerXMLD.hpp
 *  \brief  Definition of the class LoggerXMLD.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_HPC_LoggerXMLD_hpp
#define Beagle_HPC_LoggerXMLD_hpp

#include "beagle/HPC.hpp"
#include "beagle/LoggerXML.hpp"
#include "PACC/Threading.hpp"

namespace Beagle
{
namespace HPC
{
/*!
 *  \class LoggerXMLD beagle/HPC/LoggerXMLD.hpp "beagle/HPC/LoggerXMLD.hpp"
 *  \brief XML message logger.
 *  \ingroup ECF
 *  \ingroup HPC
 *  This class has two functions. The first function is derived from LoggerXML
 *  and is to log the evolutionnary process. The second function is to collect
 *  log from every nodes the Supervisor, so only one node has access to the
 *  log file and the console.
 *
 *  The second function is accomplished by two threads :
 *  \c SupervisorLoggingThread and \c ChildLoggingThread. The Supervisor runs
 *  the first thread, and the other nodes run the second.
 *
 *  \c ChildLoggingThread' function is to send to the Supervisor the content of
 *  the stream associated to the console and the log file. Instead of logging
 *  directly to the console and the log file, the nodes beneath the supervisor
 *  log into streams. The streams are sent after a certains timeout or before
 *  if they met a minimum number of characters. The timeout and the size are
 *  set by the user.
 *
 *  \c SupervisorLoggingThread' function is to collect streams that are sent
 *  by the child, and output them at the console or in the log file.
 */
class LoggerXMLD : public LoggerXML
{

public:

	//! LoggerXMLD allocator type.
	typedef AllocatorT<LoggerXMLD,Logger::Alloc> Alloc;
	//! LoggerXMLD handle type.
	typedef PointerT<LoggerXMLD,Logger::Handle> Handle;
	//! LoggerXMLD bag type.
	typedef ContainerT<LoggerXMLD,Logger::Bag> Bag;

	LoggerXMLD(void);
	virtual ~LoggerXMLD(void);

	virtual void outputMessage(const std::string& inMessage,
				   unsigned int inLevel,
				   const std::string& inType,
				   const std::string& inClass);

	virtual void outputObject(const Object& inObject,
				  unsigned int inLevel,
				  const std::string& inType,
				  const std::string& inClass);

	virtual void init(System& ioSystem);
	virtual void terminate(void);

	virtual void registerParams(System& ioSystem);

protected:
	//! Streamer used to output messages that will be sent to supervisor node for std output.
	PACC::XML::Streamer mStreamerConsoleTemp;
	//! Streamer used to output messages that will be sent to supervisor node for file log.
	PACC::XML::Streamer mStreamerFileTemp;

	//! Stream to save temporary console log to send to supervisor node.
	std::ostringstream mConsoleStreamTemp;
	//! Stream to save temporary file log to send to supervisor node.
	std::ostringstream mFileStreamTemp;

	//! Flag indicating if Evolver nodes are logged.
	Bool::Handle mLogEvolver;
	//! Flag indicating if Evaluator nodes are logged.
	Bool::Handle mLogEvaluator;
	//! Flag indicating if node type is added to log header.
	Bool::Handle mShowNodeType;
	//! Flag indicating if node rank is added to log header.
	Bool::Handle mShowRank;

	//! Type of the node.
	std::string mNodeType;
	//! Rank of the node.
	unsigned int mNodeRank;

	//! Minimum size a log has to be to be sent.
	UInt::Handle mBufferSize;
	//! Time wait before a log not meeting the size requirement is sent.
	Double::Handle mTimeout;

	//! Lock assuring that only one thread access to the logging streams.
	PACC::Threading::Mutex mStreamMutex;
	//! Thread that runs the log exchange routine.
	PACC::Threading::Thread* mThread;

	//! Handle to the MPICommunication component.
	MPICommunication::Handle mMPI;

	/*!
	 *  \class SupervisorLoggingThread beagle/HPC/LoggerXMLD.hpp "beagle/HPC/LoggerXMLD.hpp"
	 *  \brief Thread use to collect log from child nodes.
	 *  \ingroup ECF
	 *  \ingroup HPC
	 */
	class SupervisorLoggingThread : public PACC::Threading::Thread{
		public:
			SupervisorLoggingThread(LoggerXMLD* inLogger, MPICommunication::Handle inMPI);
			~SupervisorLoggingThread();
		protected:
			unsigned int computeActiveLoggingNodes() const;
			//! Bag contaning the 3 reception requests of the supervisor.
			MPICommunication::Request::Bag mRequestBag;
			//! Pointer to the LoggerXMLD object.
			LoggerXMLD* mLogger;
			//! Handle to the MPICommunication Component
			MPICommunication::Handle mMPI;
			void main(void);
	};

	/*!
	 *  \class ChildLoggingThread beagle/HPC/LoggerXMLD.hpp "beagle/HPC/LoggerXMLD.hpp"
	 *  \brief Thread use to send logs to the Supervisor.
	 *  \ingroup ECF
	 *  \ingroup HPC
	 */
	class ChildLoggingThread : public PACC::Threading::Thread{
		public:
			ChildLoggingThread(LoggerXMLD* inLogger, MPICommunication::Handle inMPI);
			~ChildLoggingThread();
		protected:
			//! Handle to the MPICommunication Component
			MPICommunication::Handle mMPI;
			//! Pointer to the LoggerXMLD object.
			LoggerXMLD* mLogger;
			void main(void);
	};

	friend class SupervisorLoggingThread;
	friend class ChildLoggingThread;

	friend class AllocatorT<LoggerXMLD,Logger::Alloc>;  // Necessary to instanticate allocators
	LoggerXMLD(const LoggerXMLD&);                       // Disable copy constructor
	void operator=(const LoggerXMLD&);                  // Disable copy operator
};

}
}

#endif // Beagle_HPC_LoggerXMLD_hpp

