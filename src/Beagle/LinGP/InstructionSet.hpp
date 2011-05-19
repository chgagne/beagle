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
 *  \file   beagle/LinGP/InstructionSet.hpp
 *  \brief  Definition of the type LinGP::InstructionSet.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_InstructionSet_hpp
#define Beagle_InstructionSet_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/System.hpp"
#include "beagle/LinGP/Instruction.hpp"


namespace Beagle
{
namespace LinGP
{

/*!
 *  \class InstructionSet beagle/LinGP/InstructionSet.hpp "beagle/LinGP/InstructionSet.hpp"
 *  \brief The linear GP instruction set class.
 *  \ingroup LinGPF
 *  \ingroup Instructions
 *  \ingroup LinGPSys
 */
class InstructionSet : public Instruction::Bag
{

public:

	//! LinGP::InstructionSet allocator type.
	typedef AllocatorT<InstructionSet,Instruction::Bag::Alloc> Alloc;
	//! LinGP::InstructionSet handle type.
	typedef PointerT<InstructionSet,Instruction::Bag::Handle> Handle;
	//! LinGP::InstructionSet mixed bag type.
	typedef ContainerT<InstructionSet,Instruction::Bag::Bag> Bag;

	explicit InstructionSet()
	{ }
	virtual ~InstructionSet()
	{ }

	Instruction::Handle          getInstructionByName(std::string inName) const;
	virtual void                 registerParams(System& ioSystem);
	virtual void                 insert(Instruction::Handle inInstruction);
	virtual Instruction::Handle  selectRandomInstruction(System& ioSystem) const;
	virtual void                 read(PACC::XML::ConstIterator inIter);
	virtual void                 write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Map mNameLUT;      //!< Look-up table (by name) of the instructions.

};

}
}

#endif // Beagle_InstructionSet_hpp

