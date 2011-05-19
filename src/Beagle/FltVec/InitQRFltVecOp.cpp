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
 *  \file   beagle/GA/src/InitQRFltVecOp.cpp
 *  \brief  Source code of class GA::InitQRFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a GA float vectors derandomized initialization operator.
 *  \param inFloatVectorSize Size of the float vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::InitQRFltVecOp::InitQRFltVecOp(unsigned int inFloatVectorSize,
                                   std::string inReproProbaName,
                                   std::string inName) :
		InitFltVecOp(inFloatVectorSize, inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the derandomized GA float vector initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitQRFltVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::InitFltVecOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized GA float vector initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitQRFltVecOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::InitFltVecOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		lQRComponent->reset(mFloatVectorSize->getWrappedValue(), ioSystem.getRandomizer());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize real-valued GA individual with derandomized numbers.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void GA::InitQRFltVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mFloatVectorSize->getWrappedValue() == 0) {
		string lMessage = "GA::InitQRFltVecOp::initIndividual: ";
		lMessage += "float vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	const Factory& lFactory = ioContext.getSystem().getFactory();
	GA::FloatVector::Alloc::Handle lFloatVectorAlloc =
		castHandleT<GA::FloatVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	GA::FloatVector::Handle lFloatVector =
		castHandleT<GA::FloatVector>(lFloatVectorAlloc->allocate());
	lFloatVector->resize(mFloatVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lFloatVector);
	Vector lQRValues(mFloatVectorSize->getWrappedValue());
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
	lQRComponent->getUniformVector(lQRValues);
	Beagle_AssertM(mFloatVectorSize->getWrappedValue() == lQRValues.size());
	for(unsigned int j=0; j<lFloatVector->size(); ++j) {
		const double lMaxVal = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
		const double lMinVal = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
		const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
		(*lFloatVector)[j] = ((lMaxVal-lMinVal)*lQRValues[j])+lMinVal;
		if(std::fabs(lIncVal)>1e-12) {
			(*lFloatVector)[j] = lIncVal * round((*lFloatVector)[j] / lIncVal);
			if((*lFloatVector)[j] > lMaxVal) (*lFloatVector)[j] = lMaxVal;
			if((*lFloatVector)[j] < lMinVal) (*lFloatVector)[j] = lMinVal;
		}
	}
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    *lFloatVector
	);
	Beagle_StackTraceEndM();
}

