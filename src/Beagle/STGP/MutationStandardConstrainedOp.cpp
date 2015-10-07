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
 *  \file   beagle/GP/src/MutationStandardConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationStandardConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree standard mutation operator.
 *  \param inInitOp Initialization operator used to mutate the GP subtree.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inMaxRegenDepthName Maximum regeneration depth in mutation parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationStandardConstrainedOp::MutationStandardConstrainedOp(GP::InitializationOp::Handle inInitOp,
        std::string inMutationPbName,
        std::string inMaxRegenDepthName,
        std::string inName) :
		MutationStandardOp(inInitOp, inMutationPbName, inMaxRegenDepthName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree standard mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationStandardConstrainedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MutationStandardOp::registerParams(ioSystem);
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
 *  \brief Standard mutate a constrained GP individual.
 *  \param ioIndividual GP individual to standard mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationStandardConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual        = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext              = castObjectT<GP::Context&>(ioContext);
	unsigned int lMaxTreeDepth         = mMaxTreeDepth->getWrappedValue();
	unsigned int lMaxRegenerationDepth = mMaxRegenerationDepth->getWrappedValue();

	unsigned int lNbNodes = 0;
	for(unsigned int i=0; i<lIndividual.size(); i++) lNbNodes += lIndividual[i]->size();
	if(lNbNodes == 0) return false;
	unsigned int lChoosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lNbNodes-1);
	unsigned int lChoosenTree = 0;
	for(; (lChoosenTree+1)<lIndividual.size(); lChoosenTree++) {
		if(lChoosenNode < lIndividual[lChoosenTree]->size()) break;
		else lChoosenNode -= lIndividual[lChoosenTree]->size();
	}

	const Factory& lFactory = ioContext.getSystem().getFactory();
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));

	unsigned int lOldGenotypeIndex = lContext.getGenotypeIndex();
	GP::Tree::Handle lOldGenotypeHandle = lContext.getGenotypeHandle();

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardConstrainedOp",
	    "Individual before GP standard mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardConstrainedOp",
	    ioIndividual
	);

	GP::Tree::Handle lActualTree = lIndividual[lChoosenTree];
	GP::Tree::Handle lNewTree    = castHandleT<GP::Tree>(lTreeAlloc->allocate());
	lNewTree->setPrimitiveSetIndex(lActualTree->getPrimitiveSetIndex());
	lNewTree->setNumberArguments(lActualTree->getNumberArguments());
	unsigned int lChoosenNodeSubTreeSize = (*lActualTree)[lChoosenNode].mSubTreeSize;
	lNewTree->insert(lNewTree->end(), lActualTree->begin(), lActualTree->begin()+lChoosenNode);
	lContext.setGenotypeIndex(lChoosenTree);
	lContext.setGenotypeHandle(lActualTree);
	lContext.emptyCallStack();
	lActualTree->setContextToNode(lChoosenNode, lContext);
	lContext.popCallStack();
	const unsigned int lMaxSubTreeDepth =
	    minOf<unsigned int>(lMaxTreeDepth - lContext.getCallStackSize(), lMaxRegenerationDepth);
	lIndividual[lChoosenTree] = lNewTree;
	lContext.setGenotypeHandle(lNewTree);

	unsigned int lAttempt=0;
	for(; lAttempt < mNumberAttempts->getWrappedValue(); lAttempt++) {
		if(mInitOp->initTree(*lNewTree, 1, lMaxSubTreeDepth, lContext) != 0) break;
	}
	if(lAttempt == mNumberAttempts->getWrappedValue()) {
		lIndividual[lChoosenTree] = lActualTree;
		lContext.setGenotypeIndex(lOldGenotypeIndex);
		lContext.setGenotypeHandle(lOldGenotypeHandle);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationStandardConstrainedOp",
		    "Unable to GP standard mutate the individual"
		);
		return false;
	}

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationStandardConstrainedOp",
	    std::string("GP standard mutate the ")+uint2ordinal(lChoosenNode+1)+
	    std::string(" node of the ")+uint2ordinal(lChoosenTree+1)+
	    std::string(" tree with max depth ")+uint2str(lMaxSubTreeDepth)
	);

	lNewTree->insert(lNewTree->end(),
	                 lActualTree->begin()+lChoosenNode+lChoosenNodeSubTreeSize,
	                 lActualTree->end());
	unsigned int lDiffSize =
	    (*lActualTree)[lChoosenNode].mSubTreeSize - (*lNewTree)[lChoosenNode].mSubTreeSize;
	for(unsigned int l=0; l<lContext.getCallStackSize(); l++) {
		(*lNewTree)[lContext.getCallStackElement(l)].mSubTreeSize -= lDiffSize;
	}

	lContext.setGenotypeIndex(lOldGenotypeIndex);
	lContext.setGenotypeHandle(lOldGenotypeHandle);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardConstrainedOp",
	    "Individual after GP standard mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationStandardConstrainedOp",
	    ioIndividual
	);

	return true;
	Beagle_StackTraceEndM();
}





