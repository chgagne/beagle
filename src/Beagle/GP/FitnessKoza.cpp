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
 *  \file   beagle/GP/src/FitnessKoza.cpp
 *  \brief  Source code of class FitnessKoza.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#include "beagle/GP.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Default construct a Koza's fitness object.
 */
GP::FitnessKoza::FitnessKoza() :
		FitnessSimple(),
		mAdjustedFitness(0.),
		mStandardizedFitness(0.),
		mRawFitness(0.),
		mHits(0)
{ }


/*!
 *  \brief Construct a valid Koza's fitness object.
 *  \param inNormalizedFitness Normalized fitness value.
 *  \param inAdjustedFitness Adjusted fitness value.
 *  \param inStandardizedFitness Standardized fitness value.
 *  \param inRawFitness Raw fitness value.
 *  \param inHits Number of hits.
 */
GP::FitnessKoza::FitnessKoza(float inNormalizedFitness,
                             float inAdjustedFitness,
                             float inStandardizedFitness,
                             float inRawFitness,
                             unsigned int inHits)
{
	Beagle_StackTraceBeginM();
	setFitness(inNormalizedFitness,
	           inAdjustedFitness,
	           inStandardizedFitness,
	           inRawFitness,
	           inHits);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the fitness.
 *  \return Exact type of the fitness.
 */
const std::string& GP::FitnessKoza::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("GP-FitnessKoza");
	return lType;
	Beagle_StackTraceEndM("const std::string& GP::FitnessKoza::getType() const")
}


/*!
 *  \brief Read a Koza's fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness values.
 */
void GP::FitnessKoza::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Fitness")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");
	}

	std::string lValid = inIter->getAttribute("valid");
	if(lValid.empty() || (lValid == "yes")) {
	
		// Check type of fitness read
		std::string lType = inIter->getAttribute("type");
		if((lType.empty() == false) && (lType != getType())) {
			std::ostringstream lOSS;
			lOSS << "type given '" << lType << "' mismatch type of the fitness '";
			lOSS << getType() << "'!";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}

		// Read values of Koza's fitness
		for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
			if(lChild->getType() == PACC::XML::eData) {
				if(lChild->getValue() == "Normalized") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no normalized fitness value present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no normalized fitness value present!");
					mValue = str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "Adjusted") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no adjusted fitness value present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no adjusted fitness value present!");
					mAdjustedFitness = str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "Standardized") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no standardized fitness value present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no standardized fitness value present!");
					mStandardizedFitness = str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "Raw") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no raw fitness value present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no raw fitness value present!");
					mRawFitness = str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "Hits") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no hits value present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no hits value present!");
					mHits = str2uint(lChild2->getValue());
				}
			}
		}

	} else if(lValid == "no") {
		setInvalid();
	}
	else {
		throw Beagle_IOExceptionNodeM(*inIter, "bad fitness validity value!");
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the fitness value and validity.
 *  \param inNormalizedFitness Normalized fitness value.
 *  \param inAdjustedFitness Adjusted fitness value.
 *  \param inStandardizedFitness Standardized fitness value.
 *  \param inRawFitness Raw fitness value.
 *  \param inHits Number of hits.
 */
void GP::FitnessKoza::setFitness(float inNormalizedFitness,
                                 float inAdjustedFitness,
                                 float inStandardizedFitness,
                                 float inRawFitness,
                                 unsigned int inHits)
{
	Beagle_StackTraceBeginM();
	if(isFinite(inNormalizedFitness)) mValue = inNormalizedFitness;
	else mValue = -FLT_MAX;
	if(isFinite(inAdjustedFitness)) mAdjustedFitness = inAdjustedFitness;
	else mAdjustedFitness = 0.;
	if(isFinite(inStandardizedFitness)) mStandardizedFitness = inStandardizedFitness;
	else mStandardizedFitness = 0.;
	if(isFinite(inRawFitness)) mRawFitness = inRawFitness;
	else mRawFitness = 0.;
	mHits = inHits;
	setValid();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a Koza's fitness into a Beagle XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness values.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::FitnessKoza::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Normalized", false);
	ioStreamer.insertStringContent(dbl2str(mValue));
	ioStreamer.closeTag();
	ioStreamer.openTag("Adjusted", false);
	ioStreamer.insertStringContent(dbl2str(mAdjustedFitness));
	ioStreamer.closeTag();
	ioStreamer.openTag("Standardized", false);
	ioStreamer.insertStringContent(dbl2str(mStandardizedFitness));
	ioStreamer.closeTag();
	ioStreamer.openTag("Raw", false);
	ioStreamer.insertStringContent(dbl2str(mRawFitness));
	ioStreamer.closeTag();
	ioStreamer.openTag("Hits", false);
	ioStreamer.insertStringContent(uint2str(mHits));
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}

