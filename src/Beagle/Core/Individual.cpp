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
 *  \file   Beagle/Core/Individual.cpp
 *  \brief  Source code of class Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.39 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/Core.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct an individual.
 *  \param inN Initial number of genotypes.
 *  \param inModel Model of genotypes.
 */
Individual::Individual(unsigned int inN, Genotype::Handle inModel) :
	Genotype::Bag(inN, inModel)
{ }


/*!
 *  \brief Add a member to the individual.
 *  \param inMember Member to add.
 */
void Individual::addMember(Member::Handle inMember)
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
 *  \brief Copy individual using the system.
 *  \param inOriginal Individual to copy into current.
 *  \param ioSystem Evolutionary system.
 */
void Individual::copy(const Individual& inOriginal, System& ioSystem)
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

	// Copy fitness
	if(inOriginal.getFitness() == NULL) {
		mFitness = NULL;
	} else {
		const std::string& lFitnessType = inOriginal.getFitness()->getType();
		Fitness::Alloc::Handle lFitnessAlloc =
		    castHandleT<Fitness::Alloc>(lFactory.getAllocator(lFitnessType));
		mFitness = castHandleT<Fitness>(lFitnessAlloc->allocate());
		mFitness->copy(*inOriginal.getFitness(), ioSystem);
	}

	// Copy genotypes
	resize(inOriginal.size());
	for(unsigned int i=0; i<inOriginal.size(); ++i) {
		const std::string& lGenotypeType = inOriginal[i]->getType();
		Genotype::Alloc::Handle lGenotypeAlloc =
		    castHandleT<Genotype::Alloc>(lFactory.getAllocator(lGenotypeType));
		(*this)[i] = castHandleT<Genotype>(lGenotypeAlloc->allocate());
		(*this)[i]->copy(*inOriginal[i], ioSystem);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Computing two individuals distances in genotype space is actually not implemented.
 */
double Individual::getDistanceGenotype(const Individual&) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getDistanceGenotype", "Individual", getType());
	return 0.0;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Computing two individuals distances in phenotype space is actually not implemented.
 */
double Individual::getDistancePhenotype(const Individual&) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getDistancePhenotype", "Individual", getType());
	return 0.0;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get name of object.
 *  \return Object name, "Individual".
 */
const std::string& Individual::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("Individual");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get size (length) of the individual, summing size of genotypes.
 *  \return Total size of individual.
 */
unsigned int Individual::getSize() const
{
	Beagle_StackTraceBeginM();
	unsigned int lSize=0;
	for(unsigned int i=0; i<size(); ++i) lSize += (*this)[i]->getSize();
	return lSize;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the individual.
 *  \return Exact type of the individual.
 */
const std::string& Individual::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("Individual");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Compare the fitness of two individual.
 *  \param  inRightObj Individual to compare to this.
 *  \return True if individuals have the same fitness, false if not.
 *  \throw  AssertException When a fitness handle of the individuals is NULL.
 *  \throw  BadCastException If the types of the parameters are not as specified.
 */
bool Individual::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Individual& lRightIndividual = castObjectT<const Individual&>(inRightObj);
	if((mFitness == NULL) && (lRightIndividual.mFitness == NULL)) return true;
	if((mFitness == NULL) || (lRightIndividual.mFitness == NULL)) return false;
	return mFitness->isEqual(*lRightIndividual.mFitness);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Compare the equality of the genotypes of two individuals.
 *  \param  inRightIndividual Individual to compare to this.
 *  \return True if individuals are identicals, false if not.
 */
bool Individual::isIdentical(const Individual& inRightIndividual) const
{
	Beagle_StackTraceBeginM();
	if(isEqual(inRightIndividual) == false) return false;
	if(size() != inRightIndividual.size()) return false;
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i]->isEqual(*inRightIndividual[i]) == false) return false;
	}
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Compare the fitness order of two individuals.
 *
 *  isLess() is called when two individuals are compared with each
 *  other.  For example, if we have two Indiviudals, 'A' and 'B', then
 *  A < B calls operator<(Object, Object), which in
 *  turn calls Beagle::Individual::isLess().
 *
 *  For more information on operator<(Object, Object) and similar
 *  functions, see the "List of all members" for Beagle::Object.
 *
 *  \param  inRightObj Individual to compare to this.
 *  \return True if left individual fitness is less that the right one, false if not.
 *  \throw  AssertException When a fitness handle of the individuals is NULL.
 *  \throw  BadCastException If the types of the parameters are not as specified.
 */
