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
 *  \file   Beagle/EC/MilestoneWriteOp.cpp
 *  \brief  Source code of class MilestoneWriteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.23 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/EC.hpp"

#include <cstdio>
#include <fstream>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a milestone writer operator.
 */
MilestoneWriteOp::MilestoneWriteOp(std::string inName) :
	Beagle::Operator(inName),
	mMilestonePrefix(NULL),
	mWritingInterval(NULL),
	mPerDemeMilestone(NULL),
	mOverwriteMilestone(NULL),
	mPopSize(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void MilestoneWriteOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::Operator::registerParams(ioSystem);

	{
		std::ostringstream lOSS;
		lOSS << "Prefix used to name the evolution milestone files. ";
		lOSS << "An empty string means no milestone.";
		Register::Description lDescription(
		    "Milestone filename prefix",
		    "String",
		    "'beagle'",
		    lOSS.str()
		);
		mMilestonePrefix = castHandleT<String>(
		                       ioSystem.getRegister().insertEntry("ms.write.prefix", new String("beagle"), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Milestone saving interval (in number of generations). ";
		lOSS << "When zero, only the last generation milestone is saved.";
		Register::Description lDescription(
		    "Milestone saving interval",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mWritingInterval = castHandleT<UInt>(
		                       ioSystem.getRegister().insertEntry("ms.write.interval", new UInt(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "If true, this flag indicates that separate milestones ";
		lOSS << "should be written after each demes processing. Otherwise milestones are ";
		lOSS << "written after the processing of a complete populations.";
		Register::Description lDescription(
		    "Milestone per deme saving flag",
		    "Bool",
		    "0",
		    lOSS.str()
		);
		mPerDemeMilestone = castHandleT<Bool>(
		                        ioSystem.getRegister().insertEntry("ms.write.perdeme", new Bool(false), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "If true, this flag indicates that old milestones should ";
		lOSS << "be over-written. Otherwise, each milestone has a different suffix.";
		Register::Description lDescription(
		    "Milestone over-writing flag",
		    "Bool",
		    "1",
		    lOSS.str()
		);
		mOverwriteMilestone = castHandleT<Bool>(
		                          ioSystem.getRegister().insertEntry("ms.write.over", new Bool(true), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of demes and size of each deme of the population. ";
		lOSS << "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lOSS << "The size of the UIntArray is the number of demes present in the ";
		lOSS << "vivarium, while each value of the vector is the size of the corresponding ";
		lOSS << "deme.";
		Register::Description lDescription(
		    "Vivarium and demes sizes",
		    "UIntArray",
		    "100",
		    lOSS.str()
		);
		mPopSize = castHandleT<UIntArray>(
		               ioSystem.getRegister().insertEntry("ec.pop.size", new UIntArray(1,100), lDescription));
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
 *  \brief Apply the characteristic operation.
 *  \param ioDeme
 *  \param ioContext Context of the evolution.
 */
void MilestoneWriteOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMilestonePrefix->getWrappedValue().empty()) return;
	if(ioContext.getContinueFlag() == true) {
		if(mWritingInterval->getWrappedValue() == 0) return;
		if((ioContext.getGeneration() % mWritingInterval->getWrappedValue()) != 0) return;
		if((mPerDemeMilestone->getWrappedValue() == false) &&
		        (ioContext.getDemeIndex() != (mPopSize->size()-1))) return;
	}
	std::string lFilename = mMilestonePrefix->getWrappedValue();
	if(mPerDemeMilestone->getWrappedValue()) {
		lFilename += "_d";
		lFilename += uint2str(ioContext.getDemeIndex()+1);
	}
	if(mOverwriteMilestone->getWrappedValue() == false) {
		lFilename += "_g";
		lFilename += uint2str(ioContext.getGeneration());
	}
	lFilename += ".obm";
#ifdef BEAGLE_HAVE_LIBZ
	if(mCompressMilestone->getWrappedValue()) lFilename += ".gz";
#endif // BEAGLE_HAVE_LIBZ
	Beagle_LogBasicM(
	    ioContext.getSystem().getLogger(),
	    std::string("Writing milestone file '")+lFilename+"'"
	);
	writeMilestone(lFilename, ioContext);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an evolution milestone.
 *  \param inFilename Filename of the milestone.
 *  \param ioContext  Context of the evolution,
 */
void MilestoneWriteOp::writeMilestone(std::string inFilename, Context& ioContext)
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
	try {
		PACC::XML::Streamer lStreamer(*lOS);
		lStreamer.insertHeader();
		lStreamer.openTag("Beagle");
		lStreamer.insertAttribute("version", BEAGLE_VERSION);
		lStreamer.insertAttribute("generation", uint2str(ioContext.getGeneration()));
		lStreamer.insertAttribute("deme", uint2str(ioContext.getDemeIndex()));
		lStreamer.insertComment("Created by MilestoneWriteOp operator");
		lStreamer.insertComment("System: configuration of the system");
		ioContext.getSystem().write(lStreamer);
		lStreamer.insertComment("Evolver: configuration of the algorithm");
		ioContext.getEvolver().write(lStreamer);
		lStreamer.insertComment("Vivarium: evolved population");
		ioContext.getVivarium().write(lStreamer);
		lStreamer.closeTag();
		*lOS << std::endl;
	} catch (...) {
		delete lOS;
		throw;
	}
	delete lOS;
	lOS = NULL;
	Beagle_StackTraceEndM();
}



