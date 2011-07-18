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
 *  \file   Beagle/Core/EvaluationOp.cpp
 *  \brief  Source code of class EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.32 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/Core.hpp"
#include <algorithm>
#include <string>
#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif //BEAGLE_HAVE_OPENMP



using namespace Beagle;


/*!
 *  \brief Construct a new evaluation operator.
 *  \param inName Name of the operator.
 */
EvaluationOp::EvaluationOp(std::string inName) :
	BreederOp(inName)
{ }


/*!
 *  \brief Apply the evaluation operation on a breeding pool, returning a evaluated bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Evaluated bred individual.
 */
Individual::Handle EvaluationOp::breed(Individual::Bag& inBreedingPool,
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

		lBredIndividual->setFitness(evaluate(*lBredIndividual, ioContext));
		lBredIndividual->getFitness()->setValid();
		History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
		if(lHistory != NULL) {
			lHistory->allocateID(*lBredIndividual);
			lHistory->trace(ioContext, std::vector<HistoryID>(), lBredIndividual, getName(), "evaluation");
		}

		ioContext.setProcessedDeme(ioContext.getProcessedDeme()+1);
		ioContext.setTotalProcessedDeme(ioContext.getTotalProcessedDeme()+1);
		ioContext.setProcessedVivarium(ioContext.getProcessedVivarium()+1);
		ioContext.setTotalProcessedVivarium(ioContext.getTotalProcessedVivarium()+1);

		Beagle_LogVerboseM(ioContext.getSystem().getLogger(), *lBredIndividual->getFitness());

		updateHallOfFameWithIndividual(*lBredIndividual, ioContext);
	}

	return lBredIndividual;
	Beagle_StackTraceEndM();
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
double EvaluationOp::getBreedingProba(BreederNode::Handle inChild)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	return inChild->getBreederOp()->getBreedingProba(inChild->getFirstChild());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the evaluation process on the invalid individuals of the deme.
 *  \param ioDeme Deme to process.
 *  \param ioContext Context of the evolution.
 */
void EvaluationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Evaluating the fitness of the individuals in the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();

	prepareStats(ioDeme,ioContext);
	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	int lNbrEvaluations = 0; // Used to count how many evaluations have been done.
	const int lSize = ioDeme.size();

#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
	static OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioContext.getSystem().getComponent("OpenMP"));
	const Factory& lFactory = ioContext.getSystem().getFactory();
	const std::string& lContextName = lFactory.getConceptTypeName("Context");
	Context::Alloc::Handle lContextAlloc = castHandleT<Context::Alloc>(lFactory.getAllocator(lContextName));
	Context::Bag lContexts(lOpenMP->getMaxNumThreads());
	for(unsigned int i = 0; i < lOpenMP->getMaxNumThreads(); ++i)
		lContexts[i] = castHandleT<Context>(lContextAlloc->clone(ioContext));
#if defined(BEAGLE_USE_OMP_NR)
#pragma omp parallel for reduction(+:lNbrEvaluations) schedule(dynamic)
#elif defined(BEAGLE_USE_OMP_R)
	const int lChunkSize = std::max((int)(lSize / lOpenMP->getMaxNumThreads()), 1);
