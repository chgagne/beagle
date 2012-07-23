/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/GA/MutationShuffleIntVecOp.hpp
 *  \brief  Definition of the class GA::MutationShuffleIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationShuffleIntVecOp_hpp
#define Beagle_GA_MutationShuffleIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationShuffleIntVecOp beagle/GA/MutationShuffleIntVecOp.hpp "beagle/GA/MutationShuffleIntVecOp.hpp"
 *  \brief Integer vector GA shuffle mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class MutationShuffleIntVecOp : public Beagle::MutationOp
{

public:

	//! GA::MutationShuffleIntVecOp allocator type.
	typedef AllocatorT<MutationShuffleIntVecOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GA::MutationShuffleIntVecOp handle type.
	typedef PointerT<MutationShuffleIntVecOp,Beagle::MutationOp::Handle>
	Handle;
	//! GA::MutationShuffleIntVecOp bag type.
	typedef ContainerT<MutationShuffleIntVecOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationShuffleIntVecOp(std::string inMutationPbName="ga.mutshuf.indpb",
	                                 std::string inIntMutatePbName="ga.mutshuf.intpb",
	                                 std::string inName="GA-MutationShuffleIntVecOp");
	virtual ~MutationShuffleIntVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Float::Handle mIntMutateProba;   //!< Single integer mutation probability.
	std::string   mIntMutatePbName;  //!< Single integer mutation probability parameter name.

};

}
}

#endif // Beagle_GA_MutationShuffleIntVecOp_hpp
