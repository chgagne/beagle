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
 *  \file   beagle/src/StatsCalcFitnessMultiObjOp.cpp
 *  \brief  Source code of class StatsCalcFitnessMultiObjOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
StatsCalcFitnessMultiObjOp::StatsCalcFitnessMultiObjOp(std::string inName) :
		StatsCalculateOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void StatsCalcFitnessMultiObjOp::calculateStatsDeme(Stats& outStats,
        Deme& ioDeme,
        Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	outStats.clear();
	outStats.clearItems();
	outStats.addItem("processed", ioContext.getProcessedDeme());
	outStats.addItem("total-processed", ioContext.getTotalProcessedDeme());

	if(ioDeme.size() == 0) {
		outStats.clear();
		outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 0, true);
		return;
	}

	outStats.clear();
	outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
	                             ioContext.getGeneration(), ioDeme.size(), true);

	const unsigned int lNbObjectives = ioDeme[0]->getFitness()->getNumberOfObjectives();
	for(unsigned int j=0; j<lNbObjectives; ++j) {
		const double lFirstIndObjJ = ioDeme[0]->getFitness()->getObjective(j);
		if(ioDeme.size() == 1) {
			outStats.push_back(Measure(std::string("objective")+uint2str(j),
			                           lFirstIndObjJ, 0.0, lFirstIndObjJ, lFirstIndObjJ));
		} else {
			double lSum = lFirstIndObjJ;
			double lPow2Sum = pow2Of<double>(lFirstIndObjJ);
			double lMax = lFirstIndObjJ;
			double lMin = lFirstIndObjJ;
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
			int lSize = ioDeme.size();
			#pragma omp parallel 
			{
				double lMaxThread = lMax;
				double lMinThread = lMin;
				#pragma omp for reduction(+:lSum, lPow2Sum)
				for(int i=1; i < lSize; ++i) {
					Beagle_AssertM(lNbObjectives == ioDeme[i]->getFitness()->getNumberOfObjectives());
					const double lIndivObjJ = ioDeme[i]->getFitness()->getObjective(j);
					lSum     += lIndivObjJ;
					lPow2Sum += pow2Of<double>(lIndivObjJ);
					lMaxThread =  maxOf<double>(lMaxThread,lIndivObjJ);
					lMinThread =  minOf<double>(lMinThread,lIndivObjJ);
				}
				#pragma omp critical (Beagle_StatsCalcFitnessMultiObjOp_Max)
				lMax = maxOf<double>(lMax, lMaxThread);
				#pragma omp critical (Beagle_StatsCalcFitnessMultiObjOp_Min)
				lMin = minOf<double>(lMin, lMinThread);
			}
#else
			for(unsigned int i=1; i<ioDeme.size(); i++) {
				Beagle_AssertM(lNbObjectives == ioDeme[i]->getFitness()->getNumberOfObjectives());
				const double lIndivObjJ = ioDeme[i]->getFitness()->getObjective(j);
				lSum     += lIndivObjJ;
				lPow2Sum += pow2Of<double>(lIndivObjJ);
				lMax     =  maxOf<double>(lMax,lIndivObjJ);
				lMin     =  minOf<double>(lMin,lIndivObjJ);
			}
#endif
			double lAverage  = lSum / ioDeme.size();
			double lStdError = (lPow2Sum - (pow2Of<double>(lSum)/ioDeme.size())) / (ioDeme.size()-1);
			lStdError        = sqrt(lStdError);
			outStats.push_back(Measure(std::string("objective")+uint2str(j),
			                           lAverage, lStdError, lMax, lMin));
		}
	}
	Beagle_StackTraceEndM();
}
