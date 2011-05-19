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
 *  \file   beagle/GP/src/EvaluationOp.cpp
 *  \brief  Source code of class GP::EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/10 20:32:21 $
 */

#include "beagle/GP.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a evaluation operator.
 *  \param inName Name of the operator.
 */
GP::EvaluationOp::EvaluationOp(std::string inName) :
		Beagle::EvaluationOp(inName)
{ }


/*!
 *  \brief Evaluate the fitness of the given GP individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the GP individual.
 */
Fitness::Handle GP::EvaluationOp::evaluate(Beagle::Individual& inIndividual,
        Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	return evaluate(castObjectT<GP::Individual&>(inIndividual), castObjectT<GP::Context&>(ioContext));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the value of the named GP primitive of the primitive sets.
 *  \param inName Name of the variable to set.
 *  \param inValue Value of the primitive.
 *  \param ioContext Context of the evaluation.
 *  \throw Beagle::RunTimeException If the named primitive is not found in any sets.
 */
void GP::EvaluationOp::setValue(std::string inName,
                                const Object& inValue,
                                GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	GP::PrimitiveSuperSet::Handle lSuperSet =
	    castHandleT<GP::PrimitiveSuperSet>(ioContext.getSystem().getComponent("GP-PrimitiveSuperSet"));
	if(lSuperSet == NULL) {
		throw Beagle_RunTimeExceptionM("There should be a GP::PrimitiveSuperSet component in the system");
	}
	bool lValueFound = false;
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "evaluation", "Beagle::GP::EvaluationOp",
	    std::string("Setting the primitives named '")+inName+
	    std::string("' to the value: ")+inValue.serialize()
	);
	for(unsigned int i=0; i<lSuperSet->size(); i++) {
		Primitive::Handle lPrimitive = (*lSuperSet)[i]->getPrimitiveByName(inName);
		if(!lPrimitive) continue;
		lValueFound = true;
		lPrimitive->setValue(inValue);
	}
	if(lValueFound == false) {
		std::string lMessage = "The primitive named '";
		lMessage += inName;
		lMessage += "' was not found in any ";
		lMessage += "of the primitive sets. Maybe the primitive was not properly inserted ";
		lMessage += "or the name is mispelled.";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	Beagle_StackTraceEndM();
}

