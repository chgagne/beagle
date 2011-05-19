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
 *  \file   beagle/GA/src/ESVector.cpp
 *  \brief  Source code of class GA::ESVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.23 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder

using namespace Beagle;


/*!
 *  \brief Construct a ES pair element.
 *  \param inValue Value of the ES pair.
 *  \param inStrategy Strategy parameter of the ES pair.
 */
GA::ESPair::ESPair(double inValue, double inStrategy) :
		mValue(inValue),
		mStrategy(inStrategy)
{ }


/*!
 *  \brief Compare equality of two ES pair.
 *  \param inRightPair Second ES pair to compare with the actual one.
 *  \return True if both value and strategy of pairs are equal, false if not.
 */
bool GA::ESPair::operator==(const GA::ESPair& inRightPair) const
{
	Beagle_StackTraceBeginM();
	return ((mValue == inRightPair.mValue) && (mStrategy == inRightPair.mStrategy));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Construct an ES vector.
 *  \param inSize Size of the vector.
 *  \param inModel Default ES pair value.
 */
GA::ESVector::ESVector(unsigned int inSize, GA::ESPair inModel) :
		std::vector<GA::ESPair>(inSize, inModel)
{ }


/*!
 *  \brief Copy ES vector into current.
 *  \param inOriginal ES vector to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void GA::ESVector::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const GA::ESVector& lOriginal = castObjectT<const GA::ESVector&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return size (length) of genotype.
 *  \return Size (length) of genotype.
 */
unsigned int GA::ESVector::getSize() const
{
	Beagle_StackTraceBeginM();
	return size();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the genotype.
 *  \return Exact type of the genotype.
 */
const std::string& GA::ESVector::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("GA-ESVector");
	return lType;
	Beagle_StackTraceEndM("const std::string& GA::ESVector::getType() const")
}


/*!
 *  \brief Swap the gene at inI with the gene at inJ.
 *	\param inI is the index of the first gene to swap.
 *	\param inJ is the index of the second gene to swap.
 */
void GA::ESVector::swap(unsigned int inI, unsigned int inJ)
{
	Beagle_StackTraceBeginM();
	Beagle_BoundCheckAssertM(inI, 0, size() -1);
	Beagle_BoundCheckAssertM(inJ, 0, size() -1);
	std::swap((*this)[inI], (*this)[inJ]);
	Beagle_StackTraceEndM("const std::string& GA::ESVector::swap(unsigned int, unsigned int) const")
}


/*!
 *  \brief  Test if two ES vectors are equals.
 *  \param  inRightObj Second ES vector used for the comparison.
 *  \return True if ES vectors are equal and of same size, false if not.
 */
bool GA::ESVector::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const GA::ESVector& lRightESV = castObjectT<const GA::ESVector&>(inRightObj);
	if(size() != lRightESV.size()) return false;
	return std::equal(begin(), end(), lRightESV.begin());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read an ES vector from a XML subtree.
 *  \param inIter XML iterator from which the ES vector is read.
 *  \param ioContext Evolutionary context.
 *  \throw IOException If a reading error occured (generally bad file format)!
 */
void GA::ESVector::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Genotype")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");
	}
	std::string lGenotypeType = inIter->getAttribute("type");
	if((lGenotypeType.empty() == false) && (lGenotypeType != getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lGenotypeType << "' mismatch type of the genotype '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	PACC::XML::ConstIterator lChild = inIter->getFirstChild();
	if((!lChild) || (lChild->getType()!=PACC::XML::eString)) {
		throw Beagle_IOExceptionNodeM(*lChild, "expected content for the ES vector!");
	}

	clear();
	std::istringstream lISS(lChild->getValue());
	do {
		char lC1='\0', lC2='\0', lC3='\0';
		double lValue=DBL_MAX, lStrategy=DBL_MAX;
		lISS >> lC1 >> lValue >> lC2 >> lStrategy >> lC3;
		if((lC1!='(') || (lC2!=',') || (lC3!=')'))
			throw Beagle_IOExceptionNodeM(*lChild, "bad format of ES vector!");
		if((lValue==DBL_MAX) || (lStrategy==DBL_MAX))
			throw Beagle_IOExceptionNodeM(*lChild, "bad format of ES vector!");
		push_back(ESPair(lValue, lStrategy));
	} while(lISS.get()==int('/'));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an ES vector into a XML streamer.
 *  \param ioStreamer XML streamer into which the ES vector is written.
 *  \param inIndent Whether output should be indented.
 */
void GA::ESVector::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("size", uint2str(size()));
	std::ostringstream lOSS;
	for(unsigned int i=0; i<size(); i++) {
		lOSS << '(' << (*this)[i].mValue << ',' << (*this)[i].mStrategy << ')';
		if(i!=(size()-1)) lOSS << '/';
	}
	ioStreamer.insertStringContent(lOSS.str());
	Beagle_StackTraceEndM();
}

