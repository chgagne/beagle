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
 *  \file   beagle/GP/CrossoverOp.hpp
 *  \brief  Definition of the class GP::CrossoverOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_CrossoverOp_hpp
#define Beagle_GP_CrossoverOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Tree.hpp"
#include "beagle/GP/Individual.hpp"

namespace Beagle
{
namespace GP
{

/*!
 *  \class CrossoverOp beagle/GP/CrossoverOp.hpp "beagle/GP/CrossoverOp.hpp"
 *  \brief GP crossover operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class CrossoverOp : public Beagle::CrossoverOp
{

public:

	//! GP::CrossoverOp allocator type.
	typedef AllocatorT<CrossoverOp,Beagle::CrossoverOp::Alloc>
	Alloc;
	//! GP::CrossoverOp handle type.
	typedef PointerT<CrossoverOp,Beagle::CrossoverOp::Handle>
	Handle;
	//! GP::CrossoverOp bag type.
	typedef ContainerT<CrossoverOp,Beagle::CrossoverOp::Bag>
	Bag;

	explicit CrossoverOp(std::string inMatingPbName="gp.cx.indpb",
	                     std::string inDistribPbName="gp.cx.distrpb",
	                     std::string inName="GP-CrossoverOp");
	virtual ~CrossoverOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
	                  Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
	void mateTrees(GP::Tree& ioTree1, unsigned int inNode1, GP::Context& ioContext1,
	               GP::Tree& ioTree2, unsigned int inNode2, GP::Context& ioContext2);

	Float::Handle mDistributionProba;  //!< Distribution probability of crossover.
	UInt::Handle  mMaxTreeDepth;       //!< Maximum allowed depth of trees.
	UInt::Handle  mNumberAttempts;     //!< Number of attempts to mate two individuals.
	std::string   mDistribPbName;      //!< Distribution probability parameter name.

};

}
}

#endif // Beagle_GP_CrossoverOp_hpp
