/*
 *  SPAM e-mail database (spambase):
 *  Machine learning using strongly-typed GP with Open BEAGLE
 *  Copyright (C) 2001-2006 by Christian Gagne and Marc Parizeau
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
 *  \file   SpambaseEvalOp.cpp
 *  \brief  Implementation of the class SpambaseEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/10 20:32:28 $
 */

#include "Beagle/GP.hpp"
#include "SpambaseEvalOp.hpp"

#include <cmath>
#include <fstream>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a new spambase evaluation operator.
 */
SpambaseEvalOp::SpambaseEvalOp() :
		GP::EvaluationOp("SpambaseEvalOp")
{ }


/*!
 *  \brief Evaluate the individual fitness for the spambase problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle SpambaseEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
	// Get reference to data set
	DataSetClassification::Handle lDataSet =
	    castHandleT<DataSetClassification>(ioContext.getSystem().getComponent("DataSet"));
	if(lDataSet == NULL) {
		throw Beagle_RunTimeExceptionM("Data set is not present in the system, could not proceed further!");
	}

	// Generate indices used as data subset for fitness evaluation
	std::vector<unsigned int> lSubSet(lDataSet->size());
	for(unsigned int i=0; i<lSubSet.size(); ++i) lSubSet[i] = i;
	std::random_shuffle(lSubSet.begin(), lSubSet.end(),
	                    ioContext.getSystem().getRandomizer());
	if(Spambase_TestSize < lSubSet.size()) {
		lSubSet.resize(Spambase_TestSize);
	}

	// Evaluate sampled test cases
	unsigned int lCorrectCount = 0;
	for(unsigned int i=0; i<Spambase_TestSize; ++i) {
		const bool lPositiveID = ((*lDataSet)[lSubSet[i]].first == 1);
		const Beagle::Vector& lData = (*lDataSet)[lSubSet[i]].second;
		for(unsigned int j=0; j<lData.size(); ++j) {
			std::ostringstream lOSS;
			lOSS << "IN" << j;
			setValue(lOSS.str(), Double(lData[j]), ioContext);
		}
		Bool lResult;
		inIndividual.run(lResult, ioContext);
		if(lResult.getWrappedValue() == lPositiveID) ++lCorrectCount;
	}
	double lFitness = double(lCorrectCount) / Spambase_TestSize;
	return new EC::FitnessSimple(lFitness);
}


