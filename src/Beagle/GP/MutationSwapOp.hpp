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
 *  \file   beagle/GP/MutationSwapOp.hpp
 *  \brief  Definition of the class GP::MutationSwapOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MutationSwapOp_hpp
#define Beagle_GP_MutationSwapOp_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/EC/MutationOp.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/Float.hpp"
#include "Beagle/Core/Bool.hpp"
#include "Beagle/GP/Context.hpp"
#include "Beagle/GP/Individual.hpp"

namespace Beagle
{
namespace GP
{

/*!
 *  \class MutationSwapOp beagle/GP/MutationSwapOp.hpp "beagle/GP/MutationSwapOp.hpp"
 *  \brief GP swap mutation operator class.
 *  
 *  This operator mutates the individuals that are subjected to it.
 *  The operator randomly chooses a primitive in the individual and
 *  replaces it with a randomly selected primitive that takes the same
 *  number of arguments.
 *
 *  For example, let's start with the tree 
 *  \code <ADD><X/><Y/></ADD> \endcode
 *  If node 1 is selected ("ADD"), then a primitive that
 *  takes two arguments would be found.  Say "SUB" was chosen, then
 *  the tree would become 
 *  \code <SUB><X/><Y/></SUB> \endcode
 *
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationSwapOp : public Beagle::EC::MutationOp
{

public:

	//! GP::MutationSwapOp allocator type.
	typedef AllocatorT<MutationSwapOp,Beagle::EC::MutationOp::Alloc>
	Alloc;
	//! GP::MutationSwapOp handle type.
	typedef PointerT<MutationSwapOp,Beagle::EC::MutationOp::Handle>
	Handle;
	//! GP::MutationSwapOp bag type.
	typedef ContainerT<MutationSwapOp,Beagle::EC::MutationOp::Bag>
	Bag;

	explicit MutationSwapOp(std::string inMutationPbName="gp.mutswap.indpb",
	                        std::string inDistribPbName="gp.mutswap.distrpb",
	                        std::string inName="GP-MutationSwapOp");
	virtual ~MutationSwapOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Float::Handle mDistributionProba;  //!< Swap mutation distribution probability.
	std::string   mDistribPbName;      //!< Swap mutation distribution prob. parameter name.

};

}
}

#endif // Beagle_GP_MutationSwapOp_hpp
