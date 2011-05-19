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
 *  \file   Beagle/EC/MigrationBuffer.cpp
 *  \brief  Definition of the class MigrationBuffer.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Clear the migration buffer.
 */
void MigrationBuffer::clear()
{
	mEmigrants.clear();
	mImmigrants.clear();
	mReplacedIndices.clear();
}


/*!
 *  \brief Copy a migration buffer into current one.
 *  \param inOriginal Migration buffer to copy.
 *  \param ioSystem Evolutionary system.
 */
void MigrationBuffer::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const MigrationBuffer& lOriginal = castObjectT<const MigrationBuffer&>(inOriginal);
	mEmigrants.resize(lOriginal.mEmigrants.size());
	const Factory& lFactory = ioSystem.getFactory();
	for(unsigned int i=0; i<lOriginal.mEmigrants.size(); ++i) {
		const std::string& lIndivTypeName = lOriginal.mEmigrants[i]->getType();
		Individual::Alloc::Handle lIndivAlloc =
		    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndivTypeName));
		mEmigrants[i] = castHandleT<Individual>(lIndivAlloc->allocate());
		mEmigrants[i]->copy(*lOriginal.mEmigrants[i], ioSystem);
	}
	mImmigrants.resize(lOriginal.mImmigrants.size());
	for(unsigned int i=0; i<lOriginal.mImmigrants.size(); ++i) {
		const std::string& lIndivTypeName = lOriginal.mImmigrants[i]->getType();
		Individual::Alloc::Handle lIndivAlloc =
		    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndivTypeName));
		mImmigrants[i] = castHandleT<Individual>(lIndivAlloc->allocate());
		mImmigrants[i]->copy(*lOriginal.mImmigrants[i], ioSystem);
	}
	mReplacedIndices = lOriginal.mReplacedIndices;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get name of object.
 *  \return Object name, "MigrationBuffer".
 */
const std::string& MigrationBuffer::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("MigrationBuffer");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the statistics.
 *  \return Exact type of the statistics.
 */
const std::string& MigrationBuffer::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("MigrationBuffer");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Select the given individual indices for future migration.
 *  \param inEmigrantIndices Indices of emigrant individuals.
 *  \param ioDeme Deme from which the individuals are migrated.
 *  \param ioContext Evolutionary context.
 */
void MigrationBuffer::insertEmigrants(const std::vector<unsigned int>& inEmigrantIndices,
                                      Deme& ioDeme,
                                      Context& ioContext)
{
	Beagle_StackTraceBeginM();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	Individual::Alloc::Handle lIndivAlloc =
	    castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
	for(unsigned int i=0; i<inEmigrantIndices.size(); ++i) {
		Beagle_AssertM(inEmigrantIndices[i] < ioDeme.size());
		Beagle_NonNullPointerAssertM(ioDeme[inEmigrantIndices[i]]);
		const std::string& lIndivTypeName = ioDeme[inEmigrantIndices[i]]->getType();
		Individual::Alloc::Handle lIndivAlloc =
		    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndivTypeName));
		Individual::Handle lIndivCopy = castHandleT<Individual>(lIndivAlloc->allocate());
		lIndivCopy->copy(*ioDeme[inEmigrantIndices[i]], ioContext.getSystem());
		mEmigrants.push_back(lIndivCopy);
		if(lHistory != NULL) {
			lHistory->allocateNewID(*(mEmigrants.back()));
			std::vector<HistoryID> lParent;
			HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[inEmigrantIndices[i]]->getMember("HistoryID"));
			if(lHID != NULL) lParent.push_back(*lHID);
			lHistory->trace(ioContext, lParent, mEmigrants.back(), "MigrationBuffer", "migration");
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Select the individuals that will be replaced by immigrants.
 *  \param inReplacedIndices Indices of individual replaced by immigrants.
 *  \param ioDeme Deme from from which the replaced individuals are taken.
 */
