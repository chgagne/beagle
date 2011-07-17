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
 *  \file   Beagle/FltVec/CrossoverBlendOp.cpp
 *  \brief  Source code of class FltVec::CrossoverBlendFltVec.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/FltVec.hpp"

#include <cfloat>
#include <float.h>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a float vector blend crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::FltVec::CrossoverBlendOp::CrossoverBlendOp(string inMatingPbName, string inName) :
		EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the real-valued FltVec blend crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::FltVec::CrossoverBlendOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual blend crossover prob.",
		    "Double",
		    "0.3",
		    "Real-valued FltVec blend crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lDescription));
	}
	CrossoverOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Maximum values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the maximum value is smaller than the ";
		lOSS << "float vector size, the limit used for the last values of the float vector ";
		lOSS << "is equal to the last value of the maximum value vector.";
		Register::Description lDescription(
		    "Maximum vector values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("ga.float.maxvalue", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Minimum  values assigned to vector's floats. ";
		lOSS << "Value can be a scalar, which limit the value for all float ";
		lOSS << "vector parameters, or a vector which limit the value for the parameters ";
		lOSS << "individually. If the minimum value is smaller than the ";
		lOSS << "float vector size, the limit used for the last values of the float vector ";
		lOSS << "is equal to the last value of the minimum value vector.";
		Register::Description lDescription(
		    "Minimum values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("ga.float.minvalue", new DoubleArray(1,-DBL_MAX), lDescription));
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
		                ioSystem.getRegister().insertEntry("ga.float.inc", new DoubleArray(1,0.0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Blend crossover alpha value",
		    "Double",
		    "0.5",
		    "FltVec blend crossover alpha parameter moduling amplitude of operation."
		);
		mAlpha = castHandleT<Double>(
		             ioSystem.getRegister().insertEntry("ga.cxblend.alpha", new Double(0.5), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mate two FltVec individuals for real-valued FltVec blend crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::FltVec::CrossoverBlendOp::mate(Beagle::Individual& ioIndiv1,
        Beagle::Context&    ioContext1,
        Beagle::Individual& ioIndiv2,
        Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;

	Beagle_LogDebugM(ioContext1.getSystem().getLogger(), ioIndiv1);
	Beagle_LogDebugM(ioContext1.getSystem().getLogger(), ioIndiv2);

	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		FltVec::FloatVector::Handle lFloatVector1 = castHandleT<FloatVector>(ioIndiv1[i]);
		FltVec::FloatVector::Handle lFloatVector2 = castHandleT<FloatVector>(ioIndiv2[i]);
		const unsigned int lSize = minOf<unsigned int>(lFloatVector1->size(), lFloatVector2->size());
		for(unsigned int j=0; j<lSize; ++j) {
			const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
			const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
			const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
			const double lU_i = ioContext1.getSystem().getRandomizer().rollUniform();
			const double lGamma_i = ((1.0+2.0*mAlpha->getWrappedValue())*lU_i)-mAlpha->getWrappedValue();
			const double lX1_i = (*lFloatVector1)[j];
			const double lX2_i = (*lFloatVector2)[j];
			(*lFloatVector1)[j] = ((1.0-lGamma_i)*lX1_i) + (lGamma_i*lX2_i);
			(*lFloatVector2)[j] = (lGamma_i*lX1_i) + ((1.0-lGamma_i)*lX2_i);
			if(lIncVal!=0.0) {
				(*lFloatVector1)[j] = lIncVal * round((*lFloatVector1)[j] / lIncVal);
				(*lFloatVector2)[j] = lIncVal * round((*lFloatVector2)[j] / lIncVal);
			}
			if((*lFloatVector1)[j] > lMaxVal) (*lFloatVector1)[j] = lMaxVal;
			if((*lFloatVector1)[j] < lMinVal) (*lFloatVector1)[j] = lMinVal;
			if((*lFloatVector2)[j] > lMaxVal) (*lFloatVector2)[j] = lMaxVal;
			if((*lFloatVector2)[j] < lMinVal) (*lFloatVector2)[j] = lMinVal;
		}
	}

	Beagle_LogDebugM(ioContext1.getSystem().getLogger(), ioIndiv1);
	Beagle_LogDebugM(ioContext1.getSystem().getLogger(), ioIndiv2);

	return true;
	Beagle_StackTraceEndM();
}


