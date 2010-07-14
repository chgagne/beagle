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
 *  \file   Beagle/EC/TermMaxFitnessOp.cpp
 *  \brief  Source code of class TermMaxFitnessOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator based on a maximum fitness value.
 *  \param inMaxFitness Default maximum fitness value.
 *  \param inName Name of the operator.
 */
TermMaxFitnessOp::TermMaxFitnessOp(float inMaxFitness, std::string inName) :
		Beagle::EC::TerminationOp(inName),
		mMaxFitnessDefault(inMaxFitness)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxFitnessOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EC::TerminationOp::registerParams(ioSystem);
	
	Register::Description lDescription(
	    "Max fitness term criterion",
	    "Float",
	    dbl2str(mMaxFitnessDefault),
	    "Fitness value to reach before stopping evolution."
	);
	mMaxFitness = castHandleT<Float>(
	                  ioSystem.getRegister().insertEntry("ec.term.maxfitness", new Float(mMaxFitnessDefault), lDescription));

	Beagle_StackTraceEndM("void TermMaxFitnessOp::registerParams(System&)");
}


/*!
 *  \brief Read a max fitness termination operator from a XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void TermMaxFitnessOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lFitnessStr = inIter->getAttribute("fitness");
	if(lFitnessStr.empty() == false) {
		mMaxFitnessDefault = str2dbl(lFitnessStr);
		if(mMaxFitness != NULL) mMaxFitness->getWrappedValue() = mMaxFitnessDefault;
	}
	Beagle_StackTraceEndM("void TermMaxFitnessOp::readWithSystem(PACC::XML::ConstIterator, System&)");
}


/*!
 *  \brief Check if the maximum fitness value is reached.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxFitnessOp::terminate(const Deme& inDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	for(unsigned int i=0; i<inDeme.size(); ++i) {
		FitnessSimple::Handle lFitness =
		    castHandleT<FitnessSimple>(inDeme[i]->getFitness());
		if(lFitness->isValid() && lFitness->getValue() >= mMaxFitness->getWrappedValue()) {
			Beagle_LogInfoM(
			    ioContext.getSystem().getLogger(),
			    "termination", "Beagle::TermMaxFitnessOp",
			    std::string("Maximum fitness value (") +
			    dbl2str(mMaxFitness->getWrappedValue()) +
			    std::string(") termination criterion reached by the ") +
			    uint2ordinal(i+1) + std::string(" individual (") +
			    dbl2str(lFitness->getValue()) + std::string(")")
			);
			ioContext.setTerminationSuccessful();
			return true;
		}
	}

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "termination", "Beagle::TermMaxFitnessOp",
	    std::string("Maximum fitness value (") +
	    dbl2str(mMaxFitness->getWrappedValue()) +
	    std::string(") termination criterion not reached ")
	);
	return false;
	Beagle_StackTraceEndM("bool TermMaxFitnessOp::terminate(const Deme& inDeme, Context& ioContext)");
}


/*!
 *  \brief Write a max fitness termination operator into a XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void TermMaxFitnessOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	if(mMaxFitness != NULL)
		ioStreamer.insertAttribute("fitness", dbl2str(mMaxFitness->getWrappedValue()));
	else ioStreamer.insertAttribute("fitness", dbl2str(mMaxFitnessDefault));
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void TermMaxFitnessOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
