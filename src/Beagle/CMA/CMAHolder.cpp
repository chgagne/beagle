/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  \file   beagle/GA/src/CMAHolder.cpp
 *  \brief  Source code of class GA::CMAHolder.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/10 20:32:21 $
 */

#include "beagle/GA.hpp"


using namespace Beagle;


/*!
 *  \brief Construct CMA value holder system component, which contain global CMA values.
 */
GA::CMAHolder::CMAHolder() :
		Beagle::Component("CMAHolder")
{ }


/*!
 *  \brief Read CMA holder system component from XML iterator.
 *  \param inIter Iterator to read CMA holder from.
 *  \param ioSystem Evolutionary system.
 */
void GA::CMAHolder::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="CMAHolder"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <CMAHolder> expected!");
	clear();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="CMAValues")) {
			std::string lIndexStr = lChild->getAttribute("index");
			if(lIndexStr.empty()) throw Beagle_IOExceptionNodeM(*lChild, "attribute 'index' expected!");
			unsigned int lIndex = str2uint(lIndexStr);
			GA::CMAValues& lValues = (*this)[lIndex];
			for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
				if(lChild2->getType()!=PACC::XML::eData) continue;
				if(lChild2->getFirstChild() == NULL)
					throw Beagle_IOExceptionNodeM(*lChild2, "child expected!");
				if(lChild2->getValue()=="B") lValues.mB.read(lChild2->getFirstChild());
				else if(lChild2->getValue()=="D")  lValues.mD.read(lChild2->getFirstChild());
				else if(lChild2->getValue()=="PC") lValues.mPC.read(lChild2->getFirstChild());
				else if(lChild2->getValue()=="PS") lValues.mPS.read(lChild2->getFirstChild());
				else if(lChild2->getValue()=="Xmean") lValues.mXmean.read(lChild2->getFirstChild());
				else if(lChild2->getValue()=="Sigma") lValues.mSigma.read(lChild2->getFirstChild());
			}
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write CMA system component into XML streamer.
 *  \param ioStreamer XML streamer to write CMA holder into.
 *  \param inIndent Whether output should be indented.
 */
void GA::CMAHolder::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	for(GA::CMAHolder::const_iterator lIterMap=begin(); lIterMap!=end(); ++lIterMap) {
		ioStreamer.openTag("CMAValues", inIndent);
		ioStreamer.insertAttribute("index", uint2str(lIterMap->first));
		ioStreamer.openTag("B", inIndent);
		lIterMap->second.mB.write(ioStreamer, inIndent);
		ioStreamer.closeTag();
		ioStreamer.openTag("D", inIndent);
		lIterMap->second.mD.write(ioStreamer, inIndent);
		ioStreamer.closeTag();
		ioStreamer.openTag("PC", inIndent);
		lIterMap->second.mPC.write(ioStreamer, inIndent);
		ioStreamer.closeTag();
		ioStreamer.openTag("PS", inIndent);
		lIterMap->second.mPS.write(ioStreamer, inIndent);
		ioStreamer.closeTag();
		ioStreamer.openTag("Xmean", inIndent);
		lIterMap->second.mXmean.write(ioStreamer, inIndent);
		ioStreamer.closeTag();
		ioStreamer.openTag("Sigma", inIndent);
		lIterMap->second.mSigma.write(ioStreamer, inIndent);
		ioStreamer.closeTag();
		ioStreamer.closeTag();
	}
	Beagle_StackTraceEndM();
}
