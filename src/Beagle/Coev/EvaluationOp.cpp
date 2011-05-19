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
 *  \file   beagle/Coev/src/EvaluationOp.cpp
 *  \brief  Source code of class Coev::EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.23 $
 *  $Date: 2007/08/17 18:09:09 $
 */

#include "beagle/Coev.hpp"

#include <string>

using namespace Beagle;


/*
 *  Initialize static evaluation operator evaluation condition.
 */
PACC::Threading::Condition Coev::EvaluationOp::smCondition;

/*
 *  Initialize static evaluation operator sets stack.
 */
Coev::EvaluationOp::EvalSetVector Coev::EvaluationOp::smEvalSets;

/*
 *  Initialize static evaluation operator trigger.
 */
unsigned int Coev::EvaluationOp::smTrigger = 0;


/*!
 *  \brief Construct an evaluation set.
 */
Coev::EvaluationOp::EvalSet::EvalSet() :
		mID(0)
{ }


/*!
 *  \brief Construct an evaluation set.
 *  \param inIndividuals Individual bag of the evaluation set.
 *  \param inContext Evolutionary context.
 *  \param inID ID of the evaluation set, for convenience purpuse, not used internally.
 */
Coev::EvaluationOp::EvalSet::EvalSet(Individual::Bag& inIndividuals,
                                     Context::Handle inContext,
                                     unsigned int inID) :
		mIndividuals(inIndividuals),
		mContext(inContext),
		mID(inID)
{ }


/*!
 *  \brief Construct a co-evolutionary evaluation operator.
 *  \param inTrigger Number of sets to accumulate before triggering evaluation procedure.
 *  \param inName Name of the co-evolutionary evaluation operator.
 */
Coev::EvaluationOp::EvaluationOp(unsigned int inTrigger, std::string inName) :
		Beagle::EvaluationOp(inName)
{
	Beagle_StackTraceBeginM();
	smCondition.lock();
	if(smTrigger == 0) smTrigger = inTrigger;
	else if(inTrigger != smTrigger) {
		std::ostringstream lOSS;
		lOSS << "trigger value given as argument to constructor of Coev::EvaluationOp (";
		lOSS << inTrigger << ") is different from the actual non-zero value of the trigger (";
		lOSS << smTrigger << ")!";
		smCondition.unlock();
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	smCondition.unlock();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add evaluation set into shared structure.
 *  \param inEvalSet Evaluation set to add.
 *  \param inBlocking If true, the add set operation block until fitness evaluation is done.
 */
void Coev::EvaluationOp::addSet(EvalSet& inEvalSet, bool inBlocking)
{
	Beagle_StackTraceBeginM();
	smCondition.lock();
	if(smTrigger == 0) {
		smCondition.unlock();
		throw Beagle_RunTimeExceptionM("co-evolution trigger value is zero!");
	}
	if(smEvalSets.size() >= smTrigger) {
		std::ostringstream lOSS;
		lOSS << "number of evaluation sets in co-evolution evaluation operator (";
		lOSS << smEvalSets.size() << ") is equal or bigger than the trigger value (";
		lOSS << smTrigger << ")!";
		smCondition.unlock();
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	// Add evaluation set
	smEvalSets.push_back(inEvalSet);
	// If all needed evaluation sets are added
	if(smEvalSets.size() == smTrigger) {
		evaluateSets(smEvalSets);
		smEvalSets.clear();
		smCondition.broadcast();
	}
	// Othewize, wait to get all needed evaluation sets
	else if(inBlocking) {
		smCondition.wait();
	}
	smCondition.unlock();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set fitness value of an individual.
 *  \param inFitness New fitness value of the individual.
 *  \param ioIndividual Individual which fitness value is set.
 *  \param ioContext Evolutionary context.
 */
void Coev::EvaluationOp::assignFitness(Fitness::Handle inFitness,
                                       Individual& ioIndividual,
                                       Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	ioIndividual.setFitness(inFitness);
	inFitness->setValid();
	ioContext.setProcessedDeme(ioContext.getProcessedDeme()+1);
	ioContext.setTotalProcessedDeme(ioContext.getTotalProcessedDeme()+1);
	ioContext.setProcessedVivarium(ioContext.getProcessedVivarium()+1);
	ioContext.setTotalProcessedVivarium(ioContext.getTotalProcessedVivarium()+1);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the evaluation operation on a breeding pool, returning a evaluated bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Evaluated bred individual.
 */
Individual::Handle Coev::EvaluationOp::breed(Individual::Bag& inBreedingPool,
        BreederNode::Handle inChild,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);

	Deme& lDeme = *ioContext.getDemeHandle();
	if((lDeme.getStats()==NULL) || lDeme.getStats()->isValid()) {
		prepareStats(lDeme, ioContext);
	}

	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	Individual::Handle lBredIndividual =
	    inChild->getBreederOp()->breed(inBreedingPool, inChild->getFirstChild(), ioContext);

	if((lBredIndividual->getFitness()==NULL) || (lBredIndividual->getFitness()->isValid()==false)) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Evaluating the fitness of a new bred individual"
		);

		Individual::Bag lNonEvalIndiv;
		lNonEvalIndiv.push_back(lBredIndividual);
		Context::Handle lContext = &ioContext;
		makeSets(lNonEvalIndiv, lContext);

		Beagle_LogVerboseM(ioContext.getSystem().getLogger(), *lBredIndividual->getFitness());

		if(mDemeHOFSize->getWrappedValue() > 0) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "Updating the deme hall-of-fame"
			);
			HallOfFame::Handle lHoF = lDeme.getHallOfFame();
			if(lHoF == NULL) {
				const Factory& lFactory = ioContext.getSystem().getFactory();
				const HallOfFame::Alloc::Handle lHoFAlloc =
					castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
				lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
				lDeme.addMember(lHoF);
			}
			lHoF->updateWithIndividual(mDemeHOFSize->getWrappedValue(), *lBredIndividual, ioContext);
		}
		if(mVivaHOFSize->getWrappedValue() > 0) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "Updating the vivarium hall-of-fame"
			);
			HallOfFame::Handle lHoF = ioContext.getVivarium().getHallOfFame();
			if(lHoF == NULL) {
				const Factory& lFactory = ioContext.getSystem().getFactory();
				const HallOfFame::Alloc::Handle lHoFAlloc =
					castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
				lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
				ioContext.getVivarium().addMember(lHoF);
			}
			lHoF->updateWithIndividual(mVivaHOFSize->getWrappedValue(), *lBredIndividual, ioContext);
		}
	}

	return lBredIndividual;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Evaluating an individual in co-evolution is not that simple. Define makeSets and
 *     evaluateSets methods instead.
 */
