/*
 *  Boolean 11-multiplexer (multiplexer)
 *  Copyright (C) 2006 by Christian Gagne
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
 *  \file   MultiplexerEvalOp.cpp
 *  \brief  Implementation of the class MultiplexerEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.3 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/GP.hpp"
#include "MultiplexerEvalOp.hpp"

#include <cmath>

using namespace Beagle;
using namespace std;

/*!
 *  \brief Construct a new Boolean 11-multiplexer evaluation operator.
 */
MultiplexerEvalOp::MultiplexerEvalOp() :
		GP::EvaluationOp("MultiplexerEvalOp"),
		mInputs(2048),
		mOutputs(2048)
{ }


/*!
 * \brief Initialize the Boolean 11-multiplexer evaluation operator by creating the test tables.
 * \param ioSystem System to use to initialize the operator.
 */
void MultiplexerEvalOp::init(System& ioSystem)
{
	Beagle_LogInfoM(
	    ioSystem.getLogger(),
	    "Configuring the Boolean 11-multiplexer problem"
	);
	GP::EvaluationOp::init(ioSystem);
	mInputs.resize(2048);
	mOutputs.resize(2048);
	for(unsigned int i=0; i<2048; ++i) {
		mInputs[i].resize(11);
		unsigned int lValue = i;
		unsigned int lDividor = 2048;
		for(unsigned int j=0; j<11; ++j) {
			lDividor /= 2;
			if(lValue >= lDividor) {
				mInputs[i][j] = true;
				lValue -= lDividor;
			} else mInputs[i][j] = false;
		}
		unsigned int lIndexOutput = 3;
		if(mInputs[i][0]) lIndexOutput += 1;
		if(mInputs[i][1]) lIndexOutput += 2;
		if(mInputs[i][2]) lIndexOutput += 4;
		mOutputs[i] = mInputs[i][lIndexOutput];
	}
}


/*!
 *  \brief Evaluate the individual fitness for the boolean 11-multiplexer problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle MultiplexerEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
	unsigned int lNbGood = 0;
	for(unsigned int i=0; i<2048; ++i) {
		setValue("A0", mInputs[i][0], ioContext);
		setValue("A1", mInputs[i][1], ioContext);
		setValue("A2", mInputs[i][2], ioContext);
		setValue("D0", mInputs[i][3], ioContext);
		setValue("D1", mInputs[i][4], ioContext);
		setValue("D2", mInputs[i][5], ioContext);
		setValue("D3", mInputs[i][6], ioContext);
		setValue("D4", mInputs[i][7], ioContext);
		setValue("D5", mInputs[i][8], ioContext);
		setValue("D6", mInputs[i][9], ioContext);
		setValue("D7", mInputs[i][10], ioContext);
		Bool lResult;
		inIndividual.run(lResult, ioContext);
		if(lResult.getWrappedValue() == mOutputs[i].getWrappedValue()) ++lNbGood;
	}
	double lFitness = double(lNbGood) / 2048.;
	return new EC::FitnessSimple(lFitness);
}

