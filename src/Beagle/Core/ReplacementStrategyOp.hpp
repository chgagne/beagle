/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/Core/ReplacementStrategyOp.hpp
 *  \brief  Definition of the class ReplacementStrategyOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_ReplacementStrategyOp_hpp
#define Beagle_Core_ReplacementStrategyOp_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/AbstractAllocT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Operator.hpp"
#include "Beagle/Core/BreederNode.hpp"
#include "Beagle/Core/BreederOp.hpp"

namespace Beagle
{


/*!
 *  \class ReplacementStrategyOp Beagle/Core/ReplacementStrategyOp.hpp "Beagle/Core/ReplacementStrategyOp.hpp"
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
	inline BreederNode::Handle getRootNode() const {
		Beagle_StackTraceBeginM();
		return mBreederRoot;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set breeder root node.
	 *  \param inRootNode Breeder root node.
	 */
	inline void setRootNode(BreederNode::Handle inRootNode) {
		Beagle_StackTraceBeginM();
		mBreederRoot = inRootNode;
		Beagle_StackTraceEndM();
	}

protected:

	void buildRoulette(RouletteT<unsigned int>& outRoulette, Context& ioContext) const;

private:

	BreederNode::Handle mBreederRoot;    //!< Breeder root node.

};

}

#endif // Beagle_Core_ReplacementStrategyOp_hpp
