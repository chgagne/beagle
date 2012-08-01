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
 *  \file   Beagle/IntVec/CrossoverOrderedOp.cpp
 *  \brief  Source code of class IntVec::CrossoverOrderedOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>


/*!
 *  \brief Construct a IntVec ordered crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::IntVec::CrossoverOrderedOp::CrossoverOrderedOp(std::string inMatingPbName, std::string inName):
	EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Mate two indice integer vector individuals for ordered crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::IntVec::CrossoverOrderedOp::mate(Beagle::Individual& ioIndiv1,
										      Beagle::Context&    ioContext1,
										      Beagle::Individual& ioIndiv2,
										      Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (before IntVec ordered crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		IntVec::IntegerVector::Handle lIndividual1 = castHandleT<IntegerVector>(ioIndiv1[i]);
		IntVec::IntegerVector::Handle lIndividual2 = castHandleT<IntegerVector>(ioIndiv2[i]);
		
		unsigned int lSize = minOf<unsigned int>(lIndividual1->size(), lIndividual2->size());
		unsigned int a = ioContext1.getSystem().getRandomizer().rollInteger(0, lSize-1);
		unsigned int b = ioContext1.getSystem().getRandomizer().rollInteger(0, lSize-1);
		
		if(a > b){std::swap(a, b);}
		bool* lHoleSet1 = new bool[lSize];
		bool* lHoleSet2 = new bool[lSize];
		for(unsigned int j = 0; j < lSize; ++j){
			if(j < a || j > b){
				lHoleSet1[(*lIndividual2)[j]] = false;
				lHoleSet2[(*lIndividual1)[j]] = false;
			}else{
				lHoleSet1[(*lIndividual2)[j]] = true;
				lHoleSet2[(*lIndividual1)[j]] = true;
			}
		}
		std::vector<unsigned int> lChild1(lSize);
		std::vector<unsigned int> lChild2(lSize);
		unsigned int k1 = b + 1, k2 = b + 1;
		for(unsigned int j = 0; j < lSize; ++j){
			if(!lHoleSet1[(*lIndividual1)[(j + b + 1)%lSize]]){
				lChild1[k1%lSize] = (*lIndividual1)[(j + b + 1)%lSize];
				++k1;
			}
			
			if(!lHoleSet2[(*lIndividual2)[(j + b + 1)%lSize]]){
				lChild2[k2%lSize] = (*lIndividual2)[(j + b + 1)%lSize];
				++k2;
			}
		}
		// Transfer the child in the ouput individuals
		for(unsigned int j = 0; j < lSize; ++j){
			if(j < a || j > b){
				(*lIndividual1)[j] = lChild1[j];
				(*lIndividual2)[j] = lChild2[j];
			}else{
				std::swap((*lIndividual1)[j], (*lIndividual2)[j]);
			}
		}
		
		delete[] lHoleSet1;
		delete[] lHoleSet2;
	}
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (after IntVec ordered crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the integer vector ordered crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::IntVec::CrossoverOrderedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription("Individual ordered crossover pb.",
										   "Double",
										   "0.3",
										   "IntVec ordered crossover probability of a single individual.");
		mMatingProba = castHandleT<Double>(ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3), lDescription));
	}
	CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}
