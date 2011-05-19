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
 *  \file   beagle/GA/src/TermCMAOp.cpp
 *  \brief  Source code of class GA::TermCMAOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/10 20:32:21 $
 */

#include "beagle/GA.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a termination operator for CMA-ES.
 *  \param inName Name of the operator.
 */
GA::TermCMAOp::TermCMAOp(std::string inName) :
		TerminationOp(inName)
{ }


/*!
 *  \brief Register the parameters of the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void GA::TermCMAOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	TerminationOp::registerParams(ioSystem);
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
		Register::Description lDescription(
		    "CMA-ES tolerance on values",
		    "Double",
		    "1e-12",
		    "Tolerance on variations of the values, used in the CMA-ES termination criterion."
		);
		mTolX = castHandleT<Double>(
		            ioSystem.getRegister().insertEntry("ga.cmaes.tolval", new Double(1e-12), lDescription));
	}
	{
		Register::Description lDescription(
		    "CMA-ES tolerance on condition",
		    "Double",
		    "1e14",
		    "Tolerance on condition number of matrix C, used in the CMA-ES termination criterion."
		);
		mTolCond = castHandleT<Double>(
		               ioSystem.getRegister().insertEntry("ga.cmaes.tolcond", new Double(1e14), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if the different CMA-ES termination criterion are reached.
 *  \param ioDeme
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool GA::TermCMAOp::terminate(const Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// If generation 0, termination criteria not tested.
	if(ioContext.getGeneration() == 0) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "CMA-ES termination criteria are not tested at generation 0"
		);
		return false;
	}

	// Get the appropriate CMA values from the CMA holder component.
	Component::Handle lHolderComponent = ioContext.getSystem().getComponent("CMAHolder");
	if(lHolderComponent==NULL)
		throw Beagle_RunTimeExceptionM("No CMA holder component found in the system!");
	GA::CMAHolder::Handle lCMAHolder = castHandleT<GA::CMAHolder>(lHolderComponent);
	if(lCMAHolder==NULL)
		throw Beagle_RunTimeExceptionM("Component named 'CMAHolder' found is not of the good type!");
	GA::CMAHolder::iterator lIterVal = lCMAHolder->find(ioContext.getDemeIndex());
	if(lIterVal == lCMAHolder->end()) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "CMA-ES termination criteria not tested as no CMA holder component are found in the system"
		);
		return false;
	}
	GA::CMAValues& lValues = lIterVal->second;

	// Get dimensionality of the problem.
	const unsigned int lN = lValues.mD.size();
	if(lN == 0) {
		std::ostringstream lOSS;
		lOSS << "CMA-ES termination criteria not tested as the dimensionality of CMA values for holder ";
		lOSS << "at index '" << ioContext.getDemeIndex();
		lOSS << "' is zero";
		Beagle_LogTraceM(ioContext.getSystem().getLogger(), lOSS.str());
		return false;
	}

	// Compute some general matrices used to evaluate termination conditions.
	Matrix lC, lBDt, lBD(lN,lN,0.0);
	for(unsigned int i=0; i<lN; ++i) {
		for(unsigned int j=0; j<lN; ++j) lBD(j,i) = lValues.mB(j,i) * lValues.mD[i];
	}
	lBD.transpose(lBDt);
	lBD.multiply(lC, lBDt);

	// conditioncov: whether condition number of matrix C exceed some value.
	double lNormC  = 0.0;
	double lNormCi = 0.0;
	for(unsigned int i=0; i<lN; ++i) {
		const double ldi2 = lValues.mD[i] * lValues.mD[i];
		if(ldi2 > lNormC) lNormC = ldi2;
		if((1.0/ldi2) > lNormCi) lNormCi = (1.0 / ldi2);
	}
	const double lC_condition = lNormC * lNormCi;
	if(lC_condition > mTolCond->getWrappedValue()) {
		std::ostringstream lOSS;
		lOSS << "Terminating as condition number of matrix C (";
		lOSS << lC_condition << ") is higher than the TolCond tolerance (";
		lOSS << mTolCond->getWrappedValue() << ") [conditioncov]";
		Beagle_LogInfoM(ioContext.getSystem().getLogger(), lOSS.str());
		return true;
	}

	const double lTolX    = mTolX->getWrappedValue();
	const double lSigma_0 = mSigma->getWrappedValue();
	const double lSigma_g = lValues.mSigma.getWrappedValue();
	const double lTolVal  = lTolX * lSigma_0;

	// [(sigma_0 * sqrt(C(i,i))) < TolX, forall(i)]: whether standard deviation of the normal
	// distribution on all components is smaller than TolX.
	unsigned int lTolXCiiCount=0;
	for(unsigned int i=0; i<lN; ++i) {
		if((lSigma_g * std::sqrt(lC(i,i))) < lTolVal) ++lTolXCiiCount;
		else break;
	}

	// [|sigma_g * P_c(i)| < TolX, forall(i)]: whether all components
	// of cumulation path p_c are smaller than TolX.
	unsigned int lTolXPcCount=0;
	for(unsigned int i=0; i<lN; ++i) {
		if(std::fabs(lSigma_g * lValues.mPC[i]) < lTolVal) ++lTolXPcCount;
		else break;
	}
	if((lTolXCiiCount==lN) && (lTolXPcCount==lN)) {
		std::ostringstream lOSS;
		lOSS << "Terminating as all standard deviations of matrix C (i.e. C(i,i) * sigma_g) (";
		for(unsigned int i=0; i<lN; ++i) {
			lOSS << (lSigma_g * std::sqrt(lC(i,i)));
			if(i!=(lN-1)) lOSS << ";";
		}
		lOSS << ") and all components of cumulation path (i.e. |sigma_g * P_c|) (";
		for(unsigned int i=0; i<lN; ++i) {
			lOSS << std::fabs(lSigma_g * lValues.mPC[i]);
			if(i!=(lN-1)) lOSS << ";";
		}
		lOSS << ") are smaller than the given minimum (TolX * sigma_0) tolerance (";
		lOSS << lTolVal << ") [TolX]";
		Beagle_LogInfoM(ioContext.getSystem().getLogger(), lOSS.str());
		return true;
	}

	// noeffectaxis: whether modifications over all the principal components of C
	// does not change xmean.
	unsigned int lNoEffectAxisCount = 0;
	for(unsigned int i=0; i<lN; ++i) {
		Vector lBDColI;
		lBD.extractColumn((Matrix&)lBDColI, i);
		lBDColI *= lValues.mSigma.getWrappedValue();
		unsigned int lNoEffectAxisCountJ = 0;
		for(unsigned int j=0; j<lN; ++j) {
			double lXModJ = lValues.mXmean[j] + lBDColI[j];
			if(lXModJ == lValues.mXmean[j]) ++lNoEffectAxisCountJ;
			else break;
		}
		if(lNoEffectAxisCountJ == lN) ++lNoEffectAxisCount;
		else break;
	}
	if(lNoEffectAxisCount == lN) {
		std::ostringstream lOSS;
		lOSS << "Terminating as perturbations of the xmean vector (";
		lOSS << lValues.mXmean.serialize();
		lOSS << ") over all principal components of C, using a actual sigma (";
		lOSS <<  lValues.mSigma.getWrappedValue();
		lOSS << "), does not change values of xmean [noeffectaxis]";
		Beagle_LogInfoM(ioContext.getSystem().getLogger(), lOSS.str());
		return true;
	}

	// noeffectcoord: whether modifications over all the coordinates does not change xmean.
	unsigned int lNoEffectCoordCount = 0;
	for(unsigned int i=0; i<lN; ++i) {
		const double lXModI =
		    lValues.mXmean[i] + (lValues.mSigma.getWrappedValue() * std::sqrt(lC(i,i)));
		if(lXModI == lValues.mXmean[i]) ++lNoEffectCoordCount;
		else break;
	}
	if(lNoEffectCoordCount == lN) {
		std::ostringstream lOSS;
		lOSS << "Terminating as perturbations over different coordinates of xmean vector (";
		lOSS << lValues.mXmean.serialize();
		lOSS << "), using a actual sigma (" <<  lValues.mSigma.getWrappedValue();
		lOSS << "), does not change values of xmean [noeffectcoord]";
		Beagle_LogInfoM(ioContext.getSystem().getLogger(), lOSS.str());
		return true;
	}

	// No stop criteria reached, continuing.
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), "None of the CMA-ES termination criteria reached");
	return false;
	Beagle_StackTraceEndM();
}

