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
 *  \file   beagle/GP/src/MutationSwapSubtreeOp.cpp
 *  \brief  Source code of class GP::MutationSwapSubtreeOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Jianjun Hu <hujianju@msu.edu>
 *  \author Marc Parizeau <parizeau@gel.ulaval.ca>
 *  $Revision: 1.21 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP swap subtree mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inDistribPbName Swap mutation distribution probability parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationSwapSubtreeOp::MutationSwapSubtreeOp(std::string inMutationPbName,
        std::string inDistribPbName,
        std::string inName) :
		Beagle::MutationOp(inMutationPbName, inName),
		mDistribPbName(inDistribPbName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationSwapSubtreeOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	{
		std::ostringstream lOSS;
		lOSS << "Swap subtree mutation probability for an individual. ";
		lOSS << "A swap subtree mutation consists to swap two subtrees of a tree in an ";
		lOSS << "individual.";
		Register::Description lProbaDescription(
		    "Swap subtree mutation prob.",
		    "Double",
		    "0.0",
		    lOSS.str()
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.0f), lProbaDescription));
	}
	Beagle::MutationOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Probability that a swap subtree is internal ";
		lOSS << "(the mutation occurs between three points, where the 2nd point is in the ";
		lOSS << "1st point's subtree, and the 3rd point is in the 2nd point's subtree) vs ";
		lOSS << "being external (the mutation occurs between two points, ";
		lOSS << "where both points are not within the other's subtree). ";
		lOSS << "Value of 1.0 means that the swap subtrees mutations are all internal ";
		lOSS << "while value of 0.0 means that swap subtrees mutations are all external.";
		Register::Description lDescription(
		    "Swap subtree mut. distrib. prob.",
		    "Float",
		    "0.5",
		    lOSS.str()
		);
		mDistributionProba = castHandleT<Float>(
		                         ioSystem.getRegister().insertEntry(mDistribPbName, new Float(0.5f), lDescription));
	}
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
		lOSS << "Maximum number of attempts to modify a GP tree in a genetic ";
		lOSS << "operation. As there is topological constraints on GP trees (i.e. tree ";
		lOSS << "depth limit), it is often necessary to try a genetic operation several times.";
		Register::Description lDescription(
		    "Max number of attempts",
		    "UInt",
		    "2",
		    lOSS.str()
		);
		mNumberAttempts = castHandleT<UInt>(
		                      ioSystem.getRegister().insertEntry("gp.try", new UInt(2), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Swap subtree mutate a GP individual.
 *  \param ioIndividual GP individual to swap subtree mutate.
 *  \param ioContext Context of the evolution.
 */
bool GP::MutationSwapSubtreeOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
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
	    "Beagle::GP::MutationSwapSubtreeOp",
	    "Individual tried for swap subtree mutation (before)"
	);
	Beagle_LogObjectDebugM(
	    lContext1.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapSubtreeOp",
	    lGPIndiv
	);

	// Mutation loop. Try the given number of attempts to mutation the individual.
	for(unsigned int lAttempt=0; lAttempt < mNumberAttempts->getWrappedValue(); lAttempt++) {

		// Calculate the number of nodes in the individual
		unsigned int lNbNodes = 0;
		for(unsigned int i=0; i<lGPIndiv.size(); i++) lNbNodes += lGPIndiv[i]->size();
		if(lNbNodes == 0) return false;

		// Choose a node of the individual to mutate.
		unsigned int lNode1 = lContext1.getSystem().getRandomizer().rollInteger(0, lNbNodes-1);

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
		    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
		    std::string("Trying a swap subtree mutation of the ")+uint2ordinal(lChoosenTree+1)+
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
				    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
				    "Tree maximum depth exceeded. GP swap subtree mutation invalid."
				);
				continue;
			}

			lGPIndiv[lChoosenTree] = lTreeClone1;
			Beagle_LogVerboseM(
			    lContext1.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
			    "GP swap subtree mutation valid"
			);
			lMatingDone = true;
			break;   // The swap subtree mutation is valid.
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
			    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
			    std::string("Trying an external swap subtree mutation of the ")+uint2ordinal(lNode1+1)+
			    std::string(" node with the subtree to the ")+uint2ordinal(lNode2+1)+
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

			// Second exchange.
			lTreeClone1->setContextToNode(lNode2Exch2, lContext1);
			lTreeClone2->setContextToNode(lNode1Exch2, *lContextHdl2);
			exchangeSubTrees(*lTreeClone1, lNode2Exch2, lContext1,
			                 *lTreeClone2, lNode1Exch2, *lContextHdl2);

			// Checking if the tree depth is respected. If not, start again.
			if(lTreeClone1->getTreeDepth() > lMaxTreeDepth) {
				Beagle_LogVerboseM(
				    lContext1.getSystem().getLogger(),
				    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
				    "Tree maximum depth exceeded. GP swap subtree mutation invalid."
				);
				continue;
			}

			lGPIndiv[lChoosenTree] = lTreeClone1;
			Beagle_LogVerboseM(
			    lContext1.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
			    "GP swap subtree mutation valid"
			);
			lMatingDone = true;
			break;   // The swap subtree mutation is valid.
		}
	}

	// Replace the contexts.
	lContext1.setGenotypeHandle(lOldTreeHandle1);
	lContext1.setGenotypeIndex(lOldTreeIndex1);

	if(lMatingDone) {
		Beagle_LogDebugM(
		    lContext1.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapSubtreeOp",
		    "Individual after swap subtree mutation"
		);
		Beagle_LogObjectDebugM(
		    lContext1.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapSubtreeOp",
		    lGPIndiv
		);
	} else {
		Beagle_LogVerboseM(
		    lContext1.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationSwapSubtreeOp",
		    "No GP swap subtree mutation done"
		);
	}

	return lMatingDone;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Exchange two GP trees on given points.
 *  \param  ioTree1 First tree to mate.
 *  \param  inNode1 Node index of the swap subtree first point.
 *  \param  ioContext1 Evolutionary context relatively to the first tree.
 *  \param  ioTree2 Second tree to mate.
 *  \param  inNode2 Node index of the swap subtree second point.
 *  \param  ioContext2 Evolutionary context relatively to the second tree.
 */
