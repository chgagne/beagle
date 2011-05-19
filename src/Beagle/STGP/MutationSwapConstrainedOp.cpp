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
 *  \file   beagle/GP/src/MutationSwapConstrainedOp.cpp
 *  \brief  Source code of class GP::MutationSwapConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree swap mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inDistribPbName Swap mutation distribution probability parameter name.
 *  \param inName Name of the operator.
 */
GP::MutationSwapConstrainedOp::MutationSwapConstrainedOp(std::string inMutationPbName,
        std::string inDistribPbName,
        std::string inName) :
		MutationSwapOp(inMutationPbName, inDistribPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree swap mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GP::MutationSwapConstrainedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MutationSwapOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "Maximum number of attempts to modify a GP tree in a genetic ";
	lOSS << "operation. As there is topological constraints on GP trees (i.e. tree ";
	lOSS << "depth limit), it is often necessary to try a genetic operation several times.";
	Register::Description lDescription(
	    "Max number of attempts",
	    "UInt",
	    "2",
	    lOSS.str()
	);
	mNumberAttempts = castHandleT<UInt>(
	                      ioSystem.getRegister().insertEntry("gp.try", new UInt(2), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Swap mutate a constrained GP individual.
 *  \param ioIndividual GP individual to swap mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GP::MutationSwapConstrainedOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Individual& lIndividual  = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext        = castObjectT<GP::Context&>(ioContext);
	double lDistrProba           = mDistributionProba->getWrappedValue();
	unsigned int lNumberAttempts = mNumberAttempts->getWrappedValue();
	bool lMutationDone           = false;

	unsigned int lNbNodes = 0;
	for(unsigned int i=0; i<lIndividual.size(); i++) lNbNodes += lIndividual[i]->size();
	if(lNbNodes == 0) return false;
	unsigned int lChoosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lNbNodes-1);
	unsigned int lChoosenTree = 0;
	for(; (lChoosenTree+1)<lIndividual.size(); lChoosenTree++) {
		if(lChoosenNode < lIndividual[lChoosenTree]->size()) break;
		else lChoosenNode -= lIndividual[lChoosenTree]->size();
	}
	GP::Tree& lTree = *lIndividual[lChoosenTree];
	if(lTree.size() == 0) return false;

	GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
	lContext.setGenotypeHandle(lIndividual[lChoosenTree]);
	lContext.setGenotypeIndex(lChoosenTree);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapConstrainedOp",
	    "Individual before constrained GP tree swap mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationSwapConstrainedOp",
	    ioIndividual
	);

	if(lTree.size() > 1) {
		bool lTypeNode = (lContext.getSystem().getRandomizer().rollUniform(0., 1.) < lDistrProba);
		while((lTree[lChoosenNode].mPrimitive->getNumberArguments() != 0) != lTypeNode) {
			lChoosenNode = lContext.getSystem().getRandomizer().rollInteger(0, lTree.size()-1);
		}
	}
	Primitive::Handle lOriginalPrimitive = lTree[lChoosenNode].mPrimitive;

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationSwapConstrainedOp",
	    std::string("Trying to constrained GP tree swap mutate the ")+uint2ordinal(lChoosenNode+1)+
	    std::string(" node (primitive: '")+lOriginalPrimitive->getName()+
	    std::string("' nb args: ")+uint2str(lOriginalPrimitive->getNumberArguments())+
	    std::string(") of the ")+uint2ordinal(lChoosenTree+1)+std::string(" tree")
	);

	GP::PrimitiveSet& lPrimitiveSet = lTree.getPrimitiveSet(lContext);
	unsigned int lNbArgsPrimit = lTree[lChoosenNode].mPrimitive->getNumberArguments();
	lTree.setContextToNode(lChoosenNode, lContext);
	for(unsigned int lAttempt=0; lAttempt < lNumberAttempts; ++lAttempt) {
		Primitive::Handle lChoosenPrimitive = lPrimitiveSet.select(lNbArgsPrimit, lContext);
		if(lChoosenPrimitive==NULL) break;

		lTree[lChoosenNode].mPrimitive = lChoosenPrimitive->giveReference(lNbArgsPrimit, lContext);

		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation", "Beagle::GP::MutationSwapConstrainedOp",
		    std::string("Trying the primitive '")+lChoosenPrimitive->getName()+
		    std::string("'")
		);

		if(lTree.validateSubTree(lChoosenNode, lContext)) {
			lMutationDone = true;
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationSwapConstrainedOp",
			    "Constrained GP tree swap mutation valid"
			);
			break;
		} else {
			lTree[lChoosenNode].mPrimitive = lOriginalPrimitive;
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "mutation", "Beagle::GP::MutationSwapConstrainedOp",
			    "Constrained GP tree swap mutation invalid"
			);
		}
	}

	lContext.setGenotypeHandle(lOldTreeHandle);
	lContext.setGenotypeIndex(lOldTreeIndex);

	if(lMutationDone) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapConstrainedOp",
		    "Individual after constrained GP swap mutation"
		);
		Beagle_LogObjectDebugM(
		    ioContext.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapConstrainedOp",
		    ioIndividual
		);
	} else {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "mutation",
		    "Beagle::GP::MutationSwapConstrainedOp",
		    "Unable to swap mutate the constrained individual"
		);
	}

	return lMutationDone;
	Beagle_StackTraceEndM();
}



