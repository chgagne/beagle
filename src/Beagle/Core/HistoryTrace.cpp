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
 *  \file   Beagle/Core/HistoryTrace.cpp
 *  \brief  Source code of class HistoryTrace.
 *  \author Alexandre Devert
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;



/*!
 *  \brief Construct a history trace.
 *  \param inN Number of entry in the history trace.
 *  \param inModel Initial value of the traces.
 */
HistoryTrace::HistoryTrace(unsigned int inN, HistoryEntry::Handle inModel) :
	HistoryEntry::Bag(inN, inModel)
{ }


/*!
 *  \brief Reading a history trace.
 */
void HistoryTrace::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();

	clear();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Trace"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Trace> expected!");

	std::string lDemeIDText = inIter->getAttribute("deme");
	if(lDemeIDText.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'deme' of the tag <Trace> expected!");
	else
		mDemeID = str2uint(lDemeIDText);

	std::string lIndividualIDText = inIter->getAttribute("id");
	if(lDemeIDText.empty())
		mIndividualID = HistoryID::eDummyID;
	else
		mIndividualID = str2uint(lIndividualIDText);

	for(PACC::XML::ConstIterator lChild(inIter->getFirstChild()); lChild; ++lChild) {
		HistoryEntry::Handle lHistoryEntry = new HistoryEntry();
		lHistoryEntry->read(lChild);
		push_back(lHistoryEntry);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a history trace into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether XML output should be indented.
 */
void HistoryTrace::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Trace", inIndent);
	ioStreamer.insertAttribute("deme", uint2str(mDemeID));
	if(mIndividualID != HistoryID::eDummyID) {
		ioStreamer.insertAttribute("id", uint2str(mIndividualID));
	}
	for(unsigned int i=0; i<size(); ++i) {
		at(i)->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
