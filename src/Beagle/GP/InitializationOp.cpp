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

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a plain new initialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the initialization operator.
 */
GP::InitializationOp::InitializationOp(std::string inReproProbaName, std::string inName) :
		Beagle::InitializationOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the generic GP intialization operator.
 *  \param ioSystem System of the evolution.
 */
void GP::InitializationOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Maximum initial tree depth",
		    "UInt",
		    "5",
		    "Maximum depth for newly initialized trees."
		);
		mMaxTreeDepth = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry("gp.init.maxdepth", new UInt(5), lDescription));
	}
	{
		Register::Description lDescription(
		    "Minimum initial tree depth",
		    "UInt",
		    "2",
		    "Minimum depth for newly initialized trees."
		);
		mMinTreeDepth = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry("gp.init.mindepth", new UInt(2), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Maximum number of GP tree in newly initialized individuals. More than one tree ";
		lOSS << "is usually useful with ADFs (and other ADx).";
		Register::Description lDescription(
		    "Maximum number of trees",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mMaxNumberTrees = castHandleT<UInt>(
		                      ioSystem.getRegister().insertEntry("gp.init.maxtree", new UInt(1), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Minimum number of GP tree in newly initialized individuals. More than one tree ";
		lOSS << "is usually useful with ADFs (and other ADx).";
		Register::Description lDescription(
		    "Minimum number of trees",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mMinNumberTrees = castHandleT<UInt>(
		                      ioSystem.getRegister().insertEntry("gp.init.mintree", new UInt(1), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Maximum number of arguments in GP tree.  The first element in the array "
		<< "specifies the maximum number of arguments in the first tree of an individual, "
		<< "the second element specifies the second tree and so on.  If there are more "
		<< "trees in an individual than elements in the array then the last element is "
		<< "recycled.  Tree arguments are usually useful with ADFs (and similar).";
		Register::Description lDescription(
		    "Max. number of tree arguments",
		    "UIntArray",
		    "0/2",
		    lOSS.str()
		);
		mMaxTreeArgs = new UIntArray;
		mMaxTreeArgs->push_back(0);
		mMaxTreeArgs->push_back(2);
		mMaxTreeArgs = castHandleT<UIntArray>(
		                   ioSystem.getRegister().insertEntry("gp.tree.maxargs", mMaxTreeArgs, lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Minimum number of arguments in GP tree.  The first element in the array "
		<< "specifies the minimum number of arguments in the first tree of an individual, "
		<< "the second element specifies the second tree and so on.  If there are more "
		<< "trees in an individual than elements in the array then the last element is "
		<< "recycled.  Tree arguments are usually useful with ADFs (and similar).";
		Register::Description lDescription(
		    "Min. number of tree arguments",
		    "UIntArray",
		    "0/2",
		    lOSS.str()
		);
		mMinTreeArgs = new UIntArray;
		mMinTreeArgs->push_back(0);
		mMinTreeArgs->push_back(2);
		mMinTreeArgs = castHandleT<UIntArray>(
		                   ioSystem.getRegister().insertEntry("gp.tree.minargs", mMinTreeArgs, lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the trees of an individual.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 *  \throw Beagle::RunTimeException If the min/max depths are incorrectly set.
 */
void GP::InitializationOp::initIndividual(Beagle::Individual& outIndividual,
        Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if (mMinTreeDepth==NULL || mMaxTreeDepth==NULL)
		throw Beagle_RunTimeExceptionM(std::string("GP::InitializationOp has not been initialized.").
		                               append(" Consider the GP::InitializationOp::registerParams() method."));
	if(*mMinTreeDepth > *mMaxTreeDepth) {
		std::string lMessage = "GP::InitializationOp::initIndividual: Minimum tree depth is superior ";
		lMessage += "to the maximum tree depth. Could not initialize the individuals!";
		throw Beagle::ValidationException(lMessage);
	}
#endif // BEAGLE_NDEBUG
	Beagle_ValidateParameterM(mMinTreeDepth->getWrappedValue()>0,"gp.init.mindepth",">0");

	GP::Individual& lIndividual = castObjectT<GP::Individual&>(outIndividual);
	GP::Context& lContext = castObjectT<GP::Context&>(ioContext);

	GP::PrimitiveSuperSet::Handle lSuperSet =
	    castHandleT<GP::PrimitiveSuperSet>(ioContext.getSystem().getComponent("GP-PrimitiveSuperSet"));
	if(lSuperSet == NULL) {
		throw Beagle_RunTimeExceptionM("There should be a GP::PrimitiveSuperSet component in the system");
	}
	const unsigned int lPrimitiveSuperSetSize = lSuperSet->size();
	const Factory& lFactory = ioContext.getSystem().getFactory();
#ifndef BEAGLE_NDEBUG
	if(lPrimitiveSuperSetSize == 0)
		throw Beagle_RunTimeExceptionM(std::string("GP::InitializationOp::initIndividual(): There ").
		                               append(" are no PrimitiveSets in the PrimitiveSuperSet.  There needs to be at least one").
		                               append(" PrimitiveSet. See the examples that are included with Beagle to learn how").
		                               append(" to create a PrimitiveSet, add Primitives to it, and then construct a System based").
		                               append(" on the PrimitiveSet."));
#endif // BEAGLE_NDEBUG

	// Choose randomly the number of individuals in tree
	const unsigned int lMaxDepth   = mMaxTreeDepth->getWrappedValue();
	const unsigned int lMinDepth   = mMinTreeDepth->getWrappedValue();
	const unsigned int lMaxNbTrees = mMaxNumberTrees->getWrappedValue();
	const unsigned int lMinNbTrees = mMinNumberTrees->getWrappedValue();
	Beagle_AssertM(lMaxNbTrees >= lMinNbTrees);
	const unsigned int lNbTrees =
	    ioContext.getSystem().getRandomizer().rollInteger(lMinNbTrees,lMaxNbTrees);
	Genotype::Alloc::Handle lGenotypeAlloc =
		castHandleT<Genotype::Alloc>(lFactory.getConceptAllocator("Genotype"));
	lIndividual.clear();
	for(unsigned int i=0; i<lNbTrees; ++i) {
		lIndividual.push_back(castHandleT<Genotype>(lGenotypeAlloc->allocate()));
	}
	GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
	unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
	for(unsigned int i=0; i<lIndividual.size(); ++i) {
		if(i<lPrimitiveSuperSetSize) lIndividual[i]->setPrimitiveSetIndex(i);
		else lIndividual[i]->setPrimitiveSetIndex(lPrimitiveSuperSetSize-1);
		const unsigned int lMaxArgs =
		    (i<mMaxTreeArgs->size()) ? (*mMaxTreeArgs)[i] : mMaxTreeArgs->back();
		const unsigned int lMinArgs =
		    (i<mMinTreeArgs->size()) ? (*mMinTreeArgs)[i] : mMinTreeArgs->back();
		Beagle_AssertM(lMaxArgs >= lMinArgs);
		const unsigned int lNbArgs =
		    ioContext.getSystem().getRandomizer().rollInteger(lMinArgs,lMaxArgs);
		lIndividual[i]->setNumberArguments(lNbArgs);
	}
	for(unsigned int i=0; i<lIndividual.size(); ++i) {
		lContext.setGenotypeHandle(lIndividual[i]);
		lContext.setGenotypeIndex(i);
		const unsigned int lTreeDepth =
		    lContext.getSystem().getRandomizer().rollInteger(lMinDepth,lMaxDepth);
		lContext.emptyCallStack();
		lIndividual[i]->clear();
		initTree(*lIndividual[i], lMinDepth, lTreeDepth, lContext);
	}

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GP::InitializationOp",
	    std::string("Initialized individual:")
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::GP::InitializationOp",
	    lIndividual
	);

	lContext.setGenotypeIndex(lOldTreeIndex);
	lContext.setGenotypeHandle(lOldTreeHandle);
	Beagle_StackTraceEndM();
}

