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
 *  \file   beagle/GP/src/ValidateOp.cpp
 *  \brief  Implementation of the type GP::ValidateOp.
 *  \author Matthew Walker
 *  $Revision: 1.9 $
 *  $Date: 2007/08/09 21:43:10 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new validation operator.
 *  \param inName Name of the operator.
 */
GP::ValidateOp::ValidateOp(std::string inName) :
		Operator(inName)
{}


/*!
 *  \brief For every individual in the deme, check that it passes
 *    validation testing.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void GP::ValidateOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Context& lContext = castObjectT<GP::Context&>(ioContext);

	// Store original values.
	GP::Individual::Handle lOldIndi = lContext.getIndividualHandle();
	unsigned int lOldIndiIndex = lContext.getIndividualIndex();

	// Loop through every individual in the deme.
	for (unsigned int i=0; i<ioDeme.size(); i++) {
		// Get individual.
		GP::Individual::Handle lIndividual = castHandleT<GP::Individual>(ioDeme[i]);

		// Set current values.
		lContext.setIndividualHandle(lIndividual);
		lContext.setIndividualIndex(i);

		// Check the individual is valid.
		if(!lIndividual->validate(lContext)) {
			throw Beagle_RunTimeExceptionM("Individual failed validation during ValidateOp check.");
		}
	}

	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    "validate", "Beagle::GP::ValidateOp",
	    std::string("Every individual passed validation testing.")
	);

	// Restore original values.
	lContext.setIndividualHandle(lOldIndi);
	lContext.setIndividualIndex(lOldIndiIndex);
	Beagle_StackTraceEndM();
}
