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
 *  \file   Beagle/Core/History.cpp
 *  \brief  Source code of class History.
 *  \author Alexandre Devert
 *  $Revision: 1.29 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a history.
 */
History::History() :
	Component("History"),
	mTraces(0),
	mTracesFirst(0),
	mTracesCount(0)
{}


/*!
 *  \brief Flush the history, it will be empty after this call.
 */
void History::flush()
{
	Beagle_StackTraceBeginM();

	mTracesFirst = mTracesCount;
	mTraces.clear();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initializing the history.
 *  \param ioSystem Evolutionary system.
 */
void History::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "Post-initializing history component"
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the state of the history from a XML subtree.
 *  \param inIter XML iterator to read the history from.
 *  \param ioSystem
 */
void History::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="History"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <History> expected!");

	std::string lTracesFirstText = inIter->getAttribute("idfirst");
	if(lTracesFirstText.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'idfirst' of the tag <History> expected!");
	else mTracesFirst = str2uint(lTracesFirstText);

	std::string lTracesCountText = inIter->getAttribute("idcounter");
	if(lTracesCountText.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'idcounter' of the tag <History> expected!");
	else mTracesCount = str2uint(lTracesCountText);

	// Read all the traces first to create the history entries.
	mTraces.clear();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Trace")) {
			HistoryTrace::Handle lHistoryTrace = new HistoryTrace();
			lHistoryTrace->read(lChild);
			mTraces.push_back(lHistoryTrace);
		}
	}

	// Read the individuals and store them in the history entries.
	const Factory& lFactory = ioSystem.getFactory();
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(lFactory.getConceptAllocator("Context"));
	Context::Handle lContext = castHandleT<Context>(lContextAlloc->allocate());
	lContext->setSystemHandle(&ioSystem);

	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Individual")) {

			// Instanciate individual
			std::string lIndivTypeStr = lChild->getAttribute("type");
			Individual::Alloc::Handle lIndivAlloc = NULL;
			if(lIndivTypeStr.empty()) {
				lIndivAlloc = castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
			} else {
				lIndivAlloc = castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndivTypeStr));
			}
			Individual::Handle lIndividual = castHandleT<Individual>(lIndivAlloc->allocate());

			// Prepare context to read that individual
			lContext->setIndividualIndex(0);
			lContext->setIndividualHandle(lIndividual);
			lIndividual->readWithContext(lChild, *lContext);

			// Get HistoryID from that individual
			HistoryID::Handle lHID = castHandleT<HistoryID>(lIndividual->getMember("HistoryID"));
			if(lHID == NULL) {
				throw Beagle_IOExceptionNodeM(*lChild, "individuals in History should have an ID!");
			}
			HistoryEntry::Handle lHistoryEntry = (*mTraces[lHID->getID()-mTracesFirst])[lHID->getVar()];
			lHistoryEntry->setIndividual(lIndividual);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the history parameters.
 *  \param ioSystem Reference to the system.
 */
void History::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    "Initializing history component"
	);
	{
		Register::Description lDescription(
		    "Flag to activate history storage",
		    "Bool",
		    "1",
		    "Flag to control the storage of History traces.  When set to zero, no traces are stored and hence memory consumption is minimized.  Note that history traces can still be found in the log file if the log level is greater than or equal to 'trace' (level 5)."
		);
		mRecordActivated = castHandleT<Bool>(
		                       ioSystem.getRegister().insertEntry("hs.record.activated", new Bool(1), lDescription));
	}
	mTracesFirst = 0;
	mTracesCount = 0;
	mTraces.clear();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the state of the history into a XML streamer.
 *  \param ioStreamer XML streamer to write the history into.
 *  \param inIndent Whether output should be indented.
 */
