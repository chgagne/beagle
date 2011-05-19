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
 *  \file   beagle/Distrib/Master/src/MilestoneWriteOp.cpp
 *  \brief  Source code of class MilestoneWriteOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.7 $
 *  $Date: 2007/09/24 20:33:01 $
 */

#include "beagle/Distrib/Master.hpp"

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct a milestone writer operator.
 *  \param inName Name of the MilestoneWrite operator.
 *  \param inDefaultAppName default application's name.  Usually dbeagle-app.
 */
Master::MilestoneWriteOp::MilestoneWriteOp(std::string inName, std::string inDefaultAppName) :
		Beagle::MilestoneWriteOp(inName),
		mDefaultAppName(inDefaultAppName)
{ }


/*!
 *  \brief Register parameters of the MilestoneWrite operator.
 *  \param ioSystem Reference to the system.
 */
void Master::MilestoneWriteOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::MilestoneWriteOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Application name",
		    "String",
		    std::string("'")+mDefaultAppName+std::string("'"),
		    "Name of the application, as registered in the server."
		);
		mAppName = castHandleT<Beagle::String>(
		               ioSystem.getRegister().insertEntry("db.app.name", new Beagle::String(mDefaultAppName), lDescription));
	}
	{
		Register::Description lDescription(
		    "IP server address",
		    "String",
		    "127.0.0.1",
		    "IP address of the master-slave server."
		);
		mServerIP = castHandleT<Beagle::String>(
		                ioSystem.getRegister().insertEntry("db.server.ip", new Beagle::String("127.0.0.1"), lDescription));
	}
	{
		Register::Description lDescription(
		    "IP server port",
		    "UInt",
		    "9123",
		    "IP port of the master-slave server."
		);
		mServerPort = castHandleT<UInt>(
		                  ioSystem.getRegister().insertEntry("db.server.port", new UInt(9123), lDescription));
	}
	{
		Register::Description lDescription(
		    "Compression level",
		    "UInt",
		    "3",
		    "Evolver communication compression."
		);
		mCompressionLevel = castHandleT<UInt>(
		                        ioSystem.getRegister().insertEntry("db.evolver.compression", new UInt(0), lDescription));
	}
#ifdef BEAGLE_HAVE_LIBZ
	{
		std::ostringstream lOSS;
		lOSS << "If true, this flag indicates that milestones will be ";
		lOSS << "compressed with gzip. Otherwise, each milestone are kept plain text.";
		Register::Description lDescription(
		    "Milestone compression flag",
		    "Bool",
		    "1",
		    lOSS.str()
		);
		mCompressMilestone = castHandleT<Bool>(
		                         ioSystem.getRegister().insertEntry("ms.write.compress", new Bool(true), lDescription));
	}
#endif // BEAGLE_HAVE_LIBZ

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an evolution milestone.
 *  \param inFilename Filename of the milestone.
 *  \param ioContext  Context of the evolution,
 */
void Master::MilestoneWriteOp::writeMilestone(std::string inFilename, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	std::string lFilenameBak = inFilename + "~";
	std::remove(lFilenameBak.c_str());
	std::rename(inFilename.c_str(), lFilenameBak.c_str());
#ifdef BEAGLE_HAVE_LIBZ
	std::ostream* lOS = NULL;
	if(mCompressMilestone->getWrappedValue()) lOS = new ogzstream(inFilename.c_str());
	else lOS = new std::ofstream(inFilename.c_str());
#else
	std::ostream* lOS = new std::ofstream(inFilename.c_str());
#endif
	PACC::XML::Streamer lStreamer(*lOS);

	lStreamer.insertHeader();
	lStreamer.openTag("Beagle");
	lStreamer.insertAttribute("version", BEAGLE_VERSION);
	lStreamer.openTag("Milestone");
	lStreamer.insertAttribute("Generation", ioContext.getGeneration());
	lStreamer.insertAttribute("Deme", ioContext.getDemeIndex());
	lStreamer.closeTag();
	ioContext.getEvolver().write(lStreamer);
	ioContext.getSystem().getRegister().write(lStreamer);

	lStreamer.openTag("Vivarium");
	ioContext.getVivarium().writeMembers(lStreamer);

	EvolClient& lClient = *EvolClient::getSingleton();
	unsigned int lNbDemes = lClient.getNumberOfDemes();
	lStreamer.openTag("Population");
	lStreamer.insertAttribute("size", uint2str(lNbDemes));

	const Factory& lFactory = ioContext.getSystem().getFactory();
	Deme::Alloc::Handle lDemeAlloc =
		castHandleT<Deme::Alloc>(lFactory.getConceptAllocator("Deme"));
	
	if(lNbDemes > 1) {
		Deme::Handle lTmpDeme = castHandleT<Deme>(lDemeAlloc->allocate());
		EvolClient* lNewClient = new EvolClient(mServerIP->getWrappedValue(),
		                                        mServerPort->getWrappedValue(),
		                                        mCompressionLevel->getWrappedValue());
		lNewClient->setEnvManager(lClient.getEnvManager());
		lNewClient->setDeme(lTmpDeme);
		lNewClient->setContext(&ioContext);
		for(unsigned int i=0; i<lNbDemes; ++i) {
			if(i == lClient.getDemeID()) ioContext.getDeme().write(lStreamer);
			else {
				EvolClient::setSingleton(lNewClient);
				lNewClient->consultDeme(i);
				EvolClient::setSingleton(&lClient);
				lTmpDeme->write(lStreamer);
				lTmpDeme->clear();
			}
		}
		delete lNewClient;
		lNewClient = NULL;
	} else ioContext.getDeme().write(lStreamer);
	lStreamer.closeTag();

	lStreamer.closeTag();
	lStreamer.closeTag();

	*lOS << std::flush;
	delete lOS;
	lOS = NULL;

	Beagle_StackTraceEndM();
}



