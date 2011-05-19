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
 *  \file   beagle/GA/src/InitQRIntVecOp.cpp
 *  \brief  Source code of class GA::InitQRIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a GA integer vectors quasi-random initialization operator.
 *  \param inIntVectorSize Size of the integer vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::InitQRIntVecOp::InitQRIntVecOp(unsigned int inIntVectorSize,
                                   std::string inReproProbaName,
                                   std::string inName) :
		GA::InitIntVecOp(inIntVectorSize, inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the GA integer vectors quasi-random initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitQRIntVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::InitIntVecOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the quasi-random ES initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitQRIntVecOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::InitIntVecOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		lQRComponent->reset(mIntVectorSize->getWrappedValue()+1, ioSystem.getRandomizer());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize integer-valued GA individual with numbers uniformly distributed in a given range.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void GA::InitQRIntVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mIntVectorSize->getWrappedValue() == 0) {
		string lMessage = "GA::InitQRIntVecOp::initIndividual: ";
		lMessage += "integer vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	const Factory& lFactory = ioContext.getSystem().getFactory();
	GA::IntegerVector::Alloc::Handle lIntegerVectorAlloc =
		castHandleT<GA::IntegerVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	GA::IntegerVector::Handle lIntegerVector = castHandleT<GA::IntegerVector>(lIntegerVectorAlloc->allocate());
	lIntegerVector->resize(mIntVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lIntegerVector);
	std::vector<long> lMaxValue(mIntVectorSize->getWrappedValue());
	std::vector<long> lMinValue(mIntVectorSize->getWrappedValue());
	for(unsigned int j=0; j<lIntegerVector->size(); ++j) {
		lMaxValue[j] = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
		lMinValue[j] = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
	}
	std::vector<long> lQRValues(mIntVectorSize->getWrappedValue());
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
	lQRComponent->getIntegerSequence(lQRValues, lMinValue, lMaxValue);
	for(unsigned int j=0; j<lIntegerVector->size(); ++j) (*lIntegerVector)[j] = lQRValues[j];
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GA::InitQRIntVecOp",
	    "Integer vector initialized as"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GA::InitQRIntVecOp",
	    *lIntegerVector
	);
	Beagle_StackTraceEndM();
}

