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
 *  \file   Beagle/Core/HallOfFame.cpp
 *  \brief  Source code of class HallOfFame.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: 2007/09/12 14:23:47 $
 */

#include "Beagle/Core.hpp"

#include <algorithm>

using namespace Beagle;



/*!
 *  \brief Construct a member item of the hall-of-fame.
 *  \param inIndividual Handle to the individual in the hall-of-fame.
 *  \param inGeneration Generation number the individual is added to the hall-of-fame.
 *  \param inDemeIndex Index in which the individual was at its insertion in the hall-of-fame.
 */
HallOfFame::Entry::Entry(Individual::Handle inIndividual,
                         unsigned int inGeneration,
                         unsigned int inDemeIndex) :
	mIndividual(inIndividual),
	mGeneration(inGeneration),
	mDemeIndex(inDemeIndex)
{ }


/*!
 *  \brief Compare equality of two members of the hall-of-fame.
 *  \param inRightEntry Other member to compare with actual member.
 *  \return True if members are equal, false if they are not.
 */
bool HallOfFame::Entry::operator==(const HallOfFame::Entry& inRightEntry) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(mIndividual);
	Beagle_NonNullPointerAssertM(inRightEntry.mIndividual);
	return mIndividual->isEqual(*inRightEntry.mIndividual);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compare order of two members of the hall-of-fame.
 *  \param inRightEntry Other member to compare with actual member.
 *  \return True if left members is less than right one, false if not.
 */
bool HallOfFame::Entry::operator<(const HallOfFame::Entry& inRightEntry) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(mIndividual);
	Beagle_NonNullPointerAssertM(inRightEntry.mIndividual);
	return mIndividual->isLess(*inRightEntry.mIndividual);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compare order of two members of the hall-of-fame.
 *  \param inRightEntry Other member to compare with actual member.
 *  \return True if left members is less than right one, false if not.
 */
bool HallOfFame::Entry::operator>(const HallOfFame::Entry& inRightEntry) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(mIndividual);
	Beagle_NonNullPointerAssertM(inRightEntry.mIndividual);
	return inRightEntry.mIndividual->isLess(*mIndividual);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Construct an hall-of-fame.
 *  \param inN Initial number of elements in hall-of-fame.
 */
HallOfFame::HallOfFame(unsigned int inN) :
	mEntries(inN)
{ }


/*!
 *  \brief Construct an hall-of-fame.
 *  \param inN Initial number of elements in hall-of-fame.
 *  \param inModel Model of elements in the hall-of-fame.
 */
HallOfFame::HallOfFame(unsigned int inN, const HallOfFame::Entry& inModel) :
	mEntries(inN, inModel)
{ }


/*!
 *  \brief Copy a hall-of-fame into current one.
 *  \param inOriginal Hall-of-fame to copy.
 *  \param ioSystem Evolutionary system.
 */
void HallOfFame::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const HallOfFame& lOriginal = castObjectT<const HallOfFame&>(inOriginal);
	const Factory& lFactory = ioSystem.getFactory();
	mEntries.resize(lOriginal.size());
	for(unsigned int i=0; i<lOriginal.size(); ++i) {
		const std::string& lIndivTypeName = lOriginal[i].mIndividual->getType();
		Individual::Alloc::Handle lIndivAlloc =
		    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndivTypeName));
		Individual::Handle lIndividual = castHandleT<Individual>(lIndivAlloc->allocate());
		lIndividual->copy(*lOriginal[i].mIndividual, ioSystem);
		(*this)[i].mIndividual = lIndividual;
		(*this)[i].mGeneration = lOriginal[i].mGeneration;
		(*this)[i].mDemeIndex  = lOriginal[i].mDemeIndex;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get name of object.
 *  \return Object name, "HallOfFame".
 */
const std::string& HallOfFame::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("HallOfFame");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the hall-of-fame.
 *  \return Exact type of the hall-of-fame.
 */
const std::string& HallOfFame::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("HallOfFame");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a hall-of-fame from an XML subtree.
 *  \param inIter XML iterator to read the hall-of-fame from.
 *  \param ioContext Evolutionary context to use to read the hall-of-fame.
 *  \throw Beagle::IOException If the format is not respected.
 */