Fitness::Handle Coev::EvaluationOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("evaluate","Coev::EvaluationOp",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply co-evolutionary evaluation operation on the deme.
 *  \param ioDeme Deme to evaluate fitness.
 *  \param ioContext Evolutionary context.
 */
void Coev::EvaluationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Evaluating the individuals fitness of the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme in co-evolution mode"
	);

	prepareStats(ioDeme, ioContext);

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Calling co-evolution evaluation hook for the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	Context::Handle lContext = &ioContext;
	makeSets(ioDeme, lContext);

	if(mDemeHOFSize->getWrappedValue() > 0) {
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "Updating the deme's hall-of-fame"
		);
		HallOfFame::Handle lHoF = ioDeme.getHallOfFame();
		if(lHoF == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const HallOfFame::Alloc::Handle lHoFAlloc =
				castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
			lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
			ioDeme.addMember(lHoF);
		}
		lHoF->updateWithDeme(mDemeHOFSize->getWrappedValue(), ioDeme, ioContext);
		lHoF->log(Logger::eVerbose, ioContext);
	}

	if(mVivaHOFSize->getWrappedValue() > 0) {
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "Updating the vivarium's hall-of-fame"
		);
		HallOfFame::Handle lHoF = ioContext.getVivarium().getHallOfFame();
		if(lHoF == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const HallOfFame::Alloc::Handle lHoFAlloc =
				castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
			lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
			ioContext.getVivarium().addMember(lHoF);
		}
		lHoF->updateWithDeme(mVivaHOFSize->getWrappedValue(), ioDeme, ioContext);
		lHoF->log(Logger::eVerbose, ioContext);
	}
	Beagle_StackTraceEndM();
}





