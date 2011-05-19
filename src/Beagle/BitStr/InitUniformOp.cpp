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
 *  \file   Beagle/BitStr/InitUniformOp.cpp
 *  \brief  Source code of class BitStr::InitUniformOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/BitStr.hpp"

#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a bit string initialization operator.
 *  \param inNumberBits Number of bits of the strings initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
BitStr::InitUniformOp::InitUniformOp(unsigned int inNumberBits,
                                     std::string inReproProbaName,
                                     std::string inName) :
		EC::InitializationOp(inReproProbaName, inName),
		mNumberBits(new UInt(inNumberBits)),
		mBitOneProba(NULL)
{ }


/*!
 *  \brief Register the parameters of the bit string initialization operator.
 *  \param ioSystem System of the evolution.
 */
void BitStr::InitUniformOp::registerParams(System& ioSystem)
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
	{
		std::ostringstream lOSS;
		lOSS << "Distribution probability of bit values. ";
		lOSS << "A probability of 1.0 means that the bits values are all initialized to 1, ";
		lOSS << "while a probability of 0.0 means that they are all initialized to 0. ";
		lOSS << "Probability of 0.5 means that the bits are uniformly, ";
		lOSS << "randomly initialized with equally 0s and 1s.";
		Register::Description lDescription(
		    "Initial bit distribution prob.",
		    "Float",
		    "0.5",
		    lOSS.str()
		);
		mBitOneProba = castHandleT<Float>(
		                   ioSystem.getRegister().insertEntry("bitstr.init.bitpb", new Float(0.5), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize an individual made of bit strings.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void BitStr::InitUniformOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mNumberBits->getWrappedValue() == 0) {
		std::string lMessage = "BitStr::InitUniformOp::initIndividual: number of bits to use to ";
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
	for(unsigned int i=0; i<lBitString->size(); ++i) {
		(*lBitString)[i] =
		    (ioContext.getSystem().getRandomizer().rollUniform() < mBitOneProba->getWrappedValue());
	}
	Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lBitString);
	Beagle_StackTraceEndM();
}
