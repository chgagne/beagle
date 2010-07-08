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
 *  \file   beagle/Core/ReplacementStrategyOp.hpp
 *  \brief  Definition of the class ReplacementStrategyOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_ReplacementStrategyOp_hpp
#define Beagle_Core_ReplacementStrategyOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/Allocator.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/AbstractAllocT.hpp"
#include "beagle/Core/Container.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Operator.hpp"
#include "beagle/Core/BreederNode.hpp"
#include "beagle/Core/BreederOp.hpp"

namespace Beagle
{


/*!
 *  \class ReplacementStrategyOp beagle/Core/ReplacementStrategyOp.hpp "beagle/Core/ReplacementStrategyOp.hpp"
 *  \brief Replacement strategy operator abstract class.
 *  \ingroup Core
 *  \ingroup Op
 *
 *  The ReplacementStrategyOp class is the entry point the a breeding tree, and have a
 *  breeder root node. Each breeder are hierachically organized into a tree. A breeder receive a
 *  breeding pool to process, and must return from it one bred individual.
 */
class ReplacementStrategyOp : public Operator
{

public:

	//! ReplacementStrategyOp allocator type.
	typedef AbstractAllocT<ReplacementStrategyOp,Operator::Alloc> Alloc;
	//! ReplacementStrategyOp handle type.
	typedef PointerT<ReplacementStrategyOp,Operator::Handle> Handle;
	//! ReplacementStrategyOp bag type.
	typedef ContainerT<ReplacementStrategyOp,Operator::Bag> Bag;

	ReplacementStrategyOp(std::string inName="Unnamed");
	virtual ~ReplacementStrategyOp()
	{ }

	/*!
	 *  \brief Apply the operation on a deme in the given context.
	 *  \param ioDeme Reference to the deme on which the operation takes place.
	 *  \param ioContext Evolutionary context of the operation.
	 */
	virtual void operate(Deme& ioDeme, Context& ioContext) = 0;

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Breeder root node.
	 */
	inline BreederNode::Handle getRootNode() const
	{
		Beagle_StackTraceBeginM();
		return mBreederRoot;
		Beagle_StackTraceEndM("BreederNode::Handle ReplacementStrategyOp::getRootNode() const");
	}

	/*!
	 *  \brief Set breeder root node.
	 *  \param inRootNode Breeder root node.
	 */
	inline void setRootNode(BreederNode::Handle inRootNode)
	{
		Beagle_StackTraceBeginM();
		mBreederRoot = inRootNode;
		Beagle_StackTraceEndM("void ReplacementStrategyOp::setRootNode(BreederNode::Handle inRootNode)");
	}

protected:

	void buildRoulette(RouletteT<unsigned int>& outRoulette, Context& ioContext) const;

private:

	BreederNode::Handle mBreederRoot;    //!< Breeder root node.

};

}

#endif // Beagle_Core_ReplacementStrategyOp_hpp
