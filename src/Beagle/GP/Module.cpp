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
 *  \file   beagle/GP/src/Module.cpp
 *  \brief  Implementation of Module.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new Module primitive.
 *  \param inIndex Index of the associated tree.
 *  \param inNumberArgs Number of arguments of the module.
 *  \param inName Name of the module primitive.
 *  \param inArgsName Name of the associated argument primitive.
 */
GP::Module::Module(unsigned int inIndex,
                   unsigned int inNumberArgs,
                   std::string inName,
                   std::string inArgsName) :
		GP::Invoker(inIndex, inNumberArgs, inName, inArgsName)
{ }


/*!
 *  \brief Generate a new Module primitive from the given specifications.
 *  \param inIndex Tree index for which the primitive is created.
 *  \param inName Name of the primitive generated.
 *  \param inArgsName Name of the arguments associated to the invoker created.
 *  \param ioContext Evolutionary context.
 */
GP::Invoker::Handle GP::Module::generateInvoker(unsigned int inIndex,
        std::string inName,
        std::string inArgsName,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Component::Handle lComponent = ioContext.getSystem().getComponent("ModuleVector");
	GP::ModuleVectorComponent::Handle lModVector =
	    castHandleT<GP::ModuleVectorComponent>(lComponent);
	if(lModVector==NULL) {
		throw Beagle_RunTimeExceptionM(std::string("GP system is not configured with a module vector. ")+
		                               std::string("Consider adding a GP::ModuleVectorComponent object to the system."));
	}
	GP::Tree::Handle lTree = (*lModVector)[inIndex];
	Beagle_AssertM(lTree != NULL);
	return new GP::Module(inIndex, lTree->getNumberArguments(), inName, inArgsName);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return indices of the trees that can be invoked by the module.
 *  \param outCandidates Indices of tree that can be selected as invokable in the actual context.
 *  \param inNumberArguments Number of arguments for which the selection is desired.
 *  \param ioContext Evolutionary context.
 */
void GP::Module::getCandidatesToInvoke(std::vector<unsigned int>& outCandidates,
                                       unsigned int inNumberArguments,
                                       GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	outCandidates.clear();
	for(unsigned int i=0; i<ioContext.getCallStackSize(); ++i) {
		if(ioContext.getGenotype()[ioContext.getCallStackElement(i)].mPrimitive->getName() == getName())
			return;
	}
	Component::Handle lComponent = ioContext.getSystem().getComponent("ModuleVector");
	GP::ModuleVectorComponent::Handle lModVector =
	    castHandleT<GP::ModuleVectorComponent>(lComponent);
	if(lModVector==NULL) {
		throw Beagle_RunTimeExceptionM(std::string("GP system is not configured with a module vector. ")+
		                               std::string("Consider adding a GP::ModuleVectorComponent object to the system."));
	}
	for(unsigned int i=0; i<lModVector->size(); ++i) {
		if((*lModVector)[i]==NULL) continue;
		const unsigned int lNbArgsTree = (*lModVector)[i]->getNumberArguments();
		if(inNumberArguments == GP::Primitive::eAny) outCandidates.push_back(i);
		else if((inNumberArguments==GP::Primitive::eBranch) && (lNbArgsTree>0))
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
GP::Tree::Handle GP::Module::getInvokedTree(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Component::Handle lComponent = ioContext.getSystem().getComponent("ModuleVector");
	GP::ModuleVectorComponent::Handle lModVector =
	    castHandleT<GP::ModuleVectorComponent>(lComponent);
	if(lModVector==NULL) {
		throw Beagle_RunTimeExceptionM(std::string("GP system is not configured with a module vector. ")+
		                               std::string("Consider adding a GP::ModuleVectorComponent object to the system."));
	}
	Beagle_AssertM(mIndex < lModVector->size());
	Beagle_AssertM((*lModVector)[mIndex] != NULL);
	return (*lModVector)[mIndex];
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Invoke GP tree to execute as ADF.
 *  \param outResult Result of GP tree invocation
 *  \param ioTree Tree to invoke.
 *  \param ioContext Evolutionary context.
 */
void GP::Module::invoke(GP::Datum& outResult, GP::Tree::Handle ioTree, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Tree::Handle lOldGenotypeHandle = ioContext.getGenotypeHandle();
	unsigned int lOldGenotypeIndex = ioContext.getGenotypeIndex();
	ioContext.setGenotypeHandle(ioTree);
	ioContext.setGenotypeIndex(mIndex);
	ioContext.incrementNodesExecuted();
	ioContext.pushCallStack(0);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "Beagle::GP::Module",
	    std::string("Interpreting the ")+uint2ordinal(mIndex+1)+std::string(" module")
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "Beagle::GP::Module",
	    *ioTree
	);
	(*ioTree)[0].mPrimitive->execute(outResult, ioContext);
	ioContext.popCallStack();
	ioContext.checkExecutionTime();
	ioContext.setGenotypeHandle(lOldGenotypeHandle);
	ioContext.setGenotypeIndex(lOldGenotypeIndex);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Validate the module position in the tree.
 *  \param ioContext Evolutionary context.
 *  \return True if the module is correctly positioned, false if not.
 *  \throw Beagle::AssertException If the context is in a bad state.
 */
bool GP::Module::validate(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	for(unsigned int i=0; i<ioContext.getCallStackSize(); ++i) {
		if(ioContext.getGenotype()[ioContext.getCallStackElement(i)].mPrimitive->getName() == getName())
			return false;
	}
	Component::Handle lComponent = ioContext.getSystem().getComponent("ModuleVector");
	GP::ModuleVectorComponent::Handle lModVector =
	    castHandleT<GP::ModuleVectorComponent>(lComponent);
	if(lModVector==NULL) {
		throw Beagle_RunTimeExceptionM(std::string("GP system is not configured with a module vector. ")+
		                               std::string("Consider adding a GP::ModuleVectorComponent object to the system."));
	}
	Beagle_AssertM((*lModVector)[mIndex]!=NULL);
	if((*lModVector)[mIndex]->getNumberArguments() != getNumberArguments()) return false;
	return GP::Primitive::validate(ioContext);
	Beagle_StackTraceEndM();
}

