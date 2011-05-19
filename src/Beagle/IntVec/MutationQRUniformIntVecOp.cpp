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
 *  \file   beagle/GA/src/MutationQRUniformIntVecOp.cpp
 *  \brief  Source code of class GA::MutationQRUniformIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct an integer vector GA quasi-random mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inIntMutatePbName Mutation integer probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::MutationQRUniformIntVecOp::MutationQRUniformIntVecOp(std::string inMutationPbName,
        std::string inIntMutatePbName,
        std::string inName) :
		GA::MutationUniformIntVecOp(inMutationPbName, inIntMutatePbName, inName)
{ }


/*!
 *  \brief Register the parameters of the integer vector GA uniform mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationQRUniformIntVecOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::MutationUniformIntVecOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the derandomized GA integer vector quasi-random mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationQRUniformIntVecOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GA::MutationUniformIntVecOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		if(ioSystem.getRegister().isRegistered("ga.init.vectorsize")) {
			UInt::Handle lIntVectorSize =
			    castHandleT<UInt>(ioSystem.getRegister()["ga.init.vectorsize"]);
			lQRComponent->reset(lIntVectorSize->getWrappedValue(), ioSystem.getRandomizer());
		} else {
			std::ostringstream lOSS;
			lOSS << "Could not post-initialize operator '" << getName() << "'. Looking for ";
			lOSS << "setting up the QuasiRandom component with a dimensionality equal to ";
			lOSS << "initial vector size parameter ('ga.init.vectorsize'), but this parameter ";
			lOSS << "is not present in the register.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Uniformly mutate an integer vector GA individual.
 *  \param ioIndividual GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GA::MutationQRUniformIntVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mIntMutateProba->getWrappedValue()>=0.0, mIntMutatePbName, "<0");

	bool lMutated = false;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GA::MutationQRUniformIntVecOp",
	    std::string("Integer uniform mutation probability is: ")+
	    dbl2str(mIntMutateProba->getWrappedValue())
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		GA::IntegerVector::Handle lIV = castHandleT<GA::IntegerVector>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GA::MutationQRUniformIntVecOp",
		    std::string("Uniformly mutating the ")+uint2ordinal(i+1)+" integer vector"
		);
		Beagle_LogObjectDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GA::MutationQRUniformIntVecOp",
		    *lIV
		);

		std::vector<long> lMaxValue(lIV->size());
		std::vector<long> lMinValue(lIV->size());
		for(unsigned int j=0; j<lIV->size(); ++j) {
			lMaxValue[j] = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
			lMinValue[j] = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
		}

		std::vector<long> lQRValues(lIV->size());
		QuasiRandom::Handle lQRComponent =
		    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
		lQRComponent->getIntegerSequence(lQRValues, lMinValue, lMaxValue);
		for(unsigned int j=0; j<lIV->size(); j++) {
			double lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
			if(lRolledPb <= mIntMutateProba->getWrappedValue()) {
				(*lIV)[j] = lQRValues[j];
				lMutated = true;
			}
		}

		if(lMutated) {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GA::MutationQRUniformIntVecOp",
			    std::string("The integer vector has been mutated using quasi-random numbers")
			);
			Beagle_LogObjectDebugM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GA::MutationQRUniformIntVecOp",
			    *lIV
			);
		} else {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GA::MutationQRUniformIntVecOp",
			    std::string("The integer vector has not been mutated")
			);
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}
