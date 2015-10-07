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
 *  \file   beagle/GP/src/PrimitiveSet.cpp
 *  \brief  Implementation of the type GP::PrimitiveSet.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Construct a new primitive set.
 *  \param inRootType Associated tree's root type. Used only with strongly-typed GP.
 */
GP::PrimitiveSet::PrimitiveSet(const std::type_info* inRootType) :
		mRootType(inRootType)
{ }


/*!
 *  \brief Get the associated tree's root type. Used only with strongly-typed GP.
 *  \return Associated tree's root type.
 */
const std::type_info* GP::PrimitiveSet::getRootType() const
{
	Beagle_StackTraceBeginM();
	return mRootType;
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Register the parameters of this set of primitive.
 *  \param ioSystem System used to initialize the primitive set.
 */
void GP::PrimitiveSet::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle_LogVerboseM(
	    ioSystem.getLogger(),
	    "initialization","Beagle::GP::PrimitiveSet",
	    "Registering parameters of primitive set"
	);
	for(unsigned int i=0; i<size(); i++) {
		Beagle_LogVerboseM(
		    ioSystem.getLogger(),
		    "initialization","Beagle::GP::PrimitiveSet",
		    std::string("Registering parameters of primitive '")+(*this)[i]->getName()+std::string("'")
		);
		(*this)[i]->registerParams(ioSystem);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert a new primitive in the set.
 *  \param inPrimitive Handle to the primitive to insert.
 *  \param inBias Selection bias associated to primitive.
 *  \throw AssertException If the given handle is NULL.
 *  \throw RunTimeException If a primitive of the same name is already in the set.
 */
void GP::PrimitiveSet::insert(GP::Primitive::Handle inPrimitive, double inBias)
{
	Beagle_StackTraceBeginM();
	// Check if the name is already used
	Beagle_NonNullPointerAssertM(inPrimitive);
	if(mNames.find(inPrimitive->getName()) != mNames.end()) {
		std::string lMessage = "In GP::PrimitiveSet::insert(Primitive): A primitive named '";
		lMessage += inPrimitive->getName();
		lMessage += "' is already in the primitive set.";
		throw Beagle_RunTimeExceptionM(lMessage);
	}

	// Insert primitive
	(*this).push_back(inPrimitive);
	mNames[inPrimitive->getName()] = inPrimitive;
	mSelectionBiases.push_back(inBias);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Select randomly a primitive.
 *  \param inNumberArguments Number of arguments for selected primitive.
 *  \param ioContext Reference to the evolutionary context.
 *  \return Handle to the selected primitive.
 */
GP::Primitive::Handle GP::PrimitiveSet::select(unsigned int inNumberArguments,
        GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Get roulette for inserted primitives.
	RouletteMap::const_iterator lRouletteIter = mRoulettes.find(inNumberArguments);
	if((lRouletteIter==mRoulettes.end()) || (lRouletteIter->second.first==false))  {
		// If the roulette haven't been processed before, evaluate stability flag.
		if(lRouletteIter==mRoulettes.end()) {
			RoulettePair& lNewRoulette = mRoulettes[inNumberArguments];
			lNewRoulette.first = true;
			for(unsigned int i=0; i<size(); ++i) {
				if((*this)[i]->isSelectionWeightStable(inNumberArguments) == false) {
					lNewRoulette.first = false;
					break;
				}
			}
		}
		// Build roulette.
		RoulettePair& lBuiltRoulette = mRoulettes[inNumberArguments];
		lBuiltRoulette.second.clear();
		for(unsigned int i=0; i<size(); ++i) {
			const double lSelectionWeight =
			    (*this)[i]->getSelectionWeight(inNumberArguments,ioContext) * mSelectionBiases[i];
			Beagle_AssertM(lSelectionWeight >= 0.0);
			if(lSelectionWeight!=0.0) lBuiltRoulette.second.insert(i, lSelectionWeight);
		}
		// Optimize if necessary.
		if(lBuiltRoulette.first) lBuiltRoulette.second.optimize();
	}

	// Get primitive from roulette
	RouletteT<unsigned int>& lRoulette = mRoulettes[inNumberArguments].second;
	if(lRoulette.empty()) return Primitive::Handle(NULL);
	return (*this)[lRoulette.select(ioContext.getSystem().getRandomizer())];
	Beagle_StackTraceEndM();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Select randomly a primitive.
 *  \param inNumberArguments Number of arguments for selected primitive.
 *  \param inDesiredType Desired type for selected primitive.
 *  \param ioContext Reference to the evolutionary context.
 *  \return Handle to the selected primitive.
 */
GP::Primitive::Handle GP::PrimitiveSet::selectWithType(unsigned int inNumberArguments,
        const std::type_info* inDesiredType,
        GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Create roulette of eligible primitives.
	RouletteT<unsigned int> lRoulette;
	for(unsigned int i=0; i<size(); ++i) {
		const std::type_info* lReturnType = (*this)[i]->getReturnType(ioContext);
		if((inDesiredType==NULL) || (lReturnType==NULL) || ((*inDesiredType)==(*lReturnType))) {
			const double lSelectionWeight =
			    (*this)[i]->getSelectionWeight(inNumberArguments,ioContext) * mSelectionBiases[i];
			Beagle_AssertM(lSelectionWeight >= 0.0);
			if(lSelectionWeight!=0.0) lRoulette.insert(i, lSelectionWeight);
		}
	}

	// Get primitive from roulette
	if(lRoulette.empty()) return Primitive::Handle(NULL);
	return (*this)[lRoulette.select(ioContext.getSystem().getRandomizer())];
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Read a primitive set from a XML subtree.
 *  \param inIter XML iterator to read the primitive set from.
 *  \param ioContext Evolutionary context.
 *  \throw IOException If size atribute not present or if the size mismatch.
 */
void GP::PrimitiveSet::readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="GP-PrimitiveSet"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <GP-PrimitiveSet> expected!");

	// Add existing primitives to the super set's primitive map.
	GP::PrimitiveSuperSet::Handle lSuperSet =
	    castHandleT<GP::PrimitiveSuperSet>(ioContext.getSystem().getComponent("GP-PrimitiveSuperSet"));
	GP::PrimitiveMap lNamesDuplicate;
	if(lSuperSet==NULL) lNamesDuplicate = mNames;
	else for(unsigned int i=0; i<size(); ++i) lSuperSet->addPrimitive((*this)[i]);
	GP::PrimitiveMap& lPrimitMap = (lSuperSet==NULL) ? lNamesDuplicate : lSuperSet->getPrimitiveMap();

	// Clean-up primitive set before reading.
	clear();
	mSelectionBiases.clear();
	mNames.clear();
	mRoulettes.clear();

	// Read primitives from XML file.
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="GP-Primitive")) {
			std::string lPrimitName = lChild->getAttribute("name");
			GP::PrimitiveMap::const_iterator lMapIter = lPrimitMap.find(lPrimitName);
			if(lMapIter == lPrimitMap.end()) {
				std::ostringstream lOSS;
				lOSS << "primitive '" << lPrimitName;
				lOSS << "' is not a known primitive of the primitive set!";
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			GP::Primitive::Handle lPrimitive = castHandleT<GP::Primitive>(lMapIter->second);
			std::string lPrimitBiasStr = lChild->getAttribute("bias");
			const double lPrimitBias = lPrimitBiasStr.empty() ? 1.0 : str2dbl(lPrimitBiasStr);
			insert(lPrimitive, lPrimitBias);
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a primitive set into a XML streamer.
 *  \param ioStreamer XML streamer to use to read the primitive set into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::PrimitiveSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("GP-PrimitiveSet", inIndent);
	for(unsigned int i=0; i<size(); ++i) {
		ioStreamer.openTag("GP-Primitive", inIndent);
		ioStreamer.insertAttribute("name", (*this)[i]->getName());
		ioStreamer.insertAttribute("bias", dbl2str(mSelectionBiases[i]));
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag(); // PrimitiveSet
	Beagle_StackTraceEndM();
}
