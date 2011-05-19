/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  \file   beagle/GA/src/MuWCommaLambdaCMAFltVecOp.cpp
 *  \brief  Source code of class MuWCommaLambdaCMAFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.29 $
 *  $Date: 2007/08/17 20:57:59 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>
#include <cmath>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Build CMA-ES (Mu_W,Lambda) replacement strategy operator.
 *  \param inLMRatioName Lamda over Mu parameter name used in the register.
 *  \param inName Name of the CMA-ES (Mu_W,Lambda) operator.
 */
GA::MuWCommaLambdaCMAFltVecOp::MuWCommaLambdaCMAFltVecOp(std::string inLMRatioName,
        std::string inName) :
		MuCommaLambdaOp(inLMRatioName,inName)
{ }


/*!
 *  \brief Generate children from the breeder tree.
 *  \param ioDeme Deme to generate children from.
 *  \param ioContext Evolutionary context.
 *  \param lNbChildren Number of children to generate.
 *  \param inN Dimensionality of the problem.
 *  \param ioCMAValues CMA values to use to generate new individual.
 *  \param inSelectionWeights Selection weights used to generate children.
 */
void GA::MuWCommaLambdaCMAFltVecOp::generateChildren(Deme& ioDeme,
        Context& ioContext,
        unsigned int inNbChildren,
        unsigned int inN,
        GA::CMAValues& ioCMAValues,
        const Vector& inSelectionWeights) const
{
	Beagle_StackTraceBeginM();
	// Check parameters and log some information
	Beagle_NonNullPointerAssertM(mElitismKeepSize);
	Beagle_ValidateParameterM(mLMRatio->getWrappedValue() >= 1.0,
	                          mLMRatioName,
	                          "The LM ratio must be higher or equal to 1.0.");
	Beagle_ValidateParameterM(mElitismKeepSize->getWrappedValue() <= ioDeme.size(),
	                          "ec.elite.keepsize",
	                          "The elistism keepsize must be less than the deme size!");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Using CMA-ES (mu_w,lambda) replacement strategy to process the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), (*this));
	const Factory& lFactory = ioContext.getSystem().getFactory();

	// Create weighted mean individual.
	std::sort(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());

	Individual::Alloc::Handle lIndividualAlloc =
		castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
	Individual::Handle lMeanInd = castHandleT<Individual>(lIndividualAlloc->allocate());
	Genotype::Alloc::Handle lGenotypeAlloc =
		castHandleT<Genotype::Alloc>(lFactory.getConceptAllocator("Genotype"));
	GA::FloatVector::Handle lMeanFloatVec =
		castHandleT<GA::FloatVector>(lGenotypeAlloc->allocate());
	lMeanFloatVec->resize(inN);
	lMeanInd->push_back(lMeanFloatVec);
	
	for(unsigned int i=0; i<inN; ++i) (*lMeanFloatVec)[i] = 0.0;
	if(ioDeme.size()==1) {
		Beagle_AssertM(ioDeme[0]->size() == 1);
		GA::FloatVector::Handle lInd = castHandleT<GA::FloatVector>((*ioDeme[0])[0]);
		(*lMeanFloatVec) = *lInd;
	} else {
		for(unsigned int i=0; i<ioDeme.size(); ++i) {
			Beagle_AssertM(ioDeme[i]->size()==1);
			GA::FloatVector::Handle lVecI = castHandleT<GA::FloatVector>((*ioDeme[i])[0]);
			Beagle_AssertM(lVecI->size()==inN);
			for(unsigned int j=0; j<inN; ++j) (*lMeanFloatVec)[j] += (inSelectionWeights[i] * (*lVecI)[j]);
		}
	}
	ioCMAValues.mXmean.resize(inN);
	for(unsigned int i=0; i<inN; ++i) ioCMAValues.mXmean[i] = (*lMeanFloatVec)[i];

	// Generate lambda children with breeder tree, first build breeder roulette
	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);

	// Keep best individuals if elitism is used
	const unsigned int lElitismKS=mElitismKeepSize->getWrappedValue();
	if(lElitismKS > 0) {
		Individual::Bag lBestInd;
		History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		for(unsigned int i=0; i<lElitismKS; ++i) {
			if(lHistory != NULL) {
				HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[0]->getMember("HistoryID"));
				std::vector<HistoryID> lParent;
				if(lHID != NULL) lParent.push_back(*lHID);
				lHistory->allocateNewID(*ioDeme[0]);
				lHistory->trace(ioContext, lParent, ioDeme[0], getName(), "elitism");
			}
			lBestInd.push_back(ioDeme[0]);
			std::pop_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
			ioDeme.pop_back();
		}
		ioDeme.clear();
		ioDeme.insert(ioDeme.end(), lBestInd.begin(), lBestInd.end());
	} else ioDeme.clear();

	// Generate the children
	Individual::Bag lBagWithMeanInd;
	lBagWithMeanInd.push_back(lMeanInd);
	for(unsigned int i=0; i<inNbChildren; ++i) {
		unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
		BreederNode::Handle lSelectedBreeder=getRootNode();
		for(unsigned int j=0; j<lIndexBreeder; ++j)
			lSelectedBreeder=lSelectedBreeder->getNextSibling();
		Beagle_NonNullPointerAssertM(lSelectedBreeder);
		Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
		Individual::Handle lBredIndiv =
		    lSelectedBreeder->getBreederOp()->breed(lBagWithMeanInd,
		                                            lSelectedBreeder->getFirstChild(),
		                                            ioContext);
		Beagle_NonNullPointerAssertM(lBredIndiv);
		ioDeme.push_back(lBredIndiv);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Generate selection weights for creating weighted mean individual.
 *  \param inPopSize Population size.
 *  \param outSelectionWeight Generated selection weights.
 *  \return Effective mu value.
 */
double GA::MuWCommaLambdaCMAFltVecOp::generateSelectionWeights(unsigned int inPopSize,
        Vector& outSelectionWeights) const
{
	Beagle_StackTraceBeginM();
	outSelectionWeights.resize(inPopSize);
	double lSumWeight=0.0;
	double lSumSquareWeight=0.0;
	for(unsigned int i=0; i<outSelectionWeights.size(); ++i) {
		outSelectionWeights[i] =  std::log(double(inPopSize+1));
		outSelectionWeights[i] -= std::log(double(i+1));
		lSumWeight             += outSelectionWeights[i];
		lSumSquareWeight       += (outSelectionWeights[i] * outSelectionWeights[i]);
	}
	for(unsigned int i=0; i<outSelectionWeights.size(); ++i) outSelectionWeights[i] /= lSumWeight;
	return ((lSumWeight * lSumWeight) / lSumSquareWeight);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get CMA values at the given index.
 *  \param inIndex Index of the CMA values to initialize.
 *  \param inN Dimensionality of the problem to solve.
 *  \param ioContext Reference to the evolutionary context.
 *  \throw Beagle::ValidationException If a parameter is invalid.
 */
GA::CMAValues& GA::MuWCommaLambdaCMAFltVecOp::getCMAValues(unsigned int inIndex,
        unsigned int inN,
        Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	// Get the appropriate CMA values from the CMA holder component.
	Component::Handle lHolderComponent = ioContext.getSystem().getComponent("CMAHolder");
	if(lHolderComponent==NULL)
		throw Beagle_RunTimeExceptionM("No CMA holder component found in the system!");
	GA::CMAHolder::Handle lCMAHolder = castHandleT<GA::CMAHolder>(lHolderComponent);
	if(lCMAHolder==NULL)
		throw Beagle_RunTimeExceptionM("Component named 'CMAHolder' found is not of the good type!");
	GA::CMAHolder::iterator lIterVal = lCMAHolder->find(ioContext.getDemeIndex());
	if((ioContext.getGeneration()<=1) || (lIterVal==lCMAHolder->end())) {
		GA::CMAValues& lValues = (*lCMAHolder)[inIndex];
		lValues.mB.setIdentity(inN);
		lValues.mD.resize(inN);
		for(unsigned int i=0; i<inN; ++i) lValues.mD[i] = 1.0;
		lValues.mPC.resize(inN);
		for(unsigned int i=0; i<inN; ++i) lValues.mPC[i] = 0.0;
		lValues.mPS.resize(inN);
		for(unsigned int i=0; i<inN; ++i) lValues.mPS[i] = 0.0;
		lValues.mSigma = mSigma->getWrappedValue();
		return lValues;
	}
	return lIterVal->second;
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Register the parameters of the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void GA::MuWCommaLambdaCMAFltVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MuCommaLambdaOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "CMA-ES sigma value",
		    "Double",
		    "0.5",
		    "Initial CMA-ES sigma value moduling the mutation step size."
		);
		mSigma = castHandleT<Double>(
		             ioSystem.getRegister().insertEntry("ga.cmaes.sigma", new Double(0.5), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Maximum values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the maximum value is smaller than the ";
		lOSS << "float vector size, the limit used for the last values of the float vector ";
		lOSS << "is equal to the last value of the maximum value vector.";
		Register::Description lDescription(
		    "Maximum vector values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("ga.float.maxvalue", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Minimum  values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the minimum value is smaller than the ";
		lOSS << "float vector size, the limit used for the last values of the float vector ";
		lOSS << "is equal to the last value of the minimum value vector.";
		Register::Description lDescription(
		    "Minimum values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("ga.float.minvalue", new DoubleArray(1,-DBL_MAX), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the CMA-ES (Mu_W+Lambda) replacement strategy operation on a deme.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 *  \throw Beagle::ValidationException If a parameter is missing or have a bad value.
 *  \throw Beagle::AssertException If an invalid condition appears.
 */
void GA::MuWCommaLambdaCMAFltVecOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Get real popsize and size of float vectors from register.
	UIntArray::Handle lPopSize;
	if(ioContext.getSystem().getRegister().isRegistered("ec.pop.size")) {
		lPopSize = castHandleT<UIntArray>(ioContext.getSystem().getRegister()["ec.pop.size"]);
	} else {
		std::ostringstream lOSS;
		lOSS << "Population size parameter 'ec.pop.size' is not found in register!";
		throw ValidationException(lOSS.str());
	}

	const unsigned int lDemeSize = (*lPopSize)[ioContext.getDemeIndex()];
	UInt::Handle lFloatVectorSize;
	if(ioContext.getSystem().getRegister().isRegistered("ga.init.vectorsize")) {
		lFloatVectorSize = castHandleT<UInt>(ioContext.getSystem().getRegister()["ga.init.vectorsize"]);
	} else {
		std::ostringstream lOSS;
		lOSS << "GA::MuWCommaLambdaCMAFltVecOp must be used in fixed-lenght float vector ";
		lOSS << "individuals. Parameter 'ga.init.vectorsize' is not in register, ";
		lOSS << "while it is needed to set initial size of the different CMA-ES matrices ";
		lOSS << "and vectors.";
		throw ValidationException(lOSS.str());
	}
	const unsigned int lN=lFloatVectorSize->getWrappedValue();

	// Get the appropriate CMA values from the CMA holder component.
	GA::CMAValues& lValues = getCMAValues(ioContext.getDemeIndex(),lN,ioContext);

	// Compute weights and effective mu
	Vector lWeight;
	double lMuEff = 0.0;
	lMuEff = generateSelectionWeights(lDemeSize, lWeight);
	if(ioDeme.size() == 1) lMuEff = 1.;

	// If the replacement strategy possess a breeder tree
	if(getRootNode()!=NULL) {
		// Generate new children.
		const unsigned int lLambda =
		    (unsigned int)std::ceil(mLMRatio->getWrappedValue()*double(lDemeSize));
		generateChildren(ioDeme, ioContext, lLambda, lN, lValues, lWeight);

		// Check if all individuals have known fitness.
		for(unsigned int i=0; i<ioDeme.size(); ++i) {
			// If there is one invalid fitness, we should exit.
			// Evaluation will be taken elsewhere (we hope), and actual operator will be recalled.
			if((ioDeme[i]->getFitness()==NULL) || (ioDeme[i]->getFitness()->isValid()==false)) return;
		}
	}

	// Keep mu best children
	Beagle_AssertM(ioDeme.size() > lDemeSize);
	std::sort(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
	ioDeme.resize(lDemeSize);

	// Update CMA-ES values.
	updateValues(ioDeme, ioContext, lN, lMuEff, lWeight, lValues);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Update covariance matrix, cumulation path and other CMA values.
 *  \param ioDeme Deme to use to update CMA values.
 *  \param ioContext Evolutionary context.
 *  \param inN Dimensionality of the problem.
 *  \param inMuEff Effective mu.
 *  \param inSelectionWeights Selection weights.
 *  \param ioCMAValues Reference to CMA values.
 *
 *  This routine is mostly inspired from matlab routine cmaes.m and the document
 *  "The CMA Evolution Strategy: A Tutorial" (October 15, 2004), both
 *  from Nikolaus Hansen.
 *  See http://www.bionik.tu-berlin.de/user/niko/cmaes_inmatlab.html
 */
void GA::MuWCommaLambdaCMAFltVecOp::updateValues(Deme& ioDeme,
        Context& ioContext,
        unsigned int inN,
        double inMuEff,
        const Vector& inSelectionWeights,
        GA::CMAValues& ioCMAValues) const
{
	Beagle_StackTraceBeginM();

	// Log messages
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "Updating covariance matrix, sigma, and cumulation paths of CMA-ES."
	);

	// Compute some constants
	const double lCC = 4.0 / (double(inN) + 4.0);
	const double lCS = (inMuEff+2.0) / (double(inN)+inMuEff+3.0);
	const double lMuCov = inMuEff;
	double lCCov = ((2.0*inMuEff)-1.0) / (((double(inN)+2.0)*(double(inN)+2.0)) + inMuEff);
	lCCov = ((1.0/lMuCov) * (2.0/((double(inN)+1.414)*(double(inN)+1.414)))) +
	        ((1.0-(1.0/lMuCov)) * minOf(1.0,lCCov));
	double lDamps = std::sqrt((inMuEff-1.0) / (double(inN)+1.0)) - 1.0;
	lDamps = 1.0 + (2.0*maxOf(0.0,lDamps)) + lCS;
	const double lChiN = std::sqrt(double(inN)) *
	                     (1.0 - (0.25/double(inN)) + (1.0/(21.0*double(inN)*double(inN))));

	// Compute new xmean
	Vector lXmean_new(inN, 0.0);
	for(unsigned int i=0; i<ioDeme.size(); ++i) {
		GA::FloatVector::Handle lVecI = castHandleT<GA::FloatVector>((*ioDeme[i])[0]);
		for(unsigned int j=0; j<inN; ++j) lXmean_new[j] += (inSelectionWeights[i] * (*lVecI)[j]);
	}

	// Compute zmean
	Vector lZmean(inN,0.0);
	const double lSigma=ioCMAValues.mSigma.getWrappedValue();
	for(unsigned int i=0; i<inN; ++i) {
		lZmean[i] = (lXmean_new[i] - ioCMAValues.mXmean[i]) / lSigma;
	}
	Matrix lBt;
	ioCMAValues.mB.transpose(lBt);
	lZmean = lBt * lZmean;
	for(unsigned int i=0; i<inN; ++i) lZmean[i] /= ioCMAValues.mD[i];

	// Update cumulation paths
	Matrix lBD(inN,inN,0.0);
	for(unsigned int i=0; i<inN; ++i) {
		for(unsigned int j=0; j<inN; ++j) lBD(j,i) = ioCMAValues.mB(j,i) * ioCMAValues.mD[i];
	}
	Vector lBZm;
	ioCMAValues.mB.multiply((Matrix&)lBZm, lZmean);
	lBZm            *= std::sqrt(lCS * (2.0-lCS) * inMuEff);
	ioCMAValues.mPS *= (1.0-lCS);
	ioCMAValues.mPS += lBZm;

	double lPSnorm = 0.0;
	for(unsigned int i=0; i<ioCMAValues.mPS.size(); ++i) lPSnorm += (ioCMAValues.mPS[i] * ioCMAValues.mPS[i]);
	lPSnorm = std::sqrt(lPSnorm);
	const double lHLeft =
	    lPSnorm / std::sqrt(1.0-std::pow(1.0-lCS, 2.0*double(ioContext.getGeneration())));
	const double lHRight = (1.5+(1.0/(double(inN)-0.5))) * lChiN;
	const bool lHSig = (lHLeft<lHRight);

	ioCMAValues.mPC *= (1.0-lCC);
	if(lHSig) {
		// Vector lBDZm;
		// lBD.multiply((Matrix&)lBDZm, lZmean);
		// lBDZm *= std::sqrt(lCC * (2.0-lCC) * inMuEff);
		// ioCMAValues.mPC += lBDZm;
		Vector lXdiff = lXmean_new - ioCMAValues.mXmean;
		lXdiff *= (std::sqrt(lCC * (2.0-lCC) * inMuEff) / lSigma);
		ioCMAValues.mPC += lXdiff;
	} else {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "CMA-ES p_c cumulation path update stalled"
		);
	}

	// Adapt covariance matrix C
	Matrix lC, lBDt;
	lBD.transpose(lBDt);
	lBD.multiply(lC, lBDt);

	double lAttnC = (1.0-lCCov);    // Attenuation factor
	if(lHSig == false) lAttnC += (lCCov * lCC * (2.0-lCC) / lMuCov);

	Matrix lR1Upd;                  // Rank one update
	ioCMAValues.mPC.transpose(lR1Upd);
	lR1Upd = ioCMAValues.mPC * lR1Upd;
	lR1Upd *= (lCCov / lMuCov);

	Matrix lRMuUpd(inN,inN,0.0);    // Rank Mu update
	for(unsigned int i=0; i<ioDeme.size(); ++i) {
		GA::FloatVector::Handle lVecI = castHandleT<GA::FloatVector>((*ioDeme[i])[0]);
		Vector lX_I(lVecI->size());
		for(unsigned int j=0; j<lVecI->size(); ++j) lX_I[j] = (*lVecI)[j];
		lX_I -= ioCMAValues.mXmean;
		Matrix lX_It;
		lX_I.transpose(lX_It);
		lX_It *= inSelectionWeights[i];
		lRMuUpd += (lX_I * lX_It);
	}
	lRMuUpd *= (lCCov * (1.0 - (1.0/lMuCov)) / (lSigma * lSigma));

	lC *= lAttnC;                   // Attenuate old matrix
	lC += lR1Upd;                   // Add rank one update
	lC += lRMuUpd;                  // Add rank mu update

	// Adapt step size sigma
	ioCMAValues.mSigma.getWrappedValue() *= std::exp((lCS/lDamps) * ((lPSnorm/lChiN)-1.0));

	// Update B and D from C
	for(unsigned int i=1; i<inN; ++i) {
		for(unsigned int j=0; j<i; ++j) lC(i,j) = lC(j,i);  // Enforce symetry
	}
	lC.computeEigens(ioCMAValues.mD, ioCMAValues.mB);     // Principal component analysis

	// Adjust D as standard deviation
	for(unsigned int i=0; i<inN; ++i) {
		ioCMAValues.mD[i] = std::sqrt(ioCMAValues.mD[i]);
	}

	// Log updated parameters.
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("CMA-ES updated B matrix (principal components): ")+ioCMAValues.mB.serialize()
	);
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("CMA-ES updated D vector (standard deviations): ")+ioCMAValues.mD.serialize()
	);
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("CMA-ES updated sigma: ")+ioCMAValues.mSigma.serialize()
	);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("CMA-ES updated p_c vector (covariance cumulation path): ")+ioCMAValues.mPC.serialize()
	);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("CMA-ES updated p_s vector (sigma cumulation path): ")+ioCMAValues.mPS.serialize()
	);
	Beagle_StackTraceEndM();
}
