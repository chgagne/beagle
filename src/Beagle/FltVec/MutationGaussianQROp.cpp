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
 *  \file   Beagle/FltVec/MutationGaussianQROp.cpp
 *  \brief  Source code of class FltVec::MutationGaussianQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/FltVec.hpp"

#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a real-valued gaussian derandomized mutation operator.
 *  \param inMutationPbName Gaussian mutation probability parameter name used in register.
 *  \param inMutateFloatPbName Single value gaussian mutation probability name used in the register.
 *  \param inMutateGaussMuName Gaussian mutation mean value parameter name used in register.
 *  \param inMutateGaussSigmaName Gaussian mutation std deviation parameter name used in register.
 *  \param inName Name of the operator.
 */
FltVec::MutationGaussianQROp::MutationGaussianQROp(std::string inMutationPbName,
        std::string inMutateFloatPbName,
        std::string inMutateGaussMuName,
        std::string inMutateGaussSigmaName,
        std::string inName) :
		FltVec::MutationGaussianOp(inMutationPbName,
		                           inMutateFloatPbName,
		                           inMutateGaussMuName,
		                           inMutateGaussSigmaName,
		                           inName)
{ }


/*!
 *  \brief Register the parameters of the derandomized GA float vector mutation operator.
 *  \param ioSystem System of the evolution.
 */
void FltVec::MutationGaussianQROp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	FltVec::MutationGaussianOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized GA float vector mutation operator.
 *  \param ioSystem System of the evolution.
 */
void FltVec::MutationGaussianQROp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	FltVec::MutationGaussianOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		if(ioSystem.getRegister().isRegistered("fltvec.init.vectorsize")) {
			UInt::Handle lFloatVectorSize =
			    castHandleT<UInt>(ioSystem.getRegister()["fltvec.init.vectorsize"]);
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
 *  \brief Gaussian mutate a real-valued individual.
 *  \param ioIndividual Real-valued individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool FltVec::MutationGaussianQROp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	bool lMutated = false;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "Derandomized Gaussian mutations with mean of " << *mMutateGaussMu <<
	    ", and standard deviation of " << *mMutateGaussSigma
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		FltVec::FloatVector::Handle lVector = castHandleT<FltVec::FloatVector>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Gaussian derandomized mutation the " << uint2ordinal(i+1) << " float vector"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);

		QuasiRandom::Handle lQRComponent =
		    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
		Vector lQRValues(lVector->size());
		lQRComponent->getGaussianVector(lQRValues);
		Beagle_AssertM(lVector->size() == lQRValues.size());

		for(unsigned int j=0; j<lVector->size(); j++) {
			const float lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
			if(lRolledPb <= mMutateFloatPb->getWrappedValue()) {
				const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
				const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
				const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
				const double lMu =
				    j<mMutateGaussMu->size() ? (*mMutateGaussMu)[j] : mMutateGaussMu->back();
				const double lSigma =
				    j<mMutateGaussSigma->size() ? (*mMutateGaussSigma)[j] : mMutateGaussSigma->back();
				Beagle_AssertM(lSigma>=0.0);
				const double lMValue = (lQRValues[j] * lSigma) + lMu;
				(*lVector)[j] += lMValue;
				if((*lVector)[j] > lMaxVal) (*lVector)[j] = lMaxVal;
				if((*lVector)[j] < lMinVal) (*lVector)[j] = lMinVal;
				if(std::fabs(lIncVal)>1e-12) {
					(*lVector)[j] = lIncVal * round((*lVector)[j] / lIncVal);
					if((*lVector)[j] > lMaxVal) (*lVector)[j] -= lIncVal;
					if((*lVector)[j] < lMinVal) (*lVector)[j] += lIncVal;
				}
				lMutated = true;
				Beagle_LogDebugM(
				    ioContext.getSystem().getLogger(),
				    "Gaussian mutating by adding " << lMValue <<
				    " to the value at the index " << j << " of the float vector"
				);
			}
		}
		if(lMutated) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The float vector has been mutated"
			);
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    *lVector
			);
		} else {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The float vector has not been mutated"
			);
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}
