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
 *  \file   beagle/GP/src/Invoker.cpp
 *  \brief  Implementation of GP::Invoker.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new Invoker primitive.
 *  \param inIndex Index of the associated tree.
 *  \param inNumberArgs Number of arguments of the Invoker.
 *  \param inName Name of the Invoker primitive.
 *  \param inArgsName Name of the associated argument primitive.
 */
GP::Invoker::Invoker(unsigned int inIndex,
                     unsigned int inNumberArgs,
                     std::string inName,
                     std::string inArgsName) :
		GP::Primitive(inNumberArgs, inName),
		mArgsName(inArgsName),
		mIndex(inIndex)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return the tag of the type of data needed as input for the primitive.
 *  \param inN Index of the argument to get the type tag.
 *  \param ioContext Evolutionary context.
 *  \return String tagging the data type needed.
 *  \throw AssertException If the index inN given is > to 1.
 *  \throw InternalException If RTTI is disabled and the method is not properly overdefined.
 */
const std::type_info* GP::Invoker::getArgType(unsigned int inN, GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN < getNumberArguments());
	if(mIndex == eGenerator) {
		std::ostringstream lOSS;
		lOSS << "Could not get the arguments typing of the '" << getName() << "' invoker primitive. ";
		lOSS << "The primitive is actually in a generator state, so the index of the tree ";
		lOSS << "refered by the primitive is not specified. It is likely that there is a problem ";
		lOSS << "in the setting of your system, as the argument typing is called only when the ";
		lOSS << "primitive is completely instantiated, that is used as the part of a GP tree, ";
		lOSS << "with an index value refering to another GP tree to invoke.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	GP::Tree::Handle lTree = getInvokedTree(ioContext);
	GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	ioContext.setGenotypeHandle(lTree);
	GP::Argument::Handle lArg =
	    castHandleT<GP::Argument>(lTree->getPrimitiveSet(ioContext).getPrimitiveByName(mArgsName));
	const std::type_info* lArgType = lArg->getReturnType(ioContext);
	ioContext.setGenotypeHandle(lOldTreeHandle);
	return lArgType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the string tagging the type of data returned by the primitive.
 *  \param  ioContext Evolutionary context.
 *  \return String tagging the type of data returned.
 *  \throw  InternalException If RTTI is disabled and the method is not properly overdefined.
 */
const std::type_info* GP::Invoker::getReturnType(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	if(mIndex == eGenerator) return NULL;
	GP::Tree::Handle lTree = getInvokedTree(ioContext);
	GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
	const unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();
	ioContext.setGenotypeHandle(lTree);
	ioContext.setGenotypeIndex(mIndex);
	const std::type_info* lReturnType = lTree->getRootType(ioContext);
	ioContext.setGenotypeIndex(lOldTreeIndex);
	ioContext.setGenotypeHandle(lOldTreeHandle);
	return lReturnType;
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Return selection weight (in primitive set) of the ADF primitive.
 *  \param inNumberArguments Number of arguments called for the selection weight.
 *  \param ioContext Evolutionary context.
 *  \return Selection weight.
 */
double GP::Invoker::getSelectionWeight(unsigned int inNumberArguments,
                                       GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	// If not a generator, return the default primitive weight value.
	if(mIndex!=eGenerator)
		return Beagle::GP::Primitive::getSelectionWeight(inNumberArguments, ioContext);

	// Get the number of possibles trees that can be invoked in the actual context.
	std::vector<unsigned int> lCandidates;
	getCandidatesToInvoke(lCandidates, inNumberArguments, ioContext);
	return double(lCandidates.size());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return a reference of the ADF primitive.
 *  \param inNumberArguments Number of arguments asked for the reference.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the reference generated.
 */
GP::Primitive::Handle GP::Invoker::giveReference(unsigned int inNumberArguments,
        GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// If not an invoker, return this directly.
	if(mIndex!=eGenerator) return this;

	// Generate new invoker.
	std::vector<unsigned int> lCandidates;
	getCandidatesToInvoke(lCandidates, inNumberArguments, ioContext);

	unsigned int lIndex = UINT_MAX;
	if(lCandidates.size() > 0) {
		lIndex = lCandidates[ioContext.getSystem().getRandomizer().rollInteger(0,lCandidates.size()-1)];
	}
	Handle lGeneratedInvoker = generateInvoker(lIndex, getName(), mArgsName, ioContext);

	// Return newly generated invoker.
	return lGeneratedInvoker;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Execute the call to the associated tree with the given parameters.
 *  \param outResult Result of the invocation.
 *  \param ioContext Evolutionary context.
 */
void GP::Invoker::execute(GP::Datum& outResult, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Tree::Handle lTree = getInvokedTree(ioContext);
	Beagle_AssertM(lTree->getNumberArguments() == getNumberArguments());
	if(getNumberArguments() == 0) {
		invoke(outResult, lTree, ioContext);
	} else {
		GP::Argument::Handle lArg =
		    castHandleT<GP::Argument>(lTree->getPrimitiveSet(ioContext).getPrimitiveByName(mArgsName));
		if(lArg==NULL) {
			std::string lMessage = "The argument named '";
			lMessage += mArgsName;
			lMessage += " associated to the invoker named '";
			lMessage += getName();
			lMessage += "' does not refer to a valid primitive in the set";
			lMessage += " of the tree associated to the invoker.";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		lArg->pushExecutionContext(getNumberArguments(), ioContext);
		invoke(outResult, lTree, ioContext);
		lArg->popExecutionContext();
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return whether selection weight in primitive set is stable.
 *  \param inNumberArguments Number of arguments for given selection weight.
 *  \return True is selection weight stable, false if not.
 */
bool GP::Invoker::isSelectionWeightStable(unsigned int inNumberArguments) const
{
	Beagle_StackTraceBeginM();
	const unsigned int lNbArgsPrimit = getNumberArguments();
	if((inNumberArguments == GP::Primitive::eAny) ||
	        (lNbArgsPrimit == GP::Primitive::eAny)) return false;
	else if(inNumberArguments == GP::Primitive::eBranch) {
		return (lNbArgsPrimit == 0);
	}
	return (lNbArgsPrimit == inNumberArguments);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read invoker primitive from XML subtree.
 *  \param inIter XML iterator to read the primitive from.
 *  \param ioContext Evolutionary context.
 */
void GP::Invoker::readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	GP::Primitive::readWithContext(inIter, ioContext);
	std::string lIndexValue = inIter->getAttribute("id");
	if(lIndexValue.empty()==false) mIndex = str2uint(lIndexValue);
	unsigned int lArgsCount = 0;
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType()==PACC::XML::eData) ++lArgsCount;
	}
	setNumberArguments(lArgsCount);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Validate the invoker position in the tree.
 *  \param ioContext Evolutionary context.
 *  \return True if the invoker is correctly positioned, false if not.
 *  \throw Beagle::AssertException If the context is in a bad state.
 */
bool GP::Invoker::validate(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	std::vector<unsigned int> lCandidates;
	getCandidatesToInvoke(lCandidates, getNumberArguments(), ioContext);
	for(unsigned int i=0; i<lCandidates.size(); ++i) {
		if(mIndex==lCandidates[i]) return GP::Primitive::validate(ioContext);
	}
	return false;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the content of a primitive into a XML stream.
 *  \param ioStreamer Streamer to write primitive into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::Invoker::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if(mIndex!=eGenerator) ioStreamer.insertAttribute("id", uint2str(mIndex));
	Beagle_StackTraceEndM();
}

