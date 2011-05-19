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
 *  \file   Beagle/EC/TermMinFitnessOp.cpp
 *  \brief  Source code of class TermMinFitnessOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/08 19:26:47 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator based on a minimum fitness value.
 *  \param inMinFitness Default minimum fitness value.
 *  \param inName Name of the operator.
 */
TermMinFitnessOp::TermMinFitnessOp(float inMinFitness, std::string inName) :
	Beagle::EC::TerminationOp(inName),
	mMinFitnessDefault(inMinFitness)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMinFitnessOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EC::TerminationOp::registerParams(ioSystem);

	Register::Description lDescription(
	    "Min fitness term criterion",
	    "Float",
	    dbl2str(mMinFitnessDefault),
	    "Minimal fitness value to reach before stopping evolution."
	);
	mMinFitness = castHandleT<Float>(
	                  ioSystem.getRegister().insertEntry("ec.term.minfitness", new Float(mMinFitnessDefault), lDescription));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a min fitness termination operator from an XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void TermMinFitnessOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lFitnessStr = inIter->getAttribute("fitness");
	if(lFitnessStr.empty() == false) {
		mMinFitnessDefault = str2dbl(lFitnessStr);
		if(mMinFitness != NULL) mMinFitness->getWrappedValue() = mMinFitnessDefault;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if the minimum fitness value is reached.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMinFitnessOp::terminate(const Deme& inDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	for(unsigned int i=0; i<inDeme.size(); ++i) {
		FitnessSimple::Handle lFitness =
		    castHandleT<FitnessSimple>(inDeme[i]->getFitness());
		if(lFitness->isValid() && lFitness->getValue() <= mMinFitness->getWrappedValue()) {
			Beagle_LogInfoM(
			    ioContext.getSystem().getLogger(),
			    std::string("Minimum fitness value (") +
			    dbl2str(mMinFitness->getWrappedValue()) +
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
	    std::string("Minimum fitness value (") +
	    dbl2str(mMinFitness->getWrappedValue()) +
	    std::string(") termination criterion not reached ")
	);
	return false;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a min fitness termination operator into an output stream.
 *  \param ioStreamer Output stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void TermMinFitnessOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	if(mMinFitness != NULL)
		ioStreamer.insertAttribute("fitness", dbl2str(mMinFitness->getWrappedValue()));
	else ioStreamer.insertAttribute("fitness", dbl2str(mMinFitnessDefault));
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
