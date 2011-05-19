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
 *  \file   Beagle/Core/Stats.cpp
 *  \brief  Source code of class Stats.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/10/09 15:16:11 $
 */

#include "Beagle/Core.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a new stats measure.
 *  \param inID Measure ID.
 *  \param inAvg Average of the measure.
 *  \param inStd Standard deviation of the measure.
 *  \param inMax Maximum measure.
 *  \param inMin Minimum measure.
 */
Measure::Measure(std::string inID, double inAvg, double inStd, double inMax, double inMin) :
	mID(inID),
	mAvg(inAvg),
	mStd(inStd),
	mMax(inMax),
	mMin(inMin)
{ }


/*!
 *  \brief Construct new stats.
 *  \param inID Stats ID.
 *  \param inGeneration Generation value.
 *  \param inPopSize Population size associated to the statistics.
 *  \param inValid If the stats are valid or not.
 */
Stats::Stats(std::string  inID,
             unsigned int inGeneration,
             unsigned int inPopSize,
             bool         inValid) :
	mID(inID),
	mGeneration(inGeneration),
	mPopSize(inPopSize),
	mValid(inValid)
{ }


/*!
 *  \brief Copy a stats into current one.
 *  \param inOriginal Stats to copy.
 *  \param ioSystem Evolutionary system.
 */
void Stats::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const Stats& lOriginal = castObjectT<const Stats&>(inOriginal);
	Stats::operator=(lOriginal);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get name of object.
 *  \return Object name, "Stats".
 */
const std::string& Stats::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("Stats");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the statistics.
 *  \return Exact type of the statistics.
 */
const std::string& Stats::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("Stats");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read stats from a XML subtree.
 *  \param inIter XML iterator to read the stats from.
 *  \param ioContext Evolutionary context to use to read the hall-of-fame.
 */
void Stats::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Stats"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Stats> expected!");

	// Be sure that the types are corresponding
	std::string lIndivType = inIter->getAttribute("type");
	if((lIndivType.empty()==false) && (lIndivType!=getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lIndivType << "' mismatch type of the statistics '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	std::string lValid = inIter->getAttribute("valid");
	clear();
	mItemMap.clear();
	if(lValid.empty() || (lValid == "yes")) {
		mValid = true;
		mID = inIter->getAttribute("id");

		std::string lGenerationStr = inIter->getAttribute("generation");
		if(lGenerationStr.empty() == false) mGeneration = str2uint(lGenerationStr);
		else mGeneration = 0;

		std::string lPopSizeStr = inIter->getAttribute("popsize");
		if(lPopSizeStr.empty() == false) mPopSize = str2uint(lPopSizeStr);
		else mPopSize = 0;

		unsigned int lSize=0;
		for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
			if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Measure")) ++lSize;
		}
		resize(lSize);
		unsigned int lIndexMeasure=0;
		for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
			if(lChild->getType() == PACC::XML::eData) {
				if(lChild->getValue() == "Item") {
					std::string lKey = lChild->getAttribute("key");
					if(lKey.empty()) {
						std::ostringstream lOSS;
						lOSS << "expected a key attribute while reading a statistics item!";
						throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
					}
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) {
						std::ostringstream lOSS;
						lOSS << "expected an item value while reading a statistics item!";
						throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
					}
					if(lChild2->getType() != PACC::XML::eString) {
						std::ostringstream lOSS;
						lOSS << "expected an item value while reading a statistics item!";
						throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str());
					}
					mItemMap.insert(std::make_pair(lKey,str2dbl(lChild2->getValue())));
				} else if(lChild->getValue() == "Measure") {
					(*this)[lIndexMeasure].mID  = lChild->getAttribute("id");
					(*this)[lIndexMeasure].mAvg = 0.0;
					(*this)[lIndexMeasure].mStd = 0.0;
					(*this)[lIndexMeasure].mMax = 0.0;
					(*this)[lIndexMeasure].mMin = 0.0;
					for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
						if(lChild2->getType() == PACC::XML::eData) {
							if(lChild2->getValue() == "Avg") {
								PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
								if(lChild3->getType() != PACC::XML::eString) continue;
								else (*this)[lIndexMeasure].mAvg = str2dbl(lChild3->getValue());
							} else if(lChild2->getValue() == "Std") {
								PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
								if(lChild3->getType() != PACC::XML::eString) continue;
								else (*this)[lIndexMeasure].mStd = str2dbl(lChild3->getValue());
							} else if(lChild2->getValue() == "Max") {
								PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
								if(lChild3->getType() != PACC::XML::eString) continue;
								else (*this)[lIndexMeasure].mMax = str2dbl(lChild3->getValue());
							} else if(lChild2->getValue() == "Min") {
								PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
								if(lChild3->getType() != PACC::XML::eString) continue;
								else (*this)[lIndexMeasure].mMin = str2dbl(lChild3->getValue());
							}
						}
					}
					++lIndexMeasure;
				}
			}
		}
	} else if(lValid == "no") mValid = false;
	else throw Beagle_IOExceptionNodeM((*inIter), "bad stats validity value!");
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a stats into a XML streamer.
 *  \param ioStreamer XML streamer used to write the stats into.
 *  \param inIndent Whether XML output should be indented.
 */
void Stats::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if(mValid) {
		if(mID.empty() == false) ioStreamer.insertAttribute("id", mID);
		ioStreamer.insertAttribute("generation", uint2str(mGeneration));
		ioStreamer.insertAttribute("popsize", uint2str(mPopSize));
		std::set<std::string> lSortedItemKey;
		for(ItemMap::const_iterator lIterMap=mItemMap.begin(); lIterMap!=mItemMap.end(); ++lIterMap) {
			lSortedItemKey.insert(lIterMap->first);
		}
		for(std::set<std::string>::const_iterator lIterSet=lSortedItemKey.begin();
		        lIterSet!=lSortedItemKey.end(); ++lIterSet) {
			ioStreamer.openTag("Item", false);
			ioStreamer.insertAttribute("key", *lIterSet);
			ItemMap::const_iterator lIterMap = mItemMap.find(*lIterSet);
			ioStreamer.insertStringContent(dbl2str(lIterMap->second,12));
			ioStreamer.closeTag();
		}
		for(unsigned int i=0; i<size(); ++i) {
			ioStreamer.openTag("Measure", inIndent);
			ioStreamer.insertAttribute("id", (*this)[i].mID);
			ioStreamer.openTag("Avg", false);
			ioStreamer.insertStringContent(dbl2str((*this)[i].mAvg,12));
			ioStreamer.closeTag();
			ioStreamer.openTag("Std", false);
			ioStreamer.insertStringContent(dbl2str((*this)[i].mStd,12));
			ioStreamer.closeTag();
			ioStreamer.openTag("Max", false);
			ioStreamer.insertStringContent(dbl2str((*this)[i].mMax,12));
			ioStreamer.closeTag();
			ioStreamer.openTag("Min", false);
			ioStreamer.insertStringContent(dbl2str((*this)[i].mMin,12));
			ioStreamer.closeTag();
			ioStreamer.closeTag();
		}
	} else ioStreamer.insertAttribute("valid", "no");
	Beagle_StackTraceEndM();
}
