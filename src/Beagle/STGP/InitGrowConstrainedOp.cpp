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
 *  \file   beagle/GP/src/InitGrowConstrainedOp.cpp
 *  \brief  Source code of class GP::InitGrowConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.26 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

#include <sstream>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree "grow" intialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GP::InitGrowConstrainedOp::InitGrowConstrainedOp(std::string inReproProbaName,
        std::string inName) :
		InitGrowOp(inReproProbaName, inName),
		mNumberAttempts(NULL)
{ }


/*!
 *  \brief Initialize a GP constrained sub-tree of a specified depth using the "grow" approach.
 *  \param ioTree Tree containing the sub-tree to initialize.
 *  \param inMinDepth Minimal depth of the sub-tree to initialize.
 *  \param inMaxDepth Maximal depth of the sub-tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \return Generated sub-tree size.
 */
unsigned int GP::InitGrowConstrainedOp::initConstrainedSubTreeGrow(GP::Tree& ioTree,
        unsigned int inMinDepth,
        unsigned int inMaxDepth,
        GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inMaxDepth >= inMinDepth);
	Beagle_AssertM(inMinDepth>0);
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
		if(inMinDepth > 1) {
			lPrimit = lPrimitSet.selectWithType(GP::Primitive::eBranch, lDesiredType, ioContext);
			if(!lPrimit) return 0;
			lPrimit = lPrimit->giveReference(GP::Primitive::eBranch, ioContext);
		} else if(inMaxDepth == 1) {
			lPrimit = lPrimitSet.selectWithType(GP::Primitive::eTerminal, lDesiredType, ioContext);
			if(!lPrimit) return 0;
			lPrimit = lPrimit->giveReference(GP::Primitive::eTerminal,ioContext);
		} else {
			lPrimit = lPrimitSet.selectWithType(GP::Primitive::eAny, lDesiredType, ioContext);
			if(!lPrimit) return 0;
			lPrimit = lPrimit->giveReference(GP::Primitive::eAny, ioContext);
		}
#else // BEAGLE_HAVE_RTTI
		if(inMinDepth > 1) {
			lPrimit = lPrimitSet.select(GP::Primitive::eBranch, ioContext);
			if(!lPrimit) {
				string lMessage = "There is no branch (primitive with argument) in the ";
				lMessage += uint2ordinal(ioContext.getGenotypeIndex()+1);
				lMessage += " primitive set!";
				throw Beagle_RunTimeExceptionM(lMessage);
			}
			lPrimit = lPrimit->giveReference(GP::Primitive::eBranch, ioContext);
		} else if(inMaxDepth == 1) {
			lPrimit = lPrimitSet.select(GP::Primitive::eTerminal, ioContext);
			if(!lPrimit) {
				string lMessage = "There is no leaf (primitive without argument) in the ";
				lMessage += uint2ordinal(ioContext.getGenotypeIndex()+1);
				lMessage += " primitive set!";
				throw Beagle_RunTimeExceptionM(lMessage);
			}
			lPrimit = lPrimit->giveReference(GP::Primitive::eTerminal,ioContext);
		} else {
			lPrimit = lPrimitSet.select(GP::Primitive::eAny, ioContext);
			if(!lPrimit) {
				string lMessage = "There is no primitive in the ";
				lMessage += uint2ordinal(ioContext.getGenotypeIndex()+1);
				lMessage += " primitive set!";
				throw Beagle_RunTimeExceptionM(lMessage);
			}
			lPrimit = lPrimit->giveReference(GP::Primitive::eAny, ioContext);
		}
#endif // BEAGLE_HAVE_RTTI
		ioTree.push_back(GP::Node(lPrimit, 0));
		ioContext.pushCallStack(lNodeIndex);
		if(lPrimit->validate(ioContext)) {
			unsigned int lSubTreeSize = 1;
			unsigned int lMinDepth = (inMinDepth > 1) ? (inMinDepth-1) : 1;
			bool lGoodInit = true;
			for(unsigned int i=0; i<lPrimit->getNumberArguments(); i++) {
				unsigned int lArgSubTreeSize =
				    initConstrainedSubTreeGrow(ioTree, lMinDepth, inMaxDepth-1, ioContext);
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
		}
		ioContext.popCallStack();
		ioTree.pop_back();
	}
	return 0;  // Could not initialize this node correctly, backtracking instead.
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the constrained GP tree "grow" intialization operator.
 *  \param ioSystem System of the evolution.
 */
void GP::InitGrowConstrainedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	InitGrowOp::registerParams(ioSystem);
	{
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
		mNumberAttempts = castHandleT<UInt>
		                  (ioSystem.getRegister().insertEntry("gp.try", new UInt(2), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize a constrained GP tree of a specified depth using the "grow" approach.
 *  \param outTree Tree to initialize.
 *  \param inMinDepth Minimal depth of the tree to initialize.
 *  \param inMaxDepth Maximal depth of the tree to initialize.
 *  \param ioContext Evolutionary context.
 *  \return Size of newly initialized tree.
 */
unsigned int GP::InitGrowConstrainedOp::initTree(GP::Tree& outTree,
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
	    "initialization", "Beagle::GP::InitGrowConstrainedOp",
	    std::string("Using the constrained \'grow\' method (with maximum depth ")+uint2str(inMaxDepth)+
	    std::string(" and minimum depth ")+uint2str(inMinDepth)+std::string(") to initialize the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+std::string(" tree")
	);

	unsigned int lSubTreeSize;
	do {
		lSubTreeSize = initConstrainedSubTreeGrow(outTree, inMinDepth, inMaxDepth, ioContext);
	} while(lSubTreeSize == 0);
	return lSubTreeSize;
	Beagle_StackTraceEndM();
}

