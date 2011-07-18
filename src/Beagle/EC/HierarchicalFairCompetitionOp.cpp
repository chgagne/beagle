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
 *  \file   Beagle/EC/HierarchicalFairCompetitionOp.cpp
 *  \brief  Source code of class Beagle::EC::HierarchicalFairCompetitionOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Build generational operator.
 *  \param inName Name of the generational operator.
 */
HierarchicalFairCompetitionOp::HierarchicalFairCompetitionOp(std::string inName) :
	ReplacementStrategyOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void HierarchicalFairCompetitionOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	ReplacementStrategyOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Percentile of fitness measure to use as HFC migration threshold ";
		lOSS << "of next deme. For example, a threshold of 0.85 means that the fitness ";
		lOSS << "used as threshold to accept migrant into following deme is taken as the ";
		lOSS << "fitness of the individual that is better than 85% of the other individuals ";
		lOSS << "in its deme.";
		Register::Description lDescription(
		    "HFC percentile",
		    "Float",
		    "0.85",
		    lOSS.str()
		);
		mHFCPercentile = castHandleT<Float>(
		                     ioSystem.getRegister().insertEntry("ec.hfc.percentile", new Float(0.85f), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Interval between each hierarchical fair competition ";
		lOSS << "migration, in number of generations. ";
		lOSS << "An interval of 0 disables HFC migrations.";
		Register::Description lDescription(
		    "Interval between HFC migrations",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mMigrationInterval = castHandleT<UInt>(
		                         ioSystem.getRegister().insertEntry("ec.hfc.interval", new UInt(1), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of individuals migrating between each deme, at a each ";
		lOSS << "hierarchical fair competition migration.";
		Register::Description lDescription(
		    "Size of each HFC migration",
		    "UInt",
		    "5",
		    lOSS.str()
		);
		mNumberMigrants = castHandleT<UInt>(
		                      ioSystem.getRegister().insertEntry("ec.hfc.size", new UInt(5), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of demes and size of each deme of the population. ";
		lOSS << "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lOSS << "The size of the UIntArray is the number of demes present in the ";
		lOSS << "vivarium, while each value of the vector is the size of the corresponding ";
		lOSS << "deme.";
		Register::Description lDescription(
		    "Vivarium and demes sizes",
		    "UIntArray",
		    "100",
		    lOSS.str()
		);
		mPopSize = castHandleT<UIntArray>(
		               ioSystem.getRegister().insertEntry("ec.pop.size", new UIntArray(1,100), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the operation on a deme in the given context.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void HierarchicalFairCompetitionOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Verify fitness threshold values.
	Beagle_AssertM(mPopSize->size()!=0);
	if(ioContext.getGeneration()==0) mFitnessThresholds.clear();
	if(mFitnessThresholds.size()!=(mPopSize->size()-1)) {
		mFitnessThresholds.resize(mPopSize->size()-1);
	}

	// Situations where HFC migration should not be applied.
	if(ioDeme.size() == 0) return;
	if(mPopSize->size() < 2) return;
	if(mMigrationInterval->getWrappedValue() == 0) return;
	if((ioContext.getGeneration() % mMigrationInterval->getWrappedValue()) != 0) return;

	// Log some information.
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "Applying HFC migration to the " << uint2ordinal(ioContext.getDemeIndex()) << " deme"
	);

	// If the deme is changed.
	bool lChanged = false;

	// Update fitness threshold.
	if(ioContext.getDemeIndex() != 0) {
		Beagle_AssertM(mHFCPercentile->getWrappedValue() < 1.0);
		const unsigned int lThresholdIndex =
		    (unsigned int)std::ceil((1.0-mHFCPercentile->getWrappedValue()) * float(ioDeme.size()-1));
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		for(unsigned int i=0; i<lThresholdIndex; ++i) {
			std::pop_heap(ioDeme.begin(), ioDeme.end()-i, IsLessPointerPredicate());
		}
		const Factory& lFactory = ioContext.getSystem().getFactory();
		std::string lFitnessType = ioDeme[0]->getFitness()->getType();
		Fitness::Alloc::Handle lFitnessAlloc =
		    castHandleT<Fitness::Alloc>(lFactory.getAllocator(lFitnessType));
		mFitnessThresholds[ioContext.getDemeIndex()-1] =
		    castHandleT<Fitness>(lFitnessAlloc->clone(*ioDeme[0]->getFitness()));
	}

	// Insert migrating individuals from previous deme.
	if(ioContext.getDemeIndex() != 0) {

		MigrationBuffer::Handle lMigBuffer =
		    castHandleT<MigrationBuffer>(ioDeme.getMember("MigrationBuffer"));
		if(lMigBuffer == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			MigrationBuffer::Alloc::Handle lMigBufferAlloc =
			    castHandleT<MigrationBuffer::Alloc>(lFactory.getConceptAllocator("MigrationBuffer"));
			lMigBuffer = castHandleT<MigrationBuffer>(lMigBufferAlloc->allocate());
			ioDeme.addMember(lMigBuffer);
		}

		const unsigned int lNbImmigrants = lMigBuffer->getNumberOfImmigrants();
		if(lNbImmigrants > 0) {
			std::vector<unsigned int> lReplacedIndiv;
			for(unsigned int i=0; i<lNbImmigrants; ++i) {
				lReplacedIndiv.push_back(ioDeme.size() + i);
			}
			ioDeme.resize(ioDeme.size() + lNbImmigrants);
			lMigBuffer->insertReplaced(lReplacedIndiv, ioDeme);
			lChanged = true;
		}
	}

	// Migrating individual out of this deme.
	if((ioContext.getDemeIndex() != (mPopSize->size()-1)) &&
	        (mFitnessThresholds[ioContext.getDemeIndex()] != NULL)) {

		MigrationBuffer::Handle lMigBuffer =
		    castHandleT<MigrationBuffer>(ioDeme.getMember("MigrationBuffer"));
		if(lMigBuffer == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			MigrationBuffer::Alloc::Handle lMigBufferAlloc =
			    castHandleT<MigrationBuffer::Alloc>(lFactory.getConceptAllocator("MigrationBuffer"));
			lMigBuffer = castHandleT<MigrationBuffer>(lMigBufferAlloc->allocate());
			ioDeme.addMember(lMigBuffer);
		}

		Fitness::Handle lThreshold = mFitnessThresholds[ioContext.getDemeIndex()];
		std::vector<unsigned int> lEmigrants;
		for(unsigned int i=0; i<ioDeme.size(); ++i) {
			if(ioDeme[i]->getFitness()->isLess(*lThreshold) == false) lEmigrants.push_back(i);
		}
		if(lEmigrants.size() > 0) {
			lChanged = true;
			lMigBuffer->insertEmigrants(lEmigrants, ioDeme, ioContext);
			Deme& lDestDeme = *ioContext.getVivarium()[ioContext.getDemeIndex()+1];
			lMigBuffer->moveMigrants(lEmigrants.size(), lDestDeme, ioContext);
			std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
			while(ioDeme.size() > 0) {
				if(ioDeme[0]->getFitness()->isLess(*lThreshold)) break;
				std::pop_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
				ioDeme.pop_back();
			}
		}
	}

	// Fill the population with randomly generated individuals, if the population is too small.
	if(ioDeme.size() < (*mPopSize)[ioContext.getDemeIndex()]) {
		const unsigned int lNbNewInd = (*mPopSize)[ioContext.getDemeIndex()] - ioDeme.size();
		Individual::Bag lNewIndividuals = generateIndividuals(lNbNewInd, ioDeme, ioContext);
		ioDeme.insert(ioDeme.end(), lNewIndividuals.begin(), lNewIndividuals.end());
		lChanged = true;
	}

	// Delete worse individuals if the population is too big.
	if(ioDeme.size() > (*mPopSize)[ioContext.getDemeIndex()]) {
		const unsigned int lNbDeletedInd = ioDeme.size() - (*mPopSize)[ioContext.getDemeIndex()];
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
		for(unsigned int i=0; i<lNbDeletedInd; ++i) {
			std::pop_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *ioDeme.back());
			ioDeme.pop_back();
		}
		lChanged = true;
	}

	// If changed, reset statistics.
	if(lChanged) {
		if(ioDeme.getStats() != NULL) ioDeme.getStats()->setInvalid();
		if(ioContext.getVivarium().getStats() != NULL) ioContext.getVivarium().getStats()->setInvalid();
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Generate a new individuals from the HFC breeder tree.
 *  \param inN Number of new individuals generated.
 *  \param ioDeme Evolved deme used to generate new individuals.
 *  \param ioContext Evolutionary context.
 *  \return New generated individuals from breeder tree.
 */
Individual::Bag
HierarchicalFairCompetitionOp::generateIndividuals(unsigned int inN,
        Deme& ioDeme, Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(getRootNode());
	Beagle_LogTraceM(
		ioContext.getSystem().getLogger(),
		"Processing using HFC replacement strategy the" <<
		uint2ordinal(ioContext.getDemeIndex()+1) << " deme"
	);
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), (*this));

	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);
	Individual::Bag lNewIndividuals;
	for(unsigned int i=0; i<inN; ++i) {
		unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
		BreederNode::Handle lSelectedBreeder=getRootNode();
		for(unsigned int j=0; j<lIndexBreeder; ++j)
			lSelectedBreeder=lSelectedBreeder->getNextSibling();
		Beagle_NonNullPointerAssertM(lSelectedBreeder);
		Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
		Individual::Handle lBredIndiv =
		    lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
		lNewIndividuals.push_back(lBredIndiv);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lBredIndiv);
	}
	return lNewIndividuals;
	Beagle_StackTraceEndM();
}


