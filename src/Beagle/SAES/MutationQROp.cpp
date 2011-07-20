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
 *  \file   beagle/SAES/src/MutationQROp.cpp
 *  \brief  Source code of class SAES::MutationQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/SAES.hpp"

#include <cfloat>
#include <cmath>
#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a ES derandomized mutation operator.
 *  \param inMutationPbName ES mutation probability parameter name used in register.
 *  \param inMinStrategyName Minimum strategy parameter parameter name.
 *  \param inName Name of the operator.
 */
SAES::MutationQROp::MutationQROp(std::string inMutationPbName,
        std::string inMinStrategyName,
        std::string inName) :
		SAES::MutationOp(inMutationPbName, inMinStrategyName, inName)
{ }


/*!
 *  \brief Register the parameters of the quasi-random ES mutation operator.
 *  \param ioSystem System of the evolution.
 */
void SAES::MutationQROp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	SAES::MutationOp::registerParams(ioSystem);
	Component::Handle lQRComponent = ioSystem.haveComponent("QuasiRandom");
	if(lQRComponent == NULL) ioSystem.addComponent(new QuasiRandom);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the quasi-random ES mutation operator.
 *  \param ioSystem System of the evolution.
 */
void SAES::MutationQROp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	SAES::MutationOp::init(ioSystem);
	QuasiRandom::Handle lQRComponent =
	    castHandleT<QuasiRandom>(ioSystem.getComponent("QuasiRandom"));
	if(lQRComponent->getDimensionality() == 0) {
		if(ioSystem.getRegister().isRegistered("es.init.vectorsize")) {
			UInt::Handle lVectorSize =
			    castHandleT<UInt>(ioSystem.getRegister()["es.init.vectorsize"]);
			lQRComponent->reset(lVectorSize->getWrappedValue()+1, ioSystem.getRandomizer());
		} else {
			std::ostringstream lOSS;
			lOSS << "Could not post-initialize operator '" << getName() << "'. Looking for ";
			lOSS << "setting up the QuasiRandom component with a dimensionality equal to ";
			lOSS << "initial vector size parameter ('es.init.vectorsize') plus one, but this ";
			lOSS << "parameter is not present in the register.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mutate an SA-ES individual.
 *  \param ioIndividual SA-ES individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True as individual are always mutated.
 */
bool SAES::MutationQROp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "Applying SA-ES derandomized mutation to an individual"
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		PairVector::Handle lVector = castHandleT<PairVector>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Mutating the " << uint2ordinal(i+1) << " SA-ES vector"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);

		Vector lQRValues(lVector->size()+1);
		QuasiRandom::Handle lQRComponent =
		    castHandleT<QuasiRandom>(ioContext.getSystem().getComponent("QuasiRandom"));
		lQRComponent->getGaussianVector(lQRValues);
		Beagle_AssertM((lVector->size()+1) == lQRValues.size());

		const double lT = 1.0 / std::sqrt(2.0 * std::sqrt(double(lVector->size())));
		const double lTPrime = 1.0 / std::sqrt(2.0 * lVector->size());
		const double lN = lQRValues[lVector->size()];
		for(unsigned int j=0; j<lVector->size(); j++) {
			const double lMaxVal      = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
			const double lMinVal      = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
			const double lMinStrategy = j<mMinStrategy->size() ? (*mMinStrategy)[j] : mMinStrategy->back();
			const double lNi = lQRValues[j];

			(*lVector)[j].mStrategy *= std::exp((lTPrime * lN) + (lT * lNi));
			if((*lVector)[j].mStrategy < lMinStrategy) (*lVector)[j].mStrategy = lMinStrategy;
			(*lVector)[j].mValue += (*lVector)[j].mStrategy * lNi;
			if((*lVector)[j].mValue > lMaxVal) (*lVector)[j].mValue = lMaxVal;
			if((*lVector)[j].mValue < lMinVal) (*lVector)[j].mValue = lMinVal;

			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "SA-ES mutating by adding " << (*lVector)[j].mStrategy * lNi <<
			    " to the value and multiplying the strategy by " <<
			    std::exp((lTPrime * lN) + (lT * lNi)) <<
			    " to mutate the pair " << j << " of the ES vector"
			);
		}

		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
	}
	return true;
	Beagle_StackTraceEndM();
}

