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
 *  \file   beagle/GP/src/StatsCalcFitnessSimpleOp.cpp
 *  \brief  Source code of class GP::StatsCalcFitnessSimpleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
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
GP::StatsCalcFitnessSimpleOp::StatsCalcFitnessSimpleOp(std::string inName) :
		Beagle::StatsCalcFitnessSimpleOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given GP deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void GP::StatsCalcFitnessSimpleOp::calculateStatsDeme(Beagle::Stats& outStats,
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
		outStats.resize(3);
		outStats[0].mID  = "fitness";
		outStats[0].mAvg = 0.0;
		outStats[0].mStd = 0.0;
		outStats[0].mMax = 0.0;
		outStats[0].mMin = 0.0;

		outStats[1].mID = "treedepth";
		outStats[1].mAvg = 0.0;
		outStats[1].mStd = 0.0;
		outStats[1].mMax = 0.0;
		outStats[1].mMin = 0.0;

		outStats[2].mID = "treesize";
		outStats[2].mAvg = 0.0;
		outStats[2].mStd = 0.0;
		outStats[2].mMax = 0.0;
		outStats[2].mMin = 0.0;
		return;
	}

	const GP::Individual::Handle lFirstIndiv =
	    castHandleT<GP::Individual>(ioDeme[0]);
	const FitnessSimple::Handle lFirstIndivFitness =
	    castHandleT<FitnessSimple>(lFirstIndiv->getFitness());

	if(ioDeme.size() == 1) {
		outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 1, true);
		outStats.resize(3);
		outStats[0].mID  = "fitness";
		outStats[0].mAvg = lFirstIndivFitness->getValue();
		outStats[0].mStd = 0.0;
		outStats[0].mMax = lFirstIndivFitness->getValue();
		outStats[0].mMin = lFirstIndivFitness->getValue();

		outStats[1].mID = "treedepth";
		outStats[1].mAvg = lFirstIndiv->getMaxTreeDepth();
		outStats[1].mStd = 0.0;
		outStats[1].mMax = outStats[1].mAvg;
		outStats[1].mMin = outStats[1].mAvg;

		outStats[2].mID = "treesize";
		outStats[2].mAvg = lFirstIndiv->getTotalNodes();
		outStats[2].mStd = 0.0;
		outStats[2].mMax = outStats[2].mAvg;
		outStats[2].mMin = outStats[2].mAvg;
		return;
	}

	double lSum = (double)lFirstIndivFitness->getValue();
	double lPow2Sum = pow2Of<double>((double)lFirstIndivFitness->getValue());
	double lMax = lFirstIndivFitness->getValue();
	double lMin = lFirstIndivFitness->getValue();

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
		const FitnessSimple::Handle lIndivFitness =
		    castHandleT<FitnessSimple>(lIndiv->getFitness());

		lSum     += (double)lIndivFitness->getValue();
		lPow2Sum += pow2Of<double>((double)lIndivFitness->getValue());
		lMax     =  maxOf<double>(lMax, (double)lIndivFitness->getValue());
		lMin     =  minOf<double>(lMin, (double)lIndivFitness->getValue());

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

	double lAverage  = lSum / ioDeme.size();
	double lStdError = (lPow2Sum - (pow2Of<double>(lSum)/ioDeme.size())) / (ioDeme.size()-1);
	lStdError        = sqrt(lStdError);

	float lDepthAverage  = (float)(lSumDepth / ioDeme.size());
	float lDepthStdError =
	    (float)(lPow2SumDepth - (pow2Of<double>(lSumDepth) / ioDeme.size())) / (ioDeme.size() - 1);
	lDepthStdError       = sqrt(lDepthStdError);

	float lSizeAverage  = (float)(lSumSize / ioDeme.size());
	float lSizeStdError =
	    (float)(lPow2SumSize - (pow2Of<double>(lSumSize) / ioDeme.size())) / (ioDeme.size() - 1);
	lSizeStdError       = sqrt(lSizeStdError);

	outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
	                             ioContext.getGeneration(),
	                             ioDeme.size(),
	                             true);
	outStats.resize(3);
	outStats[0].mID  = "fitness";
	outStats[0].mAvg = lAverage;
	outStats[0].mStd = lStdError;
	outStats[0].mMax = lMax;
	outStats[0].mMin = lMin;

	outStats[1].mID = "treedepth";
	outStats[1].mAvg = lDepthAverage;
	outStats[1].mStd = lDepthStdError;
	outStats[1].mMax = (float)lMaxDepth;
	outStats[1].mMin = (float)lMinDepth;

	outStats[2].mID = "treesize";
	outStats[2].mAvg = lSizeAverage;
	outStats[2].mStd = lSizeStdError;
	outStats[2].mMax = (float)lMaxSize;
	outStats[2].mMin = (float)lMinSize;
	Beagle_StackTraceEndM();
}