bool Individual::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Individual& lRightIndividual = castObjectT<const Individual&>(inRightObj);
	if((mFitness == NULL) || (lRightIndividual.mFitness == NULL)) return false;
	return mFitness->isLess(*lRightIndividual.mFitness);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Reading an individual with method read is undefined. Use readWithContext instead.
 */
void Individual::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read", "Individual", getType());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read individual from a XML file. If several individuals are in file, read
 *    first tagged occurence of individual.
 *  \param inFileName Filename to read individual from.
 *  \param ioSystem Evolutionary system to read individual.
 *  \return True if an individual was found and read from file, false if nothing was found.
 */
bool Individual::readFromFile(std::string inFileName, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	std::ifstream lIFS(inFileName.c_str());
	PACC::XML::Document lParser(lIFS, inFileName);
	lIFS.close();
	PACC::XML::ConstFinder lIndivFinder(lParser.getFirstDataTag());
	PACC::XML::ConstIterator lIndivTag = lIndivFinder.find("//Individual");
	if(!lIndivTag) return false;

	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioSystem.getFactory().getConceptAllocator("Context"));
	Context::Handle lContext = castHandleT<Context>(lContextAlloc->allocate());
	lContext->setSystemHandle(&ioSystem);
	lContext->setIndividualHandle(this);
	lContext->setIndividualIndex(0);

	readWithContext(lIndivTag, *lContext);

	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read fitness of an individual from an XML node.
 *  \param inIter XML iterator to read the individual's fitness from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Individual::readFitness(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	for(PACC::XML::ConstIterator lIter=inIter; lIter; ++lIter) {
		if((lIter->getType() != PACC::XML::eData) || (lIter->getValue() != "Fitness")) continue;
		const std::string& lFitnessType = lIter->getAttribute("type");
		Fitness::Alloc::Handle lFitnessAlloc = NULL;
		if(lFitnessType.empty()) {
			const std::string& lFitnessValid = lIter->getAttribute("valid");
			if(lFitnessValid == "no") {
				mFitness = NULL;
				break;
			}
			lFitnessAlloc = castHandleT<Fitness::Alloc>(lFactory.getConceptAllocator("Fitness"));
			if(lFitnessAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Fitness object can't be read, ";
				lOSS << "it appears that its type is not given and that there is not ";
				lOSS << "valid concept allocator associated to it!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		} else {
			lFitnessAlloc = castHandleT<Fitness::Alloc>(lFactory.getAllocator(lFitnessType));
			if(lFitnessAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lFitnessType << "' associated to fitness object ";
				lOSS << "is not valid!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		}
		mFitness = castHandleT<Fitness>(lFitnessAlloc->allocate());
		mFitness->readWithContext(lIter, ioContext);
		break;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read genotypes of an individual from an XML node.
 *  \param inIter XML iterator to read the individual's genotypes from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Individual::readGenotypes(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	unsigned int lPrevGenoIndex = ioContext.getGenotypeIndex();
	Genotype::Handle lPrevGenoHandle = ioContext.getGenotypeHandle();
	clear();
	for(PACC::XML::ConstIterator lIter=inIter; lIter; ++lIter) {
		if((lIter->getType() != PACC::XML::eData) || (lIter->getValue() != "Genotype")) continue;
		const std::string& lGenotypeType = lIter->getAttribute("type");
		Genotype::Alloc::Handle lGenotypeAlloc = NULL;
		if(lGenotypeType.empty()) {
			lGenotypeAlloc = castHandleT<Genotype::Alloc>(lFactory.getConceptAllocator("Genotype"));
			if(lGenotypeAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Genotype object can't be read, ";
				lOSS << "it appears that its type is not given and that there is not ";
				lOSS << "valid concept allocator associated to it!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		} else {
			lGenotypeAlloc = castHandleT<Genotype::Alloc>(lFactory.getAllocator(lGenotypeType));
			if(lGenotypeAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lGenotypeType << "' associated to genotype object ";
				lOSS << "is not valid!";
				throw Beagle_IOExceptionNodeM(*lIter, lOSS.str());
			}
		}
		Genotype::Handle lGenotype = castHandleT<Genotype>(lGenotypeAlloc->allocate());
		ioContext.setGenotypeHandle(lGenotype);
		ioContext.setGenotypeIndex(size());
		push_back(lGenotype);
		lGenotype->readWithContext(lIter, ioContext);
	}
	ioContext.setGenotypeHandle(lPrevGenoHandle);
	ioContext.setGenotypeIndex(lPrevGenoIndex);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read members of an individual from an XML node.
 *  \param inIter XML iterator to read the individual's members from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Individual::readMembers(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	mMemberMap.clear();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	for(PACC::XML::ConstIterator lIter=inIter; lIter; ++lIter) {
		if(lIter->getType() != PACC::XML::eData) continue;
		if((lIter->getValue() == "Genotype") || (lIter->getValue() == "Fitness")) continue;
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
 *  \brief Read an individual from an XML node.
 *  \param inIter XML iterator to read the individual from.
 *  \param ioContext Evolutionary context to use to read the individual.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Individual::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Individual")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Individual> expected!");
	}
	// Be sure that the types are corresponding
	const std::string& lIndivType = inIter->getAttribute("type");
	if((lIndivType.empty() == false) && (lIndivType != getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lIndivType << "' mismatch type of the individual '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	// Read members
	readMembers(inIter->getFirstChild(), ioContext);
	// Read fitness
	readFitness(inIter->getFirstChild(), ioContext);
	// Read genotypes
	readGenotypes(inIter->getFirstChild(), ioContext);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Remove given member from the individual's member map.
 *  \param inName Name of the member to remove.
 *  \return Handle to the member removed from the individual,
 *     NULL pointer if there was no member with given name.
 */
Member::Handle Individual::removeMember(const std::string& inName)
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
 *  \brief Write an individual into a XML streamer.
 *  \param ioStreamer XML streamer to write the individual into.
 *  \param inIndent Whether XML output should be indented.
 */
void Individual::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Individual", inIndent);
	ioStreamer.insertAttribute("type", getType());
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write content of an individual into a XML streamer.
 *  \param ioStreamer XML streamer to write the individual into.
 *  \param inIndent Whether XML output should be indented.
 */
void Individual::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	// Write members of the individual
	writeMembers(ioStreamer, false);
	// Write fitness
	writeFitness(ioStreamer, false);
	// Write genotypes
	writeGenotypes(ioStreamer, false);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write fitness of an individual into a XML streamer.
 *  \param ioStreamer XML streamer to write the fitness into.
 *  \param inIndent Whether XML output should be indented.
 */
void Individual::writeFitness(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if(mFitness == NULL) {
		ioStreamer.openTag("Fitness", false);
		ioStreamer.insertAttribute("valid", "no");
		ioStreamer.closeTag();
	} else {
		mFitness->write(ioStreamer, inIndent);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write genotypes of an individual into a XML streamer.
 *  \param ioStreamer XML streamer to write the genotypes into.
 *  \param inIndent Whether XML output should be indented.
 */
void Individual::writeGenotypes(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	for(unsigned int i=0; i<size(); ++i) {
		Beagle_NonNullPointerAssertM((*this)[i]);
		(*this)[i]->write(ioStreamer, inIndent);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write members of an individual into a XML streamer.
 *  \param ioStreamer XML streamer to write the members into.
 *  \param inIndent Whether XML output should be indented.
 */
void Individual::writeMembers(PACC::XML::Streamer& ioStreamer, bool inIndent) const
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
