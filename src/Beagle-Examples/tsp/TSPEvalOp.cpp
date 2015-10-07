/*
 *  Traveling Salesman Problem (tsp):
 *  Indices integer vector GA example
 *
 *  Copyright (C) 2005-2006 by Christian Gagne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   TSPEvalOp.cpp
 *  \brief  Implementation of the class TSPEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.11 $
 *  $Date: 2007/08/09 21:43:11 $
 */

#include "Beagle/EC.hpp"
#include "Beagle/IntVec.hpp"
#include "TSPEvalOp.hpp"

#include <cmath>

#define BEAGLE_TSP_PROBLEMSIZE 25


using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the TSP problem.
 */
TSPEvalOp::TSPEvalOp() :
		EvaluationOp("TSPEvalOp")
{ }


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle TSPEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inIndividual.size() == 1);
	IntVec::IntegerVector::Handle lPath = castHandleT<IntVec::IntegerVector>(inIndividual[0]);
	const unsigned int lRootIndex = (mDistanceMap->getNumberOfTowns()-1);
	double lTripDistance = mDistanceMap->getDistance(lRootIndex,(*lPath)[0]);
	for(unsigned int i=1; i<lPath->size(); ++i) {
		lTripDistance += mDistanceMap->getDistance((*lPath)[i-1],(*lPath)[i]);
	}
	lTripDistance += mDistanceMap->getDistance((*lPath)[lPath->size()-1],lRootIndex);
	return new EC::FitnessSimpleMin(float(lTripDistance));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Evolutionary system.
 */
void TSPEvalOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::EvaluationOp::registerParams(ioSystem);
	Register::Description lDescription(
		"Initial integer vectors sizes",
		"UInt",
		"0",
		"Integer vector size of initialized individuals."
	);
	mIntVectorSize = castHandleT<UInt>(
	    ioSystem.getRegister().insertEntry("ga.init.vectorsize", new UInt(0), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the TSP evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void TSPEvalOp::init(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::EvaluationOp::init(ioSystem);
	// Get reference to TSP distance map component
	mDistanceMap = castHandleT<TSPDistanceMap>(ioSystem.getComponent("TSPDistanceMap"));
	if(mDistanceMap == NULL) {
	    throw Beagle_RunTimeExceptionM("A TSP distance map component should be present in the system!");
	}
	// Overwrite value of the "ga.init.vectorsize" parameter to fit distance map
	mIntVectorSize->getWrappedValue() = mDistanceMap->getNumberOfTowns()-1;
	Beagle_StackTraceEndM();
}


