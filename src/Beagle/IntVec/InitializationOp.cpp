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
 *  \file   Beagle/IntVec/InitializationOp.cpp
 *  \brief  Source code of class IntVec::InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2008/03/20 14:01:14 $
 */

#include "Beagle/IntVec.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a integer vectors uniformly distributed initialization operator.
 *  \param inIntVectorSize Size of the integer vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
IntVec::InitializationOp::InitializationOp(unsigned int inIntVectorSize,
                                           std::string inReproProbaName,
                                           std::string inName) :
	EC::InitializationOp(inReproProbaName, inName),
	mIntVectorSize(new UInt(inIntVectorSize))
{ }


/*!
 *  \brief Initialize integer-valued individual with numbers uniformly distributed in a given range.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void IntVec::InitializationOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mIntVectorSize->getWrappedValue() == 0) {
		string lMessage = "IntVec::InitializationOp::initIndividual: ";
		lMessage += "integer vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	const Factory& lFactory = ioContext.getSystem().getFactory();
	IntVec::IntegerVector::Alloc::Handle lIntegerVectorAlloc =
		castHandleT<IntVec::IntegerVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	IntVec::IntegerVector::Handle lIntegerVector = castHandleT<IntVec::IntegerVector>(lIntegerVectorAlloc->allocate());
	lIntegerVector->resize(mIntVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lIntegerVector);
	for(unsigned int j=0; j<lIntegerVector->size(); ++j) {
		const int lMaxVal = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
		const int lMinVal = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
		Beagle_AssertM(lMaxVal >= lMinVal);
		const int lRandVal = (int)ioContext.getSystem().getRandomizer().rollInteger(0,lMaxVal-lMinVal);
		(*lIntegerVector)[j] = (lRandVal+lMinVal);
	}
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "Integer vector initialized as: " << *lIntegerVector
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Regsiter the parameters of the integer vectors uniformly distributed initialization operator.
 *  \param ioSystem System of the evolution.
 */
void IntVec::InitializationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	EC::InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Initial integer vectors sizes",
		    "UInt",
		    mIntVectorSize->serialize(),
		    "Integer vector size of initialized individuals."
		);
		mIntVectorSize = castHandleT<UInt>(
		                     ioSystem.getRegister().insertEntry("ga.init.vectorsize", mIntVectorSize, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Maximum initialization values assigned to vector's integers. ";
		lOSS << "Value can be a scalar, which limit the initialization value for all integers ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the maximum initialization value is smaller than the ";
		lOSS << "integer vector size, the limit used for the last values of the integer vector ";
		lOSS << "is equal to the last value of the maximum initialization value vector.";
		Register::Description lDescription(
		    "Maximum initialization values",
		    "IntArray",
		    int2str(INT_MAX),
		    lOSS.str()
		);
		mMaxInitValue = castHandleT<IntArray>(
		                    ioSystem.getRegister().insertEntry("ga.init.maxvalue", new IntArray(1,INT_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Minimum initialization values assigned to vector's integers. ";
		lOSS << "Value can be a scalar, which limit the initialization value for all integer ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the minimum initialization value is smaller than the ";
		lOSS << "integer vector size, the limit used for the last values of the integer vector ";
		lOSS << "is equal to the last value of the minimum initialization value vector.";
		Register::Description lDescription(
		    "Minimum initialization values",
		    "IntArray",
		    int2str(INT_MIN),
		    lOSS.str()
		);
		mMinInitValue = castHandleT<IntArray>(
		                    ioSystem.getRegister().insertEntry("ga.init.minvalue", new IntArray(1,INT_MIN), lDescription));
	}
	Beagle_StackTraceEndM();
}