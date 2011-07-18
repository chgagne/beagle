/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/EvaluationMultipleOp.cpp
 *  \brief  Source code of class EvaluationMultipleOp.
 *  \author Matthew Walker
 *  $Revision: 1.25 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

#include <algorithm>
#include <string>
#include <deque>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a new multiple-individual evaluation operator.
 *  \param inIndisPerCaseAndGroup Number of individuals per test cases and group.
 *  \param inName Name of the operator.
 */
EvaluationMultipleOp::EvaluationMultipleOp(unsigned int inIndisPerCaseAndGroup,
        std::string inName) :
	EvaluationOp(inName),
	mIndisPerCase(inIndisPerCaseAndGroup),
	mIndisPerGroup(inIndisPerCaseAndGroup)
{ }


/*!
 *  \brief Construct a new multiple-individual evaluation operator.
 *  \param inIndisPerCase Number of individuals per test cases.
 *  \param inIndisPerGroup Number of individuals per group.
 *  \param inName Name of the operator.
 */
EvaluationMultipleOp::EvaluationMultipleOp(unsigned int inIndisPerCase,
        unsigned int inIndisPerGroup,
        std::string inName) :
	EvaluationOp(inName),
	mIndisPerCase(inIndisPerCase),
	mIndisPerGroup(inIndisPerGroup)
{ }


/*!
 *  \brief Apply the evaluation operation on a breeding pool, returning a evaluated bred individual.
 *  \return Evaluated bred individual.
 */
Individual::Handle EvaluationMultipleOp::breed(Individual::Bag&,
        BreederNode::Handle,
        Context&)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("EvaluationMultipleOp", "breed", getName());
	return Individual::Handle(NULL);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add individuals to the bag such that the total equals getIndisPerGroup().
 *  \param ioIndividuals Bag of individuals to evaluate.
 *  \param ioContexts Bag of evolutionary context.
 *  \return The number of individuals added to the bag.
 *
 *  The new individuals are chosen from the current deme.
 */
