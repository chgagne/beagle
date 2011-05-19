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
 *  \file   Beagle/EC/StatsCalcFitnessSimpleOp.cpp
 *  \brief  Source code of class StatsCalcFitnessSimpleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
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
StatsCalcFitnessSimpleOp::StatsCalcFitnessSimpleOp(std::string inName) :
	Beagle::EC::StatsCalculateOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void StatsCalcFitnessSimpleOp::calculateStatsDeme(Stats& outStats,
        Deme& ioDeme,
        Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	outStats.clear();
	outStats.clearItems();
	outStats.addItem("processed", ioContext.getProcessedDeme());
	outStats.addItem("total-processed", ioContext.getTotalProcessedDeme());

	if(ioDeme.size() == 0) {
		outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 0, true);
		outStats.resize(1);
		outStats.back().mID  = "fitness";
		outStats.back().mAvg = 0.0;
		outStats.back().mStd = 0.0;
		outStats.back().mMax = 0.0;
		outStats.back().mMin = 0.0;
		return;
	}

	const FitnessSimple::Handle lFirstIndivFitness =
	    castHandleT<FitnessSimple>(ioDeme[0]->getFitness());

	if(ioDeme.size() == 1) {
		outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 1, true);
		outStats.resize(1);
		outStats.back().mID  = "fitness";
		outStats.back().mAvg = lFirstIndivFitness->getValue();
		outStats.back().mStd = 0.0;
		outStats.back().mMax = lFirstIndivFitness->getValue();
		outStats.back().mMin = lFirstIndivFitness->getValue();
		return;
	}

	double lSum = (double)lFirstIndivFitness->getValue();
	double lPow2Sum = pow2Of<double>((double)lFirstIndivFitness->getValue());
	double lMax = lFirstIndivFitness->getValue();
	double lMin = lFirstIndivFitness->getValue();

#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
	int lSize = ioDeme.size();
#pragma omp parallel
	{
		double lMaxThread = lMax;
		double lMinThread = lMin;

#pragma omp for reduction(+:lSum, lPow2Sum)
		for(int i=1; i < lSize; ++i) {
			const double lIndFitnessValue  =
			    castHandleT<FitnessSimple>(ioDeme[i]->getFitness())->getValue();
			lSum     += lIndFitnessValue;
			lPow2Sum += pow2Of<double>(lIndFitnessValue);
			lMaxThread =  maxOf<double>(lMaxThread, lIndFitnessValue);
			lMinThread =  minOf<double>(lMinThread, lIndFitnessValue);
		}
#pragma omp critical (Beagle_StatsCalcFitnessSimpleOp_Max)
		lMax = maxOf<double>(lMax, lMaxThread);
#pragma omp critical (Beagle_StatsCalcFitnessSimpleOp_Min)
		lMin = minOf<double>(lMin, lMinThread);
	}
#else
	for(unsigned int i=1; i<ioDeme.size(); i++) {
		const FitnessSimple::Handle lIndivFitness =
		    castHandleT<FitnessSimple>(ioDeme[i]->getFitness());
		lSum     += (double)lIndivFitness->getValue();
		lPow2Sum += pow2Of<double>((double)lIndivFitness->getValue());
		lMax     =  maxOf<double>(lMax, (double)lIndivFitness->getValue());
		lMin     =  minOf<double>(lMin, (double)lIndivFitness->getValue());
	}
#endif
	double lAverage  = lSum / ioDeme.size();
	double lStdError = (lPow2Sum - (pow2Of<double>(lSum)/ioDeme.size())) / (ioDeme.size()-1);
	lStdError        = sqrt(lStdError);

	outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()),
	                             ioContext.getGeneration(),
	                             ioDeme.size(),
	                             true);
	outStats.resize(1);
	outStats.back().mID  = "fitness";
	outStats.back().mAvg = lAverage;
	outStats.back().mStd = lStdError;
	outStats.back().mMax = lMax;
	outStats.back().mMin = lMin;
	Beagle_StackTraceEndM();
}


