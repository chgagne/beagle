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
 *  \file   beagle/LinGP/Instruction.hpp
 *  \brief  Definition of the type LinGP::Instruction.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:22 $
 */

/*!
 *  \defgroup Instructions GP Instructions
 *  \ingroup LinGPF
 *  \brief Linear GP instructions mechanism and library, part of the linear GP framework.
 */

#ifndef Beagle_LinGP_Instruction_hpp
#define Beagle_LinGP_Instruction_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/System.hpp"


namespace Beagle
{
namespace LinGP
{

// Forward declaration.
class Context;

/*!
 *  \class Instruction beagle/LinGP/Instruction.hpp "beagle/LinGP/Instruction.hpp"
 *  \brief The linear GP instruction class.
 *  \ingroup LinGPF
 *  \ingroup Instructions
 */
class Instruction : public Object
{

public:

	//! LinGP::Instruction allocator type.
	typedef AbstractAllocT<Instruction,Object::Alloc> Alloc;
	//! LinGP::Instruction handle type.
	typedef PointerT<Instruction,Object::Handle> Handle;
	//! LinGP::Instruction bag type.
	typedef ContainerT<Instruction,Object::Bag> Bag;

	explicit Instruction(const char* inName="");
	explicit Instruction(std::string inName);
	virtual ~Instruction()
	{ }

	virtual Handle giveReference(LinGP::Context& ioContext);
	virtual bool   isEqual(const Object& inRightObj) const;
	virtual void   registerParams(System& ioSystem);
	virtual void   read(PACC::XML::ConstIterator inIter);
	virtual void   write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Execute the caracteristic instruction operation.
	 *  \param ioContext Evolutionary context.
	 */
	virtual void execute(LinGP::Context& ioContext) =0;

	/*!
	 *  \brief Get the name of the instruction.
	 *  \return Instruction name.
	 */
	inline const std::string& getName() const
	{
		return mName;
	}

protected:

	inline void execNextInstruction(LinGP::Context& ioContext);
	inline void execInstruction(unsigned int inIndex, LinGP::Context& ioContext);

	/*!
	 *  \brief Set the name of the instruction.
	 *  \param inName New instruction name.
	 */
	inline void setName(std::string inName)
	{
		mName = inName;
	}

private:
	std::string  mName;             //!< Name of the Instruction.

};

}
}

#endif // Beagle_LinGP_Instruction_hpp