unsigned int EvaluationMultipleOp::enlargeGroup(Individual::Bag& ioIndividuals,
        Context::Bag& ioContexts)
{
	Beagle_StackTraceBeginM();
	Context& lContext = castObjectT<Context&>(*(ioContexts[0]));

	// Calculate the number of individuals to add
	unsigned int lNumToAdd = mIndisPerGroup - ioIndividuals.size();
	Beagle_LogVerboseM(
	    lContext.getSystem().getLogger(),
	    std::string("Adding ")+uint2str(lNumToAdd)+std::string(" individuals to the group (for padding)")
	);

	// Resize the bags
	unsigned int lIndisCounter = ioIndividuals.size();
	ioIndividuals.resize(mIndisPerGroup);
	ioContexts.resize(mIndisPerGroup);

	// Loop through all the individuals in the deme
	Deme& lDeme = lContext.getDeme();
	std::vector<unsigned int> lSelectableIndis;
	lSelectableIndis.resize(lDeme.size());
	unsigned int lSelectableIndisCounter = 0;
	for (unsigned int i=0; i<lDeme.size(); i++) {
		// Loop through all the individuals in the bag
		bool lAdd = true;
		for(unsigned int j=0; j<ioIndividuals.size(); j++) {
			if(lDeme[i] == ioIndividuals[j]) {
				lAdd = false;
				break;
			}
		}
		// If the individual is not already in the bag, add it as an option
		if(lAdd) {
			lSelectableIndis[lSelectableIndisCounter] = i;
			lSelectableIndisCounter++;
		}
	}

	// Check there are sufficient individuals to choose
	if(lSelectableIndis.size() < lNumToAdd) {
		throw Beagle_RunTimeExceptionM("There are insufficient individuals in the deme to perform evaluation");
	}

	// Add individuals
	for(unsigned int i=0; i<lNumToAdd; i++) {
		unsigned int lIndex =
		    lContext.getSystem().getRandomizer().rollInteger(0,lSelectableIndisCounter-1);
		unsigned int lIndiIndex = lSelectableIndis[lIndex];
		Beagle_LogVerboseM(
		    lContext.getSystem().getLogger(),
		    std::string("Adding ")+uint2ordinal(lIndiIndex+1)+
		    std::string(" individual to the group (for padding)")
		);
		Beagle_AssertM(lIndiIndex < lDeme.size());
		ioIndividuals[lIndisCounter] = lDeme[ lIndiIndex ];
		Context::Alloc::Handle lContextAlloc =
		    castHandleT<Context::Alloc>(lContext.getSystem().getFactory().getConceptAllocator("Context"));
		ioContexts[lIndisCounter] = castHandleT<Context>(lContextAlloc->clone(*(ioContexts[0])));
		ioContexts[lIndisCounter]->setIndividualHandle( ioIndividuals[lIndisCounter] );
		ioContexts[lIndisCounter]->setIndividualIndex( lIndiIndex );
		lIndisCounter++;
	}
	Beagle_AssertM( lIndisCounter==ioIndividuals.size() );

	return lNumToAdd;
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 *
 *  This method evaluates only a single individual.  For this
 *  operator, it is much better to group individuals into a bag and
 *  pass the bag to evaluate(Individual::Bag& ioIndividuals, Context::Bag& ioContexts).
 */
Fitness::Handle EvaluationMultipleOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Create a bag for the individual
	Individual::Bag lIndividuals;
	lIndividuals.resize(1);
	Beagle_AssertM(inIndividual == ioContext.getIndividual());
	lIndividuals[0] = ioContext.getIndividualHandle();

	// Create a context for the individual
	Context::Bag lContexts;
	lContexts.resize(1);
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioContext.getSystem().getFactory().getConceptAllocator("Context"));
	lContexts[0] = castHandleT<Context>(lContextAlloc->clone(ioContext));

	// Call evalutate()
	Fitness::Bag::Handle lFitnessBag = evaluateIndividuals(lIndividuals,lContexts);

	// Return fitness
	Beagle_AssertM( !lFitnessBag->empty() );
	return lFitnessBag->at(0);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Evaluate the fitness of the given bag of individuals.
 *  \param ioIndividuals Bag of individuals to evaluate.
 *  \param ioContexts Bag of evolutionary context.
 *  \return Handle to a bag of fitness values, one for each individual.
 */
