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
 *  \file   Beagle/IntVec/CrossoverPMXOp.cpp
 *  \brief  Source code of class IntVec::CrossoverPMXOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>

/*!
 *  \brief Construct a IntVec partialy matched crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::IntVec::CrossoverPMXOp::CrossoverPMXOp(std::string inMatingPbName,
										       std::string inName):
	EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Mate two indice integer vector integer vector individuals for partially matched crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::IntVec::CrossoverPMXOp::mate(Beagle::Individual& ioIndiv1,
							   		      Beagle::Context&    ioContext1,
									      Beagle::Individual& ioIndiv2,
									      Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (before integer vector partially matched crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		IntVec::IntegerVector::Handle lIndividual1 = castHandleT<IntegerVector>(ioIndiv1[i]);
		IntVec::IntegerVector::Handle lIndividual2 = castHandleT<IntegerVector>(ioIndiv2[i]);
		
		std::deque<int> lIndividual1Copy(lIndividual1->begin(), lIndividual1->end());
		std::deque<int> lIndividual2Copy(lIndividual2->begin(), lIndividual2->end());
		
		unsigned int lSize = minOf<unsigned int>(lIndividual1->size(), lIndividual2->size());
		unsigned int a = ioContext1.getSystem().getRandomizer().rollInteger(0, lSize-1);
		unsigned int b = ioContext1.getSystem().getRandomizer().rollInteger(0, lSize-1);
		
		if(a > b){std::swap(a, b);}
		for(unsigned int j = a; j <= b; ++j){
			for(unsigned int k = 0; k < lSize; ++k){
				if((*lIndividual1)[k] == lIndividual1Copy[j])
					(*lIndividual1)[k] = lIndividual2Copy[j];
				else if((*lIndividual1)[k] == lIndividual2Copy[j])
					(*lIndividual1)[k] = lIndividual1Copy[j];
				
				if((*lIndividual2)[k] == lIndividual1Copy[j])
					(*lIndividual2)[k] = lIndividual2Copy[j];
				else if((*lIndividual2)[k] == lIndividual2Copy[j])
					(*lIndividual2)[k] = lIndividual1Copy[j];
			}
		}
	}
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (after integer vector partially matched crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the IntVec partialy matched crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::IntVec::CrossoverPMXOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription("Individual partialy matched crossover pb.",
										   "Double",
										   "0.3",
										   "Integer vector partialy matched crossover probability of a single individual.");
		mMatingProba = castHandleT<Double>(ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lDescription));
	}
	EC::CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}
