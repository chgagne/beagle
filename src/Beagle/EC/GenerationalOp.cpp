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
 *  \file   Beagle/EC/GenerationalOp.cpp
 *  \brief  Source code of class GenerationalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Build generational operator.
 *  \param inName Name of the generational operator.
 */
GenerationalOp::GenerationalOp(std::string inName) :
	Beagle::ReplacementStrategyOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void GenerationalOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	ReplacementStrategyOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Elitism keep size",
	    "UInt",
	    "1",
	    "Number of individuals keep as is with strong n-elitism."
	);
	mElitismKeepSize = castHandleT<UInt>(
	                       ioSystem.getRegister().insertEntry("ec.elite.keepsize", new UInt(1), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the operation on a deme in the given context.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void GenerationalOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(getRootNode());
	Beagle_NonNullPointerAssertM(mElitismKeepSize);
	Beagle_ValidateParameterM(mElitismKeepSize->getWrappedValue() <= ioDeme.size(),
	                          "ec.elite.keepsize",
	                          "The elistism keepsize must be less than the deme size!");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "Processing using generational replacement strategy the " <<
	    uint2ordinal(ioContext.getDemeIndex()+1) << " deme"
	);
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), (*this));

	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);

	Individual::Bag lOffsprings;
	const Factory& lFactory = ioContext.getSystem().getFactory();
	if(mElitismKeepSize->getWrappedValue() > 0) {
		History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
		std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		for(unsigned int i=0; i<mElitismKeepSize->getWrappedValue(); ++i) {
			std::string lIndividualType = ioDeme[0]->getType();
			Individual::Alloc::Handle lIndividualAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			Individual::Handle lEliteIndiv = castHandleT<Individual>(lIndividualAlloc->allocate());
			lEliteIndiv->copy(*ioDeme[0], ioContext.getSystem());
			lOffsprings.push_back(lEliteIndiv);
			if(lHistory != NULL) {
				HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[0]->getMember("HistoryID"));
				std::vector<HistoryID> lParent;
				if(lHID != NULL) lParent.push_back(*lHID);
				lHistory->allocateNewID(*lEliteIndiv);
				lHistory->trace(ioContext, lParent, lEliteIndiv, getName(), "elitism");
			}
			std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
		}
	}

	for(unsigned int i=mElitismKeepSize->getWrappedValue(); i<ioDeme.size(); ++i) {
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

	for(unsigned int j=0; j<lOffsprings.size(); ++j) ioDeme[j] = lOffsprings[j];
	Beagle_StackTraceEndM();
}


