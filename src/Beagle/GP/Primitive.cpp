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
 *  \file   beagle/GP/src/Primitive.cpp
 *  \brief  Implementation of the type GP::Primitive.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.30 $
 *  $Date: 2007/08/10 20:32:21 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a primitive.
 *  \param inNumberArguments Number of arguments of the primitive.
 *  \param inName Name of the primitive.
 */
GP::Primitive::Primitive(unsigned int inNumberArguments, std::string inName) :
		NamedObject(inName),
		mNumberArguments(inNumberArguments)
{ }


/*!
 *  \brief Write primitive as C function
 */
std::string GP::Primitive::deparse(std::vector<std::string>& lSubTrees) const
{
	std::ostringstream lOSS;
	lOSS << getName() << "(";
	unsigned int lNumArgs = getNumberArguments();
	Beagle_AssertM(lNumArgs == lSubTrees.size());

	for (unsigned int i=0; i<lNumArgs; i++) {
		lOSS << lSubTrees[i];
		if (i<lNumArgs-1) lOSS << ",";
	}
	lOSS << ")";
	return lOSS.str();
}



#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the nth argument requested return type (for STGP).
 *  \param  inN Index of the argument to get the type.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) with the argument's node type.
 *  \throw  Beagle::InternalException If the method is not overdefined is a subclass.
 */
