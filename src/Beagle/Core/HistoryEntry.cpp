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
 *  \file   Beagle/Core/HistoryEntry.cpp
 *  \brief  Source code of class HistoryEntry.
 *  \author Alexandre Devert
 *  $Revision: 1.22 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/Core.hpp"
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct an uninitialized history entry.
 */
HistoryEntry::HistoryEntry()
{}


/*!
 *  \brief Construct a history entry for the individual.
 */
HistoryEntry::HistoryEntry(const std::vector<HistoryID>& inParents,
                           const std::string& inOpName,
                           const std::string& inAction) :
	mParents(inParents),
	mActions(1, inAction),
	mOpNames(1, inOpName)
{}


/*!
 *  \brief Reading a history entry.
 */
void HistoryEntry::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Entry"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Entry> expected!");

	std::string lIndividualVarText = inIter->getAttribute("var");
	if(lIndividualVarText.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'var' of the tag <Entry> expected!");
	else
		mIndividualVar = str2uint(lIndividualVarText);

	std::string lGenerationText = inIter->getAttribute("generation");
	if (lGenerationText.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'generation' of the tag <Entry> expected!");
	else
		mGeneration = str2uint(lGenerationText);

	mParents.clear();
	mActions.clear();
	mOpNames.clear();

	for(PACC::XML::ConstIterator lChild(inIter->getFirstChild()); lChild; ++lChild) {
		if(lChild->getType()==PACC::XML::eData) {
			if(lChild->getValue()=="Parent") {
				unsigned int lHistoryId = str2uint(lChild->getAttribute("id"));
				unsigned int lHistoryVar = str2uint(lChild->getAttribute("var"));
				mParents.push_back(HistoryID(lHistoryId, lHistoryVar));
			} else if(lChild->getValue()=="Operations") {
				mActions.push_back(lChild->getAttribute("action"));
				mOpNames.push_back(lChild->getAttribute("opname"));
			}
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a history entry into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether XML output should be indented.
 */
void HistoryEntry::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag("Entry", inIndent);
	ioStreamer.insertAttribute("var", uint2str(mIndividualVar));
	ioStreamer.insertAttribute("generation", uint2str(mGeneration));

	for(unsigned int i=0; i<mParents.size(); ++i) {
		ioStreamer.openTag("Parent", inIndent);
		ioStreamer.insertAttribute("id", uint2str(mParents[i].getID()));
		ioStreamer.insertAttribute("var", uint2str(mParents[i].getVar()));
		ioStreamer.closeTag();
	}

	for(unsigned int i=0; i<mOpNames.size(); ++i) {
		ioStreamer.openTag("Operations", inIndent);
		ioStreamer.insertAttribute("action", mActions[i]);
		ioStreamer.insertAttribute("opname", mOpNames[i]);
		ioStreamer.closeTag();
	}

	ioStreamer.closeTag();

	Beagle_StackTraceEndM();
}
