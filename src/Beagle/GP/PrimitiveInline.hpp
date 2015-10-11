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
 *  \file   beagle/GP/PrimitiveInline.hpp
 *  \brief  Implementation of some inlined methods of GP::Primitive.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/10 20:32:21 $
 */

#ifndef Beagle_GP_PrimitiveInline_hpp
#define Beagle_GP_PrimitiveInline_hpp

#include "beagle/GP/Tree.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Primitive.hpp"


/*!
 *  \brief Get index in the GP tree of the Nth argument to actual primitive.
 *  \param inN Number of the argument to get the index from in the GP tree.
 *  \param ioContext Evolutionary context. 
 */
inline unsigned int Beagle::GP::Primitive::getArgumentIndex(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	GP::Tree& lActualTree = ioContext.getGenotype();
	unsigned int lNodeIndex = ioContext.getCallStackTop() + 1;
	for(unsigned int i=0; i<inN; i++) lNodeIndex += lActualTree[lNodeIndex].mSubTreeSize;
	return lNodeIndex;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the value of the nth argument.
 *  \param inN Index of the argument to get.
 *  \param outResult Value of the nth argument.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If the argument index is invalid.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
inline void Beagle::GP::Primitive::getArgument(unsigned int inN,
        Beagle::GP::Datum& outResult,
        Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	using namespace Beagle;
	Beagle_UpperBoundCheckAssertM(inN,mNumberArguments-1);
	GP::Tree& lActualTree = ioContext.getGenotype();
	unsigned int lNodeIndex = getArgumentIndex(inN, ioContext);
	ioContext.incrementNodesExecuted();
	ioContext.checkExecutionTime();
	ioContext.pushCallStack(lNodeIndex);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "primitive", "Beagle::GP::Primitive",
	    std::string("Executing the primitive '")+
	    lActualTree[lNodeIndex].mPrimitive->getName()+std::string("' of the ")+uint2ordinal(lNodeIndex+1)+
	    std::string(" node of the ")+uint2ordinal(ioContext.getGenotypeIndex()+1)+" tree"
	);
	lActualTree[lNodeIndex].mPrimitive->execute(outResult, ioContext);
	ioContext.popCallStack();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the value of the arguments.
 *  \param outResults Value of the arguments. Array of the good size must be allocated.
 *  \param inSizeTDatum Size (in byte) of each datum of the array.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If the size of the bag doesn't match the nb. of arguments.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
inline void Beagle::GP::Primitive::getArguments(Beagle::GP::Datum outResults[],
        size_t inSizeTDatum,
        Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	using namespace Beagle;
	char* lCharResults = reinterpret_cast<char*>(outResults);
	GP::Tree& lActualTree = ioContext.getGenotype();
	unsigned int lNodeIndex = ioContext.getCallStackTop() + 1;
	ioContext.incrementNodesExecuted();
	ioContext.checkExecutionTime();
	ioContext.pushCallStack(lNodeIndex);
	lActualTree[lNodeIndex].mPrimitive->execute(*outResults, ioContext);
	ioContext.popCallStack();
	for(unsigned int i=1; i<mNumberArguments; i++) {
		lNodeIndex += lActualTree[lNodeIndex].mSubTreeSize;
		ioContext.incrementNodesExecuted();
		ioContext.checkExecutionTime();
		ioContext.pushCallStack(lNodeIndex);
		GP::Datum* lResultsI = reinterpret_cast<GP::Datum*>(lCharResults + (i*inSizeTDatum));
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "primitive", "Beagle::GP::Primitive",
		    std::string("Executing the primitive '")+
		    lActualTree[lNodeIndex].mPrimitive->getName()+std::string("' of the ")+
		    uint2ordinal(lNodeIndex+1)+std::string(" node of the ")+
		    uint2ordinal(ioContext.getGenotypeIndex()+1)+" tree"
		);
		lActualTree[lNodeIndex].mPrimitive->execute(*lResultsI, ioContext);
		ioContext.popCallStack();
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the value of the first argument.
 *  \param outResult Value of the first argument.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If there is no argument to the primitive.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
inline void Beagle::GP::Primitive::get1stArgument(Beagle::GP::Datum& outResult,
        Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	using namespace Beagle;
	Beagle_AssertM(getNumberArguments() >= 1);
	unsigned int lNodeIndex = ioContext.getCallStackTop() + 1;
	ioContext.incrementNodesExecuted();
	ioContext.checkExecutionTime();
	ioContext.pushCallStack(lNodeIndex);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "primitive", "Beagle::GP::Primitive",
	    std::string("Executing the primitive '")+
	    ioContext.getGenotype()[lNodeIndex].mPrimitive->getName()+std::string("' of the ")+
	    uint2ordinal(lNodeIndex+1)+std::string(" node of the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+" tree"
	);
	ioContext.getGenotype()[lNodeIndex].mPrimitive->execute(outResult, ioContext);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "primitive", "Beagle::GP::Primitive",
	    std::string("Result of executing '")+
	    ioContext.getGenotype()[lNodeIndex].mPrimitive->getName()+std::string("': ")+
	    outResult.serialize()
	);
	ioContext.popCallStack();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the value of the second argument.
 *  \param outResult Value of the second argument.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If there is no third argument to the primitive.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
inline void Beagle::GP::Primitive::get2ndArgument(Beagle::GP::Datum& outResult,
        Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	using namespace Beagle;
	Beagle_AssertM(getNumberArguments() >= 2);
	GP::Tree& lActualTree = ioContext.getGenotype();
	unsigned int lNodeIndex = getArgumentIndex(1, ioContext);
	ioContext.incrementNodesExecuted();
	ioContext.checkExecutionTime();
	ioContext.pushCallStack(lNodeIndex);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "primitive", "Beagle::GP::Primitive",
	    std::string("Executing the primitive '")+
	    lActualTree[lNodeIndex].mPrimitive->getName()+std::string("' of the ")+
	    uint2ordinal(lNodeIndex+1)+std::string(" node of the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+" tree"
	);
	lActualTree[lNodeIndex].mPrimitive->execute(outResult, ioContext);
	ioContext.popCallStack();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the value of the third argument.
 *  \param outResult Value of the third argument.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If there is no second argument to the primitive.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
inline void Beagle::GP::Primitive::get3rdArgument(Beagle::GP::Datum& outResult,
        Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	using namespace Beagle;
	Beagle_AssertM(getNumberArguments() >= 3);
	GP::Tree& lActualTree = ioContext.getGenotype();
	unsigned int lNodeIndex = getArgumentIndex(2, ioContext);
	ioContext.incrementNodesExecuted();
	ioContext.checkExecutionTime();
	ioContext.pushCallStack(lNodeIndex);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "primitive", "Beagle::GP::Primitive",
	    std::string("Executing the primitive '")+
	    lActualTree[lNodeIndex].mPrimitive->getName()+std::string("' of the ")+
	    uint2ordinal(lNodeIndex+1)+std::string(" node of the ")+
	    uint2ordinal(ioContext.getGenotypeIndex()+1)+" tree"
	);
	lActualTree[lNodeIndex].mPrimitive->execute(outResult, ioContext);
	ioContext.popCallStack();
	Beagle_StackTraceEndM();
}


/*!
  *  \brief Get the number of arguments of the primitive.
  *  \return Number of arguments of the primitive.
  */
inline unsigned int Beagle::GP::Primitive::getNumberArguments() const
{
	Beagle_StackTraceBeginM();
	return mNumberArguments;
	Beagle_StackTraceEndM();
}


/*!
  *  \brief Set the number of arguments of the primitive.
  *  \param inNumberArguments Number of arguments of the primitive.
  */
inline void Beagle::GP::Primitive::setNumberArguments(unsigned int inNumberArguments)
{
	Beagle_StackTraceBeginM();
	mNumberArguments = inNumberArguments;
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_PrimitiveInline_hpp

