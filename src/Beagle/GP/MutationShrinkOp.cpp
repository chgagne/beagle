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
 *  \file   beagle/GP/src/MutationShrinkOp.cpp
 *  \brief  Source code of class GP::MutationShrinkOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP shrink mutation operator.
 *  \param inMutationPbName Mutation shrink probability.
 *  \param inName Name of the operator.
 */
GP::MutationShrinkOp::MutationShrinkOp(std::string inMutationPbName,
                                       std::string inName) :
		Beagle::MutationOp(inMutationPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the GP shrink mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationShrinkOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << "Shrink mutation probability for an individual. ";
	lOSS << "Shrink mutation consists in replacing a branch (a node with one ";
	lOSS << "or more arguments) with one of his child node. This erases ";
	lOSS << "the chosen node and the other child nodes.";
	Register::Description lProbaDescription(
	    "Individual shrink mutation prob.",
	    "Double",
	    "0.05",
	    lOSS.str()
	);
	mMutationProba = castHandleT<Double>(
	                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.05f), lProbaDescription));
	Beagle::MutationOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Shrink mutate a GP individual.
 *  \param ioIndividual GP individual to shrink mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationShrinkOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationShrinkOp",
	    "Individual before GP shrink mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationShrinkOp",
	    lIndividual
	);

	unsigned int lChosenTree = lIndividual.chooseRandomTree(lContext);
	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationShrinkOp",
	    std::string("Applying shrink mutation to ")+uint2ordinal(lChosenTree+1)+std::string(" tree")
	);

	GP::Tree::Handle lActualTree = lIndividual[lChosenTree];
	if(lActualTree->size() < 2) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationShrinkOp",
		    std::string("Shrink mutation failed because the chosen tree has a size less than two (hence it doesn't have any primitives with arguments).")
		);
		return false;
	}

	GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = lContext.getGenotypeIndex();

	unsigned int lChosenNode = lIndividual.chooseRandomNodeWithArgs(lChosenTree, lContext);

	const Factory& lFactory = ioContext.getSystem().getFactory();
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));

	lIndividual[lChosenTree] = castHandleT<GP::Tree>(lTreeAlloc->allocate());

	// Copy lActualTree's attributes into new tree
	lIndividual[lChosenTree]->setPrimitiveSetIndex(lActualTree->getPrimitiveSetIndex());
	lIndividual[lChosenTree]->setNumberArguments(lActualTree->getNumberArguments());
	lIndividual[lChosenTree]->clear();
	lIndividual[lChosenTree]->insert(lIndividual[lChosenTree]->end(),
	                                 lActualTree->begin(),
	                                 lActualTree->begin()+lChosenNode);
	unsigned int lChosenArg = lContext.getSystem().getRandomizer().rollInteger(0,
	                          (*lActualTree)[lChosenNode].mPrimitive->getNumberArguments()-1);
	unsigned int lChosenArgIndex = lChosenNode + 1;
	for(unsigned int k=0; k<lChosenArg; k++) {
		lChosenArgIndex += (*lActualTree)[lChosenArgIndex].mSubTreeSize;
	}
	unsigned int lChosenArgSubTreeSize = (*lActualTree)[lChosenArgIndex].mSubTreeSize;
	lIndividual[lChosenTree]->insert(lIndividual[lChosenTree]->end(),
	                                 lActualTree->begin()+lChosenArgIndex,
	                                 lActualTree->begin()+lChosenArgIndex+lChosenArgSubTreeSize);
	unsigned int lChosenNodeSubTreeSize = (*lActualTree)[lChosenNode].mSubTreeSize;
	lIndividual[lChosenTree]->insert(lIndividual[lChosenTree]->end(),
	                                 lActualTree->begin()+lChosenNode+lChosenNodeSubTreeSize,
	                                 lActualTree->end());
	lActualTree->setContextToNode(lChosenNode, lContext);
	unsigned int lDiffSize =
	    (*lActualTree)[lChosenNode].mSubTreeSize - (*lActualTree)[lChosenArgIndex].mSubTreeSize;
	for(unsigned int l=0; l<(lContext.getCallStackSize()-1); l++) {
		(*lIndividual[lChosenTree])[lContext.getCallStackElement(l)].mSubTreeSize -= lDiffSize;
	}

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationShrinkOp",
	    std::string("Replacing the ")+uint2ordinal(lChosenNode+1)+
	    std::string(" node of the ")+uint2ordinal(lChosenTree+1)+
	    std::string(" tree with its ")+uint2ordinal(lChosenArg+1)+
	    std::string(" argument, that is the ")+
	    uint2ordinal(lChosenArgIndex+1)+std::string(" node")
	);

	lContext.setGenotypeHandle(lOldTreeHandle);
	lContext.setGenotypeIndex(lOldTreeIndex);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationShrinkOp",
	    "Individual after GP shrink mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationShrinkOp",
	    ioIndividual
	);

	return true;
	Beagle_StackTraceEndM();
}

