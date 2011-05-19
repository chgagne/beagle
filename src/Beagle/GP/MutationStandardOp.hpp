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
 *  \file   beagle/GP/MutationStandardOp.hpp
 *  \brief  Definition of the class GP::MutationStandardOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MutationStandardOp_hpp
#define Beagle_GP_MutationStandardOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/InitializationOp.hpp"
#include "beagle/GP/InitHalfOp.hpp"

namespace Beagle
{
namespace GP
{

/*!
 *  \class MutationStandardOp beagle/GP/MutationStandardOp.hpp "beagle/GP/MutationStandardOp.hpp"
 *  \brief GP standard mutation operator class, as described in Genetic Programming I.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationStandardOp : public Beagle::MutationOp
{

public:

	//! GP::MutationStandardOp allocator type.
	typedef AllocatorT<MutationStandardOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GP::MutationStandardOp handle type.
	typedef PointerT<MutationStandardOp,Beagle::MutationOp::Handle>
	Handle;
	//! GP::MutationStandardOp bag type.
	typedef ContainerT<MutationStandardOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationStandardOp(GP::InitializationOp::Handle inInitOp=new Beagle::GP::InitGrowOp,
	                            std::string inMutationPbName="gp.mutstd.indpb",
	                            std::string inMaxRegenDepthName="gp.mutstd.maxdepth",
	                            std::string inName="GP-MutationStandardOp");
	virtual ~MutationStandardOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
	virtual void init(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	GP::InitializationOp::Handle mInitOp;                //!< Initialization operator used to mutate
	UInt::Handle                 mMaxRegenerationDepth;  //!< Maximum depth of regeneration.
	UInt::Handle                 mMaxTreeDepth;          //!< Maximum allowed depth of trees.
	std::string                  mMaxRegenDepthName;     //!< Maximum depth of regeneration parameter name.

};

}
}

#endif // Beagle_GP_MutationStandardOp_hpp
