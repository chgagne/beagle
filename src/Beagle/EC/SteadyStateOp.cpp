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
 *  \file   Beagle/EC/SteadyStateOp.cpp
 *  \brief  Source code of class SteadyStateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

#include <algorithm>

using namespace std;
using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a steady state operator.
 *  \param inName Name of the operator.
 */
SteadyStateOp::SteadyStateOp(std::string inName) :
		Beagle::ReplacementStrategyOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void SteadyStateOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::ReplacementStrategyOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Elitism keep size",
	    "UInt",
	    "1",
	    "Number of individuals keep as is with strong n-elitism."
	);
	mElitismKeepSize = castHandleT<UInt>(
	                       ioSystem.getRegister().insertEntry("ec.elite.keepsize", new UInt(1), lDescription));

	Beagle_StackTraceEndM("void SteadyStateOp::registerParams(System&)");
}


/*!
 *  \brief Apply the operation on a deme in the given context.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void SteadyStateOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(getRootNode());
	Beagle_NonNullPointerAssertM(mElitismKeepSize);
	Beagle_ValidateParameterM(mElitismKeepSize->getWrappedValue() <= ioDeme.size(),
	                          "ec.elite.keepsize",
	                          "The elistism keepsize must be less than the deme size!");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "replacement-strategy", "Beagle::SteadyStateOp",
	    string("Processing using steady-state replacement strategy the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogObjectM(
	    ioContext.getSystem().getLogger(),
	    Logger::eTrace,
	    "replacement-strategy", "Beagle::SteadyStateOp",
	    (*this)
	);

	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);

	if(mElitismKeepSize->getWrappedValue() > 0) {
		History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
		make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
		for(unsigned int i=0; i<mElitismKeepSize->getWrappedValue(); ++i) {
			if (lHistory != NULL) {
				std::vector<HistoryID> lParent;
				HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[0]->getMember("HistoryID"));
				if(lHID != NULL) lParent.push_back(*lHID);
				lHistory->allocateNewID(*ioDeme[0]);
				lHistory->trace(ioContext, lParent, ioDeme[0], getName(), "elitism");
			}
			pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
		}
	}

	random_shuffle(ioDeme.begin(),
	               ioDeme.end()-mElitismKeepSize->getWrappedValue(),
	               ioContext.getSystem().getRandomizer());

	for(unsigned int i=0; i<(ioDeme.size()-mElitismKeepSize->getWrappedValue()); ++i) {
		unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
		BreederNode::Handle lSelectedBreeder=getRootNode();
		for(unsigned int j=0; j<lIndexBreeder; ++j)
			lSelectedBreeder=lSelectedBreeder->getNextSibling();
		Beagle_NonNullPointerAssertM(lSelectedBreeder);
		Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
		Individual::Handle lBredIndiv =
		    lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
		Beagle_NonNullPointerAssertM(lBredIndiv);
		ioDeme[i] = lBredIndiv;
	}
	Beagle_StackTraceEndM("void SteadyStateOp::operate(Deme& ioDeme, Context& ioContext)");
}
