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
 *  \file   beagle/GP/src/CrossoverConstrainedOp.cpp
 *  \brief  Source code of class GP::CrossoverConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP constrained tree crossover operator.
 *  \param inMatingPbName Individual mating probability parameter name used in register.
 *  \param inDistribPbName Distribution probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GP::CrossoverConstrainedOp::CrossoverConstrainedOp(std::string inMatingPbName,
        std::string inDistribPbName,
        std::string inName) :
		Beagle::GP::CrossoverOp(inMatingPbName, inDistribPbName, inName)
{ }


/*!
 *  \brief Build a roulette of nodes that can be selected following the constraints penalties.
 *  \param ioRoulette Roulette of nodes that can be selected following the constraints given.
 *  \param inSelectABranch True if node to select must be a branch, false if it must a leaf.
 *  \param inMaxSubTreeDepth Maximum sub tree depth allowed of the node to be selected.
 *  \param inMaxSubTreeSize Maximum sub tree size allowed of the node to be selected.
 *  \param inActualIndex Index in actual tree of the node processed.
 *  \param inTree Tree processed.
 *  \param ioContext Evolutionary context.
 *  \return Max depth of subtree processed.
 */
unsigned int GP::CrossoverConstrainedOp::buildRoulette(
    RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
    bool inSelectABranch,
    unsigned int inMaxSubTreeDepth,
    unsigned int inMaxSubTreeSize,
    unsigned int inActualIndex,
    GP::Tree& inTree,
    GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	const unsigned int lNbArgs = inTree[inActualIndex].mPrimitive->getNumberArguments();
	const unsigned int lSubTreeSize = inTree[inActualIndex].mSubTreeSize;
	const bool lGoodArity = ((inTree.size()==1) || ((lNbArgs==0) != inSelectABranch));
	ioContext.pushCallStack(inActualIndex);
	unsigned int lChildIndex = inActualIndex+1;
	unsigned int lMaxDepthDown = 0;
	for(unsigned int i=0; i<lNbArgs; ++i) {
		unsigned int lChildDepth = buildRoulette(ioRoulette,
		                           inSelectABranch,
		                           inMaxSubTreeDepth,
		                           inMaxSubTreeSize,
		                           lChildIndex,
		                           inTree,
		                           ioContext);
		lChildIndex += inTree[lChildIndex].mSubTreeSize;
		if(lChildDepth > lMaxDepthDown) lMaxDepthDown = lChildDepth;
	}
	++lMaxDepthDown;
	const unsigned int lMaxDepthUp = ioContext.getCallStackSize();
	ioContext.popCallStack();
	if(lGoodArity && (lSubTreeSize<=inMaxSubTreeSize) &&
	        (lMaxDepthDown<=inMaxSubTreeDepth) && (lMaxDepthUp<=inMaxSubTreeDepth)) {
		std::pair<unsigned int,unsigned int> lPair(ioContext.getGenotypeIndex(), inActualIndex);
		ioRoulette.insert(lPair, 1.0);
	}
	return lMaxDepthDown;
	Beagle_StackTraceEndM();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Build a roulette of nodes that can be selected following the constraints penalties.
 *  \param ioRoulette Roulette of nodes that can be selected following the constraints given.
 *  \param inSelectABranch True if node to select must be a branch, false if it must a leaf.
 *  \param inNodeReturnType Desired return type for the nodes to be selected.
 *  \param inMaxSubTreeDepth Maximum sub tree depth allowed of the node to be selected.
 *  \param inMaxSubTreeSize Maximum sub tree size allowed of the node to be selected.
 *  \param inActualIndex Index in actual tree of the node processed.
 *  \param inTree Tree processed.
 *  \param ioContext Evolutionary context.
 *  \return Max depth of subtree processed.
 */
unsigned int GP::CrossoverConstrainedOp::buildRouletteWithType(
    RouletteT< std::pair<unsigned int,unsigned int> >& ioRoulette,
    bool inSelectABranch,
    const std::type_info* inNodeReturnType,
    unsigned int inMaxSubTreeDepth,
    unsigned int inMaxSubTreeSize,
    unsigned int inActualIndex,
    GP::Tree& inTree,
    GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	const unsigned int lNbArgs = inTree[inActualIndex].mPrimitive->getNumberArguments();
	const unsigned int lSubTreeSize = inTree[inActualIndex].mSubTreeSize;
	const bool lGoodArity = ((inTree.size()==1) || ((lNbArgs==0) != inSelectABranch));
	ioContext.pushCallStack(inActualIndex);
	const std::type_info* lNodeType = inTree[inActualIndex].mPrimitive->getReturnType(ioContext);
	const bool lCompatibleTyping = ((inNodeReturnType==NULL) || (lNodeType==NULL) ||
	                                (inNodeReturnType==lNodeType));
	unsigned int lChildIndex = inActualIndex+1;
	unsigned int lMaxDepthDown = 0;
	for(unsigned int i=0; i<lNbArgs; ++i) {
		unsigned int lChildDepth = buildRouletteWithType(ioRoulette,
		                           inSelectABranch,
		                           inNodeReturnType,
		                           inMaxSubTreeDepth,
		                           inMaxSubTreeSize,
		                           lChildIndex,
		                           inTree,
		                           ioContext);
		lChildIndex += inTree[lChildIndex].mSubTreeSize;
		if(lChildDepth > lMaxDepthDown) lMaxDepthDown = lChildDepth;
	}
	++lMaxDepthDown;
	const unsigned int lMaxDepthUp = ioContext.getCallStackSize();
	ioContext.popCallStack();
	if(lGoodArity && lCompatibleTyping && (lSubTreeSize<=inMaxSubTreeSize) &&
	        (lMaxDepthDown<=inMaxSubTreeDepth) && (lMaxDepthUp<=inMaxSubTreeDepth)) {
		std::pair<unsigned int,unsigned int> lPair(ioContext.getGenotypeIndex(), inActualIndex);
		ioRoulette.insert(lPair, 1.0);
	}
	return lMaxDepthDown;
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Mate two GP individuals for a constrained tree crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool GP::CrossoverConstrainedOp::mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
                                      Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2)
{
	Beagle_StackTraceBeginM();
	// Initial parameters checks
	Beagle_AssertM(ioIndiv1.size() > 0);
	//Beagle_AssertM(ioIndiv1.size() == ioIndiv2.size());
	Beagle_ValidateParameterM(mNumberAttempts->getWrappedValue()>0,"gp.try",">0");

	// Cast method arguments.
	GP::Individual& lIndiv1   = castObjectT<GP::Individual&>(ioIndiv1);
	GP::Individual& lIndiv2   = castObjectT<GP::Individual&>(ioIndiv2);
	GP::Context&    lContext1 = castObjectT<GP::Context&>(ioContext1);
	GP::Context&    lContext2 = castObjectT<GP::Context&>(ioContext2);

	// Get parameters in local values, with the total number of nodes of an individual.
	bool             lMatingDone     = false;
	float            lDistrProba     = mDistributionProba->getWrappedValue();
	unsigned int     lMaxTreeDepth   = mMaxTreeDepth->getWrappedValue();
	GP::Tree::Handle lOldTreeHandle1 = lContext1.getGenotypeHandle();
	unsigned int     lOldTreeIndex1  = lContext1.getGenotypeIndex();
	GP::Tree::Handle lOldTreeHandle2 = lContext2.getGenotypeHandle();
	unsigned int     lOldTreeIndex2  = lContext2.getGenotypeIndex();
	unsigned int     lSizeIndiv1     = 0;
	for(unsigned int i=0; i<lIndiv1.size(); i++) lSizeIndiv1 += lIndiv1[i]->size();

	Beagle_LogDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GP::CrossoverConstrainedOp",
	    "Individuals to mate (before constrained GP crossover)"
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GP::CrossoverConstrainedOp",
	    lIndiv1
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GP::CrossoverConstrainedOp",
	    lIndiv2
	);

	// Crossover loop. Try the given number of attempts to mate two individuals.
	for(unsigned int lAttempt=0; lAttempt < mNumberAttempts->getWrappedValue(); ++lAttempt) {

		// Choose a node in all the individual node.
		unsigned int lChoosenNode1 =
		    lContext1.getSystem().getRandomizer().rollInteger(0, lSizeIndiv1-1);

		// Get the tree in which the choosen node is. Change the global node index to the tree's index.
		unsigned int lChoosenTree1 = 0;
		for(; lChoosenTree1<lIndiv1.size(); lChoosenTree1++) {
			if(lChoosenNode1 < lIndiv1[lChoosenTree1]->size()) break;
			Beagle_AssertM(lChoosenNode1 >= lIndiv1[lChoosenTree1]->size());
			lChoosenNode1 -= lIndiv1[lChoosenTree1]->size();
		}
		Beagle_AssertM(lChoosenTree1 < lIndiv1.size());

		// Choose a type of node (branch or leaf) following the distribution probability and change the
		// node for another node of the same tree if the types mismatch.
		GP::Tree& lTree1 = *lIndiv1[lChoosenTree1];
		const unsigned int lPrimitiveSetIndex1 = lTree1.getPrimitiveSetIndex();
		if(lTree1.size() > 1) {
			bool lTypeNode1 =
			    (lContext1.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba);
			while((lTree1[lChoosenNode1].mPrimitive->getNumberArguments() != 0) != lTypeNode1) {
				lChoosenNode1 = lContext1.getSystem().getRandomizer().rollInteger(0, lTree1.size()-1);
			}
		}

		// Choose type of node (branch or leaf) for the second node.
		const bool lTypeNode2 =
		    (lContext2.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba);

		// Compute max depth allowable.
		lTree1.setContextToNode(lChoosenNode1, lContext1);
		const unsigned int lTmpMaxDepth1 = lMaxTreeDepth - lContext1.getCallStackSize();
		const unsigned int lTmpMaxDepth2 = lMaxTreeDepth - lTree1.getTreeDepth(lChoosenNode1);
		const unsigned int lMaxDepthTree2 = minOf(lTmpMaxDepth1, lTmpMaxDepth2);

		// Select a node in second individual for the crossover.
		unsigned int lChoosenTree2=0;
		unsigned int lChoosenNode2=0;

#ifdef BEAGLE_HAVE_RTTI
		const std::type_info* lDesiredType = lTree1[lChoosenNode1].mPrimitive->getReturnType(lContext1);
		bool lGoodSelect = selectNodeToMateWithType(lChoosenTree2,
		                   lChoosenNode2,
		                   lTypeNode2,
		                   lDesiredType,
		                   lPrimitiveSetIndex1,
		                   lMaxDepthTree2,
		                   UINT_MAX,
		                   lIndiv2,
		                   lContext2);
#else // BEAGLE_HAVE_RTTI
		bool lGoodSelect = selectNodeToMate(lChoosenTree2,
											lChoosenNode2,
											lTypeNode2,
											lPrimitiveSetIndex1,
											lMaxDepthTree2,
											UINT_MAX,
											lIndiv2,
											lContext2);
#endif // BEAGLE_HAVE_RTTI

		// Check to see that there is at least one node that can be selected
		if(lGoodSelect==false) {
			Beagle_LogVerboseM(
			    ioContext1.getSystem().getLogger(),
			    "crossover", "Beagle::GP::CrossoverConstrainedOp",
			    std::string("Crossover attempt failed: it seems there is no corresponding nodes in second ")+
			    std::string("individual that would meet all the constraints")
			);
			continue;
		}

		// Get reference to the tree the choosen node is in.
		Beagle_AssertM(lChoosenTree2 < lIndiv2.size());
		GP::Tree& lTree2 = *lIndiv2[lChoosenTree2];
		lTree2.setContextToNode(lChoosenNode2, lContext2);

		// Mate the trees.
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GP::CrossoverConstrainedOp",
		    std::string("Trying to exchange the ")+uint2ordinal(lChoosenNode1+1)+
		    std::string(" node of the ")+uint2ordinal(lChoosenTree1+1)+
		    std::string(" tree of the first individual with the ")+uint2ordinal(lChoosenNode2+1)+
		    std::string(" node of the ")+uint2ordinal(lChoosenTree2+1)+
		    std::string(" tree of the second individual")
		);

		mateTrees(lTree1, lChoosenNode1, lContext1, lTree2, lChoosenNode2, lContext2);

		// If one tree is not valid, undo the crossover and do a new crossover attempt.
		lContext1.setGenotypeHandle(lIndiv1[lChoosenTree1]);
		lContext1.setGenotypeIndex(lChoosenTree1);
		lContext2.setGenotypeHandle(lIndiv2[lChoosenTree2]);
		lContext2.setGenotypeIndex(lChoosenTree2);
		if(lTree1.validateSubTree(lChoosenNode1,lContext1) &&
		        lTree2.validateSubTree(lChoosenNode2,lContext2)) {
			lMatingDone = true;
			Beagle_LogVerboseM(
			    ioContext1.getSystem().getLogger(),
			    "crossover", "Beagle::GP::CrossoverConstrainedOp",
			    "Constrained tree GP crossover valid"
			);
			break; // The crossover is valid.
		} else {   // Undo crossover.
			Beagle_LogVerboseM(
			    ioContext1.getSystem().getLogger(),
			    "crossover", "Beagle::GP::CrossoverConstrainedOp",
			    "Crossover attempt failed because one of the resulting trees was invalid"
			);
			mateTrees(lTree1, lChoosenNode1, lContext1, lTree2, lChoosenNode2, lContext2);
			continue;
		}
	}

	// Replace the contexts.
	lContext1.setGenotypeHandle(lOldTreeHandle1);
	lContext1.setGenotypeIndex(lOldTreeIndex1);
	lContext2.setGenotypeHandle(lOldTreeHandle2);
	lContext2.setGenotypeIndex(lOldTreeIndex2);

	if(lMatingDone) {
		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",
		    "Beagle::GP::CrossoverConstrainedOp",
		    "Individuals mated (after constrained tree GP crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",
		    "Beagle::GP::CrossoverConstrainedOp",
		    lIndiv1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",
		    "Beagle::GP::CrossoverConstrainedOp",
		    lIndiv2
		);
	} else {
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GP::CrossoverConstrainedOp",
		    "No constrained tree GP crossover done"
		);
	}

	return lMatingDone;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Select a node for mating in the given individual, following the constraints penalties.
 *  \param outSelectTreeIndex Tree index of the selected node.
 *  \param outSelectNodeIndex Index of the selected node.
 *  \param inSelectABranch True if node to select must be a branch, false if it must a leaf.
 *  \param inPrimitSetIndex Primitive set index to which the tree must be associated.
 *  \param inMaxSubTreeDepth Maximum sub tree depth allowed of the node to be selected.
 *  \param inMaxSubTreeSize Maximum sub tree size allowed of the node to be selected.
 *  \param inIndividual Individual to select the node from.
 *  \param ioContext Evolutionary context.
 *  \return True if there was node to select, false if no node respected all constraints.
 */
