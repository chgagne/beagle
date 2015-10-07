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
 *  \file   beagle/GP/src/PrimitiveSuperSet.cpp
 *  \brief  Implementation of the type GP::PrimitiveSuperSet.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.25 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct primitive super set.
 */
GP::PrimitiveSuperSet::PrimitiveSuperSet() :
		Component("GP-PrimitiveSuperSet")
{
	Beagle_StackTraceBeginM();
	addBasicPrimitives();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add common primitives to the primitive super set.
 */
void GP::PrimitiveSuperSet::addBasicPrimitives()
{
	Beagle_StackTraceBeginM();
	addPrimitive(new GP::ADF);
	addPrimitive(new GP::ADF(GP::Invoker::eGenerator, GP::Primitive::eAny, "ADFI", "ARGI"));
	addPrimitive(new GP::ADF(GP::Invoker::eGenerator, GP::Primitive::eAny, "ADFB", "ARGB"));
	addPrimitive(new GP::Module);
	addPrimitive(new GP::Module(GP::Invoker::eGenerator, GP::Primitive::eAny, "MODULEI", "ARGI"));
	addPrimitive(new GP::Module(GP::Invoker::eGenerator, GP::Primitive::eAny, "MODULEB", "ARGB"));
	addPrimitive(new GP::ArgumentT<Double>);
	addPrimitive(new GP::ArgumentT<Int>(GP::Argument::eCaching, GP::Argument::eGenerator, "ARGI"));
	addPrimitive(new GP::ArgumentT<Bool>(GP::Argument::eCaching, GP::Argument::eGenerator, "ARGB"));
	addPrimitive(new GP::IfThenElseT<Double>("IFTE"));
	addPrimitive(new GP::IfThenElseT<Int>("IFTEI"));
	addPrimitive(new GP::IfThenElseT<Bool>("IFTEB"));
	addPrimitive(new GP::AbsT<Double>("ABS"));
	addPrimitive(new GP::AbsT<Int>("ABSI"));
	addPrimitive(new GP::Add);
	addPrimitive(new GP::AddT<Int>("ADDI"));
	addPrimitive(new GP::Subtract);
	addPrimitive(new GP::SubtractT<Int>("SUBI"));
	addPrimitive(new GP::Multiply);
	addPrimitive(new GP::MultiplyT<Int>("MULI"));
	addPrimitive(new GP::Divide);
	addPrimitive(new GP::Sin);
	addPrimitive(new GP::Cos);
	addPrimitive(new GP::Exp);
	addPrimitive(new GP::Log);
	addPrimitive(new GP::EphemeralDouble);
	addPrimitive(new GP::And);
	addPrimitive(new GP::Or);
	addPrimitive(new GP::Not);
	addPrimitive(new GP::Nand);
	addPrimitive(new GP::Nor);
	addPrimitive(new GP::Xor);
	addPrimitive(new GP::BitwiseAnd);
	addPrimitive(new GP::BitwiseOr);
	addPrimitive(new GP::BitwiseNand);
	addPrimitive(new GP::BitwiseNor);
	addPrimitive(new GP::BitwiseNot);
	addPrimitive(new GP::BitwiseXor);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this primitive superset.
 *  \param ioSystem Evolutionary system used for initialization.
 */
void GP::PrimitiveSuperSet::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Component::registerParams(ioSystem);
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "initialization","Beagle::GP::PrimitiveSet",
	    "Registering the parameters of the primitive superset"
	);
	for(unsigned int i=0; i<mPrimitSets.size(); i++) {
		Beagle_LogVerboseM(
		    ioSystem.getLogger(),
		    "initialization","Beagle::GP::PrimitiveSet",
		    std::string("Initializing ")+uint2ordinal(i+1)+std::string(" primitive set")
		);
		mPrimitSets[i]->registerParams(ioSystem);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a nes set of primitives in the super set.
 *  \param inPrimitiveSet Inserted primitive set.
 */
void GP::PrimitiveSuperSet::insert(GP::PrimitiveSet::Handle inPrimitiveSet, bool inReplace)
{
	Beagle_StackTraceBeginM();
	mPrimitSets.push_back(inPrimitiveSet);
	for(unsigned int i=0; i<inPrimitiveSet->size(); ++i) addPrimitive((*inPrimitiveSet)[i], inReplace);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a primitive super set from a XML subtree.
 *  \param inIter XML iterator used to read the super set from.
 *  \param ioSystem Evolutionary system.
 *  \throw Beagle::IOException If size atribute not present.
 */
void GP::PrimitiveSuperSet::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="GP-PrimitiveSuperSet"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <GP-PrimitiveSuperSet> expected!");
	const Factory& lFactory = ioSystem.getFactory();
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(lFactory.getConceptAllocator("Context"));
	GP::Context::Handle lGPContext =
	    castHandleT<GP::Context>(lContextAlloc->allocate());
	lGPContext->setSystemHandle(System::Handle(&ioSystem));
	GP::PrimitiveSet::Alloc::Handle lPrimitSetsAlloc =
		castHandleT<GP::PrimitiveSet::Alloc>(lFactory.getConceptAllocator("GP-PrimitiveSet"));
	mPrimitSets.clear();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="GP-PrimitiveSet")) {
			GP::PrimitiveSet::Handle lPrimitSet =
				castHandleT<GP::PrimitiveSet>(lPrimitSetsAlloc->allocate());
			lPrimitSet->readWithContext(lChild, *lGPContext);
			mPrimitSets.push_back(lPrimitSet);
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a primitive super set into a Beagle XML stream.
 *  \param ioStreamer XMl streamer to write the super set into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::PrimitiveSuperSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("GP-PrimitiveSuperSet", inIndent);
	for(unsigned int i=0; i<mPrimitSets.size(); i++) mPrimitSets[i]->write(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}

