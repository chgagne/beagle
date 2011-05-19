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
 *  \file   beagle/GA/src/InitESVecOp.cpp
 *  \brief  Source code of class GA::InitESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2008/04/16 15:58:56 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct an ES vectors initialization operator.
 *  \param inESVectorSize Size of the ES vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::InitESVecOp::InitESVecOp(unsigned int inESVectorSize,
                             std::string inReproProbaName,
                             std::string inName) :
		InitializationOp(inReproProbaName, inName),
		mESVectorSize(new UInt(inESVectorSize))
{ }


/*!
 *  \brief Register the parameters of the ES vectors initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitESVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Initial ES vectors size",
		    "UInt",
		    mESVectorSize->serialize(),
		    "ES vectors size used for initialization."
		);
		mESVectorSize = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry("es.init.vectorsize", mESVectorSize, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "ES vector maximum values. This can be a scalar, which limit the values for all ES, ";
		lOSS << "or a vector which limit the values individually. If the maximum value vector size is ";
		lOSS << "smaller than the ES vector size, the limit used for the last values of the ES vector ";
		lOSS << "is equal to the last value of the maximum value vector.";
		Register::Description lDescription(
		    "Maximum ES vector values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("es.value.max", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "ES vector minimum values. This can be a scalar, which limit the values for all ES, ";
		lOSS << "or a vector which limit the values individually. If the minimum value vector size is ";
		lOSS << "smaller than the ES vector size, the limit used for the last values of the ES vector ";
		lOSS << "is equal to the last value of the minimum value vector.";
		Register::Description lDescription(
		    "Minimum ES vector values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("es.value.min", new DoubleArray(1,-DBL_MAX), lDescription));
	}
	{
		Register::Description lDescription(
		    "Initial strategy parameter value",
		    "Double",
		    "1.0",
		    "Initial strategy parameter value used to initialize ES vectors."
		);
		mInitStrategyValue = castHandleT<Double>(
		                         ioSystem.getRegister().insertEntry("es.init.strategy", new Double(1.0f), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize ES individual with initial strategy parameter.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 *
 *  Size of ES individual is given by integer vector "es.init.vectorsize". Initial strategy
 *  parameter value of ES pair is given by parameter "es.init.strategy". Initial value of ES pair
 *  is given using random numbers following a Gaussian distribution of zero mean and standard
 *  deviation equal to initial strategy parameter value.
 */
void GA::InitESVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mESVectorSize->getWrappedValue() == 0) {
		std::string lMessage = "GA::InitESVecOp::initIndividual: ";
		lMessage += "ES vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG

	const Factory& lFactory = ioContext.getSystem().getFactory();
	GA::ESVector::Alloc::Handle lESVectorAlloc =
		castHandleT<GA::ESVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	GA::ESVector::Handle lESVector = castHandleT<GA::ESVector>(lESVectorAlloc->allocate());
	lESVector->resize(mESVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lESVector);
	const double lInitStrategy = mInitStrategyValue->getWrappedValue();
	for(unsigned int j=0; j<lESVector->size(); ++j) {
		const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
		const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
		const double lMidVal = ((lMaxVal != DBL_MAX) && (lMinVal != -DBL_MAX)) ?
			(0.5 * (lMaxVal + lMinVal)) : 0.0;
		(*lESVector)[j].mValue = ioContext.getSystem().getRandomizer().rollGaussian(lMidVal,lInitStrategy);
		if((*lESVector)[j].mValue > lMaxVal) (*lESVector)[j].mValue = lMaxVal;
		if((*lESVector)[j].mValue < lMinVal) (*lESVector)[j].mValue = lMinVal;
		(*lESVector)[j].mStrategy = lInitStrategy;
	}
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lESVector);
	Beagle_StackTraceEndM();
}

