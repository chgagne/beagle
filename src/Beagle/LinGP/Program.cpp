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
 *  \file   beagle/LinGP/src/Program.cpp
 *  \brief  Implementation of the type LinGP::Program.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#include <algorithm>

#include "beagle/LinGP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a linear GP program of the size given.
 *  \param inSize Size of the Program.
 */
LinGP::Program::Program(unsigned int inSize) :
		std::vector<Instruction::Handle>(inSize)
{ }


/*!
 *  \brief Copy linear GP program into current.
 *  \param inOriginal Program to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void LinGP::Program::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const LinGP::Program& lOriginal = castObjectT<const LinGP::Program&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the genotype.
 *  \return Exact type of the genotype.
 */
const std::string& LinGP::Program::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("LinGP-Program");
	return lName;
	Beagle_StackTraceEndM("const std::string& LinGP::Program::getType() const")
}


/*!
 *  \brief Interpret the linear GP program.
 *  \param ioContext GP evolutionary context.
 *  \throw Beagle::ObjectException When Program is empty or not in contextual individual.
 *  \throw Beagle::AssertException When the contextual individual is a NULL pointer.
 */
void LinGP::Program::interpret(LinGP::Context& ioContext)
{
	if(empty()) throw Beagle_ObjectExceptionM("Could not interpret, program is empty!");
	Individual::Handle lIndiv = ioContext.getIndividualHandle();
	Beagle_NonNullPointerAssertM(lIndiv.getPointer());
	unsigned int lProgramIndex = 0;
	for(; lProgramIndex < lIndiv->size(); lProgramIndex++) {
		if(this == (*lIndiv)[lProgramIndex].getPointer()) break;
	}
	if(lProgramIndex == lIndiv->size())
		throw Beagle_ObjectExceptionM("Interpreted Program is not in the actual individual of the context!");

	Program::Handle lOldProgramHandle = ioContext.getGenotypeHandle();
	unsigned int lOldProgramIndex  = ioContext.getGenotypeIndex();
	ioContext.setGenotypeIndex(lProgramIndex);
	ioContext.setGenotypeHandle(Handle(this));
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "program", "Beagle::LinGP::Program",
	    std::string("Interpreting the ")+uint2ordinal(lProgramIndex+1)+
	    std::string(" linear program of the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
	    std::string(" individual")
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "program", "Beagle::LinGP::Program",
	    ioContext.getIndividual()
	);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "program", "Beagle::LinGP::Program",
	    std::string("Executing the linear program first instruction '")+
	    (*this)[0]->getName()+"'"
	);
	(*this)[0]->execute(ioContext);
	ioContext.setGenotypeIndex(lOldProgramIndex);
	ioContext.setGenotypeHandle(lOldProgramHandle);
}


/*!
 *  \brief Compare the equality of two linear GP programs.
 *  \param inRightObj Right program to be compare to tha actual one.
 *  \return True if the programs are identical, false if not.
 */
bool LinGP::Program::isEqual(const Object& inRightObj) const
{
	const LinGP::Program& lRightProgram = castObjectT<const LinGP::Program&>(inRightObj);
	if(size() != lRightProgram.size()) return false;
	return std::equal(begin(), end(), lRightProgram.begin());
}


/*!
 *  \brief Read a linear GP program from an XML node.
 *  \param inIter XML node to use to read the program from.
 *  \param ioContext Context to use to read the program.
 */
void LinGP::Program::readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext)
{
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Genotype")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");
	}
	std::string lGenotypeType = inIter->getAttribute("type");
	if((lGenotypeType.empty() == false) && (lGenotypeType != getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lGenotypeType << "' mismatch type of the genotype '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	LinGP::Context& lLinGPContext = castObjectT<LinGP::Context&>(ioContext);
	LinGP::InstructionSuperSet::Handle lInsSS =
	    castHandleT<LinGP::InstructionSuperSet>(lLinGPContext.getSystem().getComponent("LinGP-InstructionSuperSet"));
	LinGP::InstructionSet::Handle lInstructionSet = lInsSS->getInstructionSets()[ioContext.getGenotypeIndex()];

	clear();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() != PACC::XML::eData)
			throw Beagle_IOExceptionNodeM(*lChild, "tag expected!");

		LinGP::Instruction::Handle lInstruction =
		    lInstructionSet->getInstructionByName(lChild->getValue());
		if(lInstruction == NULL) {
			std::ostringstream lOSS;
			lOSS << "no instruction named '" <<  lChild->getValue();
			lOSS << "' found in the instruction set";
			throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
		}
		push_back(lInstruction->giveReference(lLinGPContext));
		back()->read(lChild);
	}
}


/*!
 *  \brief Write a GP Program into a XML streamer.
 *  \param ioStreamer XML streamer to write the program into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::Program::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	ioStreamer.insertAttribute("size", uint2str(size()));
	for(unsigned int i=0; i<size(); ++i) (*this)[i]->write(ioStreamer, inIndent);
}



