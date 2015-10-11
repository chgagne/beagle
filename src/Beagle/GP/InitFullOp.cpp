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
 *  \file   beagle/GP/src/InitFullOp.cpp
 *  \brief  Source code of class GP::InitFullOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.25 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a GP "full" intialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
GP::InitFullOp::InitFullOp(std::string inReproProbaName, std::string inName) :
		Beagle::GP::InitializationOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Initialize a GP sub-tree of a specified depth using the "full" approach.
 *  \param ioTree Tree containing the sub-tree to initialize.
 *  \param inSubTreeDepth Depth of the sub-tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \return Generated sub-tree size.
 */
unsigned int GP::InitFullOp::initSubTreeFull(GP::Tree& ioTree,
        unsigned int inSubTreeDepth,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inSubTreeDepth>0);
	GP::PrimitiveSet& lPrimitSet = ioTree.getPrimitiveSet(ioContext);
	GP::Primitive::Handle lPrimit = NULL;
	if(inSubTreeDepth == 1) {
		lPrimit = lPrimitSet.select(GP::Primitive::eTerminal, ioContext);
		if(!lPrimit) {
			std::string lMessage = "There is no leaf (primitive without argument) in the ";
			lMessage += uint2ordinal(ioTree.getPrimitiveSetIndex()+1);
			lMessage += " primitive set!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		lPrimit = lPrimit->giveReference(GP::Primitive::eTerminal, ioContext);
	} else {
		lPrimit = lPrimitSet.select(GP::Primitive::eBranch, ioContext);
		if(!lPrimit) {
			std::string lMessage = "There is no branch (primitive with arguments) in the ";
			lMessage += uint2ordinal(ioTree.getPrimitiveSetIndex()+1);
			lMessage += " primitive set!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		lPrimit = lPrimit->giveReference(GP::Primitive::eBranch, ioContext);
	}
	unsigned int lNodeIndex = ioTree.size();
	ioTree.push_back(GP::Node(lPrimit,0));
	unsigned int lSubTreeSize = 1;
	for(unsigned int i=0; i<ioTree[lNodeIndex].mPrimitive->getNumberArguments(); i++) {
		lSubTreeSize += initSubTreeFull(ioTree, inSubTreeDepth-1, ioContext);
	}
	ioTree[lNodeIndex].mSubTreeSize = lSubTreeSize;
	return lSubTreeSize;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize a tree.
 *  \param outTree Tree to initialize.
 *  \param inMinDepth Minimum depth to make tree.
 *  \param inMaxDepth Maximum depth to make tree.
 *  \param ioContext Evolution context.
 */
unsigned int GP::InitFullOp::initTree(GP::Tree& outTree,
                                      unsigned int inMinDepth,
                                      unsigned int inMaxDepth,
                                      GP::Context &ioContext) const
{
	Beagle_StackTraceBeginM();

	const unsigned int lDepth = inMaxDepth;
	Beagle_AssertM(lDepth>0);

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GP::InitFullOp",
	    std::string("Using the \'full\' method (with depth ")+
	    uint2str(lDepth)+std::string(") to initialize the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+std::string(" tree.")
	);

	return initSubTreeFull(outTree, lDepth, ioContext);
	Beagle_StackTraceEndM();
}


