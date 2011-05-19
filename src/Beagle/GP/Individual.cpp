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
 *  \file   beagle/GP/src/Individual.cpp
 *  \brief  Source code of class GP::Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.28 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"
#include "beagle/RouletteT.hpp"

using namespace Beagle;


/*!
 *  \brief Contruct a GP individual.
 *  \param inN Initial number of trees in vivarium.
 *  \param inModel Initial value of trees handles.
 */
GP::Individual::Individual(unsigned int inN, GP::Tree::Handle inModel) :
		Beagle::Individual(inN,inModel)
{ }


/*!
 *  \brief Randomly select a tree from the individual.
 *  \return Randomly selected tree
 *
 *  The selection is biased towards trees with a greater number of nodes
 *  (i.e. each node has an equal probability of being selected).
 */
unsigned int GP::Individual::chooseRandomTree(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	unsigned int lChosenNode = ioContext.getSystem().getRandomizer().rollInteger(0, getTotalNodes()-1);
	unsigned int lChosenTree = 0;
	for(; (lChosenTree+1)<size(); lChosenTree++) {
		if(lChosenNode < (*this)[lChosenTree]->size()) break;
		else lChosenNode -= (*this)[lChosenTree]->size();
	}
	return lChosenTree;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Randomly select a node from a specific tree in the individual.
 *  \return Randomly selected tree
 *
 *  Each node has an equal probability of being selected.
 */
unsigned int GP::Individual::chooseRandomNode(unsigned int inTree, GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(operator[](inTree)->size()!=0);
	return ioContext.getSystem().getRandomizer().rollInteger(0, operator[](inTree)->size()-1);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Randomly select a node that takes arguments from a specific
 *    tree in the individual.
 *  \return Randomly selected tree
 */
unsigned int GP::Individual::chooseRandomNodeWithArgs(unsigned int inTree,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	const GP::Tree& lTree = *(operator[](inTree));
	unsigned int lSize = lTree.size();
	if (lSize < 2) {
		std::ostringstream lOSS;
		lOSS << "In GP::Individual::chooseRandomNodeWithArgs(): ";
		lOSS << "Could not choose a node with arguments because the specified tree has fewer ";
		lOSS << "than two (" << lSize << ") nodes, hence there are no such nodes";
		lOSS << " in the tree. This occurred while calling chooseRandomNodeWithArgs() with an ";
		lOSS << "inTree value of " << inTree;
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}

	// Loop through the tree adding appropriate nodes into the roulette
	RouletteT<unsigned int> lRoulette;
	for (unsigned int i=0; i<lSize; i++) {
		if(lTree[i].mSubTreeSize > 1) lRoulette.insert(i);
	}

	// Select node with roulette
	Beagle_AssertM(!lRoulette.empty());
	return lRoulette.select(ioContext.getSystem().getRandomizer());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Randomly select a node that takes no argument from a specific
 *    tree in the individual.
 *  \return Randomly selected tree
 */
unsigned int GP::Individual::chooseRandomNodeWithoutArgs(unsigned int inTree,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	// Loop through the tree adding appropriate nodes into the roulette
	unsigned int lSize = operator[](inTree)->size();
	RouletteT<unsigned int> lRoulette;
	for (unsigned int i=0; i<lSize; i++) {
		if (operator[](inTree)->operator[](i).mSubTreeSize == 1) {
			lRoulette.insert(i);
		}
	}
	// Select node with roulette
	Beagle_AssertM(!lRoulette.empty());
	return lRoulette.select(ioContext.getSystem().getRandomizer());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Turn a GP individual into a C function
 */
std::string GP::Individual::deparse(GP::Context& ioContext,
                                    const std::string& inFunctionNamePrefix) const
{
	Beagle_StackTraceBeginM();
	// Create strings for each tree
	unsigned int lNumTrees = size();
	Beagle_AssertM(lNumTrees>0);
	std::vector<std::string> lTrees;
	lTrees.resize(lNumTrees);

	// Get text for each tree
	for (unsigned int i=0; i<lNumTrees; i++) {
		lTrees[i] = (*this)[i]->deparse();
	}

	// Store complete text in one string
	std::ostringstream lResult;

	// Create functions from non-main trees
	for (unsigned int i=lNumTrees-1; i>0; i--) {
		// Write main
		lResult << inFunctionNamePrefix << "_adf_" << i << "(void) {\n"
		<< "  return " << lTrees[i] << ";\n"
		<< "}\n\n";
	}

	// Write main
	lResult << inFunctionNamePrefix << "_main(void) {\n"
	<< "  return " <<lTrees[0] << ";\n"
	<< "}\n\n";

	return lResult.str();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the individual.
 *  \return Exact type of the individual.
 */
const std::string& GP::Individual::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("GP-Individual");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Interpret the GP individual.
 *  \param outResult Datum containing the result of the interpretation.
 *  \param ioContext GP evolutionary context.
 *  \throw Beagle::ObjectException When individual or tree are empty.
 *  \throw Beagle::AssertException When context is not correctly set.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
void GP::Individual::run(GP::Datum& outResult, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(&ioContext.getIndividual() != this) {
		std::ostringstream lOSS;
		lOSS << "In GP::Individual::run():  For the given context, ";
		lOSS << "getIndividual() did not return the same individual as was passed to this (run) ";
		lOSS << "method. Consider setting the context's individual to be the same by using the ";
		lOSS << "method Context::setIndividualHandle().";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	if(empty()) throw Beagle_ObjectExceptionM("Could not interpret, individual has no trees!");
	if((*this)[0]->empty()) throw Beagle_ObjectExceptionM("Could not interpret, 1st tree is empty!");
	Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	unsigned int lOldTreeIndex  = ioContext.getGenotypeIndex();
	ioContext.setGenotypeIndex(0);
	ioContext.setGenotypeHandle((*this)[0]);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "individual", "Beagle::GP::Individual",
	    std::string("Running the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
	    std::string(" individual")
	);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "individual", "Beagle::GP::Individual",
	    std::string("The individual is: ")
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "individual", "Beagle::GP::Individual",
	    (*this)
	);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "individual", "Beagle::GP::Individual",
	    std::string("Executing the first tree root node '")+
	    (*(*this)[0])[0].mPrimitive->getName()+"'"
	);
	ioContext.setNodesExecutionCount(0);
	ioContext.incrementNodesExecuted();
	ioContext.getExecutionTimer().reset();
	ioContext.pushCallStack(0);
	(*(*this)[0])[0].mPrimitive->execute(outResult, ioContext);
	ioContext.popCallStack();
	ioContext.checkExecutionTime();
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "individual", "Beagle::GP::Individual",
	    std::string("Result of executing the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
	    std::string(" individual: ")+outResult.serialize()
	);
	ioContext.setGenotypeIndex(lOldTreeIndex);
	ioContext.setGenotypeHandle(lOldTreeHandle);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Validate the GP individual.
 *  \param ioContext Evolutionary context.
 *  \return True if the GP individual is valid, false if not.
 */
bool GP::Individual::validate(GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	bool lResult = true;

	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    "individual", "Beagle::GP::Individual",
	    std::string("Validating ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
	    std::string(" individual")
	);

	// Store original values.
	GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();

	// Loop through each of the trees in the individual
	for (unsigned int i=0; i<size(); i++) {
		GP::Tree::Handle lTree = (*this)[i];
		if (lTree == NULL) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "individual", "Beagle::GP::Individual",
			    std::string("Skipping ")+uint2ordinal(i+1)+std::string(" tree because it's NULL-valued")
			);
			continue;
		}

		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "individual", "Beagle::GP::Individual",
		    std::string("Validating ")+uint2ordinal(i+1)+std::string(" tree")
		);

		// Store the new values
		ioContext.setGenotypeHandle(lTree);
		ioContext.setGenotypeIndex(i);

		lTree->setContextToNode(0, ioContext);
		if(!lTree->validateSubTree(0, ioContext)) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "individual", "Beagle::GP::Individual",
			    std::string("Validation of ")+uint2ordinal(i+1)+std::string(" tree failed.")
			);
			lResult = false;
			break;
		}
	}

	if(lResult) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "individual", "Beagle::GP::Individual",
		    std::string("Individual passed validation testing.")
		);
	}

	// Restore the original values.
	ioContext.setGenotypeHandle(lOldTreeHandle);
	ioContext.setGenotypeIndex(lOldTreeIndex);

	return lResult;
	Beagle_StackTraceEndM();
}
