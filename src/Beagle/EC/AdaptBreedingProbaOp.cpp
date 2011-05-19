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
 *  \file   Beagle/EC/AdaptBreedingProbaOp.cpp
 *  \brief  Source code of class AdaptBreedingProbaOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct an operator for adaptation of probability between variation operators.
 *  \param inName Name of the operator.
 */
AdaptBreedingProbaOp::AdaptBreedingProbaOp(std::string inName) :
	BreederOp(inName)
{ }


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
double AdaptBreedingProbaOp::getBreedingProba(BreederNode::Handle inChild)
{
	Beagle_StackTraceBeginM();
	double lSum=0.;
	for(BreederNode::Handle lNode=inChild; lNode!=NULL; lNode=lNode->getNextSibling()) {
		Beagle_NonNullPointerAssertM(lNode->getBreederOp());
		lSum += lNode->getBreederOp()->getBreedingProba(lNode->getFirstChild());
	}
	return lSum;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System of the evolution.
 */
void AdaptBreedingProbaOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	BreederOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Adapted breeding probabilities relative to child breeding variation operators.";
		Register::Description lDescription(
		    "Adapted breeding probabilities",
		    "DoubleArray",
		    "",
		    lOSS.str()
		);
		mAdaptedProbas = castHandleT<DoubleArray>(
		                     ioSystem.getRegister().insertEntry("ec.adaptbreed.proba", new DoubleArray, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Count of respective successful variations obtained from child breeding ";
		lOSS << "variation operators.";
		Register::Description lDescription(
		    "Successful variations counter",
		    "UIntArray",
		    "",
		    lOSS.str()
		);
		mSuccessCount = castHandleT<UIntArray>(
		                    ioSystem.getRegister().insertEntry("ec.adaptbreed.successcount", new UIntArray, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Count of respective variations done by child breeding variation operators.";
		Register::Description lDescription(
		    "Individual variations counter",
		    "UIntArray",
		    "",
		    lOSS.str()
		);
		mVariationCount = castHandleT<UIntArray>(
		                      ioSystem.getRegister().insertEntry("ec.adaptbreed.varcount", new UIntArray, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of individual variations before an adaptation of breeding probabilities.";
		Register::Description lDescription(
		    "Period between adaptations",
		    "UInt",
		    "20",
		    lOSS.str()
		);
		mAdaptationPeriod = castHandleT<UInt>(
		                        ioSystem.getRegister().insertEntry("ec.adaptbreed.period", new UInt(20), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Factor used to adapt breeding probabilities.";
		Register::Description lDescription(
		    "Adaptation factor",
		    "Double",
		    "0.9",
		    lOSS.str()
		);
		mAdaptationFactor = castHandleT<Double>(
		                        ioSystem.getRegister().insertEntry("ec.adaptbreed.factor", new Double(0.9), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the adaptation operation on a breeding pool.
 *  \param inBreedingPool Breeding pool to give to the underlying breeding tree.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the mutation operation.
 *  \return Mutated individual.
 */
Individual::Handle AdaptBreedingProbaOp::breed(Individual::Bag& inBreedingPool,
        BreederNode::Handle inChild,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// If breeding probabilities vector is empty, initialize it.
	if(mAdaptedProbas->size() == 0) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Getting breeding probabilities from underlying operators to initialize internal tables"
		);
		mSuccessCount->clear();
		mVariationCount->clear();
		double lSum = 0.;
		for(BreederNode::Handle lNode=inChild; lNode!=NULL; lNode=lNode->getNextSibling()) {
			Beagle_NonNullPointerAssertM(lNode->getBreederOp());
			const double lBreedProba =
			    lNode->getBreederOp()->getBreedingProba(lNode->getFirstChild());
			Beagle_AssertM(lBreedProba > 0.);
			lSum += lBreedProba;
			mAdaptedProbas->push_back(lBreedProba);
			mSuccessCount->push_back(0);
			mVariationCount->push_back(0);
		}
		if(lSum == 0.) {
			const double lInvSize = 1. / double(mAdaptedProbas->size());
			for(unsigned int i=0; i<mAdaptedProbas->size(); ++i) (*mAdaptedProbas)[i] = lInvSize;
		} else {
			for(unsigned int i=0; i<mAdaptedProbas->size(); ++i) (*mAdaptedProbas)[i] /= lSum;
		}
	}
	Beagle_AssertM(mAdaptedProbas->size() == mSuccessCount->size());
	Beagle_AssertM(mAdaptedProbas->size() == mVariationCount->size());

	// Get the variation operation to do from breeding probabilities and generate new individual.
	Individual::Handle lIndiv = NULL;
	const double lRndSelVarOp = ioContext.getSystem().getRandomizer().rollUniform(0.,1.);
	unsigned int lIndexThisVarOp = 0;
	double lSum = 0.;
	for(BreederNode::Handle lNode=inChild; lNode!=NULL; lNode=lNode->getNextSibling()) {
		Beagle_NonNullPointerAssertM(lNode->getBreederOp());
		lSum += (*mAdaptedProbas)[lIndexThisVarOp];
		if(lRndSelVarOp <= lSum) {
			std::ostringstream lOSS;
			lOSS << "Selecting '" << lNode->getBreederOp()->getName();
			lOSS << "' as variation operator";
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    lOSS.str()
			);
			lIndiv = lNode->getBreederOp()->breed(inBreedingPool, lNode->getFirstChild(), ioContext);
			break;
		} else ++lIndexThisVarOp;
	}

	// Look whether the mutation is successful.
	++(*mVariationCount)[lIndexThisVarOp];
	Fitness::Handle lFitParent = inBreedingPool[ioContext.getIndividualIndex()]->getFitness();
	Fitness::Handle lFitChild  = lIndiv->getFitness();
	if(lFitParent->isLess(*lFitChild)) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Variation operation successful, the offspring's fitness is better than the parent's fitness"
		);
		++(*mSuccessCount)[lIndexThisVarOp];
	} else {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Variation operation not successful, the offspring's fitness is worse than or equal to the parent's fitness"
		);
	}

	// Look if there is two variations operator that generated at least mAdaptPeriod variations.
	const unsigned int lAdaptPeriod = mAdaptationPeriod->getWrappedValue();
	if((*mVariationCount)[lIndexThisVarOp] >= lAdaptPeriod) {
		unsigned int lIndexOtherVarOp = 0;
		for(; lIndexOtherVarOp<mVariationCount->size(); ++lIndexOtherVarOp) {
			if(lIndexOtherVarOp == lIndexThisVarOp) continue;
			if((*mVariationCount)[lIndexOtherVarOp] >= lAdaptPeriod) break;
		}
		if(lIndexOtherVarOp != mVariationCount->size()) {
			const double lRatioThisSelVar = double((*mSuccessCount)[lIndexThisVarOp]) /
			                                double((*mVariationCount)[lIndexThisVarOp]);
			const double lRatioOtherSelVar = double((*mSuccessCount)[lIndexOtherVarOp]) /
			                                 double((*mVariationCount)[lIndexOtherVarOp]);
			(*mSuccessCount)[lIndexThisVarOp] = 0;
			(*mSuccessCount)[lIndexOtherVarOp] = 0;
			(*mVariationCount)[lIndexThisVarOp] = 0;
			(*mVariationCount)[lIndexOtherVarOp] = 0;
			if(lRatioThisSelVar > lRatioOtherSelVar) {
				(*mAdaptedProbas)[lIndexThisVarOp]  /= mAdaptationFactor->getWrappedValue();
				(*mAdaptedProbas)[lIndexOtherVarOp] *= mAdaptationFactor->getWrappedValue();
			} else if(lRatioThisSelVar < lRatioOtherSelVar) {
				(*mAdaptedProbas)[lIndexThisVarOp]  *= mAdaptationFactor->getWrappedValue();
				(*mAdaptedProbas)[lIndexOtherVarOp] /= mAdaptationFactor->getWrappedValue();
			} else if((*mAdaptedProbas)[lIndexThisVarOp] > (*mAdaptedProbas)[lIndexOtherVarOp]) {
				(*mAdaptedProbas)[lIndexThisVarOp]  *= mAdaptationFactor->getWrappedValue();
			} else if((*mAdaptedProbas)[lIndexThisVarOp] < (*mAdaptedProbas)[lIndexOtherVarOp]) {
				(*mAdaptedProbas)[lIndexOtherVarOp] *= mAdaptationFactor->getWrappedValue();
			}
			std::ostringstream lOSS;
			lOSS << "Adapting breeding probabilities of underlying variation operators, ";
			lOSS << "new probability values are: (";
			double lSumProbas = 0.;
			for(unsigned int i=0; i<mAdaptedProbas->size(); ++i) {
				lSumProbas += (*mAdaptedProbas)[i];
			}
			for(unsigned int i=0; i<mAdaptedProbas->size(); ++i) {
				(*mAdaptedProbas)[i] /= lSumProbas;
				lOSS << (*mAdaptedProbas)[i];
				if(i != (mAdaptedProbas->size()-1)) lOSS << ',';
				else lOSS << ")";
			}
			Beagle_LogDetailedM(
			    ioContext.getSystem().getLogger(),
			    lOSS.str()
			);
		}
	}

	return lIndiv;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the adaptation operation on the deme.
 *  \param ioDeme Current deme of individuals to mutate.
 *  \param ioContext Context of the evolution.
 */
void AdaptBreedingProbaOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("operate", "AdaptBreedingProbaOp", getName());
	Beagle_StackTraceEndM();
}

