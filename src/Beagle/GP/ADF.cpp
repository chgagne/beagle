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
 *  \file   beagle/GP/src/ADF.cpp
 *  \brief  Implementation of ADF.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new ADF primitive.
 *  \param inIndex Index of the associated tree.
 *  \param inNumberArgs Number of arguments of the ADF.
 *  \param inName Name of the ADF primitive.
 *  \param inArgsName Name of the associated argument primitive.
 */
GP::ADF::ADF(unsigned int inIndex,
             unsigned int inNumberArgs,
             std::string  inName,
             std::string  inArgsName) :
		GP::Invoker(inIndex, inNumberArgs, inName, inArgsName)
{ }


/*!
 *  \brief Generate a new ADF primitive from the given specifications.
 *  \param inIndex Tree index for which the primitive is created.
 *  \param inName Name of the primitive generated.
 *  \param inArgsName Name of the arguments associated to the invoker created.
 *  \param ioContext Evolutionary context.
 */
GP::Invoker::Handle GP::ADF::generateInvoker(unsigned int inIndex,
        std::string inName,
        std::string inArgsName,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	if(inIndex == UINT_MAX) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "Generated ADF with UINT_MAX"
		);

		return new GP::ADF(UINT_MAX, UINT_MAX, inName, inArgsName);
	} else {
		GP::Tree::Handle lTree = ioContext.getIndividual()[inIndex];

		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    std::string("Generated ADF with index ")+uint2str(inIndex)+" ("+uint2str(lTree->getNumberArguments())+" args)"
		);

		return new GP::ADF(inIndex, lTree->getNumberArguments(), inName, inArgsName);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return indices of the trees that can be invoked by the ADF.
 *  \param outCandidates Indices of tree that can be selected as invokable in the actual context.
 *  \param inNumberArguments Number of arguments for which the selection is desired.
 *  \param ioContext Evolutionary context.
 */
void GP::ADF::getCandidatesToInvoke(std::vector<unsigned int>& outCandidates,
                                    unsigned int inNumberArguments,
                                    GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	outCandidates.clear();
	for(unsigned int i=(ioContext.getGenotypeIndex()+1); i<ioContext.getIndividual().size(); ++i) {
		GP::Tree::Handle lTree = castHandleT<GP::Tree>(ioContext.getIndividual()[i]);
		const unsigned int lNbArgsTree = lTree->getNumberArguments();
		if(inNumberArguments == GP::Primitive::eAny) outCandidates.push_back(i);
		else if((inNumberArguments == GP::Primitive::eBranch) && (lNbArgsTree>0))
			outCandidates.push_back(i);
		else if(inNumberArguments == lNbArgsTree) outCandidates.push_back(i);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get reference the tree to invoke.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the invoked tree.
 */
GP::Tree::Handle GP::ADF::getInvokedTree(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(mIndex < ioContext.getIndividual().size());
	return ioContext.getIndividual()[mIndex];
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Invoke GP tree to execute as ADF.
 *  \param outResult Result of GP tree invocation
 *  \param ioTree Tree to invoke.
 *  \param ioContext Evolutionary context.
 */
void GP::ADF::invoke(GP::Datum& outResult, GP::Tree::Handle ioTree, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Tree::Handle lOldGenotypeHandle = ioContext.getGenotypeHandle();
	unsigned int lOldGenotypeIndex = ioContext.getGenotypeIndex();
	ioContext.setGenotypeHandle(ioTree);
	ioContext.setGenotypeIndex(mIndex);
	ioContext.incrementNodesExecuted();
	ioContext.pushCallStack(0);
	(*ioTree)[0].mPrimitive->execute(outResult, ioContext);
	ioContext.popCallStack();
	ioContext.checkExecutionTime();
	ioContext.setGenotypeHandle(lOldGenotypeHandle);
	ioContext.setGenotypeIndex(lOldGenotypeIndex);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Validate the ADF position in the tree.
 *  \param ioContext Evolutionary context.
 *  \return True if the ADF is correctly positioned, false if not.
 *  \throw Beagle::AssertException If the context is in a bad state.
 */
bool GP::ADF::validate(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	if(mIndex <= ioContext.getGenotypeIndex()) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "validation","Beagle::GP::ADF::validate",
		    std::string("Validation failed because the ADF's index (")+uint2str(mIndex)+
		    ") is less than or equal to the genotype index ("+uint2str(ioContext.getGenotypeIndex())+")"
		);
		return false;
	}
	if(mIndex >= ioContext.getIndividual().size()) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    std::string("Validation failed because the ADF's index (")+uint2str(mIndex)+
		    ") is greater than or equal to the individual's size ("+
		    uint2str(ioContext.getIndividual().size())+")"
		);
		return false;
	}
	if(ioContext.getIndividual()[mIndex]->getNumberArguments() != getNumberArguments()) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    std::string("Validation failed because the number of arguments for the tree (")+
		    uint2str(ioContext.getIndividual()[mIndex]->getNumberArguments())+
		    ") differed from the number of arguments for the primitive ("+
		    uint2str(getNumberArguments())+")"
		);
		return false;
	}
	return GP::Primitive::validate(ioContext);
	Beagle_StackTraceEndM();
}

