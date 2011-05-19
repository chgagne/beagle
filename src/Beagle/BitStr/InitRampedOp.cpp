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
 *  \file   Beagle/BitStr/InitRampedOp.cpp
 *  \brief  Source code of class BitStr::InitRampedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:10 $
 */


#include "Beagle/BitStr.hpp"

#include <sstream>


using namespace Beagle;


/*!
 *  \brief Construct a bit string ramped initialization operator.
 *  \param inNumberBits Number of bits of the strings initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
BitStr::InitRampedOp::InitRampedOp(unsigned int inNumberBits,
        std::string inReproProbaName,
        std::string inName) :
		EC::InitializationOp(inReproProbaName, inName),
		mNumberBits(new UInt(inNumberBits))
{ }


/*!
 *  \brief Register the parameters of the bit string ramped initialization operator.
 *  \param ioSystem System of the evolution.
 */
void BitStr::InitRampedOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	EC::InitializationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Initial number of bits in strings",
		    "UInt",
		    mNumberBits->serialize(),
		    "Number of bits used to initialize individuals."
		);
		mNumberBits = castHandleT<UInt>(
		                  ioSystem.getRegister().insertEntry("bitstr.init.numberbits", mNumberBits, lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize "ramped" an individual made of bit strings.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void BitStr::InitRampedOp::initIndividual(Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mNumberBits->getWrappedValue() == 0) {
		std::string lMessage = "BitStr::InitRampedOp::initIndividual: number of bits to use to ";
		lMessage += " initialize individuals parameter is zero. ";
		lMessage += "Could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	const Factory& lFactory = ioContext.getSystem().getFactory();
	BitStr::BitString::Alloc::Handle lBitStringAlloc =
		castHandleT<BitStr::BitString::Alloc>(lFactory.getConceptAllocator("Genotype"));
	BitStr::BitString::Handle lBitString = castHandleT<BitStr::BitString>(lBitStringAlloc->allocate());
	lBitString->resize(mNumberBits->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lBitString);
	const unsigned int lNumberTrueBits =
	    ioContext.getSystem().getRandomizer().rollInteger(0, mNumberBits->getWrappedValue());
	std::vector<unsigned int> lIndices(mNumberBits->getWrappedValue());
	for(unsigned int i=0; i<lIndices.size(); ++i) lIndices[i] = i;
	std::random_shuffle(lIndices.begin(), lIndices.end(), ioContext.getSystem().getRandomizer());
	for(unsigned int i=0; i<lNumberTrueBits; ++i) {
		(*lBitString)[lIndices[i]] = true;
	}
	for(unsigned int i=lNumberTrueBits; i<lIndices.size(); ++i) {
		(*lBitString)[lIndices[i]] = false;
	}
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lBitString);
	Beagle_StackTraceEndM();
}
