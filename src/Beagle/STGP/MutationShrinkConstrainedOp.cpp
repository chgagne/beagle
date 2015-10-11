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
 *  \file   beagle/GP/src/MutationShrinkConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationShrinkConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree shrink mutation operator.
 *  \param inMutationPbName Mutation shrink probability.
 *  \param inName Name of the operator.
 */
GP::MutationShrinkConstrainedOp::MutationShrinkConstrainedOp(std::string inMutationPbName,
        std::string inName) :
		MutationShrinkOp(inMutationPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree shrink mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationShrinkConstrainedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MutationShrinkOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "Maximum number of attempts to modify a GP tree in a genetic ";
	lOSS << "operation. As there is topological constraints on GP trees (i.e. tree ";
	lOSS << "depth limit), it is often necessary to try a genetic operation several times.";
	Register::Description lDescription("Max number of attempts", "UInt", "2", lOSS.str());
	mNumberAttempts =
	    castHandleT<UInt>(ioSystem.getRegister().insertEntry("gp.try", new UInt(2), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Shrink mutate a GP individual.
 *  \param ioIndividual GP individual to shrink mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationShrinkConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);
	unsigned int lNumberAttempts = mNumberAttempts->getWrappedValue();
	bool lMutationDone           = false;

	unsigned int lNbNodes = 0;
	for(unsigned int i=0; i<lIndividual.size(); i++) lNbNodes += lIndividual[i]->size();
	if(lNbNodes == 0) return false;
	unsigned int lChoosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lNbNodes-1);
	unsigned int lChoosenTree = 0;
	for(; (lChoosenTree+1)<lIndividual.size(); lChoosenTree++) {
		if(lChoosenNode < lIndividual[lChoosenTree]->size()) break;
		else lChoosenNode -= lIndividual[lChoosenTree]->size();
	}
	GP::Tree::Handle lActualTree = lIndividual[lChoosenTree];
	if(lActualTree->size() < 2) return false;

	GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = lContext.getGenotypeIndex();

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationShrinkConstrainedOp",
	    "Individual before constrained GP tree shrink mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationShrinkConstrainedOp",
	    ioIndividual
	);
	
	const Factory& lFactory = ioContext.getSystem().getFactory();
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));

	lIndividual[lChoosenTree] = castHandleT<GP::Tree>(lTreeAlloc->allocate());
	lIndividual[lChoosenTree]->setPrimitiveSetIndex(lActualTree->getPrimitiveSetIndex());
	lIndividual[lChoosenTree]->setNumberArguments(lActualTree->getNumberArguments());

	for(unsigned int lAttempt=0; lAttempt<lNumberAttempts; lAttempt++) {
		while((*lActualTree)[lChoosenNode].mPrimitive->getNumberArguments() == 0) {
			lChoosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lActualTree->size()-1);
		}
		lIndividual[lChoosenTree]->clear();
		lIndividual[lChoosenTree]->insert(lIndividual[lChoosenTree]->end(),
		                                  lActualTree->begin(),
		                                  lActualTree->begin()+lChoosenNode);
		unsigned int lChoosenArg =
		    lContext.getSystem().getRandomizer().rollInteger(0,
		            (*lActualTree)[lChoosenNode].mPrimitive->getNumberArguments()-1);
		unsigned int lChoosenArgIndex = lChoosenNode + 1;
		for(unsigned int k=0; k<lChoosenArg; k++) {
			lChoosenArgIndex += (*lActualTree)[lChoosenArgIndex].mSubTreeSize;
		}
		unsigned int lChoosenArgSubTreeSize = (*lActualTree)[lChoosenArgIndex].mSubTreeSize;
		lIndividual[lChoosenTree]->insert(lIndividual[lChoosenTree]->end(),
		                                  lActualTree->begin()+lChoosenArgIndex,
		                                  lActualTree->begin()+lChoosenArgIndex+lChoosenArgSubTreeSize);
		unsigned int lChoosenNodeSubTreeSize = (*lActualTree)[lChoosenNode].mSubTreeSize;
		lIndividual[lChoosenTree]->insert(lIndividual[lChoosenTree]->end(),
		                                  lActualTree->begin()+lChoosenNode+lChoosenNodeSubTreeSize,
		                                  lActualTree->end());
		lActualTree->setContextToNode(lChoosenNode, lContext);
		unsigned int lDiffSize =
		    (*lActualTree)[lChoosenNode].mSubTreeSize - (*lActualTree)[lChoosenArgIndex].mSubTreeSize;
		for(unsigned int l=0; l<(lContext.getCallStackSize()-1); l++) {
			(*lIndividual[lChoosenTree])[lContext.getCallStackElement(l)].mSubTreeSize -= lDiffSize;
		}

		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
		    std::string("Trying to replace the ")+uint2ordinal(lChoosenNode+1)+
		    std::string(" node of the ")+uint2ordinal(lChoosenTree+1)+
		    std::string(" tree with its ")+uint2ordinal(lChoosenArg+1)+
		    std::string(" argument, that is the ")+
		    uint2ordinal(lChoosenArgIndex+1)+std::string(" node")
		);

		lContext.setGenotypeHandle(lIndividual[lChoosenTree]);
		lContext.setGenotypeIndex(lChoosenTree);
		if(lIndividual[lChoosenTree]->validateSubTree(lChoosenNode, lContext)) {
			lMutationDone = true;
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
			    "Constrained GP tree shrink mutation valid"
			);
			break;
		} else if(lAttempt == (lNumberAttempts-1)) {
			lIndividual[lChoosenTree] = lActualTree;
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
			    "Constrained GP tree shrink mutation invalid"
			);
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
			    "Unable to shrink mutate the individual"
			);
			break;
		} else {
			lChoosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lActualTree->size()-1);
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
			    "Constrained GP tree shrink mutation invalid"
			);
			continue;
		}
	}

	lContext.setGenotypeHandle(lOldTreeHandle);
	lContext.setGenotypeIndex(lOldTreeIndex);

	if(lMutationDone) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
		    "Individual after constrained GP tree shrink mutation"
		);
		Beagle_LogObjectDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
		    ioIndividual
		);
	} else {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationShrinkConstrainedOp",
		    "No constrained GP tree shrink mutation done"
		);
	}

	return lMutationDone;
	Beagle_StackTraceEndM();
}

