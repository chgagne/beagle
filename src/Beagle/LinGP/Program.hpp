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
 *  \file   beagle/LinGP/Program.hpp
 *  \brief  Definition of the type LinGP::Program.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.3 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_Program_hpp
#define Beagle_LinGP_Program_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/LinGP/Instruction.hpp"


namespace Beagle
{
namespace LinGP
{


/*!
 *  \class Program beagle/LinGP/Program.hpp "beagle/LinGP/Program.hpp"
 *  \brief The linear GP program class, the genotype used in linear GP.
 *  \ingroup LinGPF
 *  \ingroup LinGPPop
 */
class Program : public Genotype, public std::vector<Instruction::Handle>
{

public:

	//! LinGP::Program allocator type.
	typedef AllocatorT<Program,Genotype::Alloc> Alloc;
	//! LinGP::Program handle type.
	typedef PointerT<Program,Genotype::Handle> Handle;
	//! LinGP::Program bag type.
	typedef ContainerT<Program,Genotype::Bag> Bag;

	explicit Program(unsigned int inSize=0);

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getType() const;
	void                        interpret(LinGP::Context& ioContext);
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}
}

#endif // Beagle_LinGP_Program_hpp
