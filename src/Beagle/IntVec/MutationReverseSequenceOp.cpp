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
 *  \file   Beagle/IntVec/MutationReverseSequenceOp.cpp
 *  \brief  Source code of class IntVec::MutationReverseSequenceOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a IntVec reverse sequence mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inName Name of the operator.
 */
IntVec::MutationReverseSequenceOp::MutationReverseSequenceOp(std::string inMutationPbName, std::string inName) :
	EC::MutationOp(inMutationPbName, inName)
{ }


/*!
 *  \brief Mutate by reversing a sequence of the genotype of a indices permutation individual.
 *  \param ioIndividual IntVec individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool IntVec::MutationReverseSequenceOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	
	bool lMutated = false;
	
	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		
		Beagle_LogVerboseM(
			ioContext.getSystem().getLogger(),
			"Reversing a sequence of the " << uint2ordinal(i+1) << " individual's genotype"
		);
		Beagle_LogDebugM(
			ioContext.getSystem().getLogger(),
			"The individual's genotype before reverse sequence mutation: " << *ioIndividual[i]
		);
		
		unsigned int a = ioContext.getSystem().getRandomizer().rollInteger(0, ioIndividual[i]->getSize()-1);
		unsigned int b = ioContext.getSystem().getRandomizer().rollInteger(0, ioIndividual[i]->getSize()-1);
		if(a > b) std::swap(a, b);
		if(a != b) lMutated = true;
		while(a < b)
			ioIndividual[i]->swap(a++, b--);
		
		if(lMutated) {
			Beagle_LogVerboseM(
				ioContext.getSystem().getLogger(),
				"A part of the individual's genotype has been reversed"
			);
			Beagle_LogDebugM(
				ioContext.getSystem().getLogger(),
				"The individual's genotype after IntVec reverse sequence mutation: " << *ioIndividual[i]
			);
		}
		else {
			Beagle_LogVerboseM(
				ioContext.getSystem().getLogger(),
				"The individual's genotype has not been mutated"
			);
		}
	}
	return lMutated;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the IntVec reverse sequence mutation operator.
 *  \param ioSystem System of the evolution.
 */
void IntVec::MutationReverseSequenceOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
			"Individual reverse sequence mutation prob.",
			"Double",
			"0.1",
			"Reverse sequence mutation probability for each IntVec individual."
		);
		mMutationProba = castHandleT<Double>(
			ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.1), lDescription));
	}
	EC::MutationOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}