void GP::MutationSwapSubtreeOp::exchangeSubTrees(GP::Tree& ioTree1,
        unsigned int inNode1,
        GP::Context& ioContext1,
        GP::Tree& ioTree2,
        unsigned int inNode2,
        GP::Context& ioContext2)
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(&ioTree1 != &ioTree2);
	unsigned int lSwapSize1 = ioTree1[inNode1].mSubTreeSize;
	unsigned int lSwapSize2 = ioTree2[inNode2].mSubTreeSize;
	if(lSwapSize1 <= lSwapSize2) {
		std::swap_ranges(ioTree1.begin()+inNode1, ioTree1.begin()+inNode1+lSwapSize1, ioTree2.begin()+inNode2);
		ioTree1.insert(ioTree1.begin()+inNode1+lSwapSize1,
		               ioTree2.begin()+inNode2+lSwapSize1,
		               ioTree2.begin()+inNode2+lSwapSize2);
		ioTree2.erase(ioTree2.begin()+inNode2+lSwapSize1, ioTree2.begin()+inNode2+lSwapSize2);
	} else {
		std::swap_ranges(ioTree1.begin()+inNode1, ioTree1.begin()+inNode1+lSwapSize2, ioTree2.begin()+inNode2);
		ioTree2.insert(ioTree2.begin()+inNode2+lSwapSize2,
		               ioTree1.begin()+inNode1+lSwapSize2,
		               ioTree1.begin()+inNode1+lSwapSize1);
		ioTree1.erase(ioTree1.begin()+inNode1+lSwapSize2, ioTree1.begin()+inNode1+lSwapSize1);
	}
	int lDiffSize = lSwapSize1 - lSwapSize2;
	for(unsigned int i=0; i<(ioContext1.getCallStackSize()-1); i++)
		ioTree1[ioContext1.getCallStackElement(i)].mSubTreeSize -= lDiffSize;
	for(unsigned int j=0; j<(ioContext2.getCallStackSize()-1); j++)
		ioTree2[ioContext2.getCallStackElement(j)].mSubTreeSize += lDiffSize;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param ioSystem Evolutionary system.
 */
void GP::MutationSwapSubtreeOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
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
void GP::MutationSwapSubtreeOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	Beagle::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("distrpb", mDistribPbName);
	Beagle_StackTraceEndM();
}
