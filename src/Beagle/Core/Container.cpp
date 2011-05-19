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
 *  \file   Beagle/Core/Container.cpp
 *  \brief  Source code of class Container.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Container constructor.
 *  \param inN Initial number of elements.
 *  \param inModel Model of initial elements.
 */
Container::Container(unsigned int inN, Object::Handle inModel) :
	std::vector<Object::Handle>(inN,inModel)
{ }


/*!
 *  \brief Compare equality of two containers.
 *  \param inRightObj Container to compare with *this.
 *  \return True if containers are equals, false otherwise.
 *  \throw BadCastException If compared containers are not of the same type.
 *  \par Note:
 *    Returns false if the container does not have the same size, returns true if the
 *    range [ 0, size()) are identical when compared element-by-element, and otherwise returns false.
 */
bool Container::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Container& lRightContainer = castObjectT<const Container&>(inRightObj);
	if(size() != lRightContainer.size()) return false;
	Container::const_iterator lFirstIter1 = begin();
	Container::const_iterator lLastIter1  = begin() + size();
	Container::const_iterator lFirstIter2 = lRightContainer.begin();
	return std::equal(lFirstIter1, lLastIter1, lFirstIter2, IsEqualPointerPredicate());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Evaluate lexicographical order of two containers.
 *  \param inRightObj Container to compare with *this.
 *  \return True if left container is less that the right one, false otherwise.
 *  \throw BadCastException If compared containers are not of the same type.
 *  \par Note:
 *    Returns true if the actual object is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool Container::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Container& lRightContainer = castObjectT<const Container&>(inRightObj);
	unsigned int lSizeCompared = minOf<unsigned int>(size(), lRightContainer.size());
	Container::const_iterator lFirstIter1 = begin();
	Container::const_iterator lLastIter1  = begin() + lSizeCompared;
	Container::const_iterator lFirstIter2 = lRightContainer.begin();
	Container::const_iterator lLastIter2  = lRightContainer.begin() + lSizeCompared;
	return std::lexicographical_compare(lFirstIter1,
	                                    lLastIter1,
	                                    lFirstIter2,
	                                    lLastIter2,
	                                    IsLessPointerPredicate());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read container from XML iterator.
 *  \param inIter XML iterator to read container from.
 *  \throw Beagle::IOException If any read error occur.
 */
void Container::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Bag"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Bag> expected!");
	PACC::XML::ConstIterator lChild=inIter->getFirstChild();
	unsigned int lSize;
	for(lSize=0; lChild; ++lSize) ++lChild;
	if(lSize!=size()) {
		std::ostringstream lOSS;
		lOSS << "number of elements found in XML (" << lSize;
		lOSS << ") mismatch real size of container (" << size() << ")!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	lChild = inIter->getFirstChild();
	for(unsigned int j=0; lChild; ++j) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="NullHandle")) {
			(*this)[j] = NULL;
		} else {
			Beagle_NonNullPointerAssertM((*this)[j]);
			(*this)[j]->read(lChild);
		}
		++lChild;
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write Container into XML streamer.
 *  \param ioStreamer XML streamer.
 *  \param inIndent Whether XML output should be indented.
 */
void Container::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Bag", inIndent);
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
