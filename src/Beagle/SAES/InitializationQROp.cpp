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
 *  \file   Beagle/SAES/InitializationQROp.cpp
 *  \brief  Source code of class SAES::InitializationQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/SAES.hpp"

#include <cfloat>
#include <float.h>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct an SA-ES vectors derandomized initialization operator.
 *  \param inVectorSize Size of the SA-ES vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
SAES::InitializationQROp::InitializationQROp(unsigned int inVectorSize,
                                             std::string inReproProbaName,
                                             std::string inName) :
		SAES::InitializationOp(inVectorSize, inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the derandomized ES initialization operator.
 *  \param ioSystem System of the evolution.
 */
void SAES::InitializationQROp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	SAES::InitializationOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized SA-ES initialization operator.
 *  \param ioSystem System of the evolution.
 */
void SAES::InitializationQROp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	SAES::InitializationOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		lQRComponent->reset(mVectorSize->getWrappedValue()+1, ioSystem.getRandomizer());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize SA-ES individual with initial strategy parameter.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 *
 *  Size of SA-ES individual is given by integer vector "saes.init.vectorsize". Initial strategy
 *  parameter value of SA-ES pair is given by parameter "saes.init.strategy". Initial value of SA-ES pair
 *  is given using random numbers following a Gaussian distribution of mean given by parameter "saes.init.mean"
 *  and standard deviation equal to initial strategy parameter value.
 */
void SAES::InitializationQROp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mVectorSize->getWrappedValue() == 0) {
		std::string lMessage = "SAES::InitializationQROp::initIndividual: ";
		lMessage += "SA-ES vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG

	const Factory& lFactory = ioContext.getSystem().getFactory();
	SAES::PairVector::Alloc::Handle lVectorAlloc =
		castHandleT<SAES::PairVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	SAES::PairVector::Handle lVector = castHandleT<SAES::PairVector>(lVectorAlloc->allocate());
	lVector->resize(mVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lVector);
	Vector lQRValues(mVectorSize->getWrappedValue()+1);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
	lQRComponent->getUniformVector(lQRValues);
	Beagle_AssertM((mVectorSize->getWrappedValue()+1) == lQRValues.size());
	for(unsigned int j=0; j<lVector->size(); ++j) {
		const double lMaxVal   = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
		const double lMinVal   = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
		const double lMeanVal  = j<mMeanInitValue->size() ? (*mMeanInitValue)[j] : mMeanInitValue->back();
		const double lStrategy = j<mInitStrategyValue->size() ? (*mInitStrategyValue)[j] : mInitStrategyValue->back();
		(*lVector)[j].mValue = (lQRValues[j] * lStrategy) + lMeanVal;
		if((*lVector)[j].mValue > lMaxVal) (*lVector)[j].mValue = lMaxVal;
		if((*lVector)[j].mValue < lMinVal) (*lVector)[j].mValue = lMinVal;
		(*lVector)[j].mStrategy = lStrategy;
	}
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
	Beagle_StackTraceEndM();
}