void History::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.insertAttribute("idfirst", mTracesFirst);
	ioStreamer.insertAttribute("idcounter", mTracesCount);

	// Write the traces.
	for(unsigned int i = 0; i < mTraces.size(); ++i) {
		mTraces[i]->write(ioStreamer, inIndent);
	}

	// Write the individuals.
	for(unsigned int i = 0; i<mTraces.size(); ++i) {
		for(unsigned int j = 0; j<mTraces[i]->size(); ++j) {
			HistoryEntry::Handle lHistoryEntry = (*mTraces[i])[j];
			Individual::Handle lIndividual = lHistoryEntry->getIndividual();
			if (lIndividual != NULL) lIndividual->write(ioStreamer, false);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add an entry for inIndividual into the history.
 */
void History::trace(Context& ioContext,
                    const std::vector<HistoryID>& inParents,
                    const Individual::Handle inIndividual,
                    const std::string& inOpName,
                    const std::string& inAction)
{
	Beagle_StackTraceBeginM();

	// Check validity of individual given
	if(inIndividual == NULL) {
		throw Beagle_RunTimeExceptionM("Handle to individual should not be NULL!");
	}
	HistoryID::Handle lHID = castHandleT<HistoryID>(inIndividual->getMember("HistoryID"));
	if(lHID == NULL) {
		std::ostringstream lOSS;
		lOSS << "Individual passed to History::trace() has no history ID; ";
		lOSS << "consider History::allocateID().";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	if(lHID->getID() < mTracesFirst) {
		std::ostringstream lOSS;
		lOSS << "Individual passed to History::trace() has history ID less than the first possible ID. ";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}

	// Create history trace.
	HistoryTrace::Handle lHistoryTrace = new HistoryTrace();
	lHistoryTrace->setDemeID(ioContext.getDemeIndex());
	lHistoryTrace->setIndividualID(lHID->getID());

	// Create history entry.
	HistoryEntry::Handle lHistoryEntry = new HistoryEntry(inParents, inOpName, inAction);
	lHistoryEntry->setGeneration(ioContext.getGeneration());
	lHistoryEntry->setIndividualVar(lHID->getVar());

	// Add the entry to the trace.
	lHistoryTrace->push_back(lHistoryEntry);

	// Add entry to the records.
	if(mRecordActivated->getWrappedValue()) {
		unsigned int lHistoryId  = lHID->getID();
		unsigned int lHistoryVar = lHID->getVar();

		// A new entry is made in the records only if the history id or variation
		// of the individual is new; otherwise the action and opname are added to
		// the existing entry for that individual.
		if(lHistoryId == mTraces.size()+mTracesFirst) {
			mTraces.push_back(lHistoryTrace);
		} else if(lHistoryVar == (*mTraces[lHistoryId-mTracesFirst]).size()) {
			(*mTraces[lHistoryId-mTracesFirst]).push_back(lHistoryEntry);
		} else {
			HistoryEntry::Handle lIndividualEntry = (*mTraces[lHistoryId-mTracesFirst])[lHistoryVar];
			lIndividualEntry->addAction(inAction);
			lIndividualEntry->addOpName(inOpName);
		}

		// Store a copy of the individual in the history records if the individual
		// allocator is set.  We need to store the individual only if it has been
		// modified from its first parent, and if so the fitness of the individual
		// will be invalid.  Later, when the individual has been evaluated and has
		// a valid fitness, we will update its fitness.
		const Factory& lFactory = ioContext.getSystem().getFactory();
		if(inIndividual->getFitness()->isValid() == false) {
			std::string lIndividualType = inIndividual->getType();
			Individual::Alloc::Handle lIndividualAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			Individual::Handle lCopyIndiv = castHandleT<Individual>(lIndividualAlloc->allocate());
			lCopyIndiv->copy(*inIndividual, ioContext.getSystem());
			lHistoryEntry->setIndividual(lCopyIndiv);
		} else {
			Individual::Handle lIndividual = (*mTraces[lHistoryId-mTracesFirst])[lHistoryVar]->getIndividual();
			if(lIndividual != NULL && lIndividual->getFitness()->isValid() == false) {
				std::string lFitnessType = inIndividual->getFitness()->getType();
				Fitness::Alloc::Handle lFitnessAlloc =
				    castHandleT<Fitness::Alloc>(lFactory.getAllocator(lFitnessType));
				Fitness::Handle lCopyFitness = castHandleT<Fitness>(lFitnessAlloc->allocate());
				lCopyFitness->copy(*inIndividual->getFitness(), ioContext.getSystem());
				lIndividual->setFitness(lCopyFitness);
			}
		}
	}

	// Log trace.
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), *lHistoryTrace);

	Beagle_StackTraceEndM();
}

