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
 *  \file   Beagle/IntVec/MutationShuffleOp.cpp
 *  \brief  Source code of class IntVec::MutationShuffleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct an indices permutation shuffle mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inIntMutatePbName Mutation integer probability parameter name used in register.
 *  \param inName Name of the operator.
 */
IntVec::MutationShuffleOp::MutationShuffleOp(std::string inMutationPbName,
        std::string inIntMutatePbName,
        std::string inName) :
	EC::MutationOp(inMutationPbName, inName),
	mIntMutatePbName(inIntMutatePbName)
{ }


/*!
 *  \brief Shuffle mutate an integer vector IntVec individual.
 *  \param ioIndividual IntVec individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool IntVec::MutationShuffleOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mIntMutateProba->getWrappedValue()>=0.0, mIntMutatePbName, "<0");

	bool lMutated = false;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "Integer shuffle mutation probability is: " << dbl2str(mIntMutateProba->getWrappedValue())
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		IntVec::IntegerVector::Handle lIV = castHandleT<IntVec::IntegerVector>(ioIndividual[i]);
		if(lIV->size() <= 2) continue;
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Shuffle mutating the " << uint2ordinal(i+1) << " integer vector"
		);
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "The integer vector before shuffle mutation: " << *lIV
		);
		for(unsigned int j=0; j<lIV->size(); j++) {
			double lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
			if(lRolledPb <= mIntMutateProba->getWrappedValue()) {
				unsigned int lSwapIndex =
				    ioContext.getSystem().getRandomizer().rollInteger(0, lIV->size()-2);
				if(lSwapIndex >= j) ++lSwapIndex;
				const int lTmpVal = (*lIV)[lSwapIndex];
				(*lIV)[lSwapIndex] = (*lIV)[j];
				(*lIV)[j] = lTmpVal;
				lMutated = true;
			}
		}
		if(lMutated) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The integer vector has been shuffle mutated"
			);
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "The integer vector after shuffle mutation: " << *lIV
			);
		} else {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The integer vector has not been mutated"
			);
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a shuffle mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param inOpMap
 */
void IntVec::MutationShuffleOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lMutationPbReadName = inIter->getAttribute("mutationpb");
	if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
	std::string lIntMutatePbReadName = inIter->getAttribute("mutintpb");
	if(lIntMutatePbReadName.empty() == false) mIntMutatePbName = lIntMutatePbReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the integer vector IntVec shuffle mutation operator.
 *  \param ioSystem System of the evolution.
 */
void IntVec::MutationShuffleOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual shuffle mutation prob.",
		    "Double",
		    "0.1",
		    "Integer vector shuffle mutation probability for each individual."
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.1f), lDescription));
	}
	EC::MutationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Int shuffle mutation probability",
		    "Double",
		    "0.1",
		    "Probability for each integer to be modified by mutation, when an individual is mutated."
		);
		mIntMutateProba = castHandleT<Double>(
		                      ioSystem.getRegister().insertEntry(mIntMutatePbName, new Double(0.1), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write shuffle mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void IntVec::MutationShuffleOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	EC::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("mutintpb", mIntMutatePbName);
	Beagle_StackTraceEndM();
}
