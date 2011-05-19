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
 *  \file   beagle/GP/src/CrossoverOp.cpp
 *  \brief  Source code of class GP::CrossoverOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.24 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP crossover operator.
 *  \param inMatingPbName Individual mating probability parameter name used in register.
 *  \param inDistribPbName Distribution probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GP::CrossoverOp::CrossoverOp(std::string inMatingPbName,
                             std::string inDistribPbName,
                             std::string inName) :
		Beagle::CrossoverOp(inMatingPbName, inName),
		mDistribPbName(inDistribPbName)
{ }


/*!
 *  \brief Register the parameters of the GP crossover operator.
 *  \param ioSystem System of the evolution.
 */
void GP::CrossoverOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual crossover probability",
		    "Double",
		    "0.8",
		    "Individual crossover probability at each generation."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.8f), lDescription));
	}
	Beagle::CrossoverOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Probability that a crossover point is a branch ";
		lOSS << "(node with sub-trees). Value of 1.0 means that all crossover points are ";
		lOSS << "branches, and value of 0.0 means that all crossover points are leaves.";
		Register::Description lDescription(
		    "Crossover distribution prob.",
		    "Float",
		    "0.9",
		    lOSS.str()
		);
		mDistributionProba = castHandleT<Float>(
		                         ioSystem.getRegister().insertEntry(mDistribPbName, new Float(0.9f), lDescription));
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
 *  \brief Mate two GP individuals for a crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool GP::CrossoverOp::mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
                           Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2)
{
	Beagle_StackTraceBeginM();
	// Initial parameters checks
	Beagle_AssertM(&ioIndiv1 != &ioIndiv2);
	Beagle_AssertM(ioIndiv1.size() > 0);
	Beagle_AssertM(ioIndiv1.size() == ioIndiv2.size());
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
	    "Beagle::GP::CrossoverOp",
	    "Individuals to mate (before GP crossover)"
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GP::CrossoverOp",
	    lIndiv1
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GP::CrossoverOp",
	    lIndiv2
	);

	// Crossover loop. Try the given number of attempts to mate two individuals.
	for(unsigned int lAttempt=0; lAttempt<mNumberAttempts->getWrappedValue(); lAttempt++) {

		// Choose a node in all the individual node.
		unsigned int lChoosenNode1 =
		    lContext1.getSystem().getRandomizer().rollInteger(0, lSizeIndiv1-1);

		// Get the tree in which the choosen node is. Change the global
		// node index to the tree's index.
		unsigned int lChoosenTree1 = 0;
		for(; lChoosenTree1<lIndiv1.size(); lChoosenTree1++) {
			if(lChoosenNode1 < lIndiv1[lChoosenTree1]->size()) break;
			Beagle_AssertM(lChoosenNode1 >= lIndiv1[lChoosenTree1]->size());
			lChoosenNode1 -= lIndiv1[lChoosenTree1]->size();
		}
		Beagle_AssertM(lChoosenTree1 < lIndiv1.size());

		// Choose a type of node (branch or leaf) following the
		// distribution probability and change the node for another node
		// of the same tree if the types mismatch.
		GP::Tree& lTree1 = *lIndiv1[lChoosenTree1];
		const unsigned int lPrimitiveSetIndex1 = lTree1.getPrimitiveSetIndex();
		if(lTree1.size() > 1) {
			bool lTypeNode1 =
			    (lContext1.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba);
			while((lTree1[lChoosenNode1].mPrimitive->getNumberArguments() != 0) != lTypeNode1) {
				lChoosenNode1 = lContext1.getSystem().getRandomizer().rollInteger(0, lTree1.size()-1);
			}
		}

		// Choose a node in the second individual from a tree with the same primitive set index.
		unsigned int lSizeIndiv2 = 0;
		for(unsigned int i=0; i<lIndiv2.size(); i++) {
			if(lIndiv2[i]->getPrimitiveSetIndex() == lPrimitiveSetIndex1) {
				lSizeIndiv2 += lIndiv2[i]->size();
			}
		}

		// Check to see that there is at least one node that can be selected
		if(lSizeIndiv2==0) {
			Beagle_LogVerboseM(
			    ioContext1.getSystem().getLogger(),
			    "crossover", "Beagle::GP::CrossoverConstrainedOp",
			    std::string("Crossover attempt failed:  The tree chosen from the first individual has")+
			    " a primitive set index of "+uint2str(lPrimitiveSetIndex1)+
			    " and there are no trees in the second individual with that primitive set index"
			);
			continue;
		}

		// Choose a node in the second individual
		unsigned int lChoosenNode2 = lContext2.getSystem().getRandomizer().rollInteger(0, lSizeIndiv2-1);

		// Find which tree the choosen node is in.
		unsigned int lChoosenTree2 = 0;
		for(; lChoosenTree2<lIndiv2.size(); lChoosenTree2++) {
			if(lIndiv2[lChoosenTree2]->getPrimitiveSetIndex() == lPrimitiveSetIndex1) {
				if(lChoosenNode2 < lIndiv2[lChoosenTree2]->size()) break;
				Beagle_AssertM(lChoosenNode2 >= lIndiv2[lChoosenTree2]->size());
				lChoosenNode2 -= lIndiv2[lChoosenTree2]->size();
			}
		}
		Beagle_AssertM(lChoosenTree2 < lIndiv2.size());
		GP::Tree& lTree2 = *lIndiv2[lChoosenTree2];

		// Choose a type of node (branch or leaf) following the
		// distribution probability and change the node for another node
		// of the same tree if the types mismatch.
		if(lTree2.size() > 1) {
			bool lTypeNode2 =
			    (lContext2.getSystem().getRandomizer().rollUniform(0.0, 1.0) < lDistrProba);
			while((lTree2[lChoosenNode2].mPrimitive->getNumberArguments() != 0) != lTypeNode2) {
				lChoosenNode2 = lContext2.getSystem().getRandomizer().rollInteger(0, lTree2.size()-1);
			}
		}

		// Set the first context to the node of the first tree.
		// Check if depth is ok. Do a new crossover attempt if not.
		lTree1.setContextToNode(lChoosenNode1, lContext1);
		unsigned int lNewDepthTree1 =
		    lContext1.getCallStackSize() + lTree2.getTreeDepth(lChoosenNode2) - 1;
		if(lNewDepthTree1 > lMaxTreeDepth) {
			Beagle_LogVerboseM(
			    ioContext1.getSystem().getLogger(),
			    "crossover", "Beagle::GP::CrossoverConstrainedOp",
			    std::string("Crossover attempt failed because the depth of the resulting tree in the ")+
			    std::string("first individual would exceed the depth constraint")
			);
			continue;
		}

		// Set the first context to the node of the second tree.
		// Check if depth is ok. Do a new crossover attempt if not.
		lTree2.setContextToNode(lChoosenNode2, lContext2);
		unsigned int lNewDepthTree2 =
		    lContext2.getCallStackSize() + lTree1.getTreeDepth(lChoosenNode1) - 1;
		if(lNewDepthTree2 > lMaxTreeDepth) {
			Beagle_LogVerboseM(
			    ioContext1.getSystem().getLogger(),
			    "crossover", "Beagle::GP::CrossoverConstrainedOp",
			    std::string("Crossover attempt failed because the depth of the resulting tree in the ")+
			    std::string("second individual would exceed the depth constraint")
			);
			continue;
		}

		// Mate the trees.
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GP::CrossoverOp",
		    std::string("Trying to mate the ")+uint2ordinal(lChoosenTree1+1)+
		    std::string(" tree of the first individual with the ")+uint2ordinal(lChoosenTree2+1)+
		    std::string(" tree of the second individual")
		);
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GP::CrossoverOp",
		    std::string("Trying to exchange the ")+uint2ordinal(lChoosenNode1+1)+
		    std::string(" node of the first tree with the ")+uint2ordinal(lChoosenNode2+1)+
		    std::string(" node of the second tree")
		);

		mateTrees(lTree1, lChoosenNode1, lContext1, lTree2, lChoosenNode2, lContext2);

		lMatingDone = true;
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GP::CrossoverOp",
		    "GP crossover valid"
		);
		break;   // The crossover is valid.
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
		    "Beagle::GP::CrossoverOp",
		    "Individuals mated (after GP crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",
		    "Beagle::GP::CrossoverOp",
		    lIndiv1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",
		    "Beagle::GP::CrossoverOp",
		    lIndiv2
		);
	} else {
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GP::CrossoverOp",
		    "No GP crossover done"
		);
	}

	return lMatingDone;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Mate two GP trees on given points.
 *  \param  ioTree1 First tree to mate.
 *  \param  inNode1 Node index of the croosover point in the first tree to mate.
 *  \param  ioContext1 Evolutionary context relatively to the first tree.
 *  \param  ioTree2 Second tree to mate.
 *  \param  inNode2 Node index of the croosover point in the second tree to mate.
 *  \param  ioContext2 Evolutionary context relatively to the second tree.
 */
void GP::CrossoverOp::mateTrees(GP::Tree& ioTree1, unsigned int inNode1, GP::Context& ioContext1,
                                GP::Tree& ioTree2, unsigned int inNode2, GP::Context& ioContext2)
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
 *  \brief Read a crossover operator for XML subtree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param System Evolutionary system.
 */
void GP::CrossoverOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string mMatingProbaReadName = inIter->getAttribute("matingpb");
	if(mMatingProbaReadName.empty() == false) mMatingProbaName = mMatingProbaReadName;
	std::string mDistribPbReadName = inIter->getAttribute("distrpb");
	if(mDistribPbReadName.empty() == false) mDistribPbName = mDistribPbReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write crossover operator into XML streamer.
 *  \param ioStreamer XML streamer to write crossover operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::CrossoverOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	Beagle::CrossoverOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("distrpb", mDistribPbName);
	Beagle_StackTraceEndM();
}

