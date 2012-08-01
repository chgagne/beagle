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
 *  \file   Beagle/IntVec/InitIndicesOp.cpp
 *  \brief  Source code of class IntVec::InitIndicesOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/IntVec.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a permutation of indices initialization operator.
 *  \param inIntVectorSize Size of the integer vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
IntVec::InitIndicesOp::InitIndicesOp(unsigned int inIntVectorSize, 
        std::string inReproProbaName,
        std::string inName) :
	InitializationOp(inReproProbaName, inName),
	mIntVectorSize(new UInt(inIntVectorSize))
{ }


/*!
 *  \brief Register the parameters of the permutation of indices initialization operator.
 *  \param ioSystem System of the evolution.
 */
void IntVec::InitIndicesOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	EC::InitializationOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Initial integer vectors sizes",
	    "UInt",
	    mIntVectorSize->serialize(),
	    "Integer vector size of initialized individuals."
	);
	mIntVectorSize = castHandleT<UInt>(
	                     ioSystem.getRegister().insertEntry("intvec.init.vectorsize", mIntVectorSize, lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize permutation of indices individual.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void IntVec::InitIndicesOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
	if(mIntVectorSize->getWrappedValue() == 0) {
		string lMessage = "IntVec::InitIndicesOp::initIndividual: ";
		lMessage += "integer vector size parameter is zero; ";
		lMessage += "could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
#endif // BEAGLE_NDEBUG
	const Factory& lFactory = ioContext.getSystem().getFactory();
	IntVec::IntegerVector::Alloc::Handle lIntegerVectorAlloc =
		castHandleT<IntVec::IntegerVector::Alloc>(lFactory.getConceptAllocator("Genotype"));
	IntVec::IntegerVector::Handle lIntegerVector = castHandleT<IntVec::IntegerVector>(lIntegerVectorAlloc->allocate());
	lIntegerVector->resize(mIntVectorSize->getWrappedValue());
	outIndividual.clear();
	outIndividual.push_back(lIntegerVector);
	for(unsigned int j=0; j<lIntegerVector->size(); ++j) (*lIntegerVector)[j] = (int)j;
	std::random_shuffle(lIntegerVector->begin(),lIntegerVector->end(),ioContext.getSystem().getRandomizer());
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "Integer vector initialized as: " << *lIntegerVector
	);
	Beagle_StackTraceEndM();
}

