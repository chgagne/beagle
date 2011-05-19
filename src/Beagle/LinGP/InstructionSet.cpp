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
 *  \file   beagle/LinGP/src/InstructionSet.cpp
 *  \brief  Implementation of the type LinGP::InstructionSet.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/10 20:32:28 $
 */

#include "beagle/LinGP.hpp"

using namespace Beagle;


/*!
 *  \brief Get the instruction of the name given.
 *  \param inName Name of the instruction to get.
 *  \return Handle to the named instruction, NULL handle if there is no given named instruction.
 */
LinGP::Instruction::Handle LinGP::InstructionSet::getInstructionByName(std::string inName) const
{
	Map::const_iterator lMapIter = mNameLUT.find(inName);
	if(lMapIter == mNameLUT.end()) return LinGP::Instruction::Handle(NULL);
	return castHandleT<LinGP::Instruction>(lMapIter->second);
}


/*!
 *  \brief Initialize the set of instruction.
 *  \param ioSystem System used to initialize the instruction set.
 */
void LinGP::InstructionSet::registerParams(System& ioSystem)
{
	for(unsigned int i=0; i<size(); i++) (*this)[i]->registerParams(ioSystem);
}


/*!
 *  \brief Insert a new instruction in the set.
 *  \param inInstruction Handle to the instruction to insert.
 *  \throw AssertException If the given handle is NULL.
 *  \throw RunTimeException If a Instruction of the same name is already in the set.
 */
void LinGP::InstructionSet::insert(LinGP::Instruction::Handle inInstruction)
{
	Beagle_NonNullPointerAssertM(inInstruction);
	if(mNameLUT.find(inInstruction->getName()) != mNameLUT.end()) {
		std::string lMessage = "An instruction named '";
		lMessage += inInstruction->getName();
		lMessage += "' is already in the instruction set.";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	mNameLUT[inInstruction->getName()] = inInstruction;
	(*this).push_back(inInstruction);
}


/*!
 *  \brief Select randomly an instruction.
 *  \param ioSystem Reference to the evolution system.
 *  \return Handle to the selected instruction.
 */
LinGP::Instruction::Handle
LinGP::InstructionSet::selectRandomInstruction(System& ioSystem) const
{
	if(empty()) return LinGP::Instruction::Handle(NULL);
	return (*this)[ioSystem.getRandomizer().rollInteger(0,size()-1)];
}


/*!
 *  \brief Read an instruction set from a XML node.
 *  \param inIter XML node to read the instruction set from.
 *  \throw IOException If size atribute not present or if the size mismatch.
 */
void LinGP::InstructionSet::read(PACC::XML::ConstIterator inIter)
{
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "InstructionSet"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <InstructionSet> expected!");
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			Instruction::Handle lInstruction = getInstructionByName(lChild->getValue());
			if(lInstruction == NULL) {
				std::ostringstream lOSS;
				lOSS << "Instruction '" << lChild->getValue();
				lOSS << "' is not in the instruction set!";
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			lInstruction->read(lChild);
		}
	}
}


/*!
 *  \brief Write an instruction set into a XML streamer.
 *  \param ioStreamer XML streamer to use to read the instruction set into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::InstructionSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	ioStreamer.openTag("InstructionSet", inIndent);
	for(unsigned int i=0; i<size(); i++) (*this)[i]->write(ioStreamer, inIndent);
	ioStreamer.closeTag();
}
