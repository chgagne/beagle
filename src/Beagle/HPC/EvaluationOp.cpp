/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
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
 *  \file   beagle/HPC/EvaluationOp.cpp
 *  \brief  Source code of class EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.32 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new evaluation operator.
 *  \param inName Name of the operator.
 */
HPC::EvaluationOp::EvaluationOp(std::string inName) :
		Beagle::EvaluationOp(inName)
{ }

/*!
 *  \brief Apply the evaluation process on the invalid individuals of the deme.
 *  \param ioDeme Deme to process.
 *  \param ioContext Context of the evolution.
 */
void HPC::EvaluationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "evaluation", "Beagle::EvaluationOp",
	    std::string("Evaluating the fitness of the individuals in the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();

	// Used to flag whether this operator has done anything
	bool lNoEvaluations = true;

	for(unsigned int i=0; i<ioDeme.size(); i++) {
		if((ioDeme[i]->getFitness() == NULL) ||
		        (ioDeme[i]->getFitness()->isValid() == false)) {

			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "evaluation", "Beagle::EvaluationOp",
			    std::string("Evaluating the fitness of the ")+uint2ordinal(i+1)+
			    " individual"
			);

			ioContext.setIndividualIndex(i);
			ioContext.setIndividualHandle(ioDeme[i]);

			ioDeme[i]->setFitness(evaluate(*ioDeme[i], ioContext));
			ioDeme[i]->getFitness()->setValid();

			lNoEvaluations = false;

			Beagle_LogObjectM(
			    ioContext.getSystem().getLogger(),
			    Logger::eVerbose,
			    "evaluation",
			    "Beagle::EvaluationOp",
			    *ioDeme[i]->getFitness()
			);
		}
	}

	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);

	// Warn if the operator has not evaluated anything
	if (lNoEvaluations) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "evaluation", "Beagle::EvaluationOp",
		    std::string("Warning!  The '")+getName()+"' operator did not evaluate any individuals.  "+
		    "Consider using the 'InvalidateFitnessOp' operator to force evaluation of every individual."
		);
	}

	Beagle_StackTraceEndM();
}
