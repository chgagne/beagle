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
 *  \file   beagle/LinGP/src/Instruction.cpp
 *  \brief  Implementation of the type LinGP::Instruction.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 17:03:05 $
 */

#include "beagle/LinGP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a Instruction.
 *  \param inNumberArguments Number of arguments of the instruction.
 *  \param inName Name of the instruction.
 */
LinGP::Instruction::Instruction(const char* inName) :
		mName(inName)
{ }


/*!
 *  \brief Construct a instruction.
 *  \param inName Name of the Instruction.
 */
LinGP::Instruction::Instruction(std::string inName) :
		mName(inName)
{ }


/*!
 *  \brief Give a reference to the current instruction.
 *  \param ioContext Reference to the evolution context.
 *  \return Handle to the refered instruction.
 */
LinGP::Instruction::Handle LinGP::Instruction::giveReference(LinGP::Context& ioContext)
{
	return LinGP::Instruction::Handle(this);
}


/*!
 *  \brief Initialize the instruction.
 *  \param ioSystem Evolutionary system used.
 */
void LinGP::Instruction::registerParams(System& ioSystem)
{ }


/*!
 *  \brief Compare equality of two instructions. Name must be the same.
 *  \param inRightObj Instruction to compare to the actual one.
 *  \return True if instruction name is identical, false if not.
 */
bool LinGP::Instruction::isEqual(const Object& inRightObj) const
{
	const LinGP::Instruction& lRightInstruction = castObjectT<const LinGP::Instruction&>(inRightObj);
	return mName == lRightInstruction.mName;
}


/*!
 *  \brief Read the instruction.
 *  \param inIter XML node used to read the instruction from.
 *  \throw IOException If instruction format is not respected.
 */
void LinGP::Instruction::read(PACC::XML::ConstIterator inIter)
{
	if(inIter->getType() != PACC::XML::eData)
		throw Beagle_IOExceptionNodeM(*inIter, "tag expected!");

	if(inIter->getValue() != mName) {
		std::ostringstream lOSS;
		lOSS << "tag <" << mName << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
}


/*!
 *  \brief Write the instruction.
 *  \param ioStreamer XML stream to write the Instruction into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::Instruction::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	ioStreamer.openTag(mName, inIndent);
	ioStreamer.closeTag();
}

