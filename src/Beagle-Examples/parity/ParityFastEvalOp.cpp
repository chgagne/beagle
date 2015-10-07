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
 *  \file   ParityFastEvalOp.cpp
 *  \brief  Implementation of the class ParityFastEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Parizeau <parizeau@gel.ulaval.ca>
 *  \author Matthew Walker
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/GP.hpp"
#include "ParityFastEvalOp.hpp"

#include <cmath>


using namespace Beagle;
using namespace std;

/*!
 *  \brief Construct a new even-6 parity evaluation operator.
 */
ParityFastEvalOp::ParityFastEvalOp() :
		GP::EvaluationOp("ParityEvalOp") // Use this name to ensure .conf files can be read by either parity or parity_fast
{ }


/*!
 * \brief Initialize the even-6 parity evaluation operator by creating the test tables.
 * \param ioSystem System to use to initialize the operator.
 */
void ParityFastEvalOp::init(System& ioSystem)
{
	mParityRows = 1;
	for (unsigned int i=0; i<ParitySizeM; ++i) {
		mParityRows *= 2;
	}

	Beagle_LogInfoM(
	    ioSystem.getLogger(),
	    string("Compiled for the even-")+uint2str(ParitySizeM)+"-parity problem"+
	    " (with "+uint2str(mParityRows)+" rows in the truth-table)"
	);

	GP::EvaluationOp::init(ioSystem);

	// Calc number of ints per input (fixed at 32 bits per int)
	mNumInts = mParityRows/32 + ((mParityRows%32)>0);
	Beagle_LogTraceM(
	    ioSystem.getLogger(),
	    string("Using ")+uint2str(mNumInts)+" int(s) to store inputs"
	);

	// Allocate space for inputs and outputs
	mInputs.resize(ParitySizeM);
	for (unsigned int i=0; i<ParitySizeM; ++i) {
		mInputs[i].resize(mNumInts);
	}
	mOutputs.resize(mNumInts);

	// Set inputs
	for (unsigned int i=0; i<ParitySizeM; ++i) {
		for (unsigned int j=0; j<mNumInts; ++j) {
			switch (i+1) {
			case 1:
				mInputs[i][j] = 0x55555555;
				break;
			case 2:
				mInputs[i][j] = 0x33333333;
				break;
			case 3:
				mInputs[i][j] = 0x0F0F0F0F;
				break;
			case 4:
				mInputs[i][j] = 0x00FF00FF;
				break;
			case 5:
				mInputs[i][j] = 0x0000FFFF;
				break;
			default: // i.e. six and above
				unsigned int lNumSet = 1;
				for (unsigned int k=0; k<i-4; ++k) {
					lNumSet *= 2;
				}
				if (j%lNumSet<lNumSet/2) mInputs[i][j] = 0x00000000;
				else mInputs[i][j] = 0xFFFFFFFF;
			}
		}
	}

	// Display inputs
	/*
	std::cerr.setf(std::ios::showbase);
	std::cerr << "\n";
	for (unsigned int i=0; i<ParitySizeM; ++i) {
	  std::string lName = "IN";
	  lName += uint2str(i);
	  std::cerr << lName << std::endl;
	  for (unsigned int j=0; j<mNumInts; ++j) {
	    std::cerr << dec << j << ": " << std::hex << mInputs[i][j] << std::endl;
	  }
	}
	*/

	// Set outputs
	for (unsigned int j=0; j<mNumInts; ++j) {
		unsigned int lOutput = mInputs[0][j];
		for (unsigned int i=1; i<ParitySizeM; ++i) {
			lOutput = lOutput ^ mInputs[i][j];
		}
		lOutput = ~lOutput;
		mOutputs[j] = lOutput;
	}

	// Display outputs
	/*
	std::cerr << "OUT\n";
	for (unsigned int j=0; j<mNumInts; ++j) {
	  std::cerr << dec << j << ": " << std::hex << mOutputs[j] << std::endl;
	}
	*/
}


/*!
 *  \brief Evaluate the individual fitness for the even-6 parity problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle ParityFastEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	unsigned int lGood = 0;
	for (unsigned int j=0; j<mNumInts; ++j) {
		// Set inputs
		for(unsigned int i=0; i<ParitySizeM; i++) {
			std::string lName = "IN";
			lName += uint2str(i);
			setValue(lName, mInputs[i][j], ioContext);
		}

		// Evaluate individual
		Int lResult;
		inIndividual.run(lResult, ioContext);
		//std::cerr << "\n\nRESULT " << dec << j << ": " << hex << lResult.getWrappedValue() << "\n";

		// Count the number of correct inputs
		unsigned int lTermK;
		if ((mParityRows - 32*j) >= 32) lTermK = 32;
		else lTermK = mParityRows%32;

		unsigned int lMask = 1;
		for (unsigned int k=0; k<lTermK; ++k) {
			lGood += ((~(lResult^mOutputs[j])) & lMask )>0;
			lMask = lMask << 1;
		}
	}
	double lFitness = double(lGood) / mParityRows;
	return new EC::FitnessSimple(lFitness);
	Beagle_StackTraceEndM();
}