Fitness::Bag::Handle EvaluationMultipleOp::evaluateIndividuals(Individual::Bag& ioIndividuals,
        Context::Bag& ioContexts)
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(ioIndividuals.size()==ioContexts.size());
	Beagle_AssertM(ioIndividuals.size()!=0);

	Context& lContext = castObjectT<Context&>(*(ioContexts[0]));

	// Check if sufficient individuals are in the bag
	Beagle_AssertM( ioIndividuals.size() <= mIndisPerGroup );
	unsigned int lNumToIgnore = 0;
	if (ioIndividuals.size() != mIndisPerGroup) {
		lNumToIgnore = enlargeGroup(ioIndividuals, ioContexts);
	}

	// Create bag of null-fitnesses
	Fitness::Bag::Handle lFitnessBagAll = new Fitness::Bag;
	lFitnessBagAll->resize( ioIndividuals.size() );
	Beagle_NonNullPointerAssertM( lFitnessBagAll );

	// Set up cases
	if (mCases == NULL) setupCases(ioIndividuals.size(),lContext);
	Case::Bag::Handle lCases = pruneIgnorableCases(lNumToIgnore);

	// Call evaluateCase for each case
	for (unsigned int i=0; i<lCases->size(); i++) {
		Beagle_LogVerboseM(
		    lContext.getSystem().getLogger(),
		    std::string("Evaluating the ")+uint2ordinal(i+1)+std::string(" case")
		);
		Case& lCase = *(lCases->at(i));
		Beagle_AssertM( lCase.mIndices.size() == mIndisPerCase );

		// Setup bags of individuals and contexts
		Individual::Bag lIndividuals;
		Context::Bag lContexts;
		lIndividuals.resize( mIndisPerCase );
		lContexts.resize( mIndisPerCase );
		for (unsigned int j=0; j<mIndisPerCase; j++) {
			unsigned int lIndex = lCase.mIndices[j];
			lIndividuals[j] = ioIndividuals[ lIndex ];
			lContexts[j] = ioContexts[ lIndex ];
		}

		// Log individual's details
		std::ostringstream lOSS;
		for (unsigned int j=0; j<lIndividuals.size(); j++) {
			if (j!=0) lOSS << ", ";
			lOSS << uint2ordinal(lContexts[j]->getIndividualIndex()+1);
		}
		Beagle_LogDebugM(
		    lContext.getSystem().getLogger(),
		    uint2ordinal(i+1) << " case: " << lOSS.str()
		);

		// Call evalutateCase()
		Fitness::Bag::Handle lFitnessBagCase =
		    evaluateCase(lIndividuals, lContexts);

		// Log resulting fitnesses
		Beagle_NonNullPointerAssertM( lFitnessBagCase );
		Beagle_LogDebugM(
		    lContext.getSystem().getLogger(),
		    "Evaluation of the case is complete. The fitnesses are as follows:"
		);
		for (unsigned int j=0; j<mIndisPerCase; j++) {
			Beagle_NonNullPointerAssertM( lFitnessBagCase->at(j) );
			Beagle_LogDebugM(
			    lContext.getSystem().getLogger(),
			    std::string("Fitness of the ")+
			    uint2ordinal(ioContexts[lCase.mIndices[j]]->getIndividualIndex()+1)+
			    std::string(" individual")
			);
			Beagle_LogDebugM(lContext.getSystem().getLogger(), *(*lFitnessBagCase)[j]);

			// Need to assign fitness values from case to lFitnessBagAll
			unsigned int lIndex = lCase.mIndices[j];
			Beagle_LogDebugM(
			    lContext.getSystem().getLogger(),
			    "Setting fitness for lFitnessBagAll at index " << uint2str(lIndex)
			);
			Beagle_AssertM(lIndex < lFitnessBagAll->size());
			if (lFitnessBagAll->at(lIndex)==NULL) {
				(*lFitnessBagAll)[lIndex] = lFitnessBagCase->at(j);
			} else {
				combineFitnesses(lFitnessBagAll->at(lIndex),
				                 lFitnessBagCase->at(j) );
				Beagle_LogDebugM(
				    lContext.getSystem().getLogger(),
				    "Fitness of the " << uint2ordinal(ioContexts[lIndex]->getIndividualIndex()+1) <<
				    " individual has been combined to"
				);
				Beagle_LogDebugM(lContext.getSystem().getLogger(), *(*lFitnessBagAll)[lIndex]);
			}
		}
	}

	Beagle_LogDebugM(
	    lContext.getSystem().getLogger(),
	    "Evaluation of all cases is complete. Fitnesses are as follows:"
	);
	for (unsigned int i=0;
	        i<ioIndividuals.size();
	        i++) {
		if (i>=mIndisPerGroup-lNumToIgnore) {
			// Nullify ignorable individuals' fitness scores
			Beagle_LogDebugM(
			    lContext.getSystem().getLogger(),
			    "Ignoring fitness of the " << uint2ordinal(ioContexts[i]->getIndividualIndex()+1) << " individual"
			);
			(*lFitnessBagAll)[i] = NULL;
			continue;
		}
		Beagle_NonNullPointerAssertM( lFitnessBagAll->at(i) );
		Beagle_LogDebugM(
		    lContext.getSystem().getLogger(),
		    "Fitness of the " << uint2ordinal(ioContexts[i]->getIndividualIndex()+1) << " individual"
		);
		Beagle_LogDebugM(lContext.getSystem().getLogger(), *(*lFitnessBagAll)[i]);
	}

	return lFitnessBagAll;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the evaluation process on the invalid individuals of the deme.
 *  \param ioDeme Deme to process.
 *  \param ioContext Context of the evolution.
 */
void EvaluationMultipleOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "Evaluating the fitness of the individuals in the " << 
	    uint2ordinal(ioContext.getDemeIndex()+1) << " deme"
	);

	Beagle_AssertM( ioDeme.size()!=0 );

	// Prepare stats
	prepareStats(ioDeme,ioContext);

	// Generate a vector of indicies into the population
	std::vector<unsigned int> lEvalVector;
	for(unsigned int i=0; i<ioDeme.size(); i++) {
		if((ioDeme[i]->getFitness() == NULL) ||
		        (ioDeme[i]->getFitness()->isValid() == false)) {
			lEvalVector.push_back(i);
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "Added " << uint2ordinal(i+1) << " individual for evaluation."
			);
		}
	}
	std::random_shuffle(lEvalVector.begin(), lEvalVector.end(),
	                    ioContext.getSystem().getRandomizer());

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "There are " << lEvalVector.size() << " individuals to be evaluated."
	);

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	while ( !lEvalVector.empty() ) {
		// Put individuals and context into bags.
		Individual::Bag lIndividuals;
		Context::Bag lContexts;
		lIndividuals.resize( mIndisPerGroup );
		lContexts.resize( mIndisPerGroup );
		unsigned int lIndiCounter =0;

		for (unsigned int i=0; i<mIndisPerGroup; i++) {
			// Set individual
			lIndividuals[i] = ioDeme[lEvalVector.back()];
			lIndiCounter++;
			// Set context
			Context::Alloc::Handle lContextAlloc =
			    castHandleT<Context::Alloc>(ioContext.getSystem().getFactory().getConceptAllocator("Context"));
			Context::Handle lContext = castHandleT<Context>(lContextAlloc->clone(ioContext));
			lContext->setIndividualIndex( lEvalVector.back() );
			lContext->setIndividualHandle( ioDeme[lEvalVector.back()] );
			lContexts[i] = lContext;
			// Remove this index from the evaluation vector
			lEvalVector.pop_back();
			if(lEvalVector.empty()) {
				lIndividuals.resize( lIndiCounter );
				lContexts.resize( lIndiCounter );
				break;
			}
		}

		// Evaluate individuals
		std::ostringstream lOSS;
		lOSS << "Evaluating the fitness of the ";
		for(unsigned int i=0; i<lIndiCounter; i++) {
			// Add to message
			if (i==lIndiCounter-1) lOSS << " and ";
			lOSS << uint2ordinal(lContexts[i]->getIndividualIndex()+1);
			if (i<lIndiCounter-2) lOSS << ", ";
		}
		lOSS << " individuals";
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    lOSS.str()
		);
		Fitness::Bag::Handle lFitnessBag = evaluateIndividuals(lIndividuals, lContexts);

		// Assign fitnesses
		for (unsigned int i=0; i<lIndiCounter; i++) {
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "Considering fitness of the " << uint2ordinal(lContexts[i]->getIndividualIndex()+1) << " individual"
			);
			Beagle_AssertM( i < lFitnessBag->size() );
			Fitness::Handle lFitness = lFitnessBag->at(i);
			Beagle_NonNullPointerAssertM( lFitness );
			lIndividuals[i]->setFitness( lFitness );
			lIndividuals[i]->getFitness()->setValid();
			if(lHistory != NULL) {
				lHistory->allocateID(*lIndividuals[i]);
				lHistory->trace(ioContext, std::vector<HistoryID>(), lIndividuals[i], getName(), "evaluation");
			}

			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    *lIndividuals[i]->getFitness()
			);
		}

		// Update stats
		updateStats(lIndividuals.size(),ioContext);
	}

	updateHallOfFameWithDeme(ioDeme,ioContext);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Removes cases that contain only references to individuals
 *    that are acting a padding.
 *  \param inNumToIgnore Number of individuals that have been added as padding.
 */
