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
 *  \file   beagle/LinGP/InstructionSuperSet.hpp
 *  \brief  Definition of the type LinGP::InstructionSuperSet.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:22 $
 */


#ifndef Beagle_LinGP_InstructionSuperSet_hpp
#define Beagle_LinGP_InstructionSuperSet_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/LinGP/InstructionSet.hpp"


namespace Beagle
{
namespace LinGP
{

/*!
 *  \class InstructionSuperSet beagle/LinGP/InstructionSuperSet.hpp
 *    "beagle/LinGP/InstructionSuperSet.hpp"
 *  \brief Linear GP instruction super set class.
 *  \ingroup LinGPF
 *  \ingroup Instructions
 *  \ingroup LinGPSys
 */
class InstructionSuperSet : public Component
{

public:

	//! LinGP::InstructionSuperSet allocator type.
	typedef AllocatorT<InstructionSuperSet,Component::Alloc> Alloc;
	//! LinGP::InstructionSuperSet handle type.
	typedef PointerT<InstructionSuperSet,Component::Handle> Handle;
	//! LinGP::InstructionSuperSet bag type.
	typedef ContainerT<InstructionSuperSet,Component::Bag> Bag;

	InstructionSuperSet()
	{ }
	virtual ~InstructionSuperSet()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void insert(InstructionSet::Handle inInstructionSet);
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Constant reference to the sets of instructions.
	 */
	inline const InstructionSet::Bag& getInstructionSets() const
	{
		Beagle_StackTraceBeginM();
		return mInstructionSets;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Reference to the sets of instructions.
	 */
	inline InstructionSet::Bag& getInstructionSets()
	{
		Beagle_StackTraceBeginM();
		return mInstructionSets;
		Beagle_StackTraceEndM();
	}

protected:

	InstructionSet::Bag mInstructionSets;  //!< Sets of instructions.

};

}
}

#endif // Beagle_LinGP_InstructionSuperSet_hpp
