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
 *  \file   Beagle/Core/IndividualBag.cpp
 *  \brief  Implementation of the class IndividualBag.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.19 $
 *  $Date: 2008/02/29 14:12:43 $
 */

#include "Beagle/Core.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct individual bag.
 *  \param inN Size of the bag.
 *  \param inModel Initial value of the composing individual handles.
 */
IndividualBag::IndividualBag(unsigned int inN, Individual::Handle inModel) :
	ContainerT<Individual,ContainerT<Genotype,Member::Bag>::Bag>(inN, inModel)
{ }


/*!
 *  \brief Extract an individual bag from a XML subtree.
 *  \throw Beagle::InternalException You should use readWithContext instead.
 */
void IndividualBag::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read", "IndividualBag", getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read an individual bag from a XML file.
 *  \param inFileName Filename to read individual bag from.
 *  \param ioSystem Evolutionary system used to read individual.
 *  \param inBagTag Name of the XML tag containing the list of individuals to read.
 *  \return True if an individual of the given name was found and read from file.
 *    false if nothing was found.
 */
bool IndividualBag::readFromFile(std::string inFileName,
                                 System& ioSystem,
                                 std::string inBagTag)
{
	Beagle_StackTraceBeginM();
	std::ifstream lIFS(inFileName.c_str());
	PACC::XML::Document lParser(lIFS, inFileName);
	lIFS.close();
	PACC::XML::ConstFinder lBagFinder(lParser.getFirstDataTag());
	PACC::XML::ConstIterator lIndivTag = lBagFinder.find(std::string("//IndividualBag")+inBagTag);
	if(!lIndivTag) return false;

	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioSystem.getFactory().getConceptAllocator("Concept"));
	Context::Handle lContext =
	    castHandleT<Context>(lContextAlloc->allocate());
	lContext->setSystemHandle(&ioSystem);

	Beagle_LogInfoM(
	    ioSystem.getLogger(),
	    std::string("Reading an individual bag from file '")+inFileName+
	    std::string("' that is in-between the XML tags '")+inBagTag+std::string("'")
	);

	readIndividuals(lIndivTag, *lContext);

	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read an individual bag with a context.
 *  \param inIter XML iterator to read the bag from.
 *  \param ioContext Evolutionary context.
 */
void IndividualBag::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="IndividualBag"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <IndividualBag> expected!");
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			readIndividuals(lChild, ioContext);
			break;
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read individuals of a bag.
 *  \param inIter XML iterator to read the individuals from.
 *  \param ioContext Evolutionary context.
 */
void IndividualBag::readIndividuals(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	clear();
	Individual::Handle lPrevIndivHandle = ioContext.getIndividualHandle();
	unsigned int lPrevIndivIndex = ioContext.getIndividualIndex();

	for(PACC::XML::ConstIterator lIter=inIter; lIter; ++lIter) {
		if(lIter->getType() != PACC::XML::eData) continue;
		if(lIter->getValue() == "NullHandle") {
			push_back(NULL);
			continue;
		} else if(lIter->getValue() != "Individual") continue;

		Individual::Alloc::Handle lIndivAlloc = NULL;
		std::string lIndivTypeName = lIter->getAttribute("type");
		const Factory& lFactory = ioContext.getSystem().getFactory();
		if(lIndivTypeName.empty()) {
			lIndivAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
		} else {
			lIndivAlloc =
			    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndivTypeName));
		}
		Individual::Handle lReadIndiv = castHandleT<Individual>(lIndivAlloc->allocate());
		const unsigned int lBackIndex = size();
		push_back(lReadIndiv);
		ioContext.setIndividualHandle(lReadIndiv);
		ioContext.setIndividualIndex(lBackIndex);
		lReadIndiv->readWithContext(lIter, ioContext);
	}

	ioContext.setIndividualIndex(lPrevIndivIndex);
	ioContext.setIndividualHandle(lPrevIndivHandle);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write bag of individuals into XML streamer.
 *  \param ioStreamer XML streamer.
 *  \param inIndent Whether XML output should be indented.
 */
void IndividualBag::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("IndividualBag", inIndent);
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] == NULL) {
			ioStreamer.openTag("NullHandle");
			ioStreamer.closeTag();
		} else {
			(*this)[i]->write(ioStreamer, inIndent);
		}
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
