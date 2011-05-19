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
 *  \file   Beagle/BitStr/BitString.cpp
 *  \brief  Source code of class BitStr::BitString.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.25 $
 *  $Date: 2007/08/17 18:09:10 $
 */


#include "Beagle/BitStr.hpp"

#include <algorithm>


using namespace Beagle;


/*!
 *  \brief Construct a bit string genotype.
 *  \param inSize Size of the bit string.
 *  \param inModel Default values of the bits.
 */
BitStr::BitString::BitString(unsigned int inSize, bool inModel) :
		std::vector<bool>(inSize, inModel)
{ }


/*!
 *  \brief Convert a binary bit string to a floating-point vector with keys.
 *  \param inKeys Decoding keys.
 *  \param inBitString Binary bit string to decode.
 *  \param outVector Floating-point number vector resulting from decoding.
 */
void BitStr::BitString::convertBin2Dec(const BitStr::BitString::DecodingKeyVector& inKeys,
                                       const std::vector<bool>& inBitString,
                                       std::vector<double>& outVector)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_LONG_LONG
	typedef unsigned long long EncodingType;
#else // BEAGLE_HAVE_LONG_LONG
	typedef unsigned long EncodingType;
#endif // BEAGLE_HAVE_LONG_LONG

#ifndef BEAGLE_NDEBUG
	for(unsigned int i=0; i<inKeys.size(); i++) {
		if(inKeys[i].mLowerBound >= inKeys[i].mUpperBound) {
			std::ostringstream lOSS;
			lOSS << "In BitStr::BitString::convertBin2Dec(): ";
			lOSS <<  "the lower bound value of the " <<  uint2ordinal(i+1);
			lOSS <<  " key (" <<  inKeys[i].mLowerBound;
			lOSS <<  ") is bigger or equal to the associated upper bound (";
			lOSS <<  inKeys[i].mUpperBound <<  ")!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		if((inKeys[i].mEncoding < 1) || (inKeys[i].mEncoding >= 8*sizeof(EncodingType))) {
			std::ostringstream lOSS;
			lOSS <<  "In BitStr::BitString::convertBin2Dec(): the ";
			lOSS <<  uint2ordinal(i+1) << " encoding value (" << inKeys[i].mEncoding;
			lOSS <<  ") is not in the valid interval (which is [1,";
			lOSS <<  (8*sizeof(EncodingType))-1 << "])!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
	}
#endif // BEAGLE_NDEBUG

	outVector.resize(inKeys.size());
	std::vector<bool>::const_iterator lBit = inBitString.begin();
	for(unsigned int j=0; j<inKeys.size(); j++) {
		EncodingType lGene=0;
		for(unsigned int k=0; k<inKeys[j].mEncoding; ++k) {
			lGene <<= 1;
			lGene |= ((*lBit++) ? 1 : 0);
		}
		EncodingType lDiv = ~0;
		lDiv <<= inKeys[j].mEncoding;
		lDiv = ~lDiv;
		double lTempVal = double(lGene) / double(lDiv);
		outVector[j] =
		    inKeys[j].mLowerBound + (lTempVal * (inKeys[j].mUpperBound - inKeys[j].mLowerBound));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Convert a gray-coded bit string to a binary bit string with keys.
 *  \param inKeys Decoding keys.
 *  \param inGrayBitString Gray-coded bit string to decode.
 *  \param outBinBitString Binary bit string resulting from decoding.
 */
void BitStr::BitString::convertGray2Bin(const BitStr::BitString::DecodingKeyVector& inKeys,
                                        const std::vector<bool>& inGrayBitString,
                                        std::vector<bool>& outBinBitString)
{
	Beagle_StackTraceBeginM();
	outBinBitString.resize(inGrayBitString.size());
	unsigned int lPosBS = 0;
	for(unsigned int i=0; i<inKeys.size(); ++i) {
		for(unsigned int j=0; j<inKeys[i].mEncoding; ++j) {
			if(j==0) outBinBitString[lPosBS] = inGrayBitString[lPosBS];
			else outBinBitString[lPosBS] =
				    ((bool)inGrayBitString[lPosBS] != (bool)outBinBitString[lPosBS-1]);
			++lPosBS;
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy bitstring into current.
 *  \param inOriginal Bitstring to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void BitStr::BitString::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const BitStr::BitString& lOriginal = castObjectT<const BitStr::BitString&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Decode the value of the binary bit string into a vector of floating-point numbers.
 *  \param inKeys Decoding keys used to tranform the bit string.
 *  \param outVector Vector of floating-point number containing the decoded values.
 */
void BitStr::BitString::decode(const BitStr::BitString::DecodingKeyVector& inKeys,
                               std::vector<double>& outVector) const
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	unsigned int lNbBits = 0;
	for(unsigned int i=0; i<inKeys.size(); i++) lNbBits += inKeys[i].mEncoding;
	if(lNbBits != size()) {
		std::ostringstream lOSS;
		lOSS << "In BitStr::BitString::decode(): ";
		lOSS <<  "The total number of bits specified by the encoding keys (";
		lOSS <<  lNbBits;
		lOSS <<  ") is different from the number of bits of the genotype string (";
		lOSS <<  size();
		lOSS <<  ") !";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
#endif // BEAGLE_NDEBUG

	convertBin2Dec(inKeys, (*this), outVector);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Decode the value of the gray-coded bit string into a vector of floating-point numbers.
 *  \param inKeys Decoding keys used to tranform the gray-coded bit string.
 *  \param outVector Vector of floating-point number containing the decoded values.
 */
void BitStr::BitString::decodeGray(const BitStr::BitString::DecodingKeyVector& inKeys,
                                   std::vector<double>& outVector) const
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	unsigned int lNbBits = 0;
	for(unsigned int i=0; i<inKeys.size(); i++) lNbBits += inKeys[i].mEncoding;

	if(lNbBits != size()) {
		std::ostringstream lOSS;
		lOSS << "In BitStr::BitString::decodeGray(): ";
		lOSS <<  "the total number of bits specified by the encoding keys (";
		lOSS <<  lNbBits;
		lOSS <<  ") is different from the number of bits of the genotype string (";
		lOSS <<  size();
		lOSS <<  ")!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
#endif // BEAGLE_NDEBUG

	std::vector<bool> lBinBitString;
	convertGray2Bin(inKeys, (*this), lBinBitString);
	convertBin2Dec(inKeys, lBinBitString, outVector);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return size (length) of genotype.
 *  \return Size (length) of genotype.
 */
unsigned int BitStr::BitString::getSize() const
{
	Beagle_StackTraceBeginM();
	return size();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the genotype.
 *  \return Exact type of the genotype.
 */
const std::string& BitStr::BitString::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("BitStr-BitString");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Swap the gene at inI with the gene at inJ.
 *	\param inI is the index of the first gene to swap.
 *	\param inJ is the index of the second gene to swap.
 */
void BitStr::BitString::swap(unsigned int inI, unsigned int inJ)
{
	Beagle_StackTraceBeginM();
	Beagle_BoundCheckAssertM(inI, 0, size() -1);
	Beagle_BoundCheckAssertM(inJ, 0, size() -1);
	this->swap((*this)[inI], (*this)[inJ]);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test if two bit strings are equals.
 *  \param  inRightObj Second bit string used for the comparison.
 *  \return True if bit strings are equal and of same size, false if not.
 */
bool BitStr::BitString::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const BitStr::BitString& lRightBS = castObjectT<const BitStr::BitString&>(inRightObj);
	if(size() != lRightBS.size()) return false;
	return std::equal(begin(), end(), lRightBS.begin());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test the ordering of two bit strings.
 *  \param  inRightObj Second bit string used for the ordering test.
 *  \return True if the actual bit string is before the second, false if not.
 *  \par Note:
 *    Returns true if the actual bit string is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool BitStr::BitString::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const BitStr::BitString& lRightBS = castObjectT<const BitStr::BitString&>(inRightObj);
	unsigned int lSizeCompared = minOf<unsigned int>(size(),lRightBS.size());
	return std::lexicographical_compare(begin(), begin()+lSizeCompared,
	                                    lRightBS.begin(), lRightBS.begin()+lSizeCompared);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a bit string from a XML subtree.
 *  \param inIter XML iterator from which the bit string is read.
 *  \param ioContext Evolutionary context.
 *  \throw IOException If a reading error occured (generally bad file format)!
 */
void BitStr::BitString::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
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
		throw Beagle_IOExceptionNodeM(*lChild, "expected content for the bit string!");
	}

	clear();
	std::istringstream lISS(lChild->getValue());
	while(lISS.eof() == false) {
		char lBuf = '\0';
		lISS >> lBuf;
		if(lBuf=='\0') break;
		if((lBuf != '0') && (lBuf != '1')) {
			throw Beagle_IOExceptionNodeM(*lChild, "bit string value is not 0 or 1!");
		}
		push_back((lBuf == '1') ? true : false);
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a bit string into a XML streamer.
 *  \param ioStreamer XML streamer into which the bit string is written.
 *  \param inIndent Whether XML output should be indented.
 */
void BitStr::BitString::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("size", uint2str(size()));
	std::ostringstream lOSS;
	for(unsigned int i=0; i<size(); i++) lOSS << ((*this)[i] ? '1' : '0');
	ioStreamer.insertStringContent(lOSS.str());
	Beagle_StackTraceEndM();
}
