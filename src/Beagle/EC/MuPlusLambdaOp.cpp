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
 *  \file   Beagle/EC/MuPlusLambdaOp.cpp
 *  \brief  Source code of class MuPlusLambdaOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.26 $
 *  $Date: 2009/01/21 20:28:16 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Build (Mu+Lambda) replacement strategy operator.
 *  \param inLMRatioName Lamda over Mu parameter name used in the register.
 *  \param inName Name of the (Mu+Lambda) operator.
 */
MuPlusLambdaOp::MuPlusLambdaOp(std::string inLMRatioName, std::string inName) :
	ReplacementStrategyOp(inName),
	mLMRatioName(inLMRatioName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MuPlusLambdaOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	ReplacementStrategyOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "(Lambda / Mu) ratio, which mean how much bigger is the child population from ";
	lOSS << "the parent population during a (Mu+Lambda), (Mu,Lambda) or NSGA2 ";
	lOSS << "replacement process.";
	Register::Description lDescription(
	    "(Lambda / Mu) ratio",
	    "Float",
	    "7.0",
	    lOSS.str()
	);
	mLMRatio = castHandleT<Float>(
	               ioSystem.getRegister().insertEntry(mLMRatioName, new Float(7.0f), lDescription));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the (Mu+Lambda) replacement strategy operation on a deme.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void MuPlusLambdaOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mLMRatio->getWrappedValue() >= 1.0,
	                          mLMRatioName,
	                          "The LM ratio must be higher or equal to 1.0.");

	// If actual deme size is 0, do nothing
	if(ioDeme.size() == 0) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "WARNING: actual deme size is 0, do nothing and return from MuPlusLambadaOp operator."
		);
		return;
	}

	// Special case: using the (mu+lambda) only to shrink back the population.
	if(getRootNode()==NULL) {
		UIntArray::Handle lPopSize;
		if(ioContext.getSystem().getRegister().isRegistered("ec.pop.size")) {
			lPopSize = castHandleT<UIntArray>(ioContext.getSystem().getRegister()["ec.pop.size"]);
		} else {
			std::ostringstream lOSS;
			lOSS << "Population size parameter \"ec.pop.size\" is not found in register!";
			throw ValidationException(lOSS.str());
		}
		const unsigned int lDemeSize = (*lPopSize)[ioContext.getDemeIndex()];
		Beagle_AssertM(ioDeme.size() >= lDemeSize);
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		for(unsigned int i=0; i<lDemeSize; ++i) {
			std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
		}
		ioDeme.erase(ioDeme.begin(), ioDeme.begin()+(ioDeme.size()-lDemeSize));
		Beagle_AssertM(ioDeme.size()==lDemeSize);
		return;
	}

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Using (mu+lambda) replacement strategy to process the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), (*this));

	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);

	Individual::Bag lOffsprings;
	for(unsigned int i=0; i<ioDeme.size(); ++i) {
		lOffsprings.push_back(ioDeme[i]);
		std::push_heap(lOffsprings.begin(), lOffsprings.end(), IsLessPointerPredicate());
	}

	unsigned int lLambda =
	    (unsigned int)std::ceil(mLMRatio->getWrappedValue()*float(ioDeme.size()));
	for(unsigned int i=0; i<lLambda; ++i) {
		unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
		BreederNode::Handle lSelectedBreeder=getRootNode();
		for(unsigned int j=0; j<lIndexBreeder; ++j)
			lSelectedBreeder=lSelectedBreeder->getNextSibling();
		Beagle_NonNullPointerAssertM(lSelectedBreeder);
		Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
		Individual::Handle lBredIndiv =
		    lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
		Beagle_NonNullPointerAssertM(lBredIndiv);
		lOffsprings.push_back(lBredIndiv);
	}

	Beagle_AssertM(lOffsprings.size() > ioDeme.size());
	for(unsigned int i=0; i<lOffsprings.size(); ++i) {
		if((lOffsprings[i]->getFitness()==NULL) || (lOffsprings[i]->getFitness()->isValid()==false)) {
			ioDeme.clear();
			ioDeme.insert(ioDeme.end(), lOffsprings.begin(), lOffsprings.end());
			return;
		}
	}

	std::make_heap(lOffsprings.begin(), lOffsprings.end(), IsLessPointerPredicate());
	for(unsigned int j=0; j<ioDeme.size(); ++j) {
		ioDeme[j] = lOffsprings[0];
		std::pop_heap(lOffsprings.begin(), (lOffsprings.end()-j), IsLessPointerPredicate());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a MuPlusLamdaOp operator from a XML subtree.
 *  \param inIter XML iterator to use to read MuPlusLamdaOp operator.
 *  \param ioSystem Evolutionary system.
 */
void MuPlusLambdaOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lLMRatioReadName = inIter->getAttribute("ratio_name");
	if(lLMRatioReadName.empty() == false) mLMRatioName = lLMRatioReadName;

	ReplacementStrategyOp::readWithSystem(inIter, ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write MuPlusLamdaOp operator into XML streamer.
 *  \param ioStreamer XML streamer to write MuPlusLamdaOp operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void MuPlusLambdaOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	ioStreamer.insertAttribute("ratio_name", mLMRatioName);
	for(BreederNode::Handle lNode=getRootNode(); lNode!=NULL; lNode=lNode->getNextSibling()) {
		lNode->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}

