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
 *  \file   beagle/GP/src/InitFullConstrainedOp.cpp
 *  \brief  Source code of class GP::InitFullConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.24 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

#include <sstream>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree "full" intialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GP::InitFullConstrainedOp::InitFullConstrainedOp(std::string inReproProbaName,
        std::string inName) :
		InitFullOp(inReproProbaName, inName),
		mNumberAttempts(NULL)
{ }


/*!
 *  \brief Initialize a GP constrained sub-tree of a specified depth using the "full" approach.
 *  \param ioTree Tree containing the sub-tree to initialize.
 *  \param inSubTreeDepth Depth of the sub-tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \return Generated sub-tree size.
 */
unsigned int GP::InitFullConstrainedOp::initConstrainedSubTreeFull(GP::Tree& ioTree,
        unsigned int inSubTreeDepth,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inSubTreeDepth>0);

	GP::PrimitiveSet& lPrimitSet = ioTree.getPrimitiveSet(ioContext);
	GP::Primitive::Handle lPrimit = NULL;
	const unsigned int lNodeIndex = ioTree.size();
	for(unsigned int lAttempt=0; lAttempt < mNumberAttempts->getWrappedValue(); ++lAttempt) {
#ifdef BEAGLE_HAVE_RTTI
		const std::type_info* lDesiredType = NULL;
		if(ioTree.size()==0) lDesiredType = ioTree.getRootType(ioContext);
		else {
			const unsigned int lParentIndex = ioContext.getCallStackTop();
			unsigned int lArgsIndexChild = 0;
			for(unsigned int lChildIndex=(lParentIndex+1);
			        lChildIndex!=lNodeIndex; lChildIndex += ioTree[lChildIndex].mSubTreeSize) {
				Beagle_AssertM(lChildIndex <= ioTree.size());
				++lArgsIndexChild;
				Beagle_AssertM(lArgsIndexChild < ioTree[lParentIndex].mPrimitive->getNumberArguments());
			}
			lDesiredType = ioTree[lParentIndex].mPrimitive->getArgType(lArgsIndexChild, ioContext);
		}
		if(inSubTreeDepth == 1) {
			lPrimit = lPrimitSet.selectWithType(GP::Primitive::eTerminal, lDesiredType, ioContext);
			if(!lPrimit) return 0;
			lPrimit = lPrimit->giveReference(GP::Primitive::eTerminal, ioContext);
		} else {
			lPrimit = lPrimitSet.selectWithType(GP::Primitive::eBranch, lDesiredType, ioContext);
			if(!lPrimit) return 0;
			lPrimit = lPrimit->giveReference(GP::Primitive::eBranch, ioContext);
		}
#else // BEAGLE_HAVE_RTTI
		if(inSubTreeDepth == 1) {
			lPrimit = lPrimitSet.select(GP::Primitive::eTerminal, ioContext);
			if(!lPrimit) {
				string lMessage = "There is no leaf (primitive without argument) in the ";
				lMessage += uint2ordinal(ioContext.getGenotypeIndex());
				lMessage += " primitive set!";
				throw Beagle_RunTimeExceptionM(lMessage);
			}
			lPrimit = lPrimit->giveReference(GP::Primitive::eTerminal, ioContext);
		} else {
			lPrimit = lPrimitSet.select(GP::Primitive::eBranch, ioContext);
			if(!lPrimit) {
				string lMessage = "There is no branch (primitive with arguments) in the ";
				lMessage += uint2ordinal(ioContext.getGenotypeIndex());
				lMessage += " primitive set!";
				throw Beagle_RunTimeExceptionM(lMessage);
			}
			lPrimit = lPrimit->giveReference(GP::Primitive::eBranch, ioContext);
		}
#endif // BEAGLE_HAVE_RTTI
		ioTree.push_back(GP::Node(lPrimit, 0));
		ioContext.pushCallStack(lNodeIndex);
		if(lPrimit->validate(ioContext)) {
			unsigned int lSubTreeSize = 1;
			bool lGoodInit = true;
			for(unsigned int i=0; i<lPrimit->getNumberArguments(); i++) {
				unsigned int lArgSubTreeSize =
				    initConstrainedSubTreeFull(ioTree, inSubTreeDepth-1, ioContext);
				if(lArgSubTreeSize == 0) {
					for(unsigned int j=1; j<lSubTreeSize; j++) ioTree.pop_back();
					lGoodInit = false;
					break;
				}
				lSubTreeSize += lArgSubTreeSize;
			}
			if(lGoodInit) {
				ioContext.popCallStack();
				ioTree[lNodeIndex].mSubTreeSize = lSubTreeSize;
				return lSubTreeSize;
			}
		} else {
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "initialization", "Beagle::GP::InitFullConstrainedOp",
			    "Primitive failed validation testing"
			);
		}
		ioContext.popCallStack();
		ioTree.pop_back();
	}
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GP::InitFullConstrainedOp",
	    "Could not correctly initialize this node; backtracking instead."
	);
	return 0; // Could not initialize this node correctly, backtracking instead.
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the GP "full" intialization operator.
 *  \param ioSystem System of the evolution.
 */
void GP::InitFullConstrainedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	InitFullOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "Maximum number of attempts to modify a GP tree in a genetic ";
	lOSS << "operation. As there is topological constraints on GP trees (i.e. tree ";
	lOSS << "depth limit), it is often necessary to try a genetic operation several times.";
	Register::Description lDescription("Max number of attempts", "UInt", "2", lOSS.str());
	mNumberAttempts =
	    castHandleT<UInt>(ioSystem.getRegister().insertEntry("gp.try", new UInt(2), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize a constrained GP tree of a specified depth using the "full" approach.
 *  \param outTree Tree to initialize.
 *  \param inMinDepth Minimum depth to make tree.
 *  \param inMaxDepth Maximum depth to make tree.
 *  \param ioContext Evolutionary context.
 */
unsigned int GP::InitFullConstrainedOp::initTree(GP::Tree& outTree,
        unsigned int inMinDepth,
        unsigned int inMaxDepth,
        GP::Context &ioContext) const
{
	Beagle_StackTraceBeginM();

	const unsigned int lDepth = inMaxDepth;
	Beagle_AssertM(lDepth>0);

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GP::InitFullConstrainedOp",
	    std::string("Using the constrained \'full\' method (with depth ")+
	    uint2str(lDepth)+std::string(") to initialize the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+std::string(" tree.")
	);

	unsigned int lSubTreeSize;
	do {
		lSubTreeSize = initConstrainedSubTreeFull(outTree, lDepth, ioContext);
	} while (lSubTreeSize == 0);
	return lSubTreeSize;
	Beagle_StackTraceEndM();
}

