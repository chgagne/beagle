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
 *  \file   Beagle/FltVec/InitGaussianOp.cpp
 *  \brief  Source code of class FltVec::InitGaussianOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.25 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/FltVec.hpp"

#include <cmath>
#include <cfloat>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a float vector Gaussian distributed initialization operator.
 *  \param inFloatVectorSize Size of the float vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
FltVec::InitGaussianOp::InitGaussianOp(unsigned int inFloatVectorSize,
                                       std::string inReproProbaName,
                                       std::string inName) :
		EC::InitializationOp(inReproProbaName, inName),
		mFloatVectorSize(new UInt(inFloatVectorSize))
{ }


/*!
 *  \brief Register the parameters of the GA float vectors Gaussian distributed initialization operator.
 *  \param ioSystem System of the evolution.
 */
void FltVec::InitGaussianOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	EC::InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Initial float vectors sizes",
		    "UInt",
		    mFloatVectorSize->serialize(),
		    "Float vector size of initialized individuals."
		);
		mFloatVectorSize = castHandleT<UInt>(
		                       ioSystem.getRegister().insertEntry("fltvec.initgauss.vectorsize", mFloatVectorSize, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Maximum initialization values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the initialization value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the maximum initialization value is smaller than the ";
		lOSS << "float vector size, the limit used for the last values of the float vector ";
		lOSS << "is equal to the last value of the maximum initialization value vector.";
		Register::Description lDescription(
		    "Maximum initialization values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxInitValue = castHandleT<DoubleArray>(
		                    ioSystem.getRegister().insertEntry("fltvec.initgauss.maxvalue", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Minimum initialization values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the initialization value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the minimum initialization value is smaller than the ";
		lOSS << "float vector size, the limit used for the last values of the float vector ";
		lOSS << "is equal to the last value of the minimum initialization value vector.";
		Register::Description lDescription(
		    "Minimum initialization values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinInitValue = castHandleT<DoubleArray>(
		                    ioSystem.getRegister().insertEntry("fltvec.initunif.minvalue", new DoubleArray(1,-DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Increments of valid values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the value is not evenly divisible by the ";
		lOSS << "increment, the value will be set to the closest valid ";
		lOSS << "value.";
		Register::Description lDescription(
		    "Increments of valid values",
		    "DoubleArray",
		    "0.0",
		    lOSS.str()
		);
		mIncValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("fltvec.float.inc", new DoubleArray(1,0.0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Mean of Gaussian distribution used to initialize the vector's floats. ";
		lOSS << "Value can be a scalar, which parametrizes the initialization value for all float ";
		lOSS << "vector parameters, or a vector which parametrizes the value for the parameters ";
		lOSS << "individually. If the minimum initialization value is smaller than the ";
		lOSS << "float vector size, the mean used for the last values of the float vector ";
		lOSS << "is equal to the last value of the mu vector.";
		Register::Description lDescription(
		    "Gaussian mean initialization values",
		    "DoubleArray",
		    "0.0",
		    lOSS.str()
		);
		mMean = castHandleT<DoubleArray>(
		                    ioSystem.getRegister().insertEntry("fltvec.initgauss.mean", new DoubleArray(1,0.0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Standard deviation of Gaussian distribution used to initialize the vector's floats. ";
		lOSS << "Value can be a scalar, which parametrizes the initialization value for all float ";
		lOSS << "vector parameters, or a vector which parametrizes the value for the parameters ";
		lOSS << "individually. If the minimum initialization value is smaller than the ";
		lOSS << "float vector size, the mean used for the last values of the float vector ";
		lOSS << "is equal to the last value of the mu vector.";
		Register::Description lDescription(
		    "Gaussian stdev initialization values",
		    "DoubleArray",
		    "1.0",
		    lOSS.str()
		);
		mStdev = castHandleT<DoubleArray>(
		                    ioSystem.getRegister().insertEntry("fltvec.initgauss.stdev", new DoubleArray(1,1.0), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize real-valued individual with numbers following a Gaussian distribution of given variance.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void FltVec::InitGaussianOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mFloatVectorSize->getWrappedValue() == 0) {
		string lMessage = "FltVec::InitGaussianOp::initIndividual: ";
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
	for(unsigned int j=0; j<lVector->size(); ++j) {
		const double lMaxVal = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
		const double lMinVal = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
		const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
		const double lMean   = j<mMean->size() ? (*mMean)[j] : mMean->back();
		const double lStdev  = j<mStdev->size() ? (*mStdev)[j] : mStdev->back();
		(*lVector)[j] = ioContext.getSystem().getRandomizer().rollGaussian(lMean,lStdev);
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

