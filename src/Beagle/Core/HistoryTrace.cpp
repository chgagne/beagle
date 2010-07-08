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
 *  \file   beagle/Core/HistoryTrace.cpp
 *  \brief  Source code of class HistoryTrace.
 *  \author Alexandre Devert
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "beagle/Core.hpp"

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

	Beagle_StackTraceEndM("void HistoryTrace::read(PACC::XML::ConstIterator inIter)");
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
	Beagle_StackTraceEndM("void HistoryTrace::write(PACC::XML::Streamer&,bool) const");
}
