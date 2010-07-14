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
 *  \file   Beagle/EC/InvalidateFitnessOp.cpp
 *  \brief  Source code of class InvalidateFitnessOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct an InvalidateFitnessOp.
 *  \param inName Name of the operator.
 */
InvalidateFitnessOp::InvalidateFitnessOp(std::string inName) :
		BreederOp(inName)
{ }


/*!
 *  \brief Apply the fitness invalidation operation on a breeding pool, returning a bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Invalidated bred individual.
 */
Individual::Handle InvalidateFitnessOp::breed(Individual::Bag& inBreedingPool,
        BreederNode::Handle inChild,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	Individual::Handle lBredIndividual =
	    inChild->getBreederOp()->breed(inBreedingPool, inChild->getFirstChild(), ioContext);
	if((lBredIndividual->getFitness()!=NULL) && (lBredIndividual->getFitness()->isValid())) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "invalidate-fitness", "Beagle::InvalidateFitnessOp",
		    "Invalidating the fitness of the following bred individual"
		);
		Beagle_LogObjectDebugM(
		    ioContext.getSystem().getLogger(),
		    "invalidate-fitness",
		    "Beagle::InvalidateFitnessOp",
		    *lBredIndividual
		);
		lBredIndividual->getFitness()->setInvalid();
	}
	return lBredIndividual;
	Beagle_StackTraceEndM("Individual::Handle InvalidateFitnessOp::breed(Individual::Bag& inBreedingPool, BreederNode::Handle inChild, Context& ioContext)");
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
double InvalidateFitnessOp::getBreedingProba(BreederNode::Handle inChild)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	return inChild->getBreederOp()->getBreedingProba(inChild->getFirstChild());
	Beagle_StackTraceEndM("float InvalidateFitnessOp::getBreedingProba(BreederNode::Handle inChild)");
}


/*!
 *  \brief Invalidates the fitness of every individual in ioDeme.
 *  \param ioDeme The deme to operate on.
 *  \param ioContext Evolutionary context.
 */
void InvalidateFitnessOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "invalidate-fitness", "Beagle::InvalidateFitnessOp",
	    std::string("Invalidating the fitness of every individual in the ")+uint2ordinal(ioContext.getDemeIndex()+1)+
	    std::string(" deme")
	);
	for(unsigned int i=0; i<ioDeme.size(); i++) {
		if(ioDeme[i]->getFitness() != NULL) {
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "invalidate-fitness", "Beagle::InvalidateFitnessOp",
			    std::string("Invalidating the fitness of the ")+uint2ordinal(i+1)+
			    " individual of the "+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
			);
			ioDeme[i]->getFitness()->setInvalid();
		}
	}
	Beagle_StackTraceEndM("void InvalidateFitnessOp::operate(Deme& ioDeme, Context& ioContext)");
}
