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
 *  \file   Beagle/Core/BreederNode.hpp
 *  \brief  Definition of the class BreederNode.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:11 $
 */


#ifndef Beagle_Core_BreederNode_hpp
#define Beagle_Core_BreederNode_hpp

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
#include "Beagle/Core/Individual.hpp"


namespace Beagle
{

// Forward declaration.
class BreederOp;

/*!
 *  \class BreederNode Beagle/Core/BreederNode.hpp "Beagle/Core/BreederNode.hpp"
 *  \brief Breeder node class, the skeleton of the breeder trees.
 *  \ingroup Core
 *  \ingroup Op
 */
class BreederNode : public Object
{

public:

	//! BreederNode allocator type.
	typedef AllocatorT<BreederNode,Object::Alloc>
	Alloc;
	//! BreederNode handle type.
	typedef PointerT<BreederNode,Object::Handle>
	Handle;
	//! BreederNode bag type.
	typedef ContainerT<BreederNode,Object::Bag>
	Bag;

	BreederNode(PointerT<BreederOp,Operator::Handle> inBreederOp=NULL,
	            PointerT<BreederNode,Object::Handle> inFirstChild=NULL,
	            PointerT<BreederNode,Object::Handle> inNextSibling=NULL);
	virtual ~BreederNode()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Breeder operator associated to node.
	 */
	inline PointerT<BreederOp,Operator::Handle> getBreederOp() const {
		Beagle_StackTraceBeginM();
		return mBreederOp;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return First child to the breeder.
	 */
	inline PointerT<BreederNode,Object::Handle> getFirstChild() const {
		Beagle_StackTraceBeginM();
		return mChild;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Next sibbling to the breeder.
	 */
	inline PointerT<BreederNode,Object::Handle> getNextSibling() const {
		Beagle_StackTraceBeginM();
		return mSibling;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set breeder operator associated to actual node.
	 *  \param inBreederOp New breeder operator associated to breeder node.
	 */
	inline void setBreederOp(PointerT<BreederOp,Operator::Handle> inBreederOp) {
		Beagle_StackTraceBeginM();
		mBreederOp = inBreederOp;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set first child of the breeder.
	 *  \param inChild New first child.
	 */
	inline void setFirstChild(PointerT<BreederNode,Object::Handle> inChild) {
		Beagle_StackTraceBeginM();
		mChild = inChild;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set next sibbling of the breeder.
	 *  \param inSibling New next sibling.
	 */
	inline void setNextSibling(PointerT<BreederNode,Object::Handle> inSibling) {
		Beagle_StackTraceBeginM();
		mSibling = inSibling;
		Beagle_StackTraceEndM();
	}

private:
	PointerT<BreederOp,Operator::Handle> mBreederOp;  //!< Breeder operator associated to the node.
	PointerT<BreederNode,Object::Handle> mChild;      //!< First breeder node child.
	PointerT<BreederNode,Object::Handle> mSibling;    //!< Next breeder node sibling.

public:
	virtual void read(PACC::XML::ConstIterator inIter);       // Undefined read method!

};

}

#endif // Beagle_Core_BreederNode_hpp

