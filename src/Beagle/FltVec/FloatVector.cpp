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
 *  \file   Beagle/FltVec/FloatVector.cpp
 *  \brief  Source code of class FltVec::FloatVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.25 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/FltVec.hpp"

#include <algorithm>
#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder

using namespace Beagle;


/*!
 *  \brief Construct a float vector.
 *  \param inSize Size of the vector.
 *  \param inModel Default values of the floats.
 */
FltVec::FloatVector::FloatVector(unsigned int inSize, float inModel) :
		std::vector<double>(inSize, inModel)
{ }


/*!
 *  \brief Copy float vector into current.
 *  \param inOriginal Float vector to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void FltVec::FloatVector::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const FltVec::FloatVector& lOriginal = castObjectT<const FltVec::FloatVector&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return size (length) of genotype.
 *  \return Size (length) of genotype.
 */
unsigned int FltVec::FloatVector::getSize() const
{
	Beagle_StackTraceBeginM();
	return size();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the genotype.
 *  \return Exact type of the genotype.
 */
const std::string& FltVec::FloatVector::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("FltVec-FloatVector");
	return lType;
	Beagle_StackTraceEndM()
}


/*!
 *  \brief Swap the gene at inI with the gene at inJ.
 *	\param inI is the index of the first gene to swap.
 *	\param inJ is the index of the second gene to swap.
 */
void FltVec::FloatVector::swap(unsigned int inI, unsigned int inJ)
{
	Beagle_StackTraceBeginM();
	Beagle_BoundCheckAssertM(inI, 0, size() -1);
	Beagle_BoundCheckAssertM(inJ, 0, size() -1);
	std::swap((*this)[inI], (*this)[inJ]);
	Beagle_StackTraceEndM()
}


/*!
 *  \brief  Test if two float vectors are equals.
 *  \param  inRightObj Second float vector used for the comparison.
 *  \return True if float vectors are equal and of same size, false if not.
 */
bool FltVec::FloatVector::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FltVec::FloatVector& lRightVector = castObjectT<const FltVec::FloatVector&>(inRightObj);
	if(size() != lRightVector.size()) return false;
	const std::vector<double>& lLeft = *this;
	const std::vector<double>& lRight = lRightVector;
	return (lLeft == lRight);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test the ordering of two float vectors.
 *  \param  inRightObj Second float vector used for the ordering test.
 *  \return True if the actual float vector is before the second and of the same size, false if not.
 */
bool FltVec::FloatVector::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FltVec::FloatVector& lRightVector = castObjectT<const FltVec::FloatVector&>(inRightObj);
	if(size() != lRightVector.size()) return false;
	const std::vector<double>& lLeft = *this;
	const std::vector<double>& lRight = lRightVector;
	return (lLeft < lRight);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a float vector from a XML subtree.
 *  \param inIter XML iterator from which the float vector is read.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If a reading error occured (generally bad file format)!
 */
void FltVec::FloatVector::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
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
	if((!lChild) || (lChild->getType()!=PACC::XML::eString))
		throw Beagle_IOExceptionNodeM(*lChild, "expected content for the float vector!");

	clear();
	std::istringstream lISS(lChild->getValue());
	while(lISS.good()) {
		double lValue=DBL_MAX;
		lISS >> lValue;
		push_back(lValue);
		if(lISS.good()==false) break;
		int lDelim=lISS.get();
		if((lISS.good()==false) || (lDelim==-1)) break;
	}
	
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a float vector into a XML streamer.
 *  \param ioStreamer XML streamer into which the float vector is written.
 *  \param inIndent Whether output should be indented.
 */
void FltVec::FloatVector::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("size", uint2str(size()));
	std::ostringstream lOSS;
	for(unsigned int i=0; i<size(); ++i) {
		lOSS << (*this)[i];
		if(i!=(size()-1)) lOSS << ';';
	}
	ioStreamer.insertStringContent(lOSS.str());
	Beagle_StackTraceEndM();
}
