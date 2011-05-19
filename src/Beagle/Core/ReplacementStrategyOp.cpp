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
 *  \file   Beagle/Core/ReplacementStrategyOp.cpp
 *  \brief  Implementation of class ReplacementStrategyOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/Core.hpp"

#include <cmath>

using namespace Beagle;


/*!
 *  \brief Build replacement strategy operator.
 *  \param inName Name of the replacement strategy operator.
 */
ReplacementStrategyOp::ReplacementStrategyOp(std::string inName) :
	Operator(inName)
{ }


/*!
 *  \brief Build routing tables for breeding children.
 *  \param outRoulette Normalized probability tables.
 *  \param ioContext Evolutionary context used.
 */
void ReplacementStrategyOp::buildRoulette(RouletteT<unsigned int>& outRoulette,
        Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(getRootNode());

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "Building routing tables of the replacement strategy"
	);

	outRoulette.clear();
	unsigned int i=0;
	for(BreederNode::Handle lChild=getRootNode(); lChild!=NULL; lChild=lChild->getNextSibling()) {
		outRoulette.insert(i++, lChild->getBreederOp()->getBreedingProba(lChild->getFirstChild()));
	}

	if(std::fabs(1.0 - outRoulette.back().first) > 0.0001) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    std::string("Sum of probabilities of breeder operators children to ") +
		    std::string("replacement strategy named '") + getName() +
		    std::string("' is different from 1.0 (value: ") + dbl2str(outRoulette.back().first) +
		    std::string(")")
		);
	}

	outRoulette.optimize();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Regsiter the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void ReplacementStrategyOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if(getRootNode() != NULL) getRootNode()->registerParams(ioSystem);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void ReplacementStrategyOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if(getRootNode() != NULL) getRootNode()->init(ioSystem);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a replacement strategy operator for XML subtree.
 *  \param inIter XML iterator to use to read replacement strategy operator.
 *  \param ioSystem Evolutionary system.
 */
void ReplacementStrategyOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	setRootNode(NULL);
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			std::string lOpName = lChild->getValue();
			Operator::Alloc::Handle lOpAlloc =
			    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
			if(lOpAlloc == NULL) continue;

			BreederOp::Handle lOp;
			try {
				lOp = castHandleT<BreederOp>(lOpAlloc->allocate());
			} catch(BadCastException&) {
				std::ostringstream lOSS;
				lOSS << "Operator '" << lChild->getValue() << "' is not a breeder operator!";
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			lOp->setName(lOpName);

			if(getRootNode() == NULL) {
				setRootNode(new BreederNode(lOp));
				getRootNode()->readWithSystem(lChild, ioSystem);
			} else {
				BreederNode::Handle lBreederChild;
				for(lBreederChild=getRootNode(); lBreederChild->getNextSibling()!=NULL;
				        lBreederChild=lBreederChild->getNextSibling());
				lBreederChild->setNextSibling(new BreederNode(lOp));
				lBreederChild->getNextSibling()->readWithSystem(lChild, ioSystem);
			}
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write replacement strategy operator into XML streamer.
 *  \param ioStreamer XML streamer to write replacement strategy operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void ReplacementStrategyOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag(getName(), inIndent);
	for(BreederNode::Handle lNode=getRootNode(); lNode!=NULL; lNode=lNode->getNextSibling()) {
		lNode->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();

	Beagle_StackTraceEndM();
}

