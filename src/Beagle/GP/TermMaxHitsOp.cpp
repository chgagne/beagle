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
 *  \file   beagle/GP/src/TermMaxHitsOp.cpp
 *  \brief  Implementation of the class TermMaxHitsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/08 19:26:41 $
 */

#include "beagle/GP.hpp"

#include <cmath>

using namespace Beagle;


/*!
 *  \brief Construct a new TermMaxHitsOp termination operator.
 *  \param inMaxHits Number of hits required to stop the evolution.
 *  \param inName Name of the evaluation operator.
 */
GP::TermMaxHitsOp::TermMaxHitsOp(unsigned int inMaxHits, std::string inName) :
		TerminationOp(inName),
		mMaxHits(NULL),
		mMaxHitsDefault(inMaxHits)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System used to initialize the operator.
 */
void GP::TermMaxHitsOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	TerminationOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "Number of hits required in an individual ";
	lOSS << "in order for the evolution process to terminate. ";
	lOSS << "If 0, termination criteria is ignored.";
	Register::Description lDescription(
	    "Max hits term criterion",
	    "UInt",
	    uint2str(mMaxHitsDefault),
	    lOSS.str()
	);
	mMaxHits = castHandleT<UInt>(
	               ioSystem.getRegister().insertEntry("gp.term.maxhits", new UInt(mMaxHitsDefault), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a max hits termination operator from a XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void GP::TermMaxHitsOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lHitsStr = inIter->getAttribute("hits");
	if(lHitsStr.empty() == false) {
		mMaxHitsDefault = str2uint(lHitsStr);
		if(mMaxHits != NULL) mMaxHits->getWrappedValue() = mMaxHitsDefault;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if the evolution must terminate.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool GP::TermMaxHitsOp::terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMaxHits->getWrappedValue() == 0) return false;
	for(unsigned int i=0; i<inDeme.size(); i++) {
		const GP::FitnessKoza::Handle lFitness =
		    castHandleT<const GP::FitnessKoza>(inDeme[i]->getFitness());
		if(mMaxHits->getWrappedValue() <= lFitness->getHits()) {
			Beagle_LogInfoM(
			    ioContext.getSystem().getLogger(),
			    "termination", "Beagle::GP::TermMaxHitsOp",
			    std::string("Maximum number of hits (") +
			    uint2str(mMaxHits->getWrappedValue()) +
			    std::string(") termination criterion reached")
			);
			Beagle_LogInfoM(
			    ioContext.getSystem().getLogger(),
			    "termination", "Beagle::GP::TermMaxHitsOp",
			    std::string("The ")+uint2ordinal(i+1) +
			    std::string(" individual of the deme has ") +
			    uint2str(lFitness->getHits())+std::string(" hits")
			);
			ioContext.setTerminationSuccessful();
			return true;
		}
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "termination", "Beagle::GP::TermMaxHitsOp",
	    std::string("Maximum number of hits (") +
	    uint2str(mMaxHits->getWrappedValue()) +
	    std::string(") termination criterion not reached")
	);
	return false;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an max hits termination operator into a XML stream.
 *  \param ioStreamer XML streamer in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::TermMaxHitsOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	if(mMaxHits != NULL) ioStreamer.insertAttribute("hits", uint2str(mMaxHits->getWrappedValue()));
	else ioStreamer.insertAttribute("hits", uint2str(mMaxHitsDefault));
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


