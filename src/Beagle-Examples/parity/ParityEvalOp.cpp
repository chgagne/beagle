/*
 *  Even-6 parity (parity):
 *  A GP example using ADFs with Open BEAGLE
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
 *  \file   ParityEvalOp.cpp
 *  \brief  Implementation of the class ParityEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Parizeau <parizeau@gel.ulaval.ca>
 *  $Revision: 1.17 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/GP.hpp"
#include "ParityEvalOp.hpp"

#include <cmath>


using namespace Beagle;
using namespace std;

/*!
 *  \brief Construct a new even-6 parity evaluation operator.
 */
ParityEvalOp::ParityEvalOp() :
		GP::EvaluationOp("ParityEvalOp"),
		mInputs(ParitySizeM),
		mOutputs(ParitySizeM)
{ }


/*!
 * \brief Initialize the even-6 parity evaluation operator by creating the test tables.
 * \param ioSystem System to use to initialize the operator.
 */
void ParityEvalOp::init(System& ioSystem)
{
	Beagle_LogInfoM(
	    ioSystem.getLogger(),
	    string("Configured for the even-")+uint2str(ParityFanInM)+" parity problem"+
	    " (with "+uint2str(ParitySizeM)+" rows in the truth-table)"
	);

	EvaluationOp::init(ioSystem);
	mInputs.resize(ParitySizeM);
	mOutputs.resize(ParitySizeM);
	for(unsigned int i=0; i<ParitySizeM; i++) {
		mInputs[i].resize(ParityFanInM);
		unsigned int lValue = i;
		unsigned int lDividor = ParitySizeM;
		bool lParity  = true;
		for(unsigned int j=0; j<ParityFanInM; j++) {
			lDividor /= 2;
			if(lValue >= lDividor) {
				mInputs[i][j] = true;
				lParity = !lParity;
				lValue  -= lDividor;
			} else mInputs[i][j] = false;
		}
		mOutputs[i] = lParity;
	}
}


/*!
 *  \brief Evaluate the individual fitness for the even-6 parity problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle ParityEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
	unsigned int lGood = 0;
	for(unsigned int i=0; i<ParitySizeM; i++) {
		for(unsigned int j=0; j<ParityFanInM; j++) {
			std::string lName = "IN";
			lName += uint2str(j);
			setValue(lName, mInputs[i][j], ioContext);
		}
		Bool lResult;
		inIndividual.run(lResult, ioContext);
		if(lResult.getWrappedValue() == mOutputs[i].getWrappedValue()) lGood++;
	}
	double lFitness = double(lGood) / ParitySizeM;
	return new EC::FitnessSimple(lFitness);
}
