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
		    "Invalidating the fitness of the following bred individual"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lBredIndividual);
		lBredIndividual->getFitness()->setInvalid();
	}
	return lBredIndividual;
	Beagle_StackTraceEndM();
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
	Beagle_StackTraceEndM();
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
	    "Invalidating the fitness of every individual in the " <<
	    uint2ordinal(ioContext.getDemeIndex()+1) << " deme"
	);
	for(unsigned int i=0; i<ioDeme.size(); i++) {
		if(ioDeme[i]->getFitness() != NULL) {
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "Invalidating the fitness of the " << uint2ordinal(i+1) <<
			    " individual of the " << uint2ordinal(ioContext.getDemeIndex()+1) << " deme"
			);
			ioDeme[i]->getFitness()->setInvalid();
		}
	}
	Beagle_StackTraceEndM();
}
