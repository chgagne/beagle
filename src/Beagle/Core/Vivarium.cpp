/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/Core/Vivarium.cpp
 *  \brief  Source code of class Vivarium.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.23 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "Beagle/Core.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a vivarium.
 *  \param inN Initial number of demes in vivarium.
 *  \param inModel Initial value of demes handles.
 */
Vivarium::Vivarium(unsigned int inN, Deme::Handle inModel) :
	Deme::Bag(inN, inModel)
{ }


/*!
 *  \brief Add a member to the vivarium.
 *  \param inMember Member to add.
 */
void Vivarium::addMember(Member::Handle inMember)
{
	Beagle_StackTraceBeginM();
	MemberMap::const_iterator lIterMap = mMemberMap.find(inMember->getName());
	if(lIterMap != mMemberMap.end()) {
		std::ostringstream lOSS;
		lOSS << "A member named '" << inMember->getName();
		lOSS << "' is already present in the member map; ";
		lOSS << "could not add it.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	mMemberMap[inMember->getName()] = inMember;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy vivarium using the system.
 *  \param inOriginal Vivarium to copy into current one.
 *  \param ioSystem Evolutionary system.
 */
void Vivarium::copy(const Vivarium& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const Factory& lFactory = ioSystem.getFactory();

	// Copy members
	mMemberMap.clear();
	for(MemberMap::const_iterator lIterMap=inOriginal.mMemberMap.begin();
	        lIterMap!=inOriginal.mMemberMap.end(); ++lIterMap) {
		Member::Handle lOrigMember = castHandleT<Member>(lIterMap->second);
		const std::string& lMemberType = lOrigMember->getType();
		Member::Alloc::Handle lMemberAlloc =
		    castHandleT<Member::Alloc>(lFactory.getAllocator(lMemberType));
		Member::Handle lMember = castHandleT<Member>(lMemberAlloc->allocate());
		lMember->copy(*lOrigMember, ioSystem);
		mMemberMap[lIterMap->first] = lMember;
	}

	// Copy demes
	resize(inOriginal.size());
	for(unsigned int i=0; i<inOriginal.size(); ++i) {
		const std::string& lDemeTypeName = inOriginal[i]->getType();
		Deme::Alloc::Handle lDemeAlloc =
		    castHandleT<Deme::Alloc>(lFactory.getAllocator(lDemeTypeName));
		Deme::Handle lDeme = castHandleT<Deme>(lDemeAlloc->allocate());
		lDeme->copy(*inOriginal[i], ioSystem);
		(*this)[i] = lDeme;
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact name of the vivarium.
 *  \return Exact name of the vivarium.
 */
const std::string& Vivarium::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("Vivarium");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the vivarium.
 *  \return Exact type of the vivarium.
 */
const std::string& Vivarium::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("Vivarium");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Reading a vivarium with method read is undefined. Use readWithContext instead.
 */
void Vivarium::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read","Vivarium",getType());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read members of a vivarium from an XML iterator.
 *  \param inIter XML iterator to read the members from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Vivarium::readMembers(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	mMemberMap.clear();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	for(PACC::XML::ConstIterator lIter=inIter; lIter; ++lIter) {
		if(lIter->getType() != PACC::XML::eData) continue;
		if(lIter->getValue() == "Population") continue;
		const std::string& lMemberName = lIter->getValue();
		const std::string& lMemberType = lIter->getAttribute("type");
		Member::Alloc::Handle lMemberAlloc = NULL;
		if(lMemberType.empty()) {
			lMemberAlloc = castHandleT<Member::Alloc>(lFactory.getConceptAllocator(lMemberName));
			if(lMemberAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Member of name '" << lMemberName << "' can't be read; ";
				lOSS << "it appears that its type is not given and that there is not ";
				lOSS << "valid concept allocator associated to it!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		} else {
			lMemberAlloc = castHandleT<Member::Alloc>(lFactory.getAllocator(lMemberType));
			if(lMemberAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "It seems that type '" << lMemberType << "' associated to member '";
				lOSS << lMemberName << "' is not valid!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		}
		Member::Handle lMember = castHandleT<Member>(lMemberAlloc->allocate());
		lMember->readWithContext(lIter, ioContext);
		mMemberMap[lMemberName] = lMember;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a vivarium population from an XML iterator.
 *  \param inIter XML iterator to read the vivarium population from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Vivarium::readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Population")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Population> expected!");
	}
	const Factory& lFactory = ioContext.getSystem().getFactory();
	unsigned int lPrevDemeIndex = ioContext.getDemeIndex();
	Deme::Handle lPrevDemeHandle = ioContext.getDemeHandle();
	clear();
	for(PACC::XML::ConstIterator lIter=inIter->getFirstChild(); lIter; ++lIter) {
		if((lIter->getType()!=PACC::XML::eData) || (lIter->getValue()!="Deme")) continue;
		const std::string& lDemeType = lIter->getAttribute("type");
		Deme::Alloc::Handle lDemeAlloc = NULL;
		if(lDemeType.empty()) {
			Deme::Alloc::Handle lDemeAlloc =
			    castHandleT<Deme::Alloc>(lFactory.getConceptAllocator("Deme"));
			if(lDemeAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Deme object can't be read, ";
				lOSS << "it appears that its type is not given and that there is not ";
				lOSS << "valid concept allocator associated to it!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		} else {
			lDemeAlloc = castHandleT<Deme::Alloc>(lFactory.getAllocator(lDemeType));
			if(lDemeAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lDemeType << "' associated to deme object ";
				lOSS << "is not valid!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		}
		Deme::Handle lDeme = castHandleT<Deme>(lDemeAlloc->allocate());
		ioContext.setDemeHandle(lDeme);
		ioContext.setDemeIndex(size());
		push_back(lDeme);
		lDeme->readWithContext(lIter, ioContext);
	}
	ioContext.setDemeHandle(lPrevDemeHandle);
	ioContext.setDemeIndex(lPrevDemeIndex);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a vivarium from an XML node.
 *  \param inIter XML iterator to read the vivarium from.
 *  \param ioContext Evolutionary context to use to read the vivarium.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Vivarium::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Vivarium")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Vivarium> expected!");
	}
	// Be sure that the types are corresponding
	const std::string& lVivariumType = inIter->getAttribute("type");
	if((lVivariumType.empty()==false) && (lVivariumType!=getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lVivariumType << "' mismatch type of the vivarium '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	// Read members
	readMembers(inIter->getFirstChild(), ioContext);
	// Read population
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Population")) {
			readPopulation(lChild, ioContext);
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Remove given member from the vivarium's member map.
 *  \param inName Name of the member to remove.
 *  \return Handle to the member removed from the vivarium,
 *     NULL pointer if there was no member with given name.
 */
Member::Handle Vivarium::removeMember(const std::string& inName)
{
	Beagle_StackTraceBeginM();
	MemberMap::iterator lIterMap = mMemberMap.find(inName);
	if(lIterMap == mMemberMap.end()) return NULL;
	Member::Handle lMember = castHandleT<Member>(lIterMap->second);
	mMemberMap.erase(lIterMap);
	return lMember;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the vivarium in a XML streamer.
 *  \param ioStreamer XML streamer to write the vivarium into.
 *  \param inIndent Whether XML output should be indented.
 */
void Vivarium::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Vivarium", inIndent);
	ioStreamer.insertAttribute("type", getType());
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write content of the vivarium in a XML streamer.
 *  \param ioStreamer XML streamer to write the vivarium into.
 *  \param inIndent Whether XML output should be indented.
 */
void Vivarium::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	writeMembers(ioStreamer, inIndent);
	writePopulation(ioStreamer, inIndent);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write members of the vivarium in a XML streamer.
 *  \param ioStreamer XML streamer to write the members into.
 *  \param inIndent Whether XML output should be indented.
 */
void Vivarium::writeMembers(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	for(MemberMap::const_iterator lIterMap=mMemberMap.begin();
	        lIterMap!=mMemberMap.end(); ++lIterMap) {
		ioStreamer.openTag(lIterMap->first, inIndent);
		Member::Handle lMember = castHandleT<Member>(lIterMap->second);
		ioStreamer.insertAttribute("type", lMember->getType());
		lMember->writeContent(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the actual vivarium population in a XML streamer.
 *  \param ioStreamer XML streamer to write the vivarium population into.
 *  \param inIndent Whether XML output should be indented.
 */
void Vivarium::writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Population", inIndent);
	ioStreamer.insertAttribute("size", uint2str(size()));
	for(unsigned int i=0; i<size(); ++i) {
		Beagle_NonNullPointerAssertM((*this)[i]);
		(*this)[i]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
