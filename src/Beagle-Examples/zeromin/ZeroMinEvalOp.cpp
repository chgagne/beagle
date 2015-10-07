/*
 *  ZeroMin (zeromin):
 *  Minimization GA example with Open BEAGLE
 *
 *  Copyright (C) 2003-2006 by Christian Gagne and Marc Parizeau
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
 *  \file   ZeroMinEvalOp.cpp
 *  \brief  Implementation of the class ZeroMinEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"
#include "ZeroMinEvalOp.hpp"

#include <cmath>

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the ZeroMin problem.
 */
ZeroMinEvalOp::ZeroMinEvalOp() :
		EvaluationOp("ZeroMinEvalOp")
{ }


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the GA individual.
 */
Fitness::Handle ZeroMinEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_AssertM(inIndividual.size() == 1);
	BitStr::BitString::Handle lBitString = castHandleT<BitStr::BitString>(inIndividual[0]);
	unsigned int lCount = 0;
	for(unsigned int i=0; i<lBitString->size(); ++i) {
		if((*lBitString)[i] == true) ++lCount;
	}
	return new EC::FitnessSimpleMin(float(lCount));
}
