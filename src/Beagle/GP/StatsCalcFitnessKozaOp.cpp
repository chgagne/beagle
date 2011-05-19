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
 *  \file   beagle/GP/src/StatsCalcFitnessKozaOp.cpp
 *  \brief  Source code of class StatsCalcFitnessKozaOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/17 20:57:59 $
 */

#include "beagle/GP.hpp"

#include <cmath>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
GP::StatsCalcFitnessKozaOp::StatsCalcFitnessKozaOp(std::string inName) :
		StatsCalculateOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void GP::StatsCalcFitnessKozaOp::calculateStatsDeme(Beagle::Stats& outStats,
        Beagle::Deme& ioDeme,
        Beagle::Context& ioContext) const
{
	Beagle_StackTraceBeginM();

	outStats.clear();
	outStats.clearItems();
	outStats.addItem("processed", ioContext.getProcessedDeme());
	outStats.addItem("total-processed", ioContext.getTotalProcessedDeme());

	if(ioDeme.size() == 0) {
		outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 0, true);

		outStats.resize(7);
		outStats[0].mID = "normalized";
		outStats[0].mAvg = 0.0;
		outStats[0].mStd = 0.0;
		outStats[0].mMax = 0.0;
		outStats[0].mMin = 0.0;

		outStats[1].mID = "adjusted";
		outStats[1].mAvg = 0.0;
		outStats[1].mStd = 0.0;
		outStats[1].mMax = 0.0;
		outStats[1].mMin = 0.0;

		outStats[2].mID = "standardized";
		outStats[2].mAvg = 0.0;
		outStats[2].mStd = 0.0;
		outStats[2].mMax = 0.0;
		outStats[2].mMin = 0.0;

		outStats[3].mID = "raw";
		outStats[3].mAvg = 0.0;
		outStats[3].mStd = 0.0;
		outStats[3].mMax = 0.0;
		outStats[3].mMin = 0.0;

		outStats[4].mID = "hits";
		outStats[4].mAvg = 0.0;
		outStats[4].mStd = 0.0;
		outStats[4].mMax = 0.0;
		outStats[4].mMin = 0.0;

		outStats[5].mID = "treedepth";
		outStats[5].mAvg = 0.0;
		outStats[5].mStd = 0.0;
		outStats[5].mMax = 0.0;
		outStats[5].mMin = 0.0;

		outStats[6].mID = "treesize";
		outStats[6].mAvg = 0.0;
		outStats[6].mStd = 0.0;
		outStats[6].mMax = 0.0;
		outStats[6].mMin = 0.0;
		return;
	}

	const GP::Individual::Handle lFirstIndiv =
	    castHandleT<GP::Individual>(ioDeme[0]);
	const GP::FitnessKoza::Handle lFirstIndivFitness =
	    castHandleT<GP::FitnessKoza>(lFirstIndiv->getFitness());

	if(ioDeme.size() == 1) {
		outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 1, true);

		outStats.resize(7);
		outStats[0].mID = "normalized";
		outStats[0].mAvg = lFirstIndivFitness->getNormalizedFitness();
		outStats[0].mStd = 0.0;
		outStats[0].mMax = lFirstIndivFitness->getNormalizedFitness();
		outStats[0].mMin = lFirstIndivFitness->getNormalizedFitness();

		outStats[1].mID = "adjusted";
		outStats[1].mAvg = lFirstIndivFitness->getAdjustedFitness();
		outStats[1].mStd = 0.0;
		outStats[1].mMax = lFirstIndivFitness->getAdjustedFitness();
		outStats[1].mMin = lFirstIndivFitness->getAdjustedFitness();

		outStats[2].mID = "standardized";
		outStats[2].mAvg = lFirstIndivFitness->getStandardizedFitness();
		outStats[2].mStd = 0.0;
		outStats[2].mMax = lFirstIndivFitness->getStandardizedFitness();
		outStats[2].mMin = lFirstIndivFitness->getStandardizedFitness();

		outStats[3].mID = "raw";
		outStats[3].mAvg = lFirstIndivFitness->getRawFitness();
		outStats[3].mStd = 0.0;
		outStats[3].mMax = lFirstIndivFitness->getRawFitness();
		outStats[3].mMin = lFirstIndivFitness->getRawFitness();

		outStats[4].mID = "hits";
		outStats[4].mAvg = lFirstIndivFitness->getHits();
		outStats[4].mStd = 0.0;
		outStats[4].mMax = lFirstIndivFitness->getHits();
		outStats[4].mMin = lFirstIndivFitness->getHits();

		outStats[5].mID = "treedepth";
		outStats[5].mAvg = lFirstIndiv->getMaxTreeDepth();
		outStats[5].mStd = 0.0;
		outStats[5].mMax = outStats[5].mAvg;
		outStats[5].mMin = outStats[5].mAvg;

		outStats[6].mID = "treesize";
		outStats[6].mAvg = lFirstIndiv->getTotalNodes();
		outStats[6].mStd = 0.0;
		outStats[6].mMax = outStats[6].mAvg;
		outStats[6].mMin = outStats[6].mAvg;
		return;
	}

	double lSumNrm         = (double)lFirstIndivFitness->getNormalizedFitness();
	double lPow2SumNrm     = pow2Of<double>(lSumNrm);
	float  lMaxNrm         = lFirstIndivFitness->getNormalizedFitness();
	float  lMinNrm         = lFirstIndivFitness->getNormalizedFitness();
	double lSumAdj         = (double)lFirstIndivFitness->getAdjustedFitness();
	double lPow2SumAdj     = pow2Of<double>(lSumAdj);
	float  lMaxAdj         = lFirstIndivFitness->getAdjustedFitness();
	float  lMinAdj         = lFirstIndivFitness->getAdjustedFitness();
	double lSumStd         = (double)lFirstIndivFitness->getStandardizedFitness();
	double lPow2SumStd     = pow2Of<double>(lSumStd);
	float  lMaxStd         = (double)lFirstIndivFitness->getStandardizedFitness();
	float  lMinStd         = lFirstIndivFitness->getStandardizedFitness();
	double lSumRaw         = lFirstIndivFitness->getRawFitness();
	double lPow2SumRaw     = pow2Of<double>(lSumRaw);
	float  lMaxRaw         = (double)lFirstIndivFitness->getRawFitness();
	float  lMinRaw         = lFirstIndivFitness->getRawFitness();
	double lSumHit         = lFirstIndivFitness->getHits();
	double lPow2SumHit     = pow2Of<double>(lSumHit);
	unsigned int lMaxHit   = lFirstIndivFitness->getHits();
	unsigned int lMinHit   = lFirstIndivFitness->getHits();

	unsigned int lMaxDepth = lFirstIndiv->getMaxTreeDepth();
	unsigned int lMinDepth = lMaxDepth;
	double lSumDepth       = (double)lMaxDepth;
	double lPow2SumDepth   = pow2Of<double>(lSumDepth);
	unsigned int lMaxSize  = lFirstIndiv->getTotalNodes();
	unsigned int lMinSize  = lMaxSize;
	double lSumSize        = (double)lMaxSize;
	double lPow2SumSize    = pow2Of<double>(lSumSize);

	for(unsigned int i=1; i<ioDeme.size(); i++) {
		const GP::Individual::Handle lIndiv =
		    castHandleT<GP::Individual>(ioDeme[i]);
		const GP::FitnessKoza::Handle lIndivFitness =
		    castHandleT<GP::FitnessKoza>(lFirstIndiv->getFitness());
		lSumNrm     += (double)lIndivFitness->getNormalizedFitness();
		lPow2SumNrm += pow2Of<double>((double)lIndivFitness->getNormalizedFitness());
		lSumAdj     += (double)lIndivFitness->getAdjustedFitness();
		lPow2SumAdj += pow2Of<double>((double)lIndivFitness->getAdjustedFitness());
		lSumStd     += (double)lIndivFitness->getStandardizedFitness();
		lPow2SumStd += pow2Of<double>(lIndivFitness->getStandardizedFitness());
		lSumRaw     += (double)lIndivFitness->getRawFitness();
		lPow2SumRaw += pow2Of<double>((double)lIndivFitness->getRawFitness());
		lSumHit     += (double)lIndivFitness->getHits();
		lPow2SumHit += pow2Of<double>((double)lIndivFitness->getHits());
		if(lIndivFitness->getNormalizedFitness() > lMaxNrm) {
			lMaxNrm = lIndivFitness->getNormalizedFitness();
			lMaxAdj = lIndivFitness->getAdjustedFitness();
			lMaxStd = lIndivFitness->getStandardizedFitness();
			lMaxRaw = lIndivFitness->getRawFitness();
			lMaxHit = lIndivFitness->getHits();
		}
		if(lIndivFitness->getNormalizedFitness() < lMinNrm) {
			lMinNrm = lIndivFitness->getNormalizedFitness();
			lMinAdj = lIndivFitness->getAdjustedFitness();
			lMinStd = lIndivFitness->getStandardizedFitness();
			lMinRaw = lIndivFitness->getRawFitness();
			lMinHit = lIndivFitness->getHits();
		}

		unsigned int lTmpDepth = lIndiv->getMaxTreeDepth();
		lSumDepth     += (double)lTmpDepth;
		lPow2SumDepth += pow2Of<double>((double)lTmpDepth);
		lMaxDepth     =  maxOf(lMaxDepth, lTmpDepth);
		lMinDepth     =  minOf(lMinDepth, lTmpDepth);

		unsigned int lTmpSize = lIndiv->getTotalNodes();
		lSumSize     += (double)lTmpSize;
		lPow2SumSize += pow2Of<double>((double)lTmpSize);
		lMaxSize     =  maxOf(lMaxSize, lTmpSize);
		lMinSize     =  minOf(lMinSize, lTmpSize);
	}

	float lNrmAverage  = (float)(lSumNrm / ioDeme.size());
	float lNrmStdError =
	    (float)(lPow2SumNrm - (pow2Of<double>(lSumNrm) / ioDeme.size())) / (ioDeme.size() - 1);
	lNrmStdError       = sqrt(lNrmStdError);

	float lAdjAverage  = (float)(lSumAdj / ioDeme.size());
	float lAdjStdError =
	    (float)(lPow2SumAdj - (pow2Of<double>(lSumAdj) / ioDeme.size())) / (ioDeme.size() - 1);
	lAdjStdError       = sqrt(lAdjStdError);

	float lStdAverage  = (float)(lSumStd / ioDeme.size());
	float lStdStdError =
	    (float)(lPow2SumStd - (pow2Of<double>(lSumStd) / ioDeme.size())) / (ioDeme.size() - 1);
	lStdStdError       = sqrt(lStdStdError);

	float lRawAverage  = (float)(lSumRaw / ioDeme.size());
	float lRawStdError =
	    (float)(lPow2SumRaw - (pow2Of<double>(lSumRaw) / ioDeme.size())) / (ioDeme.size() - 1);
	lRawStdError       = sqrt(lRawStdError);

	float lHitAverage  = (float)(lSumHit / ioDeme.size());
	float lHitStdError =
	    (float)(lPow2SumHit - (pow2Of<double>(lSumHit) / ioDeme.size())) / (ioDeme.size() - 1);
	lHitStdError       = sqrt(lHitStdError);

	float lDepthAverage  = (float)(lSumDepth / ioDeme.size());
	float lDepthStdError =
	    (float)(lPow2SumDepth - (pow2Of<double>(lSumDepth) / ioDeme.size())) / (ioDeme.size() - 1);
	lDepthStdError       = sqrt(lDepthStdError);

	float lSizeAverage  = (float)(lSumSize / ioDeme.size());
	float lSizeStdError =
	    (float)(lPow2SumSize - (pow2Of<double>(lSumSize) / ioDeme.size())) / (ioDeme.size() - 1);
	lSizeStdError       = sqrt(lSizeStdError);

	outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
	                             ioContext.getGeneration(), ioDeme.size(), true);

	outStats.resize(7);
	outStats[0].mID = "normalized";
	outStats[0].mAvg = lNrmAverage;
	outStats[0].mStd = lNrmStdError;
	outStats[0].mMax = lMaxNrm;
	outStats[0].mMin = lMinNrm;

	outStats[1].mID = "adjusted";
	outStats[1].mAvg = lAdjAverage;
	outStats[1].mStd = lAdjStdError;
	outStats[1].mMax = lMaxAdj;
	outStats[1].mMin = lMinAdj;

	outStats[2].mID = "standardized";
	outStats[2].mAvg = lStdAverage;
	outStats[2].mStd = lStdStdError;
	outStats[2].mMax = lMaxStd;
	outStats[2].mMin = lMinStd;

	outStats[3].mID = "raw";
	outStats[3].mAvg = lRawAverage;
	outStats[3].mStd = lRawStdError;
	outStats[3].mMax = lMaxRaw;
	outStats[3].mMin = lMinRaw;

	outStats[4].mID = "hits";
	outStats[4].mAvg = lHitAverage;
	outStats[4].mStd = lHitStdError;
	outStats[4].mMax = (float)lMaxHit;
	outStats[4].mMin = (float)lMinHit;

	outStats[5].mID = "treedepth";
	outStats[5].mAvg = lDepthAverage;
	outStats[5].mStd = lDepthStdError;
	outStats[5].mMax = (float)lMaxDepth;
	outStats[5].mMin = (float)lMinDepth;

	outStats[6].mID = "treesize";
	outStats[6].mAvg = lSizeAverage;
	outStats[6].mStd = lSizeStdError;
	outStats[6].mMax = (float)lMaxSize;
	outStats[6].mMin = (float)lMinSize;
	
	Beagle_StackTraceEndM();
}