void MigrationBuffer::insertReplaced(const std::vector<unsigned int>& inReplacedIndices,
                                     Deme& ioDeme)
{
	Beagle_StackTraceBeginM();
	for(unsigned int i=0; i<inReplacedIndices.size(); ++i) {
		mReplacedIndices.push_back(inReplacedIndices[i]);
	}
	mergeImmigrantsToDeme(ioDeme);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Merge individuals in immigrant buffer into actual deme, if possible.
 *  \param ioDeme Deme associated to migration buffer.
 */
void MigrationBuffer::mergeImmigrantsToDeme(Deme& ioDeme)
{
	Beagle_StackTraceBeginM();
	if(ioDeme.size() == 0) return;
	while((mImmigrants.empty()==false) && (mReplacedIndices.empty()==false)) {
		Beagle_AssertM(mReplacedIndices.front() < ioDeme.size());
		ioDeme[mReplacedIndices.front()] = mImmigrants.front();
		mReplacedIndices.pop_front();
		mImmigrants.pop_front();
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Move migrants between two demes.
 *  \param inNbMigrants Number of migrants to move.
 *  \param ioDestDeme Destination deme of the migrants.
 *  \param ioContext Evolutionary context.
 */
void MigrationBuffer::moveMigrants(unsigned int inNbMigrants,
                                   Deme& ioDestDeme,
                                   Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(inNbMigrants > mEmigrants.size()) {
		std::ostringstream lOSS;
		lOSS << "Number of migrants asked (" << inNbMigrants << ") is higher than the number of ";
		lOSS << "emigrants available (" << mEmigrants.size() << ") in original deme. Could not proceed.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}

	MigrationBuffer::Handle lDestMigBuffer =
	    castHandleT<MigrationBuffer>(ioDestDeme.getMember("MigrationBuffer"));
	if(lDestMigBuffer == NULL) {
		const Factory& lFactory = ioContext.getSystem().getFactory();
		MigrationBuffer::Alloc::Handle lMigBufferAlloc =
		    castHandleT<MigrationBuffer::Alloc>(lFactory.getConceptAllocator("MigrationBuffer"));
		lDestMigBuffer = castHandleT<MigrationBuffer>(lMigBufferAlloc->allocate());
		ioDestDeme.addMember(lDestMigBuffer);
	}

	// Move individual between migrations buffer.
	for(unsigned int i=0; i<inNbMigrants; ++i) {
		lDestMigBuffer->mImmigrants.push_back(mEmigrants.front());
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "Emigrant moved into the migration buffer of another deme"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *mEmigrants.front());
		mEmigrants.pop_front();
	}
	lDestMigBuffer->mergeImmigrantsToDeme(ioDestDeme);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a migration buffer from an XML iterator.
 *  \param inIter XML iterator to read the migration buffer from.
 *  \param ioContext Evolutionary context to use to read the migration buffer.
 *  \throw Beagle::IOException If the format is not respected.
 */
void MigrationBuffer::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="MigrationBuffer")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <MigrationBuffer> expected!");
	}

	// Be sure that the types are corresponding
	const std::string& lIndivType = inIter->getAttribute("type");
	if((lIndivType.empty()==false) && (lIndivType!=getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lIndivType << "' mismatch type of the migration buffer '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	// Store current context
	unsigned int lOldIndivIndex = ioContext.getIndividualIndex();
	Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();

	// Clear migration buffer before reading anything.
	clear();

	// Loop through <MigrationBuffer> elements.
	const Factory& lFactory = ioContext.getSystem().getFactory();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		// Read emigrants
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Emigrants")) {
			for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
				if((lChild2->getType()!=PACC::XML::eData) || (lChild2->getValue()=="Individual")) {
					throw Beagle_IOExceptionNodeM(*inIter, "tag <Individual> expected!");
				}
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
						lOSS << "Type '" << lIndividualType << "' associated to migration buffer ";
						lOSS << "is not valid!";
						throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
					}
				}
				Individual::Handle lIndividual = castHandleT<Individual>(lIndividualAlloc->allocate());
				ioContext.setIndividualHandle(lIndividual);
				ioContext.setIndividualIndex(mEmigrants.size());
				lIndividual->readWithContext(lChild2,ioContext);
				mEmigrants.push_back(lIndividual);
			}
		}
		// Read immigrants
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Immigrants")) {
			for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
				if((lChild2->getType()!=PACC::XML::eData) || (lChild2->getValue()=="Individual")) {
					throw Beagle_IOExceptionNodeM(*inIter, "tag <Individual> expected!");
				}
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
						lOSS << "Type '" << lIndividualType << "' associated to migration buffer ";
						lOSS << "is not valid!";
						throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
					}
				}
				Individual::Handle lIndividual = castHandleT<Individual>(lIndividualAlloc->allocate());
				ioContext.setIndividualHandle(lIndividual);
				ioContext.setIndividualIndex(mImmigrants.size());
				lIndividual->readWithContext(lChild2,ioContext);
				mImmigrants.push_back(lIndividual);
			}
		}
		// Read replaced individual indices
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="ReplacedIndices")) {
			for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
				if(lChild2->getType() != PACC::XML::eString) {
					throw Beagle_IOExceptionNodeM(*lChild2, "expected list of index!");
				}
				std::istringstream lISS(lChild2->getValue());
				while(lISS.good()) {
					unsigned int lValue;
					lISS >> lValue;
					mReplacedIndices.push_back(lValue);
					if(lISS.good()==false) break;
					int lDelim=lISS.get();
					if((lISS.good()==false) || (lDelim==-1)) break;
				}
			}
		}
	}

	// Restore context
	ioContext.setIndividualHandle(lOldIndivHandle);
	ioContext.setIndividualIndex(lOldIndivIndex);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the migration buffer in a XML streamer.
 *  \param ioStreamer XML streamer to write migration buffer into.
 *  \param inIndent Whether XML output should be indented.
 */
void MigrationBuffer::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	// Write emigrants.
	ioStreamer.openTag("Emigrants", inIndent);
	for(unsigned int i=0; i<mEmigrants.size(); ++i) {
		Beagle_NonNullPointerAssertM(mEmigrants[i]);
		mEmigrants[i]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();

	// Write immigrants.
	ioStreamer.openTag("Immigrants", inIndent);
	for(unsigned int i=0; i<mImmigrants.size(); ++i) {
		Beagle_NonNullPointerAssertM(mImmigrants[i]);
		mImmigrants[i]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();

	// Write replaced individual indices.
	ioStreamer.openTag("ReplacedIndices", inIndent);
	if(mReplacedIndices.empty() == false) {
		std::ostringstream lOSS;
		for(unsigned int i=0; i<mReplacedIndices.size(); ++i) {
			lOSS << mReplacedIndices[i];
			if(i != (mReplacedIndices.size()-1)) lOSS << ',';
		}
		ioStreamer.insertStringContent(lOSS.str());
	}
	ioStreamer.closeTag();

	Beagle_StackTraceEndM();
}
