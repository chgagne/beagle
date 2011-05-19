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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/InstructionSuperSet.cpp
 *  \brief  Implementation of the type LinGP::InstructionSuperSet.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 17:03:05 $
 */

#include "beagle/LinGP.hpp"

using namespace Beagle;


/*!
 *  \brief Initialize the super set of instructions.
 */
void LinGP::InstructionSuperSet::registerParams(System& ioSystem)
{
	for(unsigned int i=0; i<mInstructionSets.size(); i++) {
		mInstructionSets[i]->registerParams(ioSystem);
	}
}


/*!
 *  \brief Insert a nes set of instructions in the super set.
 *  \param inInstructionSet Inserted instruction set.
 */
void LinGP::InstructionSuperSet::insert(LinGP::InstructionSet::Handle inInstructionSet)
{
	mInstructionSets.push_back(inInstructionSet);
}

/*!
 *  \brief Read an instruction super set from a Beagle XML stream.
 *  \param inIter XML node used to read the super set from.
 *  \throw Beagle::IOException If size atribute not present.
 */
void LinGP::InstructionSuperSet::read(PACC::XML::ConstIterator inIter)
{
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "InstructionSuperSet"))
		throw Beagle_IOExceptionNodeM((*inIter), "tag <InstructionSuperSet> expected!");
	unsigned int lPSIndex = 0;
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="InstructionSet")) {
			mInstructionSets[lPSIndex++]->read(lChild);
		}
	}
}


/*!
 *  \brief Write an instruction super set into a Beagle XML stream.
 *  \param ioStreamer XMl streamer to write the super set into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::InstructionSuperSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	ioStreamer.openTag("InstructionSuperSet", inIndent);
	for(unsigned int i=0; i<mInstructionSets.size(); i++) {
		mInstructionSets[i]->write(ioStreamer);
	}
	ioStreamer.closeTag();
}