bool GP::CrossoverConstrainedOp::selectNodeToMate(unsigned int& outSelectTreeIndex,
        unsigned int& outSelectNodeIndex,
        bool inSelectABranch,
        unsigned int inPrimitSetIndex,
        unsigned int inMaxSubTreeDepth,
        unsigned int inMaxSubTreeSize,
        GP::Individual& inIndividual,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	RouletteT< std::pair<unsigned int,unsigned int> > lRoulette;
	GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	const unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();
	ioContext.emptyCallStack();
	for(unsigned int i=0; i<inIndividual.size(); ++i) {
		if(inIndividual[i]->getPrimitiveSetIndex() != inPrimitSetIndex) continue;
		ioContext.setGenotypeHandle(inIndividual[i]);
		ioContext.setGenotypeIndex(i);
		buildRoulette(lRoulette,
		              inSelectABranch,
		              inMaxSubTreeDepth,
		              inMaxSubTreeSize,
		              0,
		              *inIndividual[i],
		              ioContext);
	}
	ioContext.setGenotypeIndex(lOldTreeIndex);
	ioContext.setGenotypeHandle(lOldTreeHandle);
	if(lRoulette.size() == 0) return false;
	std::pair<unsigned int,unsigned int> lSelectedNode =
	    lRoulette.select(ioContext.getSystem().getRandomizer());
	outSelectTreeIndex = lSelectedNode.first;
	outSelectNodeIndex = lSelectedNode.second;
	return true;
	Beagle_StackTraceEndM();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Select a node for mating in the given individual, following the constraints penalties.
 *  \param outSelectTreeIndex Tree index of the selected node.
 *  \param outSelectNodeIndex Index of the selected node.
 *  \param inSelectABranch True if node to select must be a branch, false if it must a leaf.
 *  \param inNodeReturnType Desired return type for the nodes to be selected.
 *  \param inPrimitSetIndex Primitive set index to which the tree must be associated.
 *  \param inMaxSubTreeDepth Maximum sub tree depth allowed of the node to be selected.
 *  \param inMaxSubTreeSize Maximum sub tree size allowed of the node to be selected.
 *  \param inIndividual Individual to select the node from.
 *  \param ioContext Evolutionary context.
 *  \return True if there was node to select, false if no node respected all constraints.
 */
bool GP::CrossoverConstrainedOp::selectNodeToMateWithType(unsigned int& outSelectTreeIndex,
        unsigned int& outSelectNodeIndex,
        bool inSelectABranch,
        const std::type_info* inNodeReturnType,
        unsigned int inPrimitSetIndex,
        unsigned int inMaxSubTreeDepth,
        unsigned int inMaxSubTreeSize,
        GP::Individual& inIndividual,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	RouletteT< std::pair<unsigned int,unsigned int> > lRoulette;
	GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	const unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();
	ioContext.emptyCallStack();
	for(unsigned int i=0; i<inIndividual.size(); ++i) {
		if(inIndividual[i]->getPrimitiveSetIndex() != inPrimitSetIndex) continue;
		ioContext.setGenotypeHandle(inIndividual[i]);
		ioContext.setGenotypeIndex(i);
		buildRouletteWithType(lRoulette,
		                      inSelectABranch,
		                      inNodeReturnType,
		                      inMaxSubTreeDepth,
		                      inMaxSubTreeSize,
		                      0,
		                      *inIndividual[i],
		                      ioContext);
	}
	ioContext.setGenotypeIndex(lOldTreeIndex);
	ioContext.setGenotypeHandle(lOldTreeHandle);
	if(lRoulette.size() == 0) return false;
	std::pair<unsigned int,unsigned int> lSelectedNode =
	    lRoulette.select(ioContext.getSystem().getRandomizer());
	outSelectTreeIndex = lSelectedNode.first;
	outSelectNodeIndex = lSelectedNode.second;
	return true;
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI
