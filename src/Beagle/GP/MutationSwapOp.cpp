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
 *  \file   beagle/GP/src/MutationSwapOp.cpp
 *  \brief  Source code of class GP::MutationSwapOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.27 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP swap mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inDistribPbName Swap mutation distribution probability parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationSwapOp::MutationSwapOp(std::string inMutationPbName,
                                   std::string inDistribPbName,
                                   std::string inName) :
		Beagle::MutationOp(inMutationPbName, inName),
		mDistribPbName(inDistribPbName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationSwapOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	{
		std::ostringstream lOSS;
		lOSS << "Swap mutation probability for an individual. ";
		lOSS << "Swap mutation consists in exchanging the primitive associated to a ";
		lOSS << "node by one having the same number of arguments.";
		Register::Description lDescription(
		    "Individual swap mutation prob.",
		    "Double",
		    "0.05",
		    lOSS.str()
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.05f), lDescription));
	}
	Beagle::MutationOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Probability that a swap mutation point is a branch (node with sub-trees). ";
		lOSS << "Value of 1.0 means that all swap mutation points are branches, ";
		lOSS << "and value of 0.0 means that all swap mutation points are leaves. ";
		lOSS << "Swap mutation consists in exchanging the primitive associated to a ";
		lOSS << "node by one having the same number of arguments.";
		Register::Description lDescription(
		    "Swap mutation distrib. prob.",
		    "Float",
		    "0.5",
		    lOSS.str()
		);
		mDistributionProba = castHandleT<Float>(
		                         ioSystem.getRegister().insertEntry(mDistribPbName, new Float(0.5f), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Swap mutate a GP individual.
 *  \param ioIndividual GP individual to swap mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationSwapOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);
	double lDistrProba           = mDistributionProba->getWrappedValue();

	// Store original values.
	GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = lContext.getGenotypeIndex();

	// Select the tree to mutate
	unsigned int lNbNodes = 0;
	for(unsigned int i=0; i<lIndividual.size(); i++) lNbNodes += lIndividual[i]->size();
	if(lNbNodes == 0) return false;
	unsigned int lChosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lNbNodes-1);
	unsigned int lChosenTree = 0;
	for(; (lChosenTree+1)<lIndividual.size(); lChosenTree++) {
		if(lChosenNode < lIndividual[lChosenTree]->size()) break;
		else lChosenNode -= lIndividual[lChosenTree]->size();
	}
	GP::Tree& lTree = *lIndividual[lChosenTree];
	if(lTree.size() == 0) return false;

	// Store the new values
	lContext.setGenotypeHandle(lIndividual[lChosenTree]);
	lContext.setGenotypeIndex(lChosenTree);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapOp",
	    "Individual before GP swap mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapOp",
	    ioIndividual
	);

	// Choose the node to mutate.
	if(lTree.size() > 1) {
		bool lTypeNode = (lContext.getSystem().getRandomizer().rollUniform(0., 1.) < lDistrProba);
		while((lTree[lChosenNode].mPrimitive->getNumberArguments() != 0) != lTypeNode) {
			lChosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lTree.size()-1);
		}
	}
	Primitive::Handle lOriginalPrimitive = lTree[lChosenNode].mPrimitive;

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationSwapOp",
	    std::string("Swap mutating the ")+uint2ordinal(lChosenNode+1)+
	    std::string(" node (primitive: '")+lOriginalPrimitive->getName()+
	    std::string("' nb args: ")+uint2str(lOriginalPrimitive->getNumberArguments())+
	    std::string(") of the ")+uint2ordinal(lChosenTree+1)+std::string(" tree")
	);

	GP::PrimitiveSet& lPrimitiveSet = lTree.getPrimitiveSet(lContext);
	unsigned int lNbArgsPrimit = lTree[lChosenNode].mPrimitive->getNumberArguments();

	// Select primitive to replace choosen one.
	Primitive::Handle lChosenPrimitive = lPrimitiveSet.select(lNbArgsPrimit, lContext);
	if (lChosenPrimitive==NULL) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationSwapOp",
		    std::string("Swap mutation failed because no primitive could be found that had ")+
		    uint2str(lNbArgsPrimit)+std::string(" arguments")
		);
		return false;
	} else {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationSwapOp",
		    std::string("Swap mutation chose primitive '")+lChosenPrimitive->getName()+std::string("'")
		);
	}

	// Replace choose primitive.
	lTree[lChosenNode].mPrimitive = lChosenPrimitive->giveReference(lNbArgsPrimit, lContext);

	// Restore the original values.
	lContext.setGenotypeHandle(lOldTreeHandle);
	lContext.setGenotypeIndex(lOldTreeIndex);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationSwapOp",
	    "Individual after GP swap mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapOp",
	    ioIndividual
	);

	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param ioSystem Evolutionary system.
 */
void GP::MutationSwapOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string mMutationPbReadName = inIter->getAttribute("mutationpb");
	if(mMutationPbReadName.empty() == false) mMutationPbName = mMutationPbReadName;
	std::string mDistribPbReadName = inIter->getAttribute("distrpb");
	if(mDistribPbReadName.empty() == false) mDistribPbName = mDistribPbReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::MutationSwapOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	Beagle::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("distrpb", mDistribPbName);
	Beagle_StackTraceEndM();
}

