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
 *  \file   Beagle/IntVec/MutationUniformQROp.cpp
 *  \brief  Source code of class IntVec::MutationUniformQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/IntVec.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct an integer vector quasi-random uniform mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inIntMutatePbName Mutation integer probability parameter name used in register.
 *  \param inName Name of the operator.
 */
IntVec::MutationUniformQROp::MutationUniformQROp(std::string inMutationPbName,
        std::string inIntMutatePbName,
        std::string inName) :
	IntVec::MutationUniformOp(inMutationPbName, inIntMutatePbName, inName)
{ }


/*!
 *  \brief Initialize the integer vector quasi-random uniform mutation operator.
 *  \param ioSystem System of the evolution.
 */
void IntVec::MutationUniformQROp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	IntVec::MutationUniformOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		if(ioSystem.getRegister().isRegistered("intvec.init.vectorsize")) {
			UInt::Handle lIntVectorSize =
			    castHandleT<UInt>(ioSystem.getRegister()["intvec.init.vectorsize"]);
			lQRComponent->reset(lIntVectorSize->getWrappedValue(), ioSystem.getRandomizer());
		} else {
			std::ostringstream lOSS;
			lOSS << "Could not post-initialize operator '" << getName() << "'. Looking for ";
			lOSS << "setting up the QuasiRandom component with a dimensionality equal to ";
			lOSS << "initial vector size parameter ('intvec.init.vectorsize'), but this parameter ";
			lOSS << "is not present in the register.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Uniformly mutate an integer vector IntVec individual.
 *  \param ioIndividual IntVec individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool IntVec::MutationUniformQROp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mIntMutateProba->getWrappedValue()>=0.0, mIntMutatePbName, "<0");

	bool lMutated = false;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "Integer uniform mutation probability is: " << dbl2str(mIntMutateProba->getWrappedValue())
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		IntVec::IntegerVector::Handle lIV = castHandleT<IntVec::IntegerVector>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Uniformly mutating the " << uint2ordinal(i+1) << " integer vector"
		);
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
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
			    "The integer vector has been mutated using quasi-random numbers"
			);
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    *lIV
			);
		} else {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The integer vector has not been mutated"
			);
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Register the parameters of the integer vector uniform mutation operator.
 *  \param ioSystem System of the evolution.
 */
void IntVec::MutationUniformQROp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	IntVec::MutationUniformOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}
