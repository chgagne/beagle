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
 *  \file   beagle/Distrib/Island/src/TransportComponent.cpp
 *  \brief  Source code of class TransportComponent.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:33 $
 */


#include "beagle/Distrib/Island.hpp"
#include <string>
#include <sstream>

using namespace Beagle;
using namespace Beagle::Distrib;


Island::TransportComponent::TransportComponent(const std::string& inName) :
		Component(inName)
{}


void Island::TransportComponent::getIndices(const unsigned int inNumberToGet,
        std::vector< unsigned int >& ioIndices,
        const unsigned int inDemeIndex)
{
	Beagle_StackTraceBeginM();
	if (inDemeIndex >= mIndices.size()) {
		throw Beagle_RunTimeExceptionM
		(std::string("inDemeIndex (")+uint2str(inDemeIndex)+
		 ") is greater than or equal to the size of mIndices ("+uint2str(mIndices.size())+")");
	}
	/*
	if (mIndices[inDemeIndex].size() < inNumberToGet) {
	  throw Beagle_RunTimeExceptionM
	    (std::string("The number of indices for the ")+uint2ordinal(inDemeIndex+1)+
	     " deme ("+uint2str(mIndices[inDemeIndex].size())+")"+
	     " is less than the number to get ("+uint2str(inNumberToGet)+")");
	}
	*/

	// Check that sufficient indices exist in buffer
	unsigned int lNumberToGet;
	if (inNumberToGet > mIndices[inDemeIndex].size()) {
		lNumberToGet = mIndices[inDemeIndex].size();
	} else {
		lNumberToGet = inNumberToGet;
	}

	// Move indices to ioIndices
	ioIndices.clear();
	ioIndices.insert(ioIndices.end(),
	                 mIndices[inDemeIndex].begin(),mIndices[inDemeIndex].begin()+lNumberToGet);
	mIndices[inDemeIndex].erase(mIndices[inDemeIndex].begin(),
	                            mIndices[inDemeIndex].begin()+lNumberToGet);

	Beagle_AssertM(ioIndices.size() <= inNumberToGet);
	Beagle_StackTraceEndM();
}


void Island::TransportComponent::insertIndices(const std::vector< unsigned int >& inIndices,
        const unsigned int inDemeIndex)
{
	Beagle_StackTraceBeginM();
	std::deque< unsigned int >& lDeque = mIndices[inDemeIndex];
	lDeque.insert(lDeque.end(), inIndices.begin(), inIndices.end());
	Beagle_StackTraceEndM();
}


void Island::TransportComponent::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	// Get the value of ec.pop.size
	UIntArray::Handle lPopSize;
	if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
		lPopSize =
		    castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
	} else {
		throw Beagle_RunTimeExceptionM("Variable 'ec.pop.size' was not found in Register");
	}

	// Initialize mIndicies
	mIndices.resize(lPopSize->size());
	Beagle_StackTraceEndM();
}


void Island::TransportComponent::readWithSystem(PACC::XML::ConstIterator inIter,
        System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mIndices.clear();
	UIntArray::Handle lPopSize;
	if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
		lPopSize =
		    castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
	} else {
		throw Beagle_RunTimeExceptionM("Variable 'ec.pop.size' was not found in Register");
	}
	mIndices.resize(lPopSize->size());

	// Find 'Deme's
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Deme")) {
			// Get deme's index
			Beagle_AssertM(lChild->isDefined("index"));
			unsigned int lDemeIndex = str2uint(lChild->getAttribute("index"));
			// Get number of indices
			Beagle_AssertM(lChild->isDefined("size"));
			unsigned int lSize = str2uint(lChild->getAttribute("size"));
			mIndices[lDemeIndex].resize(lSize);
			// Find 'Index's
			unsigned int lCount = 0;
			for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
				if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Index")) {
					PACC::XML::ConstIterator lNode=lChild2->getFirstChild();
					Beagle_AssertM( lNode->getType() == PACC::XML::eString );
					std::istringstream lISS( lNode->getValue() );
					lISS >> mIndices[lDemeIndex][lCount];
					++lCount;
				}
			}
			// Check that the number of indices read equaled the number specified
			if (lCount!=lSize) throw Beagle_RunTimeExceptionM
				(std::string("When reading TransportComponent, the number of indices specified within 'Deme' ")+
				 "tag with 'index' attribute of "+uint2str(lDemeIndex)+" ("+uint2str(lSize)+
				 ") did not match the number of 'Index' tags that were found ("+uint2str(lCount)+")");
		}
	}

	Beagle_StackTraceEndM();
}


void Island::TransportComponent::writeContent(PACC::XML::Streamer& ioStreamer,
        bool inIndent) const
{
	Beagle_StackTraceBeginM();
	for (unsigned int i=0; i<mIndices.size(); ++i) {
		if (!mIndices[i].empty()) {
			ioStreamer.openTag("Deme",inIndent);
			ioStreamer.insertAttribute("index",i);
			ioStreamer.insertAttribute("size",mIndices[i].size());
			for (unsigned int j=0; j<mIndices[i].size(); ++j) {
				ioStreamer.openTag("Index",inIndent);
				ioStreamer.insertStringContent(uint2str(mIndices[i][j]));
				ioStreamer.closeTag(); // Index
			}
			ioStreamer.closeTag(); // Deme
		}
	}
	Beagle_StackTraceEndM();
}