#pragma omp parallel for reduction(+:lNbrEvaluations) schedule(static, lChunkSize)
#endif
#endif
	for(int i=0; i < lSize; ++i) {
		if((ioDeme[i]->getFitness() == NULL) ||
		        (ioDeme[i]->getFitness()->isValid() == false)) {

			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    std::string("Evaluating the fitness of the ")+uint2ordinal(i+1)+
			    " individual"
			);

#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
			lContexts[lOpenMP->getThreadNum()]->setIndividualIndex(i);
			lContexts[lOpenMP->getThreadNum()]->setIndividualHandle(ioDeme[i]);
			ioDeme[i]->setFitness(evaluate(*ioDeme[i], *lContexts[lOpenMP->getThreadNum()]));
#else
			ioContext.setIndividualIndex(i);
			ioContext.setIndividualHandle(ioDeme[i]);
			ioDeme[i]->setFitness(evaluate(*ioDeme[i], ioContext));
#endif
			ioDeme[i]->getFitness()->setValid();
			if(lHistory != NULL) {
#pragma omp critical (Beagle_History)
				{
					lHistory->allocateID(*ioDeme[i]);
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
					lHistory->trace(*lContexts[lOpenMP->getThreadNum()], std::vector<HistoryID>(), ioDeme[i], getName(), "evaluation");
#else
					lHistory->trace(ioContext, std::vector<HistoryID>(), ioDeme[i], getName(), "evaluation");
#endif
				}
			}


			++lNbrEvaluations;

			Beagle_LogVerboseM(ioContext.getSystem().getLogger(), *ioDeme[i]->getFitness());
		}
	}

	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);

	updateStats(lNbrEvaluations,ioContext);
	updateHallOfFameWithDeme(ioDeme,ioContext);

	// Warn if the operator has not evaluated anything
	if(lNbrEvaluations == 0) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    std::string("Warning!  The '")+getName()+"' operator did not evaluate any individuals.  "+
		    "Consider using the 'InvalidateFitnessOp' operator to force evaluation of every individual."
		);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Prepare statistics to be processed.
 *  \param ioDeme Deme for which statistics will be processed.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::prepareStats(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Stats::Handle lDemeStats = ioDeme.getStats();

	if(lDemeStats == NULL) {
		const Factory& lFactory = ioContext.getSystem().getFactory();
		Stats::Alloc::Handle lStatsAlloc =
		    castHandleT<Stats::Alloc>(lFactory.getConceptAllocator("Stats"));
		lDemeStats = castHandleT<Stats>(lStatsAlloc->allocate());
		ioDeme.addMember(lDemeStats);
	}

	if(lDemeStats->isValid()) {
		ioContext.setProcessedDeme(0);
		if((ioContext.getGeneration()!=0) && (ioDeme.getStats()->existItem("total-processed"))) {
			ioContext.setTotalProcessedDeme((unsigned int)ioDeme.getStats()->getItem("total-processed"));
		} else ioContext.setTotalProcessedDeme(0);
		ioDeme.getStats()->setInvalid();
	} else {
		ioContext.setProcessedDeme(0);
		ioContext.setTotalProcessedDeme(0);
	}

	if(ioContext.getDemeIndex()==0) {

		Stats::Handle lVivaStats = ioContext.getVivarium().getStats();

		if(lVivaStats == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			Stats::Alloc::Handle lStatsAlloc =
			    castHandleT<Stats::Alloc>(lFactory.getConceptAllocator("Stats"));
			lVivaStats = castHandleT<Stats>(lStatsAlloc->allocate());
			ioContext.getVivarium().addMember(lVivaStats);
		}

		if(lVivaStats->isValid()) {
			ioContext.setProcessedVivarium(0);
			if((ioContext.getGeneration()!=0) && (lVivaStats->existItem("total-processed"))) {
				ioContext.setTotalProcessedVivarium((unsigned int)lVivaStats->getItem("total-processed"));
			} else ioContext.setTotalProcessedVivarium(0);
			lVivaStats->setInvalid();
		} else {
			ioContext.setProcessedVivarium(0);
			ioContext.setTotalProcessedVivarium(0);
		}

	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void EvaluationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	BreederOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Number of individuals kept in vivarium's hall-of-fame ";
		lOSS << "(best individuals so far). Note that a hall-of-fame contains only ";
		lOSS << "copies of the best individuals so far and is not used by the evolution ";
		lOSS << "process.";
		Register::Description lDescription(
		    "Vivarium's hall-of-fame size",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mVivaHOFSize = castHandleT<UInt>(
		                   ioSystem.getRegister().insertEntry("ec.hof.vivasize", new UInt(1), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of individuals kept in each deme's hall-of-fame ";
		lOSS << "(best individuals so far). Note that a hall-of-fame contains only ";
		lOSS << "copies of the best individuals so far and is not used by the evolution ";
		lOSS << "process.";
		Register::Description lDescription(
		    "Demes' hall-of-fame size",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mDemeHOFSize = castHandleT<UInt>(
		                   ioSystem.getRegister().insertEntry("ec.hof.demesize", new UInt(0), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test the fitness of a given individual.
 *  \param inIndividual Handle to the individual to test.
 *  \param ioSystem Handle to the system to use to test the individual.
 *  \par Note:
 *    This method is provided as a mean to test some individuals after an evolution.
 */
Fitness::Handle EvaluationOp::test(Individual::Handle inIndividual, System::Handle ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogInfoM(ioSystem->getLogger(), "Testing the following individual:" << (*inIndividual));

	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioSystem->getFactory().getConceptAllocator("Context"));
	Context::Handle lContext = castHandleT<Context>(lContextAlloc->allocate());
	lContext->setSystemHandle(ioSystem);
	lContext->setIndividualHandle(inIndividual);
	Fitness::Handle lFitness = evaluate(*inIndividual, *lContext);

	Beagle_LogInfoM(ioSystem->getLogger(), "New fitness of the individual: " << *lFitness);

	return lFitness;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Update the hall of fame with current deme.
 *  \param ioDeme Deme to use to update the hall-of-fame.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::updateHallOfFameWithDeme(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
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
		Beagle_LogVerboseM(ioContext.getSystem().getLogger(), *lHoF);
	}
	if(mVivaHOFSize->getWrappedValue() > 0) {
		Beagle_LogDetailedM(ioContext.getSystem().getLogger(), "Updating the vivarium's hall-of-fame");
		HallOfFame::Handle lHoF = ioContext.getVivarium().getHallOfFame();
		if(lHoF == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const HallOfFame::Alloc::Handle lHoFAlloc =
			    castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
			lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
			ioContext.getVivarium().addMember(lHoF);
		}
		lHoF->updateWithDeme(mVivaHOFSize->getWrappedValue(), ioDeme, ioContext);
		Beagle_LogVerboseM(ioContext.getSystem().getLogger(), *lHoF);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Update the hall of fame with the current individual.
 *  \param ioIndividual Individual to update the hall-of-fame.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::updateHallOfFameWithIndividual(Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mDemeHOFSize->getWrappedValue() > 0) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Updating the deme hall-of-fame"
		);
		HallOfFame::Handle lHoF = ioContext.getDeme().getHallOfFame();
		if(lHoF == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const HallOfFame::Alloc::Handle lHoFAlloc =
			    castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
			lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
			ioContext.getDeme().addMember(lHoF);
		}
		lHoF->updateWithIndividual(mDemeHOFSize->getWrappedValue(), ioIndividual, ioContext);
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
		lHoF->updateWithIndividual(mVivaHOFSize->getWrappedValue(), ioIndividual, ioContext);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Update the stats on processed individuals
 *  \param inNumProcessed Number of processed.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::updateStats(unsigned int inNumProcessed, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	ioContext.setProcessedDeme(ioContext.getProcessedDeme()+inNumProcessed);
	ioContext.setTotalProcessedDeme(ioContext.getTotalProcessedDeme()+inNumProcessed);
	ioContext.setProcessedVivarium(ioContext.getProcessedVivarium()+inNumProcessed);
	ioContext.setTotalProcessedVivarium(ioContext.getTotalProcessedVivarium()+inNumProcessed);
	Beagle_StackTraceEndM();
}


