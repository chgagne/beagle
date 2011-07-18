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
 *  \file   Beagle/FltVec/MutationGaussianOp.cpp
 *  \brief  Source code of class FltVec::MutationGaussianOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/FltVec.hpp"

#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a real-valued GA Gaussian mutation operator.
 *  \param inMutationPbName Gaussian mutation probability parameter name used in register.
 *  \param inMutateFloatPbName Single value Gaussian mutation probability name used in the register.
 *  \param inMutateGaussMuName Gaussian mutation mean value parameter name used in register.
 *  \param inMutateGaussSigmaName Gaussian mutation std deviation parameter name used in register.
 *  \param inName Name of the operator.
 */
FltVec::MutationGaussianOp::MutationGaussianOp(std::string inMutationPbName,
        std::string inMutateFloatPbName,
        std::string inMutateGaussMuName,
        std::string inMutateGaussSigmaName,
        std::string inName) :
		MutationOp(inMutationPbName, inName),
		mMutateFloatPbName(inMutateFloatPbName),
		mMutateGaussMuName(inMutateGaussMuName),
		mMutateGaussSigmaName(inMutateGaussSigmaName)
{ }


/*!
 *  \brief Register the parameters of the real-valued Gaussian mutation operator.
 *  \param ioSystem System of the evolution.
 */
void FltVec::MutationGaussianOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Indiv. Gaussian mutation prob.",
		    "Double",
		    "1.0",
		    "Gaussian mutation probability for each real-valued individual."
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(1.0f), lDescription));
	}
	MutationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Value's Gaussian mutation prob.",
		    "Float",
		    "0.1",
		    "Single value Gaussian mutation probability for the real-valued individual."
		);
		mMutateFloatPb = castHandleT<Float>(
		                     ioSystem.getRegister().insertEntry(mMutateFloatPbName, new Float(0.1f), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Mean of values added to real-values vector numbers at each Gaussian mutation. ";
		lOSS << "Value can be a scalar, which state the mu value for all float ";
		lOSS << "vector parameters, or a vector which state the mu for the parameters ";
		lOSS << "individually. If the mu vector size is smaller than the ";
		lOSS << "float vector size, the mu used for the last values of the float vector ";
		lOSS << "is equal to the last value of the mu vector.";
		Register::Description lDescription(
		    "Gaussian mutation mean",
		    "DoubleArray",
		    "0.0",
		    lOSS.str()
		);
		mMutateGaussMu = castHandleT<DoubleArray>(
		                     ioSystem.getRegister().insertEntry(mMutateGaussMuName, new DoubleArray(1,0.0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Standard deviation of values added to real-values vector numbers at ";
		lOSS << "each Gaussian mutation. Value can be a scalar, which state the sigma for ";
		lOSS << "all float vector parameters, or a vector which state the sigma ";
		lOSS << "for the parameters individually. If the sigma vector size is smaller than the ";
		lOSS << "float vector size, the sigma used for the last values of the ";
		lOSS << "float vector is equal to the last value of the sigma vector.";
		Register::Description lDescription(
		    "Gaussian mutation std deviation",
		    "DoubleArray",
		    "0.1",
		    lOSS.str()
		);
		mMutateGaussSigma = castHandleT<DoubleArray>(
		                        ioSystem.getRegister().insertEntry(mMutateGaussSigmaName, new DoubleArray(1,0.1), lDescription));
	}
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
		                ioSystem.getRegister().insertEntry("fltvec.float.maxvalue", new DoubleArray(1,DBL_MAX), lDescription));
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
		                ioSystem.getRegister().insertEntry("fltvec.float.minvalue", new DoubleArray(1,-DBL_MAX), lDescription));
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
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Gaussian mutate a real-valued GA individual.
 *  \param ioIndividual Real-valued GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool FltVec::MutationGaussianOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	bool lMutated = false;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    string("Gaussian mutations with mean of ")+
	    mMutateGaussMu->serialize()+
	    string(", and standard deviation of ")+
	    mMutateGaussSigma->serialize()
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		FltVec::FloatVector::Handle lVector = castHandleT<FltVec::FloatVector>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    string("Gaussian mutation the ")+uint2ordinal(i+1)+" float vector"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
		for(unsigned int j=0; j<lVector->size(); j++) {
			const float lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
			if(lRolledPb <= mMutateFloatPb->getWrappedValue()) {
				const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
				const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
				const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
				const double lMu =
				    j<mMutateGaussMu->size() ? (*mMutateGaussMu)[j] : mMutateGaussMu->back();
				const double lSigma =
				    j<mMutateGaussSigma->size() ? (*mMutateGaussSigma)[j] : mMutateGaussSigma->back();
				Beagle_AssertM(lSigma>=0.0);
				const double lMValue = ioContext.getSystem().getRandomizer().rollGaussian(lMu,lSigma);
				(*lVector)[j] += lMValue;
				if((*lVector)[j] > lMaxVal) (*lVector)[j] = lMaxVal;
				if((*lVector)[j] < lMinVal) (*lVector)[j] = lMinVal;
				if(std::fabs(lIncVal)>1e-12) {
					(*lVector)[j] = lIncVal * round((*lVector)[j] / lIncVal);
					if((*lVector)[j] > lMaxVal) (*lVector)[j] -= lIncVal;
					if((*lVector)[j] < lMinVal) (*lVector)[j] += lIncVal;
				}
				lMutated = true;
				Beagle_LogDebugM(
				    ioContext.getSystem().getLogger(),
				    string("Gaussian mutating by adding ")+dbl2str(lMValue)+
				    string(" to the value at the index ")+uint2str(j)+
				    string(" of the float vector")
				);
			}
		}
		if(lMutated) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The float vector has been mutated"
			);
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
		} else {
			Beagle_LogVerboseM(ioContext.getSystem().getLogger(), "The float vector has not been mutated");
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a Gaussian mutation operator from a XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param ioSystem Evolutionary system.
 */
void FltVec::MutationGaussianOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	string lMutationPbReadName = inIter->getAttribute("mutationpb");
	if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
	string lMutateFloatPbReadName = inIter->getAttribute("mutfloatpb");
	if(lMutateFloatPbReadName.empty() == false) mMutateFloatPbName = lMutateFloatPbReadName;
	string lMutateGaussMuReadName = inIter->getAttribute("mutgaussmu");
	if(lMutateGaussMuReadName.empty() == false) mMutateGaussMuName = lMutateGaussMuReadName;
	string lMutateGaussSigmaReadName = inIter->getAttribute("mutgausssigma");
	if(lMutateGaussSigmaReadName.empty() == false) mMutateGaussSigmaName = lMutateGaussSigmaReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write Gaussian mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void FltVec::MutationGaussianOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	EC::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("mutfloatpb", mMutateFloatPbName);
	ioStreamer.insertAttribute("mutgaussmu", mMutateGaussMuName);
	ioStreamer.insertAttribute("mutgausssigma", mMutateGaussSigmaName);
	Beagle_StackTraceEndM();
}


