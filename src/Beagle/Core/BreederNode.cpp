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
 *  \file   Beagle/Core/BreederNode.cpp
 *  \brief  Source code of class BreederNode.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct breeder node.
 *  \param inBreederOp Breeder operator associated to node.
 *  \param inFirstChild First breeder node child to actual node.
 *  \param inNextSibling Next sibling breeder node to actual node.
 */
BreederNode::BreederNode(PointerT<BreederOp,Operator::Handle> inBreederOp,
                         PointerT<BreederNode,Object::Handle> inFirstChild,
                         PointerT<BreederNode,Object::Handle> inNextSibling) :
	mBreederOp(inBreederOp),
	mChild(inFirstChild),
	mSibling(inNextSibling)
{ }


/*!
 *  \brief Register the parameters if this breeder node.
 *  \param ioSystem Evolutionary system to use to initialize the system.
 */
void BreederNode::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if(mBreederOp != NULL) {
		Beagle_LogTraceM(
		    ioSystem.getLogger(),
		    std::string("Initializing operator '") + mBreederOp->getName() + "'"
		);
		mBreederOp->registerParams(ioSystem);
	}
	if(mChild != NULL) mChild->registerParams(ioSystem);
	if(mSibling != NULL) mSibling->registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this breeder node.
 *  \param ioSystem Evolutionary system to use to post-init the system.
 */
void BreederNode::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if(mBreederOp != NULL) {
		Beagle_LogTraceM(
		    ioSystem.getLogger(),
		    std::string("Calling post-initialization hook of operator '") +
		    mBreederOp->getName() + "'"
		);
		mBreederOp->init(ioSystem);
	}
	if(mChild != NULL) mChild->init(ioSystem);
	if(mSibling != NULL) mSibling->init(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Reading a breeder node with method read is undefined. Use readWithSystem instead.
 */
void BreederNode::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read","BreederNode",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a breeder tree for XML tree.
 *  \param inIter XML iterator to use to read breeder node.
 *  \param ioSystem Evolutionary system.
 */
void BreederNode::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	setBreederOp(NULL);
	setFirstChild(NULL);
	setNextSibling(NULL);

	if(inIter->getType() == PACC::XML::eData) {
		std::string lOpName = inIter->getValue();
		Operator::Alloc::Handle lOpAlloc =
		    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
		if(lOpAlloc == NULL) {
			std::ostringstream lOSS;
			lOSS << "Operator '" << lOpName << "' is not a breeder operator!";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}
		try {
			mBreederOp = castHandleT<BreederOp>(lOpAlloc->allocate());
		} catch(BadCastException&) {
			std::ostringstream lOSS;
			lOSS << "Operator '" << lOpName << "' is not a breeder operator!";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}
		mBreederOp->setName(lOpName);
		mBreederOp->readWithSystem(inIter, ioSystem);
	}

	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			std::string lOpName = lChild->getValue();
			Operator::Alloc::Handle lOpAlloc =
			    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
			if(lOpAlloc == NULL) continue;

			PointerT<BreederOp,Operator::Handle> lOp;
			try {
				lOp = castHandleT<BreederOp>(lOpAlloc->allocate());
			} catch(BadCastException&) {
				std::ostringstream lOSS;
				lOSS << "Operator '" << lOpName << "' is not a breeder operator!";
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			lOp->setName(lOpName);
			if(getFirstChild() == NULL) {
				setFirstChild(new BreederNode(lOp));
				getFirstChild()->readWithSystem(lChild, ioSystem);
			} else {
				PointerT<BreederNode,Object::Handle> lBreederChild;
				for(lBreederChild=getFirstChild(); lBreederChild->getNextSibling()!=NULL;
				        lBreederChild=lBreederChild->getNextSibling());
				lBreederChild->setNextSibling(new BreederNode(lOp));
				lBreederChild->getNextSibling()->readWithSystem(lChild, ioSystem);
			}
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write breeder operator into XML streamer.
 *  \param ioStreamer XML streamer to write breeder operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void BreederNode::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if(mBreederOp != NULL) {
		ioStreamer.openTag(mBreederOp->getName(), inIndent);
		mBreederOp->writeContent(ioStreamer, inIndent);
	}
	for(BreederNode::Handle lChild=getFirstChild(); lChild!=NULL;
	        lChild=lChild->getNextSibling()) {
		lChild->write(ioStreamer, inIndent);
	}
	if(mBreederOp != NULL) ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}

