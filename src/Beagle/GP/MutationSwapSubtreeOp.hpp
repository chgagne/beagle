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
 *  \file   beagle/GP/MutationSwapSubtreeOp.hpp
 *  \brief  Definition of the class GP::MutationSwapSubtreeOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Jianjun Hu <hujianju@msu.edu>
 *  \author Marc Parizeau <parizeau@gel.ulaval.ca>
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MutationSwapSubtreeOp_hpp
#define Beagle_GP_MutationSwapSubtreeOp_hpp

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

namespace Beagle
{
namespace GP
{

/*!
 *  \brief GP swap subtree mutation operator class.
 *
 *  There is two kind of swap subtree mutation operations: internal mutation and external mutation.
 *  Internal swap subtree mutation is done by using three mutation points, where the 2nd point is 
 *  within the 1st point's subtree, and the 3rd point is within the 2nd point's subtree. The 
 *  exchange is done by 1) exchanging the 2nd point with the 1st point, 2) exchanging the 1st 
 *  point with the 3rd point, and 3) exchanging the 3rd point with the 2nd point. The structure of the 
 *  tree, which is originally 1-2-3, is changed for 2-1-3. The external swap subtree mutation is 
 *  done by exchanging two subtrees of the same individual, where the two subtrees are not within 
 *  the other subtree.
 */
class MutationSwapSubtreeOp : public Beagle::MutationOp
{

public:

	//! GP::MutationSwapSubtreeOp allocator type.
	typedef AllocatorT<MutationSwapSubtreeOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GP::MutationSwapSubtreeOp handle type.
	typedef PointerT<MutationSwapSubtreeOp,Beagle::MutationOp::Handle>
	Handle;
	//! GP::MutationSwapSubtreeOp bag type.
	typedef ContainerT<MutationSwapSubtreeOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationSwapSubtreeOp(std::string inMutationPbName="gp.mutsst.indpb",
	                               std::string inDistribPbName="gp.mutsst.distrpb",
	                               std::string inName="GP-MutationSwapSubtreeOp");
	virtual ~MutationSwapSubtreeOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	void exchangeSubTrees(GP::Tree& ioTree1, unsigned int inNode1, GP::Context& ioContext1,
	                      GP::Tree& ioTree2, unsigned int inNode2, GP::Context& ioContext2);

	Float::Handle mDistributionProba;  //!< Distribution probability of subtree swap.
	UInt::Handle  mMaxTreeDepth;       //!< Maximum allowed depth of trees.
	UInt::Handle  mNumberAttempts;     //!< Number of attempts in each initialization.
	std::string   mDistribPbName;      //!< Swap subtree mutation distribution prob. parameter name.

};

}
}

#endif // Beagle_GP_MutationSwapSubtreeOp_hpp
