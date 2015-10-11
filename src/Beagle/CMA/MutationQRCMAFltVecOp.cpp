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
 *  \file   beagle/GA/src/MutationQRCMAFltVecOp.cpp
 *  \brief  Source code of class GA::MutationQRCMAFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>

using namespace Beagle;


/*!
 *  \brief Construct derandomized CMA-ES mutation operator.
 *  \param inMutationPbName CMA-ES mutation probability parameter name used in register.
 *  \param inName Name of the derandomized CMA-ES mutation operator.
 */
GA::MutationQRCMAFltVecOp::MutationQRCMAFltVecOp(std::string inMutationPbName,
        std::string inName) :
		GA::MutationCMAFltVecOp(inMutationPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the derandomized CMA-ES mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationQRCMAFltVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::MutationCMAFltVecOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized CMA-ES mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationQRCMAFltVecOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::MutationCMAFltVecOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		if(ioSystem.getRegister().isRegistered("ga.init.vectorsize")) {
			UInt::Handle lFloatVectorSize =
			    castHandleT<UInt>(ioSystem.getRegister()["ga.init.vectorsize"]);
			lQRComponent->reset(lFloatVectorSize->getWrappedValue(), ioSystem.getRandomizer());
		} else {
			std::ostringstream lOSS;
			lOSS << "Could not post-initialize operator '" << getName() << "'. Looking for ";
			lOSS << "setting up the QuasiRandom component with a dimensionality equal to ";
			lOSS << "initial vector size parameter ('ga.init.vectorsize'), but this parameter ";
			lOSS << "is not present in the register.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply derandomized CMA-ES mutation on a real-valued GA individual.
 *  \param ioIndividual Real-valued GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GA::MutationQRCMAFltVecOp::mutate(Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	
	if(ioIndividual.size()!=1) throw Beagle_RunTimeExceptionM(
		    "CMA-ES mutation can't be applied on individuals composed by more than one float vector");

	// Get the appropriate CMA values from the CMA holder component.
	Component::Handle lHolderComponent = ioContext.getSystem().getComponent("CMAHolder");
	if(lHolderComponent==NULL)
		throw Beagle_RunTimeExceptionM("No CMA holder component found in the system!");
	GA::CMAHolder::Handle lCMAHolder = castHandleT<GA::CMAHolder>(lHolderComponent);
	if(lCMAHolder==NULL)
		throw Beagle_RunTimeExceptionM("Component named 'CMAHolder' found is not of the good type!");
	GA::CMAHolder::iterator lIterVal = lCMAHolder->find(ioContext.getDemeIndex());
	if(lIterVal == lCMAHolder->end()) {
		std::ostringstream lOSS;
		lOSS << "CMA values of index '" << ioContext.getDemeIndex() << "' not found in CMA holder ";
		lOSS << "component. Consider having a breeding pipeline with an appropriate CMA replacement ";
		lOSS << "strategy that would correctly initialize the CMA values at the given index.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	GA::CMAValues& lValues = lIterVal->second;

	GA::FloatVector::Handle lVector=castHandleT<GA::FloatVector>(ioIndividual[0]);
	Beagle_AssertM(lVector->size()==lValues.mD.size());
	const double lSigma=lValues.mSigma.getWrappedValue();
	Vector lArz(lVector->size());
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
	lQRComponent->getGaussianVector(lArz);
	Beagle_AssertM(lVector->size() == lArz.size());
	for(unsigned int i=0; i<lArz.size(); ++i) {
		double lStdErr = lSigma * lValues.mD[i];
		if(isNaN(lStdErr) || (lStdErr<0.)) {
			lArz[i] = 0.0;
			std::ostringstream lOSS;
			lOSS << "WARNING: standard deviation for derandomized CMA-ES mutation over the ";
			lOSS << uint2ordinal(i+1);
			lOSS << " principal component is NaN or negative (" << lStdErr << "). ";
			lOSS << "Mutation of this component has been cancelled.";
			Beagle_LogBasicM(ioContext.getSystem().getLogger(), lOSS.str());
		} else lArz[i] *= lStdErr;
	}
	Vector lMutFactor;
	lValues.mB.multiply((Matrix&)lMutFactor, lArz);
	Beagle_AssertM(lMutFactor.size()==lVector->size());
	for(unsigned int i=0; i<lVector->size(); ++i) {
		const double lMaxVal = i<mMaxValue->size() ? (*mMaxValue)[i] : mMaxValue->back();
		const double lMinVal = i<mMinValue->size() ? (*mMinValue)[i] : mMinValue->back();
		(*lVector)[i] += lMutFactor[i];
		if((*lVector)[i] > lMaxVal) (*lVector)[i] = lMaxVal;
		if((*lVector)[i] < lMinVal) (*lVector)[i] = lMinVal;
	}

	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
	
	return true;
	Beagle_StackTraceEndM();
}
