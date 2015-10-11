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
 *  \file   beagle/GP/src/InitGrowOp.cpp
 *  \brief  Source code of class GP::InitGrowOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.29 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a GP "grow" intialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
GP::InitGrowOp::InitGrowOp(std::string inReproProbaName, std::string inName) :
		Beagle::GP::InitializationOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree "grow" intialization operator.
 *  \param ioSystem System of the evolution.
 */
void GP::InitGrowOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	GP::InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Flag for whether the 'grow' method works as defined by Koza",
		    "Bool",
		    "0",
		    "Set this flag if you wish to initialize individuals using the 'grow' method as defined by Koza in GPI (and as used in GPII, GPIII, and probably GPIV---although it is not made entirely clear).  If this flag is set to true then individuals are grown with a minimum depth of 2.  If this flag is set to false then the minimum depth of individuals is paired to the register variable 'gp.init.mindepth'.  Irrespective of this flag, maximum depth of individuals will range according to the values of 'gp.init.mindepth' and 'gp.init.maxdepth'; note that if you wish to entirely replicate Koza's method then mindepth and maxdepth should be set to the same value."
		);
		mKozaGrow = castHandleT<Bool>
		            (ioSystem.getRegister().insertEntry("gp.init.kozagrow", new Bool(false), lDescription));
	}

	Beagle_StackTraceEndM();
}



/*!
 *  \brief Initialize a GP sub-tree of a specified depth using the "grow" approach.
 *  \param ioTree Tree containing the sub-tree to initialize.
 *  \param inMinDepth Minimal depth of the sub-tree to initialize.
 *  \param inMaxDepth Maximal depth of the sub-tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \return Generated sub-tree size.
 */
unsigned int GP::InitGrowOp::initSubTreeGrow(GP::Tree& ioTree,
        unsigned int inMinDepth,
        unsigned int inMaxDepth,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inMaxDepth >= inMinDepth);
	Beagle_AssertM(inMinDepth>0);
	GP::PrimitiveSet& lPrimitSet = ioTree.getPrimitiveSet(ioContext);
	GP::Primitive::Handle lPrimit = NULL;
	if(inMinDepth > 1) {
		lPrimit = lPrimitSet.select(GP::Primitive::eBranch, ioContext);
		if(!lPrimit) {
			std::string lMessage = "There is no branch (primitive with arguments) in the ";
			lMessage += uint2ordinal(ioContext.getGenotypeIndex()+1);
			lMessage += " primitive set!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		lPrimit = lPrimit->giveReference(GP::Primitive::eBranch, ioContext);
	} else if(inMaxDepth == 1) {
		lPrimit = lPrimitSet.select(GP::Primitive::eTerminal, ioContext);
		if(!lPrimit) {
			std::string lMessage = "There is no leaf (primitive without argument) in the ";
			lMessage += uint2ordinal(ioContext.getGenotypeIndex()+1);
			lMessage += " primitive set!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		lPrimit = lPrimit->giveReference(GP::Primitive::eTerminal, ioContext);
	} else {
		lPrimit = lPrimitSet.select(GP::Primitive::eAny, ioContext);
		if(!lPrimit) {
			std::string lMessage = "There is no primitive in the ";
			lMessage += uint2ordinal(ioContext.getGenotypeIndex()+1);
			lMessage += " primitive set!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		lPrimit = lPrimit->giveReference(GP::Primitive::eAny, ioContext);
	}
	unsigned int lNodeIndex = ioTree.size();
	ioTree.push_back(GP::Node(lPrimit, 1));
	unsigned int lSubTreeSize = 1;
	unsigned int lMinDepth = (inMinDepth > 1) ? (inMinDepth-1) : 1;
	for(unsigned int i=0; i<ioTree[lNodeIndex].mPrimitive->getNumberArguments(); i++) {
		lSubTreeSize += initSubTreeGrow(ioTree, lMinDepth, inMaxDepth-1, ioContext);
	}
	ioTree[lNodeIndex].mSubTreeSize = lSubTreeSize;
	return lSubTreeSize;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize a tree.
 *  \param outTree Tree to initialize.
 *  \param inMinDepth Minimum depth of to make tree.
 *  \param inMaxDepth Maximum depth of to make tree.
 *  \param ioContext Evolution context.
 *  \return Size of newly initialized tree.
 */
unsigned int GP::InitGrowOp::initTree(GP::Tree& outTree,
                                      unsigned int inMinDepth,
                                      unsigned int inMaxDepth,
                                      GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();

	if (mKozaGrow->getWrappedValue()) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "initialization", "Beagle::GP::InitGrowConstrainedOp",
		    "Setting the minimum depth to 2 (as per 'gp.init.kozagrow')"
		);
		inMinDepth=2;
	}

	Beagle_AssertM(inMaxDepth >= inMinDepth);
	Beagle_AssertM(inMinDepth>0);

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GP::InitGrowOp",
	    std::string("Using the \'grow\' method (with maximum depth ")+uint2str(inMaxDepth)+
	    std::string(" and minimum depth ")+uint2str(inMinDepth)+std::string(") to initialize the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+std::string(" tree")
	);

	return initSubTreeGrow(outTree, inMinDepth, inMaxDepth, ioContext);
	Beagle_StackTraceEndM();
}

