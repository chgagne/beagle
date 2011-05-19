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
 *  \file   beagle/GP/src/MutationSwapSubtreeConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationSwapSubtreeConstrainedOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Jianjun Hu <hujianju@msu.edu>
 *  \author Marc Parizeau <parizeau@gel.ulaval.ca>
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP swap subtree mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inDistribPbName Swap mutation distribution probability parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationSwapSubtreeConstrainedOp::MutationSwapSubtreeConstrainedOp(
    std::string inMutationPbName,
    std::string inDistribPbName,
    std::string inName) :
		MutationSwapSubtreeOp(inMutationPbName, inDistribPbName, inName)
{ }


/*!
 *  \brief Swap subtree mutate a constrained GP individual.
 *  \param ioIndividual GP individual to swap subtree mutate.
 *  \param ioContext Context of the evolution.
 */
bool GP::MutationSwapSubtreeConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Initial parameters checks.
	Beagle_AssertM(ioIndividual.size() > 0);
	Beagle_ValidateParameterM(mNumberAttempts->getWrappedValue()>0, "gp.try", ">0");

	// Cast method arguments.
	GP::Individual&     lGPIndiv      = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context&        lContext1     = castObjectT<GP::Context&>(ioContext);
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioContext.getSystem().getFactory().getConceptAllocator("Context"));
	GP::Context::Handle lContextHdl2  = castHandleT<GP::Context>(lContextAlloc->clone(lContext1));

	const Factory& lFactory = ioContext.getSystem().getFactory();
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));

	// Get parameters in local values, with the total number of nodes of the mutated individual.
	bool             lMatingDone     = false;
	float            lDistrProba     = mDistributionProba->getWrappedValue();
	unsigned int     lMaxTreeDepth   = mMaxTreeDepth->getWrappedValue();
	GP::Tree::Handle lOldTreeHandle1 = lContext1.getGenotypeHandle();
	unsigned int     lOldTreeIndex1  = lContext1.getGenotypeIndex();
	unsigned int     lSizeIndiv      = 0;
	for(unsigned int i=0; i<lGPIndiv.size(); ++i) lSizeIndiv += lGPIndiv[i]->size();

	// Some outputs.
	Beagle_LogDebugM(
	    lContext1.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapSubtreeConstrainedOp",
	    "Individual tried for constrained swap subtree mutation (before)"
	);
	Beagle_LogObjectDebugM(
	    lContext1.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapSubtreeConstrainedOp",
	    lGPIndiv
	);

	// Mutation loop. Try the given number of attempts to mutation the individual.
	for(unsigned int lAttempt=0; lAttempt < mNumberAttempts->getWrappedValue(); lAttempt++) {

		// Choose a node of the individual to mutate.
		unsigned int lNode1 = lContext1.getSystem().getRandomizer().rollInteger(0, lGPIndiv.size()-1);

		// Get the tree in which the choosen node is. Change the global node index to the tree's index.
		unsigned int lChoosenTree = 0;
		for(; lChoosenTree<lGPIndiv.size(); ++lChoosenTree) {
			if(lNode1 < lGPIndiv[lChoosenTree]->size()) break;
			Beagle_AssertM(lNode1 >= lGPIndiv[lChoosenTree]->size());
			lNode1 -= lGPIndiv[lChoosenTree]->size();
		}
		Beagle_AssertM(lChoosenTree < lGPIndiv.size());

		// Cannot do anything with an tree of size <= 1.
		if(lGPIndiv[lChoosenTree]->size() <= 1) continue;

		// Some outputs.
		Beagle_LogVerboseM(
		    lContext1.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
		    std::string("Trying a constrained swap subtree mutation of the ")+uint2ordinal(lChoosenTree+1)+
		    std::string(" tree")
		);

		// Make two clones of the choosen tree.
		GP::Tree::Handle lTreeClone1 = castHandleT<GP::Tree>(lTreeAlloc->clone(*lGPIndiv[lChoosenTree]));
		GP::Tree::Handle lTreeClone2 = castHandleT<GP::Tree>(lTreeAlloc->clone(*lGPIndiv[lChoosenTree]));

		// Now we decide whether the swap subtree mutation is internal or external.
		bool lMutationType = lContext1.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba;

		// Cannot do an internal mutation when there is only one branch in the tree.
		if(lTreeClone1->size() == (*lTreeClone1)[0].mPrimitive->getNumberArguments()+1)
			lMutationType = false;

		// This is special case, a linear tree. Cannot do an external mutation.
		if(lTreeClone1->size() == (*lTreeClone1)[1].mSubTreeSize+1) {
			if(lTreeClone1->size()==2) continue;   // Cannot do anything here with the tree.
			lMutationType = true;
		}

		// lMutationType is true -> internal mutation
		if(lMutationType)  {

			// If the selected node is a terminal, or a branch with a subtree made only of terminals,
			// choose another node in the same tree.
			while((*lTreeClone1)[lNode1].mSubTreeSize ==
			        ((*lTreeClone1)[lNode1].mPrimitive->getNumberArguments()+1)) {
				lNode1 = lContext1.getSystem().getRandomizer().rollInteger(0, lTreeClone1->size()-1);
			}

			// Choosing the second node, a branch in lNode1's subtree.
			unsigned int lSubTreeSizeN1 = (*lTreeClone1)[lNode1].mSubTreeSize;
			unsigned int lN2OffN1 =
			    lContext1.getSystem().getRandomizer().rollInteger(1, lSubTreeSizeN1-1);
			unsigned int lNode2 = lNode1 + lN2OffN1;
			while((*lTreeClone1)[lNode2].mPrimitive->getNumberArguments() == 0) {
				lN2OffN1 = lContext1.getSystem().getRandomizer().rollInteger(1, lSubTreeSizeN1-1);
				lNode2 = lNode1 + lN2OffN1;
			}

			// Choosing the third node, any node in lNode2's subtree.
			unsigned int lSubTreeSizeN2 = (*lTreeClone1)[lNode2].mSubTreeSize;
			unsigned int lN3OffN2 =
			    lContext1.getSystem().getRandomizer().rollInteger(1, lSubTreeSizeN2-1);

			// Ok, now we can exchange the subtrees.

			// New value of lNode1 and lNode2 for the second exchange.
			unsigned int lNode3Exch2 = lNode1 + lN3OffN2;
			unsigned int lNode1Exch2 = lNode2;

			// New value of lNode1 and lNode2 for the third exchange.
			unsigned int lNode2Exch3 = lNode1 + lN3OffN2 + lN2OffN1;
			unsigned int lNode3Exch3 = lNode2;

			// First exchange.
			lTreeClone1->setContextToNode(lNode1, lContext1);
			lTreeClone2->setContextToNode(lNode2, *lContextHdl2);
			exchangeSubTrees(*lTreeClone1, lNode1, lContext1,
			                 *lTreeClone2, lNode2, *lContextHdl2);

			// Second exchange.
			lTreeClone1->setContextToNode(lNode3Exch2, lContext1);
			lTreeClone2->setContextToNode(lNode1Exch2, *lContextHdl2);
			exchangeSubTrees(*lTreeClone1, lNode3Exch2, lContext1,
			                 *lTreeClone2, lNode1Exch2, *lContextHdl2);

			// Third exchange.
			lTreeClone1->setContextToNode(lNode2Exch3, lContext1);
			lTreeClone2->setContextToNode(lNode3Exch3, *lContextHdl2);
			exchangeSubTrees(*lTreeClone1, lNode2Exch3, lContext1,
			                 *lTreeClone2, lNode3Exch3, *lContextHdl2);

			// Checking if the tree depth is respected. If not, start again.
			if(lTreeClone1->getTreeDepth() > lMaxTreeDepth) {
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
				    "Tree maximum depth exceeded. Constrained GP swap subtree mutation invalid."
				);
				continue;
			}

			// As we are using topologically contrained tree, valid the new tree. If it is not valid,
			// do a new mutation attempt.
			lContext1.setGenotypeHandle(lTreeClone1);
			lContext1.setGenotypeIndex(lChoosenTree);
			lTreeClone1->setContextToNode(lNode1, lContext1);
			if(lTreeClone1->validateSubTree(lNode1, lContext1)) {
				lGPIndiv[lChoosenTree] = lTreeClone1;
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
				    "Constrained GP swap subtree mutation valid"
				);
				lMatingDone = true;
				break; // The swap subtree mutation is valid.
			} else {
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
				    "Constrained GP swap subtree mutation invalid"
				);
				continue;
			}
		}

		// lMutationType is false -> external mutation
		else {

			// Deterniming the minimal node index to use.
			unsigned int lMinNodeIndex = 0;
			for(; lTreeClone1->size() == ((*lTreeClone1)[lMinNodeIndex].mSubTreeSize+lMinNodeIndex);
			        ++lMinNodeIndex) {
				if(lMinNodeIndex == (lTreeClone1->size()-1)) continue; // Can't do anything with linear tree.
			}

			// Change lNode1 if less than minimum node index.
			if(lNode1 < lMinNodeIndex)
				lNode1 = lContext1.getSystem().getRandomizer().rollInteger(lMinNodeIndex,
				         lTreeClone1->size()-1);

			// Choosing second swap subtree mutation point.
			std::vector<unsigned int> lValidN2;
			for(unsigned int i=lMinNodeIndex; i<lTreeClone1->size(); ++i) {
				if((i>=lNode1) && (i<lNode1+(*lTreeClone1)[lNode1].mSubTreeSize)) continue;
				else if((lNode1>=i) && (lNode1<(i+(*lTreeClone1)[i].mSubTreeSize))) continue;
				else lValidN2.push_back(i);
			}
			unsigned int lNode2 =
			    lValidN2[lContext1.getSystem().getRandomizer().rollInteger(0, lValidN2.size()-1)];

			Beagle_LogVerboseM(
			    lContext1.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
			    std::string("Trying an external constrained swap subtree mutation of the ")+
			    uint2ordinal(lNode1+1)+std::string(" node with the subtree to the ")+uint2ordinal(lNode2+1)+
			    std::string(" node")
			);

			// Ok, now we can exchange the subtrees.

			// New value of lNode1 and lNode2 for the second exchange.
			unsigned int lNode1Exch2 = lNode1;
			unsigned int lNode2Exch2 = lNode2;
			if(lNode1 < lNode2) {
				lNode2Exch2 += (*lTreeClone1)[lNode2].mSubTreeSize;
				lNode2Exch2 -= (*lTreeClone1)[lNode1].mSubTreeSize;
			} else {
				lNode1Exch2 += (*lTreeClone1)[lNode1].mSubTreeSize;
				lNode1Exch2 -= (*lTreeClone1)[lNode2].mSubTreeSize;
			}

			// First exchange.
			lTreeClone1->setContextToNode(lNode1, lContext1);
			lTreeClone2->setContextToNode(lNode2, *lContextHdl2);
			exchangeSubTrees(*lTreeClone1, lNode1, lContext1,
			                 *lTreeClone2, lNode2, *lContextHdl2);

			// Second exchange
			lTreeClone1->setContextToNode(lNode2Exch2, lContext1);
			lTreeClone2->setContextToNode(lNode1Exch2, *lContextHdl2);
			exchangeSubTrees(*lTreeClone1, lNode2Exch2, lContext1,
			                 *lTreeClone2, lNode1Exch2, *lContextHdl2);

			// Checking if the tree depth is respected. If not, start again.
			if(lTreeClone1->getTreeDepth() > lMaxTreeDepth) {
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
				    "Tree maximum depth exceeded. Constrained GP swap subtree mutation invalid."
				);
				continue;
			}

			// As we are using topologically contrained tree, valid the new tree. If it is not valid,
			// do a new mutation attempt.
			lContext1.setGenotypeHandle(lTreeClone1);
			lContext1.setGenotypeIndex(lChoosenTree);
			lTreeClone1->setContextToNode(lNode1Exch2, lContext1);
			bool lNode1Valid = lTreeClone1->validateSubTree(lNode1Exch2,lContext1);
			lTreeClone1->setContextToNode(lNode2Exch2, lContext1);
			bool lNode2Valid = lTreeClone1->validateSubTree(lNode2Exch2,lContext1);
			if(lNode1Valid && lNode2Valid) {
				lGPIndiv[lChoosenTree] = lTreeClone1;
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
				    "Constrained GP swap subtree mutation valid"
				);
				lMatingDone = true;
				break; // The swap subtree mutation is valid.
			} else {
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeConstrainedOp",
				    "Constrained GP swap subtree mutation invalid"
				);
				continue;
			}
		}
	}

	// Replace the contexts.
	lContext1.setGenotypeHandle(lOldTreeHandle1);
	lContext1.setGenotypeIndex(lOldTreeIndex1);

	if(lMatingDone) {
		Beagle_LogDebugM(
		    lContext1.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapSubtreeConstrainedOp",
		    "Individual after constrained swap subtree mutation"
		);
		Beagle_LogObjectDebugM(
		    lContext1.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapSubtreeConstrainedOp",
		    lGPIndiv
		);
	} else {
		Beagle_LogVerboseM(
		    lContext1.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapSubtreeConstrainedOp",
		    "No constrained GP swap subtree mutation done"
		);
	}

	return lMatingDone;
	Beagle_StackTraceEndM();
}



