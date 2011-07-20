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
 *  \file   Beagle/FltVec/InitGaussianQROp.cpp
 *  \brief  Source code of class FltVec::InitGaussianQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/FltVec.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a float vectors derandomized Gaussian initialization operator.
 *  \param inFloatVectorSize Size of the float vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
FltVec::InitGaussianQROp::InitGaussianQROp(unsigned int inFloatVectorSize,
                                           std::string inReproProbaName,
                                           std::string inName) :
		FltVec::InitGaussianOp(inFloatVectorSize, inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the derandomized Gaussian float vector initialization operator.
 *  \param ioSystem System of the evolution.
 */
void FltVec::InitGaussianQROp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	FltVec::InitGaussianOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized Gaussian float vector initialization operator.
 *  \param ioSystem System of the evolution.
 */
void FltVec::InitGaussianQROp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	FltVec::InitGaussianOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		lQRComponent->reset(mFloatVectorSize->getWrappedValue(), ioSystem.getRandomizer());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize real-valued individual with derandomized Gaussian values.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void FltVec::InitGaussianQROp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mFloatVectorSize->getWrappedValue() == 0) {
		string lMessage = "FltVec::InitGaussianQROp::initIndividual: ";
		lMessage += "float vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	const Factory& lFactory = ioContext.getSystem().getFactory();
	FltVec::FloatVector::Alloc::Handle lVectorAlloc =
		castHandleT<FltVec::FloatVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	FltVec::FloatVector::Handle lVector =
		castHandleT<FltVec::FloatVector>(lVectorAlloc->allocate());
	lVector->resize(mFloatVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lVector);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
	Vector lQRValues(lVector->size());
	lQRComponent->getGaussianVector(lQRValues);
	Beagle_AssertM(mFloatVectorSize->getWrappedValue() == lQRValues.size());
	for(unsigned int j=0; j<lVector->size(); ++j) {
		const double lMaxVal = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
		const double lMinVal = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
		const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
		const double lMean   = j<mMean->size() ? (*mMean)[j] : mMean->back();
		const double lStdev  = j<mStdev->size() ? (*mStdev)[j] : mStdev->back();
		(*lVector)[j] = (lQRValues[j] * lStdev) + lMean;
		if((*lVector)[j] > lMaxVal) (*lVector)[j] = lMaxVal;
		if((*lVector)[j] < lMinVal) (*lVector)[j] = lMinVal;
		if(std::fabs(lIncVal)>1e-12) {
			(*lVector)[j] = lIncVal * round((*lVector)[j] / lIncVal);
			if((*lVector)[j] > lMaxVal) (*lVector)[j] -= lIncVal;
			if((*lVector)[j] < lMinVal) (*lVector)[j] += lIncVal;
		}
	}
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    *lVector
	);
	Beagle_StackTraceEndM();
}

