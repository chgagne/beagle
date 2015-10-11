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
 *  \file   beagle/GP/src/MutationStandardOp.cpp
 *  \brief  Source code of class GP::MutationStandardOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP standard mutation operator.
 *  \param inInitOp Initialization operator used to mutate the GP subtree.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inMaxRegenDepthName Maximum regeneration depth in mutation parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationStandardOp::MutationStandardOp(GP::InitializationOp::Handle inInitOp,
        std::string inMutationPbName,
        std::string inMaxRegenDepthName,
        std::string inName) :
		Beagle::MutationOp(inMutationPbName, inName),
		mInitOp(inInitOp),
		mMaxRegenDepthName(inMaxRegenDepthName)
{ }


/*!
 *  \brief Register the parameters of the GP standard mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationStandardOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	mInitOp->registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Standard mutation probability for an individual. ";
		lOSS << "A standard mutation replaces a sub-tree with a randomly generated one.";
		Register::Description lProbaDescription(
		    "Individual std mutation prob.",
		    "Double",
		    "0.05",
		    lOSS.str()
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.05f), lProbaDescription));
	}
	Beagle::MutationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Maximum tree depth",
		    "UInt",
		    "17",
		    "Maximum allowed depth for the trees."
		);
		mMaxTreeDepth = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry("gp.tree.maxdepth", new UInt(17), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Maximum depth for standard mutation. ";
		lOSS << "A standard mutation replaces a sub-tree with a randomly generated one.";
		Register::Description lDescription(
		    "Maximum regeneration depth",
		    "UInt",
		    "5",
		    lOSS.str()
		);
		mMaxRegenerationDepth = castHandleT<UInt>(
		                            ioSystem.getRegister().insertEntry(mMaxRegenDepthName, new UInt(5), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Standard mutate a GP individual.
 *  \param ioIndividual GP individual to standard mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationStandardOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationStandardOp",
	    std::string("Mutating individual with GP::MutationStandardOp")
	);

	GP::Individual& lIndividual        = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext              = castObjectT<GP::Context&>(ioContext);
	unsigned int lMaxTreeDepth         = mMaxTreeDepth->getWrappedValue();
	unsigned int lMaxRegenerationDepth = mMaxRegenerationDepth->getWrappedValue();

	// Store original context values
	unsigned int lOldGenotypeIndex = lContext.getGenotypeIndex();
	GP::Tree::Handle lOldGenotypeHandle = lContext.getGenotypeHandle();

	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    ioIndividual
	);

	// Choose tree and node to mutate
	unsigned int lChosenTree = lIndividual.chooseRandomTree(lContext);
	unsigned int lChosenNode = lIndividual.chooseRandomNode(lChosenTree, lContext);
	GP::Tree::Handle lOriginalTree = lIndividual[lChosenTree];
	unsigned int lChosenNodeSubTreeSize = (*lOriginalTree)[lChosenNode].mSubTreeSize;

	// Choose tree depth for mutation
	lContext.setGenotypeIndex(lChosenTree);
	lContext.setGenotypeHandle(lOriginalTree);
	lContext.emptyCallStack();

	lOriginalTree->setContextToNode(lChosenNode, lContext);
	lContext.popCallStack();
	unsigned int lChosenNodeDepth = lContext.getCallStackSize();

	// Check that chosen node is not deeper than the maximum depth of a tree
	if(lChosenNodeDepth > lMaxTreeDepth) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationStandardOp",
		    std::string("Chosen node's depth (")+uint2str(lChosenNodeDepth)+
		    std::string(") is greater than the maximum depth allowed for a tree (")+
		    uint2str(lMaxTreeDepth)+std::string("; specified by the register variable ")+
		    std::string("'gp.tree.maxdepth').  Mutation attempt failed")
		);
		return false;
	}

	// Calculate maximum mutation depth
	unsigned int lMaxMutationDepth = lMaxTreeDepth-lChosenNodeDepth+1;
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationStandardOp",
	    std::string("Chosen node's depth is ")+uint2str(lChosenNodeDepth)+
	    std::string(" and a tree's maximum depth is ")+uint2str(lMaxTreeDepth)+
	    std::string(" (specified by the register variable 'gp.tree.maxdepth'), ")+
	    std::string("hence the maximum depth for the mutation tree is ")+uint2str(lMaxMutationDepth)
	);

	// Ensure maximum mutation depth is not larger than maximum regeneration depth
	if(lMaxMutationDepth > lMaxRegenerationDepth) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationStandardOp",
		    std::string("Maximum mutation depth (")+uint2str(lMaxMutationDepth)+
		    std::string(") is greater than the maximum regeneration depth (")+
		    uint2str(lMaxRegenerationDepth)+std::string("; as specified by the register variable '")+
		    mMaxRegenDepthName+std::string("').  Reducing maximum mutation depth to ")+
		    uint2str(lMaxRegenerationDepth)
		);
		lMaxMutationDepth = lMaxRegenerationDepth;
	}

	// Randomly choose depth
	Beagle_AssertM(lMaxMutationDepth>=1);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationStandardOp",
	    std::string("GP standard mutate the ")+uint2ordinal(lChosenNode+1)+
	    std::string(" node of the ")+uint2ordinal(lChosenTree+1)+
	    std::string(" tree with maximum depth ")+uint2str(lMaxMutationDepth)
	);

	// Create new tree
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationStandardOp",
	    std::string("Creating new tree")
	);

	const Factory& lFactory = ioContext.getSystem().getFactory();
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));

	GP::Tree::Handle lNewTree = castHandleT<GP::Tree>(lTreeAlloc->allocate());
	lNewTree->setPrimitiveSetIndex(lOriginalTree->getPrimitiveSetIndex());
	lNewTree->setNumberArguments(lOriginalTree->getNumberArguments());

	// Replace original tree with new tree
	lIndividual[lChosenTree] = lNewTree;
	lContext.setGenotypeHandle(lNewTree);

	// Initialize the tree that will be the mutation
	GP::Tree::Handle lInitTree = castHandleT<GP::Tree>(lTreeAlloc->allocate());
	lInitTree->setPrimitiveSetIndex(lNewTree->getPrimitiveSetIndex());
	lInitTree->setNumberArguments(lNewTree->getNumberArguments());
	mInitOp->initTree(*lInitTree, 1, lMaxMutationDepth, lContext);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationStandardOp",
	    "Subtree generated by mutation is"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    *lInitTree
	);

	// Copy start of original tree into new tree
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    "Copying unchanged part of original tree to new tree"
	);
	lNewTree->insert(lNewTree->end(), lOriginalTree->begin(), lOriginalTree->begin()+lChosenNode);

	// Append mutation to new tree
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    "Appending mutation to new tree"
	);
	lNewTree->insert(lNewTree->end(), lInitTree->begin(), lInitTree->end());

	// Append the rest of the original tree
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    "Appending the remainder of the original tree to new tree"
	);
	lNewTree->insert(lNewTree->end(),
	                 lOriginalTree->begin()+lChosenNode+lChosenNodeSubTreeSize,
	                 lOriginalTree->end());

	// Correct subtree size data
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    "Correcting the 'mSubTreeSize' fields of tree"
	);
	lNewTree->fixSubTreeSize();

	// Restore original context values
	lContext.setGenotypeIndex(lOldGenotypeIndex);
	lContext.setGenotypeHandle(lOldGenotypeHandle);

	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardOp",
	    ioIndividual
	);

	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationStandardOp::init(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mInitOp->init(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param ioSystem Evolutionary system.
 */
void GP::MutationStandardOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lMutationPbReadName = inIter->getAttribute("mutationpb");
	if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
	std::string lMaxRegenDepthReadName = inIter->getAttribute("maxdepth");
	if(lMaxRegenDepthReadName.empty() == false) mMaxRegenDepthName = lMaxRegenDepthReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::MutationStandardOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	Beagle::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("maxdepth", mMaxRegenDepthName);
	Beagle_StackTraceEndM();
}

