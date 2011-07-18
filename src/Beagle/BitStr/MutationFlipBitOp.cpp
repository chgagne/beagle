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
 *  \file   Beagle/BitStr/MutationFlipBitOp.cpp
 *  \brief  Source code of class BitStr::MutationFlipBitOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.24 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/BitStr.hpp"

#include <algorithm>
#include <string>


using namespace Beagle;


/*!
 *  \brief Construct a bit string flip bit mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inBitMutatePbName Mutation bit probability parameter name used in register.
 *  \param inName Name of the operator.
 */
BitStr::MutationFlipBitOp::MutationFlipBitOp(std::string inMutationPbName,
        std::string inBitMutatePbName,
        std::string inName) :
		EC::MutationOp(inMutationPbName, inName),
		mBitMutatePbName(inBitMutatePbName)
{ }


/*!
 *  \brief Register the parameters of the bit string BitStr flip bit mutation operator.
 *  \param ioSystem System of the evolution.
 */
void BitStr::MutationFlipBitOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual flip mutation prob.",
		    "Double",
		    "1.0",
		    "Bit flip mutation probability for each BitStr individual."
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(1.0f), lDescription));
	}
	EC::MutationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Flip mutation probability",
		    "Float",
		    "0.01",
		    "Probability for each bit to be inverted by mutation, when an individual is mutated."
		);
		mBitMutateProba = castHandleT<Float>(
		                      ioSystem.getRegister().insertEntry(mBitMutatePbName, new Float(0.01f), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Bit flip mutate a bit string individual.
 *  \param ioIndividual Individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool BitStr::MutationFlipBitOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mBitMutateProba->getWrappedValue()>=0.0, mBitMutatePbName, "<0");
	bool lMutated = false;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Bit flip mutation probability is: ")+dbl2str(mBitMutateProba->getWrappedValue())
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		BitStr::BitString::Handle lBS = castHandleT<BitStr::BitString>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    std::string("Flip mutating the ")+uint2ordinal(i+1)+" bitstring"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lBS);
		for(unsigned int j=0; j<lBS->size(); j++) {
			double lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
			if(lRolledPb <= mBitMutateProba->getWrappedValue()) {
				(*lBS)[j] = !(*lBS)[j];
				lMutated = true;
			}
		}
		if(lMutated) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    std::string("The bitstring has been flip mutated")
			);
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lBS);
		} else {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    std::string("The bitstring has not been mutated")
			);
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a bit flip mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param ioSystem Evolutionary system.
 */
void BitStr::MutationFlipBitOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lMutationPbReadName = inIter->getAttribute("mutationpb");
	if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
	std::string lBitMutatePbReadName = inIter->getAttribute("mutbitpb");
	if(lBitMutatePbReadName.empty() == false) mBitMutatePbName = lBitMutatePbReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write flip bit mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void BitStr::MutationFlipBitOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	EC::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("mutbitpb", mBitMutatePbName);
	Beagle_StackTraceEndM();
}
