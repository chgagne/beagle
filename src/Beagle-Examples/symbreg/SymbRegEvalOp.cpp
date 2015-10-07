/*
 *  Symbolic regression (symbreg):
 *  A simple GP example with Open BEAGLE
 *
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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
 *  \file   SymbRegEvalOp.cpp
 *  \brief  Implementation of the class SymbRegEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/10 20:32:28 $
 */

#include "Beagle/GP.hpp"
#include "SymbRegEvalOp.hpp"

#include <cmath>

using namespace Beagle;

/*!
 *  \brief Construct a new symbolic regression evaluation operator.
 */
SymbRegEvalOp::SymbRegEvalOp() :
		GP::EvaluationOp("SymbRegEvalOp")
{ }


/*!
 *  \brief Evaluate the individual fitness for the symbolic regression problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle SymbRegEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
	double lSquareError = 0.;
	for(unsigned int i=0; i<mDataSet->size(); i++) {
		Beagle_AssertM((*mDataSet)[i].second.size() == 1);
		const Double lX((*mDataSet)[i].second[0]);
		setValue("X", lX, ioContext);
		const Double lY((*mDataSet)[i].first);
		Double lResult;
		inIndividual.run(lResult, ioContext);
		const double lError = lY-lResult;
		lSquareError += (lError*lError);
	}
	const double lMSE  = lSquareError / mDataSet->size();
	const double lRMSE = sqrt(lMSE);
	const double lFitness = 1. / (1. + lRMSE);
	return new EC::FitnessSimple(lFitness);
}


/*!
 * \brief Initialize the operator by sampling the function to regress.
 * \param ioSystem System to use to sample.
 */
void SymbRegEvalOp::init(System& ioSystem)
{
	GP::EvaluationOp::init(ioSystem);

	// Get reference to data set component
	mDataSet = castHandleT<Beagle::DataSetRegression>(ioSystem.getComponent("DataSetRegression"));
	if(mDataSet == NULL) {
		throw Beagle_RunTimeExceptionM("'DataSetRegression' component not found in System, impossible to proceed further!");
	}

	// If data set is empty, initialize it with random samples of quartic polynomial
	if(mDataSet->size() == 0) {
		mDataSet->resize(20);
		for(unsigned int i=0; i<mDataSet->size(); i++) {
			const double lX = ioSystem.getRandomizer().rollUniform(-1., 1.);
			const double lY = lX*(lX*(lX*(lX+1.)+1.)+1.);
			(*mDataSet)[i].first = lY;
			(*mDataSet)[i].second.resize(1);
			(*mDataSet)[i].second[0] = lX;
		}
	}
}
