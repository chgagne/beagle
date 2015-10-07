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
 *  \file   beagle/GP/src/ModuleExpandOp.cpp
 *  \brief  Implementation of ModuleExpandOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct an expand operator for evolutionary module acquisition.
 *  \param inName Name of the operator.
 */
GP::ModuleExpandOp::ModuleExpandOp(std::string inName) :
		Operator(inName)
{ }


/*!
 *  \brief Register the parameters of the GP expand operator for evolutionary module acquisition.
 *  \param ioSystem Evolutionary system.
 */
void GP::ModuleExpandOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::Operator::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Module primitive name for EMA",
		    "String",
		    "MODULE",
		    "Name of the GP primitive to use as module reference in the GP trees"
		);
		mModulePrimitName = castHandleT<String>(
		                        ioSystem.getRegister().insertEntry("gp.ema.modulename", new String("MODULE"), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Probability of an individual being expanded. Expansion randomly selects a ";
		lOSS << "module primitive and inserts it back into the tree.";
		Register::Description lDescription(
		    "Probability of an individual being expanded",
		    "Float",
		    "0.2",
		    lOSS.str()
		);
		mExpandProba = castHandleT<Float>(
		                   ioSystem.getRegister().insertEntry("gp.ema.expandpb", new Float(0.2f), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Expand given module of a GP tree.
 *  \param inNodeToExpand Index of node to expand in GP tree.
 *  \param ioTree Tree from which module will be expanded.
 *  \param ioContext Evolutionary context.
 */
void GP::ModuleExpandOp::expand(unsigned int inNodeToExpand,
                                GP::Tree& ioTree,
                                GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Log tree before expansion.
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "GP::ModuleExpandOp",
	    "Tree before expansion"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA",
	    "GP::ModuleExpandOp",
	    ioTree
	);

	// Get the module index and reference.
	Module::Handle lModuleInstance = castHandleT<Module>(ioTree[inNodeToExpand].mPrimitive);
	unsigned int lModuleIndex = lModuleInstance->getIndex();
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "GP::ModuleExpandOp",
	    std::string("Expanding ")+uint2ordinal(lModuleIndex+1)+
	    std::string(" module (called from ")+uint2ordinal(inNodeToExpand+1)+
	    std::string(" node of the tree)")
	);
	ModuleVectorComponent::Handle lModuleVectorComponent =
	    castHandleT<ModuleVectorComponent>(ioContext.getSystem().getComponent("ModuleVector"));
	if(lModuleVectorComponent==NULL) {
		throw Beagle_RunTimeExceptionM(std::string("GP system is not configured with a module vector. ")+
		                               std::string("Consider adding a GP::ModuleVectorComponent object to the system."));
	}
	Beagle::GP::Tree::Handle lModule = (*lModuleVectorComponent)[lModuleIndex];
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA",
	    "GP::ModuleExpandOp",
	    *lModule
	);

	// Generate new tree.
	const Factory& lFactory = ioContext.getSystem().getFactory();
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));
	GP::Tree::Handle lNewTree = castHandleT<GP::Tree>(lTreeAlloc->allocate());
	std::string lArgName = lModuleInstance->getArgsName();
	ioTree.setContextToNode(inNodeToExpand, ioContext);
	for(unsigned int i=0; i<lModule->size(); ++i) {
		if((*lModule)[i].mPrimitive->getName() != lArgName) {
			lNewTree->push_back(GP::Node((*lModule)[i].mPrimitive));
		} else {
			GP::Argument::Handle lArg = castHandleT<GP::Argument>((*lModule)[i].mPrimitive);
			const unsigned int lChildIndex =
			    ioTree[inNodeToExpand].mPrimitive->getChildrenNodeIndex(lArg->getIndex(), ioContext);
			lNewTree->insert(lNewTree->end(), ioTree.begin()+lChildIndex,
			                 ioTree.begin()+lChildIndex+ioTree[lChildIndex].mSubTreeSize);
		}
	}
	ioTree.erase(ioTree.begin()+inNodeToExpand,
	             ioTree.begin()+inNodeToExpand+ioTree[inNodeToExpand].mSubTreeSize);
	ioTree.insert(ioTree.begin()+inNodeToExpand, lNewTree->begin(), lNewTree->end());
	ioTree.fixSubTreeSize();

	// Log results.
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA",
	    "GP::ModuleExpandOp",
	    "Tree after expansion"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA",
	    "GP::ModuleExpandOp",
	    ioTree
	);
	
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Expand modules in trees of actual population.
 *  \param ioDeme Deme to apply operation.
 *  \param ioContext Evolutionary context.
 */
void GP::ModuleExpandOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "GP::ModuleExpandOp",
	    std::string("Expanding modules of the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	if(mExpandProba->getWrappedValue()==0.0) return;

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "GP::ModuleExpandOp",
	    std::string("Expanding modules with probability ")+
	    dbl2str(mExpandProba->getWrappedValue())
	);

	GP::Context& lGPContext = castObjectT<GP::Context&>(ioContext);
	GP::Individual::Handle lOldIndividualHandle = lGPContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = lGPContext.getIndividualIndex();
	GP::Tree::Handle lOldGenotypeHandle = lGPContext.getGenotypeHandle();
	unsigned int lOldGenotypeIndex = lGPContext.getGenotypeIndex();
	const std::string lModuleName = mModulePrimitName->getWrappedValue();
	for(unsigned int i=0; i<ioDeme.size(); i++) {
		for(unsigned int j=0; j<ioDeme[i]->size(); ++j) {
			GP::Tree& lTree = castObjectT<GP::Tree&>(*(*ioDeme[i])[j]);
			for(unsigned int k=0; k<lTree.size(); ++k) {
				if(lTree[k].mPrimitive->getName() == lModuleName) {
					if(ioContext.getSystem().getRandomizer().rollUniform() <= mExpandProba->getWrappedValue()) {
						lGPContext.setIndividualHandle(castHandleT<GP::Individual>(ioDeme[i]));
						lGPContext.setIndividualIndex(i);
						lGPContext.setGenotypeHandle(castHandleT<GP::Tree>((*ioDeme[i])[j]));
						lGPContext.setGenotypeIndex(j);
						Beagle_LogVerboseM(
						    ioContext.getSystem().getLogger(),
						    "expand", "ExpandOp",
						    std::string("Expanding the ") +uint2ordinal(i+1)+" individual"
						);
						expand(k, lTree, lGPContext);
					}
				}
			}
		}
	}
	lGPContext.setGenotypeHandle(lOldGenotypeHandle);
	lGPContext.setGenotypeIndex(lOldGenotypeIndex);
	lGPContext.setIndividualHandle(lOldIndividualHandle);
	lGPContext.setIndividualIndex(lOldIndividualIndex);

	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "EMA", "GP::ModuleExpandOp",
	    *(ioContext.getSystem().getComponent("ModuleVector"))
	);
	Beagle_StackTraceEndM();
}
