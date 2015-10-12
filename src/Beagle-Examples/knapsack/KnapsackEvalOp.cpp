/*
 *  Multiobjective 0/1 Knapsack (knapsack):
 *  Multiobjective GA example
 *
 *  Copyright (C) 2003-2007 by Christian Gagne and Marc Parizeau
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
 *  \file   KnapsackEvalOp.cpp
 *  \brief  Implementation of the class KnapsackEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 18:09:18 $
 */

#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"
#include "Beagle/EMO.hpp"
#include "KnapsackEvalOp.hpp"
#include <cmath>

#define BEAGLE_KNAPSACK_NBOBJECTS 24


using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the Knapsack problem.
 */
KnapsackEvalOp::KnapsackEvalOp() :
		EvaluationOp("KnapsackEvalOp"),
		mSumWeight(0.0)
{ }


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle KnapsackEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_AssertM(inIndividual.size() == 1);
	BitStr::BitString::Handle lBitString = castHandleT<BitStr::BitString>(inIndividual[0]);
	float lValue  = 0.0;
	float lWeight = 0.0;
	for(unsigned int i=0; i<lBitString->size(); ++i) {
		if((*lBitString)[i]) {
			lValue  += (*mObjectValues)[i];
			lWeight += (*mObjectWeights)[i];
		}
	}
	EMO::FitnessMultiObj::Handle lFitness = new EMO::FitnessMultiObj(2);
	(*lFitness)[0] = lValue;
	(*lFitness)[1] = (mSumWeight - lWeight);
	return lFitness;
}


/*!
 *  \brief Register the knapsack parameters.
 *  \param ioSystem Evolutionary system.
 */
void KnapsackEvalOp::registerParams(System& ioSystem)
{
	Beagle::EvaluationOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Values of the available objects that can be put into the knapsack. ";
		lOSS << "If the object values is not specified, it will be randomly generated ";
		lOSS << "at the initialization time.";
		Register::Description lDescription(
		    "Knapsack object values",
		    "Vector",
		    "",
		    lOSS.str()
		);
		mObjectValues = castHandleT<Vector>(
		                    ioSystem.getRegister().insertEntry("ks.object.values", new Vector(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Weights of the available objects that can be put into the knapsack. ";
		lOSS << "If the object weights is not specified, it will be randomly generated ";
		lOSS << "at the initialization time.";
		Register::Description lDescription(
		    "Knapsack object weights",
		    "Vector",
		    "",
		    lOSS.str()
		);
		mObjectWeights = castHandleT<Vector>(
		                     ioSystem.getRegister().insertEntry("ks.object.weights", new Vector(0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Initial integer vectors sizes",
		    "UInt",
		    "0",
		    "Integer vector size of initialized individuals."
		);
		mIntVectorSize = castHandleT<UInt>(
		                     ioSystem.getRegister().insertEntry("ga.init.vectorsize", new UInt(0), lDescription));
	}
}


/*!
 *  \brief Initialize the knapsack evaluation operator by generating object's value/weight.
 *  \param ioSystem Evolutionary system.
 */
void KnapsackEvalOp::init(System& ioSystem)
{
	Beagle::EvaluationOp::init(ioSystem);

	float lSumValue  = 0.0;
	if((mObjectValues->size()==0) && (mObjectWeights->size()==0)) {
		mObjectValues->resize(BEAGLE_KNAPSACK_NBOBJECTS);
		mObjectWeights->resize(BEAGLE_KNAPSACK_NBOBJECTS);
		mSumWeight = 0.0;
		for(unsigned int i=0; i<BEAGLE_KNAPSACK_NBOBJECTS; ++i) {
			lSumValue  += (*mObjectValues)[i]  = ioSystem.getRandomizer().rollUniform(1.0, 10.0);
			mSumWeight += (*mObjectWeights)[i] = ioSystem.getRandomizer().rollUniform(1.0, 10.0);
		}
	} else if(mObjectValues->size() != mObjectWeights->size()) {
		std::ostringstream lOSS;
		lOSS << "Objects value and weight of evaluation operator (registered as ";
		lOSS << "'ks.objects.value' and 'ks.objects.weight' parameters) must be of the same size!";
		throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
	} else {
		mSumWeight = 0.0;
		for(unsigned int i=0; i<mObjectValues->size(); ++i) {
			lSumValue  += (*mObjectValues)[i];
			mSumWeight += (*mObjectWeights)[i];
		}
	}

	mIntVectorSize->getWrappedValue() = mObjectValues->size();

	Beagle_LogInfoM(
	    ioSystem.getLogger(),
	    std::string("Sum of possible knapsack value is ")+dbl2str(lSumValue)+
	    std::string(", while sum of possible knapsack weight is ")+dbl2str(mSumWeight)
	);
	Beagle_LogInfoM(
	    ioSystem.getLogger(),
	    "Objects value and weights that can be put knapsack"
	);
	Beagle_LogM(
	    ioSystem.getLogger(),
	    Logger::eInfo,
	    *mObjectValues
	);
	Beagle_LogM(
	    ioSystem.getLogger(),
	    Logger::eInfo,
	    *mObjectWeights
	);
}