const std::type_info* GP::Primitive::getArgType(unsigned int inN, GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	throw Beagle::InternalException(std::string("Method 'getArgType' of class 'GP::Primitive'").
	                                append(" is called but is not properly overdefined in primitive '").append(getName()).
	                                append("'"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the primitive return type (for STGP).
 *  \param  inDesiredType Desired return type.
 *  \param  inNumberArguments Number of arguments to get the return type from.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) with the primitive return type.
 *  \throw  Beagle::InternalException If the method is not overdefined is a subclass.
 */
const std::type_info* GP::Primitive::getReturnType(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	throw Beagle::InternalException(std::string("Method 'getReturnType' of class 'GP::Primitive'").
	                                append(" is called but is not properly overdefined in primitive '").append(getName()).
	                                append("'"));
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief  Return the children's node index.
 *  \param  inN Index of the children as arguments (index 0 is first arguments).
 *  \param  ioContext Actual evolutionary context.
 *  \return Index of the given nth children.
 *  \throw  Beagle::AssertException If the children index is out-of-bound.
 */
unsigned int GP::Primitive::getChildrenNodeIndex(unsigned int inN, GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_UpperBoundCheckAssertM(inN,mNumberArguments-1);
	GP::Tree& lActualTree = ioContext.getGenotype();
	unsigned int lNodeIndex = ioContext.getCallStackTop() + 1;
	for(unsigned int i=0; i<inN; i++) lNodeIndex += lActualTree[lNodeIndex].mSubTreeSize;
	return lNodeIndex;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the selection weight multiplier for the primitive.
 *  \param inNumberArguments Number of arguments for the selection weight.
 *  \param  ioContext Evolutionary context.
 *  \return Selection weight multiplier.
 */
double GP::Primitive::getSelectionWeight(unsigned int inNumberArguments, GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	if(inNumberArguments == GP::Primitive::eAny) return 1.0;
	else if(inNumberArguments == GP::Primitive::eBranch) {
		if(mNumberArguments > 0) return 1.0;
		else return 0.0;
	} else if(inNumberArguments == mNumberArguments) return 1.0;
	return 0.0;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the primitive value.
 *  \param outValue Object in which the actual primitive value is written.
 *  \throw Beagle::ObjectException If the method is not overdefined is a subclass.
 */
void GP::Primitive::getValue(Object& outValue)
{
	Beagle_StackTraceBeginM();
	throw Beagle::InternalException(std::string("Method 'getValue' of class 'GP::Primitive'").
	                                append(" is called but is not properly overdefined in primitive '").append(getName()).
	                                append("'"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Give a reference to the current primitive.
 *  \param inNumberArguments Number of arguments asked for.
 *  \param ioContext Reference to the evolution context.
 *  \return Handle to the refered primitive.
 */
GP::Primitive::Handle GP::Primitive::giveReference(unsigned int inNumberArguments,
        GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	return GP::Primitive::Handle(this);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return whether primitive's getValue and setValue are effective.
 *  \return True if getValue/setValue method are effective, false if not.
 */
bool GP::Primitive::haveValue() const
{
	Beagle_StackTraceBeginM();
	return false;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this primitive.
 *  \param ioSystem Evolutionary system used.
 */
void GP::Primitive::registerParams(System& ioSystem)
{ }


/*!
 *  \brief Compare equality of two primitives. Name, number of args. and attribute must be the same.
 *  \param inRightObj Primitive to compare to the actual one.
 *  \return True if primitive are identical, false if not.
 */
bool GP::Primitive::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const GP::Primitive& lRightPrimitive = castObjectT<const GP::Primitive&>(inRightObj);
	if((getName() != lRightPrimitive.getName()) ||
	        (mNumberArguments != lRightPrimitive.mNumberArguments)) return false;
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Stability of selection weight. A weight is said stable if it doesn't change over time.
 *  \param inNumberArguments Number of arguments to check stability of selection weight.
 *  \return True if selection weight is stable, false if not.
 */
bool GP::Primitive::isSelectionWeightStable(unsigned int inNumberArguments) const
{
	Beagle_StackTraceBeginM();
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the primitive from XML subtree.
 *  \param inIter XML iterator used to read the primitive from.
 *  \param ioContext Evolutionary context.
 *  \throw IOException If primitive format is not respected.
 */
void GP::Primitive::readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(inIter->getType() != PACC::XML::eData)
		throw Beagle_IOExceptionNodeM(*inIter, "tag expected!");

	if(inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the value of a primitive.
 *  \param inValue Value of the primitive.
 *  \throw Beagle::ObjectException If the method is not overdefined is a subclass.
 */
void GP::Primitive::setValue(const Object& inValue)
{
	Beagle_StackTraceBeginM();
	throw Beagle::InternalException(std::string("Method 'setValue' of class 'GP::Primitive'").
	                                append(" is called but is not properly overdefined in primitive '").append(getName()).
	                                append("'"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Validate the primitive connections in the tree when using contrained or strongly-typed GP.
 *  \param ioContext Evolutionary context.
 *  \return True if the tree topology is correct, false if not.
 *  \throw Beagle::AssertException If the context is in a bad state.
 */
bool GP::Primitive::validate(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_RTTI

	Beagle_AssertM(ioContext.getCallStackSize() != 0);
	GP::Tree& lActualTree = ioContext.getGenotype();

	// If the primitive is the root of the tree, check its type is correct.
	if(ioContext.getCallStackTop()==0) {
		const std::type_info* lTreeRootType = lActualTree.getRootType(ioContext);
		const std::type_info* lReturnType = getReturnType(ioContext);
		if((lTreeRootType==NULL) || ((lReturnType!=NULL) && ((*lTreeRootType)==(*lReturnType)))) {
			// Valid.
			return true;
		} else {
			// Not valid.
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "primitive", "Beagle::GP::Primitive",
			    std::string("Validation of '").append(getName()).append("' primitive failed. ").
			    append("The primitive is at the root of the ").
			    append(uint2ordinal(ioContext.getGenotypeIndex()+1)).append(" tree and its type ('").
			    append(lReturnType->name()).append("') does not match the type required as tree root ").
			    append("('").append(lTreeRootType->name()).append("').")
			);
			return false;
		}
	}

	unsigned int lParentIndex    = ioContext.getCallStackElement(ioContext.getCallStackSize()-2);
	unsigned int lChildIndex     = lParentIndex + 1;
	unsigned int lArgsIndexChild = 0;
	while(lChildIndex != ioContext.getCallStackTop()) {
		lChildIndex += lActualTree[lChildIndex].mSubTreeSize;
		Beagle_AssertM(lChildIndex < lActualTree.size());
		lArgsIndexChild++;
		Beagle_AssertM(lArgsIndexChild < lActualTree[lParentIndex].mPrimitive->getNumberArguments());
	}

	// Check that the return type of this node matches the type expected by this node's parent.
	unsigned int lActualNodeIndex = ioContext.getCallStackTop();
	ioContext.popCallStack();
	const std::type_info* lDesiredType =
	    lActualTree[lParentIndex].mPrimitive->getArgType(lArgsIndexChild, ioContext);
	ioContext.pushCallStack(lActualNodeIndex);
	const std::type_info* lReturnedType = getReturnType(ioContext);
	if((lDesiredType==NULL) || (lReturnedType==NULL) || ((*lDesiredType)==(*lReturnedType))) {
		// Valid.
		return true;
	} else {
		// Not valid.
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "primitive", "Beagle::GP::Primitive",
		    std::string("Validation of '").append(getName()).append("' primitive failed.").
		    append(" The primitive is at the ").
		    append(uint2ordinal(ioContext.getCallStackTop()+1)).append(" node of the ").
		    append(uint2ordinal(ioContext.getGenotypeIndex()+1)).append(" tree and its type ('").
		    append(lReturnedType->name()).append("') does not match the desired type ('").
		    append(lDesiredType->name()).append("') required by its parent ('").
		    append(lActualTree[lParentIndex].mPrimitive->getName()).append("') at the ").
		    append(uint2ordinal(lParentIndex)).append(" node of the same tree.")
		);
		return false;
	}

#else // BEAGLE_HAVE_RTTI

	return true;

#endif // BEAGLE_HAVE_RTTI
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the primitive.
 *  \param ioStreamer XML streamer to write the primitive into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::Primitive::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the primitive content.
 *  \param ioStreamer XML streamer to write the primitive into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::Primitive::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
	{ }

