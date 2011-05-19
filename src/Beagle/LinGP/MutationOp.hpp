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
 *  \file   beagle/LinGP/MutationOp.hpp
 *  \brief  Definition of the class LinGP::MutationOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_MutationOp_hpp
#define Beagle_LinGP_MutationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle
{
namespace LinGP
{

/*!
 *  \class MutationOp beagle/LinGP/MutationOp.hpp "beagle/LinGP/MutationOp.hpp"
 *  \brief Linear GP mutation operator class.
 *  \ingroup LinGPF
 */
class MutationOp : public Beagle::MutationOp
{

public:

	//! LinGP::MutationOp allocator type.
	typedef AllocatorT<MutationOp,Beagle::MutationOp::Alloc> Alloc;
	//! LinGP::MutationOp handle type.
	typedef PointerT<MutationOp,Beagle::MutationOp::Handle> Handle;
	//! LinGP::MutationOp bag type.
	typedef ContainerT<MutationOp,Beagle::MutationOp::Bag> Bag;

	explicit MutationOp(std::string inMutationPbName="lingp.mut.indpb",
	                    std::string inInstructMutatePbName="lingp.mut.instructpb",
	                    std::string inName="LinGP-MutationOp");
	virtual ~MutationOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Float::Handle  mInstructMutateProba;   //!< Single instruction mutation probability.
	std::string    mInstructMutatePbName;  //!< Single instruction mutation probability parameter name.

};

}
}

#endif // Beagle_LinGP_MutationOp_hpp
