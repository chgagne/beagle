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
 *  \file   Beagle/SAES/InitializationOp.cpp
 *  \brief  Source code of class SAES::InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2008/04/16 15:58:56 $
 */

#include "Beagle/SAES.hpp"

#include <cfloat>
#include <float.h>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct an SA-ES initialization operator.
 *  \param inVectorSize Size of the vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
SAES::InitializationOp::InitializationOp(unsigned int inVectorSize,
                                         std::string inReproProbaName,
                                         std::string inName) :
		EC::InitializationOp(inReproProbaName, inName),
		mVectorSize(new UInt(inVectorSize))
{ }


/*!
 *  \brief Register the parameters of the SA-ES initialization operator.
 *  \param ioSystem System of the evolution.
 */
void SAES::InitializationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Initial SA-ES vectors size",
		    "UInt",
		    mVectorSize->serialize(),
		    "SA-ES vectors size used for initialization."
		);
		mVectorSize = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry("saes.init.vectorsize", mVectorSize, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "SA-ES vector maximum values. This can be a scalar, which limit the values on all dimensions, ";
		lOSS << "or a vector which limit the values individually. If the maximum value vector size is ";
		lOSS << "smaller than the vector size, the limit used for the last values of the vector ";
		lOSS << "is equal to the last value of the maximum value vector.";
		Register::Description lDescription(
		    "Maximum SA-ES vector values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("saes.value.max", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "SA-ES vector minimum values. This can be a scalar, which limit the values on all dimensions, ";
		lOSS << "or a vector which limit the values individually. If the minimum value vector size is ";
		lOSS << "smaller than the vector size, the limit used for the last values of the vector ";
		lOSS << "is equal to the last value of the minimum value vector.";
		Register::Description lDescription(
		    "Minimum SA-ES vector values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("saes.value.min", new DoubleArray(1,-DBL_MAX), lDescription));
	}
	{
		Register::Description lDescription(
		    "Mean value for SA-ES initialization",
		    "DoubleArray",
		    "0.0",
		    "Mean of values used when initializing SA-ES vectors."
		);
		mMeanInitValue = castHandleT<DoubleArray>(
		                         ioSystem.getRegister().insertEntry("saes.init.mean", new DoubleArray(1,0.0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Initial strategy parameter value",
		    "DoubleArray",
		    "1.0",
		    "Initial strategy parameter value used to initialize SA-ES vectors."
		);
		mInitStrategyValue = castHandleT<DoubleArray>(
		                         ioSystem.getRegister().insertEntry("saes.init.strategy", new DoubleArray(1,1.0), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize SA-ES individual.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 *
 *  Size of SA-ES individual is given by integer vector "saes.init.vectorsize". Initial strategy
 *  parameter value of ES pair is given by parameter "saes.init.strategy". Initial value of ES pair
 *  is given using random numbers following a Gaussian distribution of mean given by parameter
 *  "saes.init.mean" and standard deviation equal to initial strategy parameter value.
 */
void SAES::InitializationOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mVectorSize->getWrappedValue() == 0) {
		std::string lMessage = "SAES::InitializationOp::initIndividual: ";
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
	for(unsigned int j=0; j<lVector->size(); ++j) {
		const double lMaxVal   = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
		const double lMinVal   = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
		const double lMeanVal  = j<mMeanInitValue->size() ? (*mMeanInitValue)[j] : mMeanInitValue->back();
		const double lStrategy = j<mInitStrategyValue->size() ? (*mInitStrategyValue)[j] : mInitStrategyValue->back();
		(*lVector)[j].mValue = ioContext.getSystem().getRandomizer().rollGaussian(lMeanVal,lStrategy);
		if((*lVector)[j].mValue > lMaxVal) (*lVector)[j].mValue = lMaxVal;
		if((*lVector)[j].mValue < lMinVal) (*lVector)[j].mValue = lMinVal;
		(*lVector)[j].mStrategy = lStrategy;
	}
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
	Beagle_StackTraceEndM();
}

