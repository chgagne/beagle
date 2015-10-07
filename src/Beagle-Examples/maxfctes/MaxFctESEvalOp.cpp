/*
 *  Function maximization with Evolution Strategy (maxfctes):
 *  An ES example with Open BEAGLE
 *
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
 *  \file   MaxFctESEvalOp.cpp
 *  \brief  Implementation of the class MaxFctESEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/EC.hpp"
#include "Beagle/ES.hpp"
#include "MaxFctESEvalOp.hpp"

#include <cmath>

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for maximizing the function.
 */
MaxFctESEvalOp::MaxFctESEvalOp() :
		EvaluationOp("MaxFctESEvalOp")
{ }


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle MaxFctESEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_AssertM(inIndividual.size() == 1);
	GA::ESVector::Handle lESVector = castHandleT<GA::ESVector>(inIndividual[0]);
	Beagle_AssertM(lESVector->size() == 5);
	double lU   = 10.0;
	double lSum = 0.0;
	for(unsigned int i=0; i<5; ++i) {
		double lXi = (*lESVector)[i].mValue;
		if(lXi > 200.0)  lXi = 200.0;
		if(lXi < -200.0) lXi = -200.0;
		lSum += (lXi*lXi) + (lU*lU);
		lU += lXi;
	}
	lSum += (lU*lU);
	double lF = 161.8 / lSum;
	return new FitnessSimple(lF);
}