EvaluationMultipleOp::Case::Bag::Handle
EvaluationMultipleOp::pruneIgnorableCases(unsigned int inNumToIgnore)
{
	Beagle_StackTraceBeginM();
	if (inNumToIgnore==0) return mCases;

	Case::Bag::Handle lCases = new Case::Bag;
	lCases->resize( mCases->size() );
	unsigned int lCaseCounter = 0;

	// Check every case
	for (unsigned int i=0; i<mCases->size(); i++) {
		// Check if any of the indices are not able to be ignored
		Case& lCase = *(mCases->at(i));
		bool lAdd = false;
		for (unsigned int j=0; j<lCase.mIndices.size(); j++) {
			if (lCase.mIndices[j] < mIndisPerGroup-inNumToIgnore) {
				lAdd=true;
				break;
			}
		}
		// If it cannot be ignored, add it to the bag of "un-ignorables"
		if (lAdd) {
			(*lCases)[lCaseCounter] = (*mCases)[i];
			lCaseCounter++;
		}
	}

	lCases->resize(lCaseCounter);
	return lCases;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Sets up cases.
 *  \param inSize Number of individuals in a group.
 *  \param ioContext Evolutionary context.
 *
 *  A case is a vector of indices into a bag of individuals.  This
 *  method creates cases.  The number of cases is decided by inSize.
 *
 *  For example, if inSize is 4 (set by setIndisPerGroup), then the
 *  indices will range from 0 to 3.  If each case contains just two
 *  individuals (as set by setIndisPerCase) then the cases will be:
 *  (0,1) (0,2) (0,3)
 *  (1,2) (1,3)
 *  (2,3)
 *  Where (x,y) dictates a case when individual x will compete against
 *  indiviudal y.
 */
void EvaluationMultipleOp::setupCases(unsigned int inSize, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), "Creating evaluation cases");
	Beagle_AssertM(inSize >= mIndisPerCase);
	Beagle_AssertM(inSize == mIndisPerGroup);

	// Create cases with each case containing one individual
	mCases = new Case::Bag;
	Beagle_NonNullPointerAssertM( mCases );
	for (unsigned int i=0; i<inSize; i++) {
		Case::Handle lCase = new Case;
		lCase->mIndices.push_back( i );
		setupCaseRecursive(inSize,i,lCase);
	}

	// Log cases
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    mCases->size() << " evaluation cases were created"
	);
	for (unsigned int i=0; i<mCases->size(); i++) {
		std::ostringstream lOSS;
		for (unsigned int j=0; j<mCases->at(i)->mIndices.size(); j++) {
			if (j!=0) lOSS << ", ";
			lOSS << mCases->at(i)->mIndices[j];
		}
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    uint2ordinal(i+1) << " case: " << lOSS.str()
		);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Sets up cases by adding one more individual into a clone of
 *    inCase, and then calling this method for each one of those cases.
 *  \param inSize Number of individuals in a group.
 *  \param inLastIndex The index of the last individual that was
 *    inserted into the case.
 *  \param inCase The case on which to work.
 */
void EvaluationMultipleOp::setupCaseRecursive(unsigned int inSize,
        unsigned int inLastIndex,
        Case::Handle inCase)
{
	Beagle_StackTraceBeginM();
	unsigned int lCaseSize = inCase->mIndices.size();

	// Check if the case is finished.
	if (lCaseSize == mIndisPerCase) {
		unsigned int lSize = mCases->size();
		mCases->resize( lSize + 1 );
		(*mCases)[lSize]=inCase;
		return;
	}

	// Case isn't finished, add another individual to it.
	Beagle_AssertM( lCaseSize<mIndisPerCase );
	for (unsigned int i=inLastIndex+1;
	        i<inSize;
	        i++) {
		Case::Handle lNewCase = new Case(*(inCase));
		lNewCase->mIndices.push_back(i);
		setupCaseRecursive(inSize,i,lNewCase);
	}
	Beagle_StackTraceEndM();
}