void HallOfFame::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="HallOfFame")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <HallOfFame> expected!");
	}
	// Be sure that the types are corresponding
	const std::string& lIndivType = inIter->getAttribute("type");
	if((lIndivType.empty()==false) && (lIndivType!=getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lIndivType << "' mismatch type of the hall-of-fame '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
	clear();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Member")) {
			HallOfFame::Entry lEntry;
			const std::string lGenerationAttr = lChild->getAttribute("generation");
			if(lGenerationAttr.empty()) lEntry.mGeneration = 0;
			else lEntry.mGeneration = str2uint(lGenerationAttr);
			const std::string& lDemeAttr = lChild->getAttribute("deme");
			if(lDemeAttr.empty()) lEntry.mDemeIndex = 0;
			else lEntry.mDemeIndex = str2uint(lDemeAttr);
			PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
			if(lChild2 && (lChild2->getType()==PACC::XML::eData) && (lChild2->getValue()=="Individual")) {
				const std::string& lIndividualType = lChild2->getAttribute("type");
				Individual::Alloc::Handle lIndividualAlloc = NULL;
				if(lIndividualType.empty()) {
					lIndividualAlloc = castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
					if(lIndividualAlloc == NULL) {
						std::ostringstream lOSS;
						lOSS << "Individual object can't be read, ";
						lOSS << "it appears that its type is not given and that there is not ";
						lOSS << "valid concept allocator associated to it!";
						throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
					}
				} else {
					lIndividualAlloc = castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
					if(lIndividualAlloc == NULL) {
						std::ostringstream lOSS;
						lOSS << "Type '" << lIndividualType << "' associated to hall-of-fame object ";
						lOSS << "is not valid!";
						throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
					}
				}
				lEntry.mIndividual = castHandleT<Individual>(lIndividualAlloc->allocate());
				ioContext.setIndividualHandle(lEntry.mIndividual);
				ioContext.setIndividualIndex(size());
				lEntry.mIndividual->readWithContext(lChild2,ioContext);
				mEntries.push_back(lEntry);
			}
		}
	}
	ioContext.setIndividualHandle(lOldIndivHandle);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Sort an hall-of-fame from the best to the worse individual.
 */
void HallOfFame::sort()
{
	Beagle_StackTraceBeginM();
	std::sort(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Update the hall-of-fame with the actual deme's individuals, if necessary.
 *  \param inSizeHOF The desired size of the hall-of-fame.
 *  \param inDeme Deme to add to the hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the HOF had been modified, false if not.
 */
bool HallOfFame::updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// If new HOF size is zero, so empty the HOF and return.
	if(inSizeHOF == 0) {
		if(mEntries.empty()) return false;
		clear();
		Beagle_LogVerboseM(ioContext.getSystem().getLogger(), "Hall-of-fame cleared");
		return true;
	}

	bool lHOFModified = false;             // Is the HOF modified?
	unsigned int lAddedIndividuals = 0;    // How many new individuals added to the HOF.

	// Make the hall-of-fame a heap with the worse individuals on the top.
	std::make_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());

	// If the heap is too big, resizing it.
	while(mEntries.size() > inSizeHOF) {
		std::pop_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *mEntries.back().mIndividual);
		mEntries.pop_back();
		lHOFModified = true;
	}

	// Temporary buffer of individuals.
	Individual::Bag lTempPop;
	// Insert pointer of all the individuals of the deme in the buffer.
	lTempPop.insert(lTempPop.end(), inDeme.begin(), inDeme.end());
	// Make the buffer a STL heap with the fittest individual on the top.
	std::make_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());

	// If the old HOF is smaller than the desired new size,
	// fill with the best individuals of the temp buffer.
	while((lTempPop.empty() == false) && (mEntries.size() < inSizeHOF)) {

		// Test if individual on the top of the heap is identical to one
		// individual in the heap.
		bool lIsIdentical = false;
		for(unsigned int i=0; i<mEntries.size(); i++) {
			if(lTempPop[0]->isIdentical(*mEntries[i].mIndividual)) {
				lIsIdentical = true;
				break;
			}
		}

		// If the individual on the top of the heap is not identical, add it to the
		// new HOF.
		if(lIsIdentical == false) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const std::string& lIndividualType = lTempPop[0]->getType();
			Individual::Alloc::Handle lIndividualAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			if(lIndividualAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lIndividualType << "' associated to individual to add to hall-of-fame ";
				lOSS << "is not valid!";
				throw Beagle_RunTimeExceptionM(lOSS.str());
			}
			Individual::Handle lIndivCopy = castHandleT<Individual>(lIndividualAlloc->allocate());
			lIndivCopy->copy(*lTempPop[0], ioContext.getSystem());
			HallOfFame::Entry lEntry(lIndivCopy, ioContext.getGeneration(), ioContext.getDemeIndex());
			mEntries.push_back(lEntry);
			std::push_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lTempPop[0]);
			++lAddedIndividuals;
			lHOFModified = true;
		}

		// Remove it from the temporary buffer.
		std::pop_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());
		lTempPop.pop_back();
	}

	// Ok, now the new HOF is of the good size. Let's see if any individuals that are
	// still in the temporary buffer can be added to the new HOF.

	// While there is individuals in the temporary buffer that can be added to the new HOF.
	while((lTempPop.empty() == false) && (mEntries[0].mIndividual->isLess(*lTempPop[0]))) {

		// Test if individual on the top of the heap is identical to one
		// individual in the heap.
		bool lIsIdentical = false;
		for(unsigned int i=0; i<mEntries.size(); i++) {
			if(lTempPop[0]->isIdentical(*mEntries[i].mIndividual)) {
				lIsIdentical = true;
				break;
			}
		}

		// If the individual on the top of the heap is not identical, add it to the
		// new HOF.
		if(lIsIdentical == false) {
			// STL heap pop of the worse individual of the HOF
			std::pop_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());
			
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *mEntries.back().mIndividual);

			// Replace the popped individual with the best of the temporary buffer.
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const std::string& lIndividualType = lTempPop[0]->getType();
			Individual::Alloc::Handle lIndividualAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			if(lIndividualAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lIndividualType << "' associated to individual to add to hall-of-fame ";
				lOSS << "is not valid!";
				throw Beagle_RunTimeExceptionM(lOSS.str());
			}
			mEntries.back().mIndividual = castHandleT<Individual>(lIndividualAlloc->allocate());
			mEntries.back().mIndividual->copy(*lTempPop[0], ioContext.getSystem());
			mEntries.back().mGeneration = ioContext.getGeneration();
			mEntries.back().mDemeIndex  = ioContext.getDemeIndex();

			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lTempPop[0]);

			// STL push heap of the newly added individual.
			std::push_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());

			// A new individual is added to the HOF.
			++lAddedIndividuals;
			lHOFModified = true;
		}

		// STL heap pop of the best individual of the temporary buffer.
		std::pop_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());
		lTempPop.pop_back();
	}

	if(lHOFModified) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "There are " << lAddedIndividuals << " individuals added to the hall-of-fame"
		);
	}

	// Exit return whether the HOF has been modified.
	return lHOFModified;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if an individual should be added to the hall-of-fame, and add it if necessary.
 *  \param inSizeHOF The desired size of the hall-of-fame.
 *  \param inIndividual Individual that is checked for addition to the hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the HOF had been modified, false if not.
 */
