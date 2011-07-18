/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/OneCommaLambdaAdaptedOp.cpp
 *  \brief  Source code of class OneCommaLambdaAdaptedOp.
 *  \author Christian Gagne
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Build a (1,lambda_adapt) replacement strategy operator.
 *  \param inName Name of the (1,lambda_adapt) operator.
 */
OneCommaLambdaAdaptedOp::OneCommaLambdaAdaptedOp(std::string inName) :
	Beagle::ReplacementStrategyOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void OneCommaLambdaAdaptedOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	ReplacementStrategyOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Factor used to adapt lambda for (1,lambda_adapt)",
		    "Float",
		    "0.9",
		    "Factor used to change lambda value for each adaptation."
		);
		mLambdaFactorAdapt = castHandleT<Float>(
		                         ioSystem.getRegister().insertEntry("ec.1cladp.adaptfactor", new Float(0.9), lDescription));
	}
	{
		Register::Description lDescription(
		    "Adapted number of offsprings generated",
		    "UInt",
		    "5",
		    std::string("Actual adapted value of lambda, that is the number of children generated ")+
		    std::string("at each generation in the (1,lambda_adapt) remplacement strategy.")
		);
		mLambda = castHandleT<UInt>(
		              ioSystem.getRegister().insertEntry("ec.1cladp.lambda", new UInt(5), lDescription));
	}
	{
		Register::Description lDescription(
		    "Initial lambda value",
		    "UInt",
		    "5",
		    std::string("Initial value used as number of children generated at each generation in the ")+
		    std::string("(1,lambda_adapt) remplacement strategy.")
		);
		mLambdaInit = castHandleT<UInt>(
		                  ioSystem.getRegister().insertEntry("ec.1cladp.lambdainit", new UInt(5), lDescription));
	}

	{
		Register::Description lDescription(
		    "Minimum lambda value",
		    "UInt",
		    "2",
		    "Minimum lambda value possible."
		);
		mMinLambda = castHandleT<UInt>(
		                 ioSystem.getRegister().insertEntry("ec.1cladp.minlambda", new UInt(2), lDescription));
	}
	{
		Register::Description lDescription(
		    "Maximum lambda value",
		    "UInt",
		    "200",
		    "Maximum lambda value possible."
		);
		mMaxLambda = castHandleT<UInt>(
		                 ioSystem.getRegister().insertEntry("ec.1cladp.maxlambda", new UInt(200), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the (1,lambda_adapt) replacement strategy operation on a deme.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void OneCommaLambdaAdaptedOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Using (1,lambda_adapt) replacement strategy to process the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), (*this));

	Beagle_AssertM(ioDeme.size() == 1);
	Beagle_AssertM(mLambda->getWrappedValue() >= 1);

	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);

	Individual::Handle lParentIndividual = ioDeme[0];
	Individual::Handle lBestOffspring = NULL;
	unsigned int lNbOffspringsBetterThanParent = 0;

	for(unsigned int i=0; i<mLambda->getWrappedValue(); ++i) {

		// Generate new individual
		unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
		BreederNode::Handle lSelectedBreeder=getRootNode();
		for(unsigned int j=0; j<lIndexBreeder; ++j)
			lSelectedBreeder = lSelectedBreeder->getNextSibling();
		Beagle_NonNullPointerAssertM(lSelectedBreeder);
		Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
		Individual::Handle lBredIndiv =
		    lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
		Beagle_NonNullPointerAssertM(lBredIndiv);

		// See if generated individual is best so far
		if((lBredIndiv->getFitness() == NULL) || (lBredIndiv->getFitness()->isValid() == false)) {
			throw Beagle_RunTimeExceptionM("Generated children should have a valid fitness!");
		}
		if((lBestOffspring == NULL) || ((*lBredIndiv) > (*lBestOffspring))) {
			lBestOffspring = lBredIndiv;
		}
		if((*lBredIndiv) > (*lParentIndividual)) ++lNbOffspringsBetterThanParent;

	}

	// Too many good offsprings, reduce population size
	if(lNbOffspringsBetterThanParent > 1) {
		unsigned int lNewLambda = (unsigned int)std::floor(double(mLambda->getWrappedValue()) *
		                          mLambdaFactorAdapt->getWrappedValue());
		if(lNewLambda == mLambda->getWrappedValue()) --lNewLambda;
		if(lNewLambda < mMinLambda->getWrappedValue()) lNewLambda = mMinLambda->getWrappedValue();
		if(lNewLambda > mMaxLambda->getWrappedValue()) lNewLambda = mMaxLambda->getWrappedValue();
		mLambda->getWrappedValue() = lNewLambda;
		std::ostringstream lOSS;
		lOSS << "Too many offsprings better than the parent (" << lNbOffspringsBetterThanParent;
		lOSS << "), lambda value has been decreased to " << lNewLambda << " offsprings per generation";
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    lOSS.str()
		);
	}
	// No good offspring, increase population size
	else if(lNbOffspringsBetterThanParent == 0) {
		unsigned int lNewLambda = (unsigned int)std::ceil(double(mLambda->getWrappedValue()) /
		                          mLambdaFactorAdapt->getWrappedValue());
		if(lNewLambda == mLambda->getWrappedValue()) ++lNewLambda;
		if(lNewLambda < mMinLambda->getWrappedValue()) lNewLambda = mMinLambda->getWrappedValue();
		if(lNewLambda > mMaxLambda->getWrappedValue()) lNewLambda = mMaxLambda->getWrappedValue();
		mLambda->getWrappedValue() = lNewLambda;
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "No offspring better than the parent, lambda value has been increased to " << 
			lNewLambda << " offsprings per generation"
		);
	}
	// One good offspring, everything is ok
	else {
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "One offspring better than the parent is correct, lambda value (" <<
			mLambda->getWrappedValue() << ") is unchanged"
		);
	}

	// Use best children as parent for next generation
	ioDeme[0] = lBestOffspring;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void OneCommaLambdaAdaptedOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	ReplacementStrategyOp::init(ioSystem);
	Beagle_ValidateParameterM(
	    mMaxLambda->getWrappedValue() >= mMinLambda->getWrappedValue(),
	    "ec.1cladp.maxLambda",
	    "Maximum lambda value should be higher than minimum lambda value!"
	);
	mLambda->getWrappedValue() = mLambdaInit->getWrappedValue();
	if(mLambda->getWrappedValue() > mMaxLambda->getWrappedValue()) {
		mLambda->getWrappedValue() = mMaxLambda->getWrappedValue();
	}
	if(mLambda->getWrappedValue() < mMinLambda->getWrappedValue()) {
		mLambda->getWrappedValue() = mMinLambda->getWrappedValue();
	}

	Beagle_StackTraceEndM();
}
