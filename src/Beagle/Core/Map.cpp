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
 *  \file   Beagle/Core/Map.cpp
 *  \brief  Source code of class Map.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/09/21 17:45:46 $
 */

#include "Beagle/Core.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Compare equality of two maps.
 *  \param inRightObj Map to compare to this.
 *  \return True if maps are equals, false if not.
 *  \throw Beagle::BadCastException If compared maps Objects are not of the same type.
 *  \par Note:
 *    Returns true if the size of maps is equal and if the maps
 *    are identical when compared element-by-element, and otherwise returns false.
 */
bool Map::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Map& lRightMap = castObjectT<const Map&>(inRightObj);
	if(size() != lRightMap.size()) return false;
	return std::equal(begin(), end(), lRightMap.begin(), IsEqualMapPairPredicate());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Evaluate lexicographical order of two maps.
 *  \param inRightObj Map to compare to this.
 *  \return True if left map is less that the right one, false if not.
 *  \throw Beagle::BadCastException If compared map Objects are not of the same type.
 *  \par Note:
 *    Returns true if the actual object is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool Map::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const Map& lRightMap = castObjectT<const Map&>(inRightObj);
#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP) | defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	typedef std::map< std::string, Object::Handle, std::less<std::string> > RBTreeMap;
	RBTreeMap lLeftRBTMap, lRightRBTMap;
	for(const_iterator lIter = begin(); lIter != end(); ++lIter) {
		lLeftRBTMap[lIter->first] = lIter->second;
	}
	for(const_iterator lIter = lRightMap.begin(); lIter != lRightMap.end(); ++lIter) {
		lRightRBTMap[lIter->first] = lIter->second;
	}
	return std::lexicographical_compare(lLeftRBTMap.begin(),
	                                    lLeftRBTMap.end(),
	                                    lRightRBTMap.begin(),
	                                    lRightRBTMap.end(),
	                                    IsLessMapPairPredicate());
#else // defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP) | defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	return std::lexicographical_compare(begin(),
	                                    end(),
	                                    lRightMap.begin(),
	                                    lRightMap.end(),
	                                    IsLessMapPairPredicate());
#endif // defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP) | defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a map from a XML node.
 *  \param inIter XML iterator to read the Map from.
 *  \throw Beagle::IOException If reading from the stream problems occured.
 */
void Map::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Map"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Map> expected!");

	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Entry")) {
			const std::string& lKey = lChild->getAttribute("key");
			if(lKey.empty())
				throw Beagle_IOExceptionNodeM(*lChild, "no key attribute for actual entry!");
			if(find(lKey) == end()) {
				std::ostringstream lOSS;
				lOSS << "entry '" << lKey << "' doesn't exist in current map!" << std::flush;
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
			(*this)[lKey]->read(lChild2);
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a map into a XML streamer.
 *  \param ioStreamer XML streamer to read the Map to.
 *  \param inIndent Whether XML output should be indented.
 */
void Map::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Map", inIndent);
#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP) | defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	typedef std::map< std::string, Object::Handle, std::less<std::string> > RBTreeMap;
	RBTreeMap lRBTMap;
	for(const_iterator lIter = begin(); lIter != end(); ++lIter) {
		lRBTMap[lIter->first] = lIter->second;
	}
	for(RBTreeMap::const_iterator lIter = lRBTMap.begin(); lIter != lRBTMap.end(); ++lIter) {
		ioStreamer.openTag("Entry", inIndent);
		ioStreamer.insertAttribute("key", lIter->first);
		lIter->second->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
#else // defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP) | defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	for(const_iterator lIter = begin(); lIter != end(); ++lIter) {
		ioStreamer.openTag("Entry", inIndent);
		ioStreamer.insertAttribute("key", lIter->first);
		lIter->second->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
#endif // defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP) | defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}