bool HallOfFame::updateWithIndividual(unsigned int inSizeHOF,
                                      const Individual& inIndividual,
                                      Context& ioContext)
{
	Beagle_StackTraceBeginM();

	if(inSizeHOF == 0) {
		if(inSizeHOF != mEntries.size()) {
			clear();
			Beagle_LogVerboseM(ioContext.getSystem().getLogger(), "Hall-of-fame cleared");
			return true;
		} else return false;
	}

	std::make_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());

	bool lHOFModified = false;
	if(mEntries.empty() || mEntries[0].mIndividual->isLess(inIndividual)) {
		bool lIsIdentical = false;
		for(unsigned int i=0; i<mEntries.size(); ++i) {
			if(inIndividual.isIdentical(*mEntries[i].mIndividual)) {
				lIsIdentical = true;
				break;
			}
		}
		if(lIsIdentical == false) {
			Beagle_LogVerboseM(
				ioContext.getSystem().getLogger(),
			    "An individual is added to the hall-of-fame"
			);
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), inIndividual);

			const Factory& lFactory = ioContext.getSystem().getFactory();
			const std::string& lIndividualType = inIndividual.getType();
			Individual::Alloc::Handle lIndividualAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			if(lIndividualAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lIndividualType << "' of individual to add to hall-of-fame ";
				lOSS << "is not valid!";
				throw Beagle_RunTimeExceptionM(lOSS.str());
			}
			HallOfFame::Entry lEntry;
			lEntry.mIndividual = castHandleT<Individual>(lIndividualAlloc->allocate());
			lEntry.mIndividual->copy(inIndividual, ioContext.getSystem());
			lEntry.mGeneration = ioContext.getGeneration();
			lEntry.mDemeIndex  = ioContext.getDemeIndex();
			mEntries.push_back(lEntry);
			std::push_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());
			lHOFModified = true;
		}
	}

	while(mEntries.size() > inSizeHOF) {
		std::pop_heap(mEntries.begin(), mEntries.end(), std::greater<HallOfFame::Entry>());
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *mEntries.back().mIndividual);
		mEntries.pop_back();
		lHOFModified = true;
	}

	return lHOFModified;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the actual hall-of-fame in a XML streamer.
 *  \param ioStreamer XML streamer to write the hall-of-fame into.
 *  \param inIndent Whether XML output should be indented.
 */
void HallOfFame::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	std::vector<HallOfFame::Entry> lTemp(mEntries.begin(), mEntries.end());
	std::sort(lTemp.begin(), lTemp.end(), std::greater<HallOfFame::Entry>());
	ioStreamer.insertAttribute("size", uint2str(lTemp.size()));
	for(unsigned int i=0; i<lTemp.size(); ++i) {
		ioStreamer.openTag("Member", inIndent);
		ioStreamer.insertAttribute("generation", uint2str(lTemp[i].mGeneration));
		ioStreamer.insertAttribute("deme", uint2str(lTemp[i].mDemeIndex));
		Beagle_NonNullPointerAssertM(lTemp[i].mIndividual);
		lTemp[i].mIndividual->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
	Beagle_StackTraceEndM();
}
