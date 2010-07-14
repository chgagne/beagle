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
 *  \file   Beagle/EC/SelectRouletteOp.cpp
 *  \brief  Source code of class SelectRouletteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a proportionnal roulette selection operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the proportionnal roulette selection operator.
 */
SelectRouletteOp::SelectRouletteOp(std::string inReproProbaName, std::string inName) :
		Beagle::EC::SelectionOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Apply roulette selection operation.
 *  \param ioDeme Deme on which roulette selection is applied.
 *  \param ioContext Evolutionary context.
 */
void SelectRouletteOp::selectManyIndividuals(unsigned int inN,
	                                         Beagle::Individual::Bag& ioPool,
	                                         Beagle::Context& ioContext,
	                                         std::vector<unsigned int>& outSelections)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "selection", "Beagle::SelectionOp",
	    std::string("Selecting ")+uint2str(inN)+" individuals from a pool of "+uint2str(ioPool.size())+
	    " individuals"
	);
	outSelections.clear();
	outSelections.resize(inN, 0);
	if(inN == 0) return;
	if(ioPool.size() == 0) {
		throw Beagle_RunTimeExceptionM("Pool for selection is empty!");	
	}
	RouletteT<unsigned int> lRoulette;
	for(unsigned int i=0; i<ioContext.getDeme().size(); ++i) {
		Beagle_NonNullPointerAssertM(ioContext.getDeme()[i]);
		FitnessSimple::Handle lFitness = castHandleT<FitnessSimple>(ioContext.getDeme()[i]->getFitness());
		lRoulette.insert(i, lFitness->getValue());
	}
	lRoulette.optimize();
	for(unsigned int i=0; i<ioPool.size(); ++i) {
		Beagle_NonNullPointerAssertM(ioPool[i]);
		FitnessSimple::Handle lFitness = castHandleT<FitnessSimple>(ioPool[i]->getFitness());
		lRoulette.insert(i, lFitness->getValue());
	}
	for(unsigned int i=0; i<inN; ++i) {
		outSelections[ lRoulette.select(ioContext.getSystem().getRandomizer()) ]++;
	}
	Beagle_StackTraceEndM("void SelectRouletteOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Select an individual using proportional roulette selection.
 *  \param ioPool Pool of individuals on which roulette selection is applied.
 *  \param ioContext Evolutionary context.
 *  \warning To use roulette selection, every individuals composing the population must have
 *    non-negative fitness value. Roulette selection is valid only with FitnessSimple object
 *    and derived (FitnessSimpleMin, FitnessKoza).
 */
unsigned int SelectRouletteOp::selectOneIndividual(Individual::Bag& ioPool, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(ioPool.size() == 0) {
		throw Beagle_RunTimeExceptionM("Pool for selection is empty!");	
	}
	RouletteT<unsigned int> lRoulette;
	for(unsigned int i=0; i<ioContext.getDeme().size(); ++i) {
		Beagle_NonNullPointerAssertM(ioContext.getDeme()[i]);
		FitnessSimple::Handle lFitness = castHandleT<FitnessSimple>(ioContext.getDeme()[i]->getFitness());
		lRoulette.insert(i, lFitness->getValue());
	}
	for(unsigned int i=0; i<ioPool.size(); ++i) {
		Beagle_NonNullPointerAssertM(ioPool[i]);
		FitnessSimple::Handle lFitness = castHandleT<FitnessSimple>(ioPool[i]->getFitness());
		lRoulette.insert(i, lFitness->getValue());
	}
	return lRoulette.select(ioContext.getSystem().getRandomizer());
	Beagle_StackTraceEndM("unsigned int SelectRouletteOp::selectIndividual(Individual::Bag& ioPool, Context& ioContext)");
}

