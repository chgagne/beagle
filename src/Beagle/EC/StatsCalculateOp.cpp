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
 *  \file   Beagle/EC/StatsCalculateOp.cpp
 *  \brief  Source code of class StatsCalculateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
StatsCalculateOp::StatsCalculateOp(std::string inName) :
	Beagle::Operator(inName),
	mPopSize(NULL),
	mNbDemesCalculated(0),
	mGenerationCalculated(0)
{ }


/*!
 *  \brief  Calculate statistics of a given vivarium.
 *  \param  outStats Computed stats of the deme.
 *  \param  ioVivarium Vivarium to evaluate the statistics.
 *  \param  ioContext Context of the evolution.
 */
void StatsCalculateOp::calculateStatsVivarium(Stats& outStats,
        Vivarium& ioVivarium,
        Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	outStats.clear();
	outStats.clearItems();

	unsigned int lPopSize = 0;
	for(unsigned int i=0; i<ioVivarium.size(); ++i) {
		Beagle::Stats::Handle lDemeStats = ioVivarium[i]->getStats();
		Beagle_NonNullPointerAssertM(lDemeStats);
		lPopSize += lDemeStats->getPopSize();
	}
	outStats.setGenerationValues("vivarium", ioContext.getGeneration(), lPopSize, true);
	outStats.addItem("processed", ioContext.getProcessedVivarium());
	outStats.addItem("total-processed", ioContext.getTotalProcessedVivarium());

	if(ioVivarium.size() == 0) return;

	outStats.resize(ioVivarium[0]->getStats()->size());
	for(unsigned int i=0; i<outStats.size(); ++i) {
		double lSumXi     = 0.0;
		double lSumXiPow2 = 0.0;
		double lMax       = (*ioVivarium[0]->getStats())[i].mMax;
		double lMin       = (*ioVivarium[0]->getStats())[i].mMin;

		for(unsigned int j=0; j<ioVivarium.size(); ++j) {
			Beagle_AssertM(outStats.size() == ioVivarium[j]->getStats()->size());
			const Measure& lMeasure  = (*ioVivarium[j]->getStats())[i];
			unsigned int   lDemeSize = ioVivarium[j]->getStats()->getPopSize();
			if(lDemeSize != 0) {
				double lSumFit  =  lMeasure.mAvg * lDemeSize;
				double lTmpVar1 =  pow2Of<double>(lSumFit) / lDemeSize;
				double lTmpVar2 =  pow2Of<double>(lMeasure.mStd) * (lDemeSize-1);
				lSumXi          += lSumFit;
				lSumXiPow2      += lTmpVar1 + lTmpVar2;
				lMax            =  maxOf<double>(lMax, lMeasure.mMax);
				lMin            =  minOf<double>(lMin, lMeasure.mMin);
			}
		}

		outStats[i].mID  = (*ioVivarium[0]->getStats())[i].mID;
		if(lPopSize == 0) {
			outStats[i].mAvg = 0.0;
			outStats[i].mStd = 0.0;
			outStats[i].mMax = 0.0;
			outStats[i].mMin = 0.0;
		} else if(lPopSize == 1) {
			outStats[i].mAvg = lSumXi;
			outStats[i].mStd = 0.0;
			outStats[i].mMax = lMax;
			outStats[i].mMin = lMin;
		} else {
			double lAverage  = lSumXi / lPopSize;
			double lVariance =
			    lSumXiPow2 - (pow2Of<double>(lAverage*((double)lPopSize)) / ((double)lPopSize));
			double lStdError = sqrt(lVariance / (lPopSize - 1));
			outStats[i].mAvg = lAverage;
			outStats[i].mStd = lStdError;
			outStats[i].mMax = lMax;
			outStats[i].mMin = lMin;
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void StatsCalculateOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

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

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Calculate the statistics of the current deme/generation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void StatsCalculateOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    string("Calculating stats for the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	if(ioContext.getGeneration() != mGenerationCalculated) {
		mGenerationCalculated = ioContext.getGeneration();
		mNbDemesCalculated = 0;
	}

	if(ioDeme.getStats() == NULL) {
		const Factory& lFactory = ioContext.getSystem().getFactory();
		Stats::Alloc::Handle lStatsAlloc =
		    castHandleT<Stats::Alloc>(lFactory.getConceptAllocator("Stats"));
		Stats::Handle lStats = castHandleT<Stats>(lStatsAlloc->allocate());
		ioDeme.addMember(lStats);
	}

	if(ioDeme.getStats()->isValid() == false) {
		calculateStatsDeme(*ioDeme.getStats(), ioDeme, ioContext);
		ioDeme.getStats()->setValid();
	}

	Beagle_LogStatsM(ioContext.getSystem().getLogger(), *ioDeme.getStats());

	if(++mNbDemesCalculated == mPopSize->size()) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "Calculating stats for the vivarium"
		);

		if(ioContext.getVivarium().getStats() == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			Stats::Alloc::Handle lStatsAlloc =
			    castHandleT<Stats::Alloc>(lFactory.getConceptAllocator("Stats"));
			Stats::Handle lStats = castHandleT<Stats>(lStatsAlloc->allocate());
			ioContext.getVivarium().addMember(lStats);
		}

		calculateStatsVivarium(*ioContext.getVivarium().getStats(),
		                       ioContext.getVivarium(),
		                       ioContext);
		ioContext.getVivarium().getStats()->setValid();

		Beagle_LogStatsM(ioContext.getSystem().getLogger(), *ioContext.getVivarium().getStats());
	}
	Beagle_StackTraceEndM();
}

