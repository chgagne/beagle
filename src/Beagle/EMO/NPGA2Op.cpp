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
 *  \file   beagle/src/NPGA2Op.cpp
 *  \brief  Source code of class NPGA2Op.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Build NPGA2 selection operator.
 *  \param inName Name of the NPGA2 operator.
 */
NPGA2Op::NPGA2Op(std::string inName) :
		Operator(inName)
{ }


/*!
 *  \brief Evaluate niche count of an individual over a pool of individuals.
 *  \param inEvalIndividual Individual for which we need to evaluate crowding distance.
 *  \param inIndividualPool Pool of individuals to evaluate distance on.
 *  \return Niche count value.
 */
float NPGA2Op::evalNicheCount(const Individual& inEvalIndividual,
                              const Individual::Bag& inIndividualPool) const
{
	Beagle_StackTraceBeginM();
	double lNicheCount = 0.;
	const Fitness::Handle lEvalFitness = inEvalIndividual.getFitness();
	for(unsigned int i=0; i<inIndividualPool.size(); ++i) {
		float lDistance = lEvalFitness->getDistance(*inIndividualPool[i]->getFitness());
		if(lDistance < mNicheRadius->getWrappedValue()) {
			lNicheCount += (1.0 - (lDistance / mNicheRadius->getWrappedValue()));
		}
	}
	return lNicheCount;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Evolutionary system.
 */
void NPGA2Op::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Operator::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "NPGA2 tournaments size",
		    "UInt",
		    "2",
		    "Number of participants to tournaments in NPGA2 selection."
		);
		mNumberParticipants = castHandleT<UInt>(
		                          ioSystem.getRegister().insertEntry("ec.npga2.tournsize", new UInt(2), lDescription));
	}
	{
		Register::Description lDescription(
		    "NPGA2 niche radius",
		    "Float",
		    "1.0",
		    "Nice radius (sigma share) used to evaluate niche count in NPGA2 selection."
		);
		mNicheRadius = castHandleT<Float>(
		                   ioSystem.getRegister().insertEntry("ec.npga2.nicheradius", new Float(1.0f), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply NPGA2 multiobjective selection operator.
 *  \param ioDeme Deme on which selection operator is applied.
 *  \param ioContext Evolutionary context.
 */
void NPGA2Op::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(ioDeme.size() == 0) return;

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Applying NPGA2 multiobjective selection on the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	std::vector<bool> lAlreadySelectedIndiv(ioDeme.size(), false);
	Individual::Bag   lSelectedIndividual;
	const Factory&    lFactory = ioContext.getSystem().getFactory();

	// Generate new generation by selection
	for(unsigned int i=0; i<ioDeme.size(); ++i) {

		// First participant
		unsigned int lFirstParticipant =
		    ioContext.getSystem().getRandomizer().rollInteger(0, ioDeme.size()-1);
		std::vector<unsigned int> lNDParticipants(1, lFirstParticipant);

		// Following participants to tournament
		for(unsigned int j=1; j<mNumberParticipants->getWrappedValue(); ++j) {
			unsigned int lParticipant =
			    ioContext.getSystem().getRandomizer().rollInteger(0, ioDeme.size()-1);
			// Test if participant is dominated or dominate other participants
			bool lIsDominated = false;
			Fitness::Handle lPartFitness = ioDeme[lParticipant]->getFitness();
			for(unsigned int k=0; k<lNDParticipants.size(); ++k) {
				Fitness::Handle lFitnessNDk = ioDeme[lNDParticipants[k]]->getFitness();
				if(lPartFitness->isDominated(*lFitnessNDk)) {
					lIsDominated = true;
				} else if(lFitnessNDk->isDominated(*lPartFitness)) {
					lNDParticipants.erase(lNDParticipants.begin()+k);
				}
			}
			if(lIsDominated==false) lNDParticipants.push_back(lParticipant);
		}

		// Test if there is a tie. If so evaluate niche count.
		Beagle_AssertM(lNDParticipants.size() != 0);
		unsigned int lWinner = lNDParticipants[0];
		if(lNDParticipants.size() > 1) {
			float lLowestNicheCount = evalNicheCount(*ioDeme[lNDParticipants[0]], lSelectedIndividual);
			for(unsigned int j=1; j<lNDParticipants.size(); ++j) {
				float lNicheCount = evalNicheCount(*ioDeme[lNDParticipants[j]], lSelectedIndividual);
				if(lNicheCount < lLowestNicheCount) {
					lLowestNicheCount = lNicheCount;
					lWinner = lNDParticipants[j];
				}
			}
		}

		// Put winner in selected individual bag
		if(lAlreadySelectedIndiv[lWinner]) {
			std::string lIndividualType = ioDeme[lWinner]->getType();
			Individual::Alloc::Handle lIndividualAlloc =
				castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			Individual::Handle lIndividual = castHandleT<Individual>(lIndividualAlloc->allocate());
			lIndividual->copy(*ioDeme[lWinner], ioContext.getSystem());
			lSelectedIndividual.push_back(lIndividual);
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    uint2ordinal(lWinner+1)+" individual selected again in NPGA2 selection process"
			);
		} else {
			lSelectedIndividual.push_back(ioDeme[lWinner]);
			lAlreadySelectedIndiv[lWinner] = true;
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    uint2ordinal(lWinner+1)+" individual selected in NPGA2 selection process"
			);
		}
	}

	// Copy selected individuals into deme
	for(unsigned int j=0; j<ioDeme.size(); ++j) ioDeme[j] = lSelectedIndividual[j];
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem Evolutionary system.
 */
void NPGA2Op::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if(ioSystem.getRegister().isRegistered("ec.hof.vivasize")) {
		UInt::Handle lVivaHOFSize =
		    castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hof.vivasize"));
		if(lVivaHOFSize->getWrappedValue() > 0) {
			Beagle_LogInfoM(
			    ioSystem.getLogger(),
			    std::string("WARNING: the vivarium hall-of-fame size (parameter 'ec.hof.vivasize') ")+
			    std::string("is non-zero; the hall-of-fame is not meaningful in a multiobjective ")+
			    std::string("EA context")
			);
		}
	}
	if(ioSystem.getRegister().isRegistered("ec.hof.demesize")) {
		UInt::Handle lVivaHOFSize =
		    castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hof.demesize"));
		if(lVivaHOFSize->getWrappedValue() > 0) {
			Beagle_LogInfoM(
			    ioSystem.getLogger(),
			    std::string("WARNING: the demes hall-of-fame size (parameter 'ec.hof.demesize') ")+
			    std::string("is non-zero; the hall-of-fame is not meaningful in a multiobjective ")+
			    std::string("EA context")
			);
		}
	}

	Beagle_StackTraceEndM();
}

