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
 *  \file   Beagle/IntVec/CrossoverNWOXOp.cpp
 *  \brief  Source code of class IntVec::CrossoverNWOXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>


/*!
 *  \brief Construct an integer vector non-wrapping ordered crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::IntVec::CrossoverNWOXOp::CrossoverNWOXOp(std::string inMatingPbName,
											     std::string inName):
	EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the integer vector non-wrapping ordered crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::IntVec::CrossoverNWOXOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription("Individual non-wrapping ordered crossover pb.",
										   "Double",
										   "0.3",
										   "Integer vector non-wrapping ordered crossover probability of a single individual.");
		mMatingProba = castHandleT<Double>(ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3), lDescription));
	}
	EC::CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mate two indice integer vector individuals for non-wrapping ordered crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::IntVec::CrossoverNWOXOp::mate(Beagle::Individual& ioIndiv1,
									   	   Beagle::Context&    ioContext1,
									       Beagle::Individual& ioIndiv2,
									       Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (before integer vector non-wrapping ordered crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		IntegerVector::Handle lIndividual1 = castHandleT<IntegerVector>(ioIndiv1[i]);
		IntegerVector::Handle lIndividual2 = castHandleT<IntegerVector>(ioIndiv2[i]);
		
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
		std::vector<unsigned int> lChild1;
		std::vector<unsigned int> lChild2;
		for(unsigned int j = 0; j < lSize; ++j){
			if(!lHoleSet1[(*lIndividual1)[j]])
				lChild1.push_back((*lIndividual1)[j]);
			
			if(!lHoleSet2[(*lIndividual2)[j]])
				lChild2.push_back((*lIndividual2)[j]);
		}
		lChild1.insert(lChild1.begin() + a, lIndividual2->begin() + a, lIndividual2->begin() + (b + 1));
		lChild2.insert(lChild2.begin() + a, lIndividual1->begin() + a, lIndividual1->begin() + (b + 1));
		
		// Transfer the child in the output individuals
		for(unsigned int j = 0; j < lSize; ++j){
			(*lIndividual1)[j] = lChild1[j];
			(*lIndividual2)[j] = lChild2[j];
		}
		
		delete[] lHoleSet1;
		delete[] lHoleSet2;
	}
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (after integer vector non-wrapping ordered crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	return true;
	Beagle_StackTraceEndM();
}
