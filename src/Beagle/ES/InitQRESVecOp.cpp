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
 *  \file   beagle/GA/src/InitQRESVecOp.cpp
 *  \brief  Source code of class GA::InitQRESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct an ES vectors derandomized initialization operator.
 *  \param inESVectorSize Size of the ES vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
GA::InitQRESVecOp::InitQRESVecOp(unsigned int inESVectorSize,
                                 std::string inReproProbaName,
                                 std::string inName) :
		GA::InitESVecOp(inESVectorSize, inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the derandomized ES initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitQRESVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::InitESVecOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized ES initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitQRESVecOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::InitESVecOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		lQRComponent->reset(mESVectorSize->getWrappedValue()+1, ioSystem.getRandomizer());
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
void GA::InitQRESVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mESVectorSize->getWrappedValue() == 0) {
		std::string lMessage = "GA::InitQRESVecOp::initIndividual: ";
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
	Vector lQRValues(mESVectorSize->getWrappedValue()+1);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
	lQRComponent->getUniformVector(lQRValues);
	Beagle_AssertM((mESVectorSize->getWrappedValue()+1) == lQRValues.size());
	const double lInitStrategy = mInitStrategyValue->getWrappedValue();
	for(unsigned int j=0; j<lESVector->size(); ++j) {
		const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
		const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
		(*lESVector)[j].mValue = (lQRValues[j] * lInitStrategy);
		if((*lESVector)[j].mValue > lMaxVal) (*lESVector)[j].mValue = lMaxVal;
		if((*lESVector)[j].mValue < lMinVal) (*lESVector)[j].mValue = lMinVal;
		(*lESVector)[j].mStrategy = lInitStrategy;
	}
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lESVector);
	Beagle_StackTraceEndM();
}

