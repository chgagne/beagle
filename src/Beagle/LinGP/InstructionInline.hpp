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
 *  \file   beagle/LinGP/PrimitiveInline.hpp
 *  \brief  Implementation of some inlined methods of LinGP::Instruction.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/01 19:49:30 $
 */

#ifndef Beagle_LinGP_InstructionInline_hpp
#define Beagle_LinGP_InstructionInline_hpp

#include "beagle/LinGP/Program.hpp"
#include "beagle/LinGP/Context.hpp"
#include "beagle/LinGP/Instruction.hpp"


/*!
 *  \brief Execute next instruction.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If Argument index is invalid.
 */
inline void Beagle::LinGP::Instruction::execNextInstruction(LinGP::Context& ioContext)
{
	if((ioContext.getExecIndex()+1) >= ioContext.getGenotype().size()) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "primitive", "Beagle::LinGP::Primitive",
		    "End of program reached"
		);
		return;
	}
	if((ioContext.getExecLimit() != 0) &&
	        (ioContext.getExecCount() >= ioContext.getExecLimit())) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "primitive", "Beagle::LinGP::Primitive",
		    "Maximum number of instruction executions reached"
		);
		return;
	}

	unsigned int lActualExecIndex = ioContext.getExecIndex();
	ioContext.setExecIndex(lActualExecIndex+1);
	ioContext.setExecCount(ioContext.getExecCount()+1);
	ioContext.getGenotype()[lActualExecIndex+1]->execute(ioContext);
	ioContext.setExecIndex(lActualExecIndex);
}


/*!
 *  \brief Execute instruction specified by index.
 *  \param inIndex Execute instruction at specified index.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException If Argument index is invalid.
 */
inline void Beagle::LinGP::Instruction::execInstruction(unsigned int inIndex,
        LinGP::Context& ioContext)
{
	if(inIndex >= ioContext.getGenotype().size()) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "primitive", "Beagle::LinGP::Primitive",
		    "End of program reached"
		);
		return;
	}
	if((ioContext.getExecLimit() != 0) &&
	        (ioContext.getExecCount() >= ioContext.getExecLimit())) {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "primitive", "Beagle::LinGP::Primitive",
		    "Maximum number of instruction executions reached"
		);
		return;
	}

	unsigned int lActualExecIndex = ioContext.getExecIndex();
	ioContext.setExecIndex(inIndex);
	ioContext.setExecCount(ioContext.getExecCount()+1);
	ioContext.getGenotype()[inIndex]->execute(ioContext);
	ioContext.setExecIndex(lActualExecIndex);
}


#endif // Beagle_LinGP_InstructionInline_hpp

