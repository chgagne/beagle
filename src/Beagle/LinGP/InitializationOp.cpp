/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/InitializationOp.cpp
 *  \brief  Source code of class LinGP::InitializationOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.8 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#include <sstream>

#include "beagle/LinGP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a linear GP initialization operator.
 *  \param inBitStringSize Size of the programs initialized.
 *  \param inName Name of the operator.
 */
LinGP::InitializationOp::InitializationOp(IntArray inMinInitSize,
        IntArray inMaxInitSize,
        std::string inName) :
		Beagle::InitializationOp(inName),
		mMinInitSize(new IntArray(inMinInitSize)),
		mMaxInitSize(new IntArray(inMaxInitSize))
{ }


/*!
 *  \brief Register parameters of the linear GP initialization operator.
 *  \param ioSystem System of the evolution.
 */
void LinGP::InitializationOp::registerParams(Beagle::System& ioSystem)
{
	Beagle::InitializationOp::registerParams(ioSystem);
	{
		std::string lDefaultValue;
		for(unsigned int i=0; i<mMinInitSize->size(); i++) {
			lDefaultValue += uint2str((*mMinInitSize)[i]);
			if(i != (mMinInitSize->size()-1)) lDefaultValue += "/";
		}
		std::ostringstream lOSS;
		lOSS << "Minimum program size for individuals at initialization. ";
		lOSS << "The size of the IntArray is the number of program composing the ";
		lOSS << "linear GP individuals, while each value of the vector is the minimum ";
		lOSS << "number of instructions composing the programs. ";
		lOSS << "IntArray format is S1/S2/.../Sn, where Si is ith value.";
		Register::Description lDescription(
		    "Min individual init. sizes",
		    "IntArray",
		    lDefaultValue,
		    lOSS.str()
		);
		mMinInitSize = castHandleT<IntArray>(
		                   ioSystem.getRegister().insertEntry("lingp.init.minsize", mMinInitSize, lDescription));
	}
	{
		std::string lDefaultValue;
		for(unsigned int i=0; i<mMaxInitSize->size(); i++) {
			lDefaultValue += uint2str((*mMaxInitSize)[i]);
			if(i != (mMaxInitSize->size()-1)) lDefaultValue += "/";
		}
		std::ostringstream lOSS;
		lOSS << "Maximum program size for individuals at initialization. ";
		lOSS << "The size of the IntArray is the number of program composing the ";
		lOSS << "linear GP individuals, while each value of the vector is the maximum ";
		lOSS << "number of instructions composing the programs. ";
		lOSS << "IntArray format is S1/S2/.../Sn, where Si is ith value.";
		Register::Description lDescription(
		    "Max individual init. sizes",
		    "IntArray",
		    lDefaultValue,
		    lOSS.str()
		);
		mMaxInitSize = castHandleT<IntArray>(
		                   ioSystem.getRegister().insertEntry("lingp.init.maxsize", mMaxInitSize, lDescription));
	}
}


/*!
 *  \brief Initialize an linear GP individual.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void LinGP::InitializationOp::initIndividual(Beagle::Individual& outIndividual,
        Beagle::Context& ioContext)
{
#ifndef BEAGLE_NDEBUG
	if(mMinInitSize->size() == 0) {
		std::string lMessage = "LinGP::InitializationOp::initIndividual: ";
		lMessage += "Size of the minimum iniitalization size parameter is zero. ";
		lMessage += "Could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	if(mMaxInitSize->size() == 0) {
		std::string lMessage = "LinGP::InitializationOp::initIndividual: ";
		lMessage += "Size of the maximum iniitalization size parameter is zero. ";
		lMessage += "Could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	if(mMinInitSize->size() != mMaxInitSize->size()) {
		std::string lMessage = "LinGP::InitializationOp::initIndividual: ";
		lMessage += "Size of the maximum and minimum iniitalization size parameter mismatch. ";
		lMessage += "Could not initialize the individuals!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}

	for(unsigned int k=0; k<mMinInitSize->size(); k++) {
		if((*mMinInitSize)[k] == 0) {
			std::string lMessage = "LinGP::InitializationOp::initIndividual: Value of the ";
			lMessage += uint2ordinal(k);
			lMessage += " minimum initialization size parameter is zero. ";
			lMessage += "Could not initialize the individuals!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		if((*mMaxInitSize)[k] == 0) {
			std::string lMessage = "LinGP::InitializationOp::initIndividual: Value of the ";
			lMessage += uint2ordinal(k);
			lMessage += " maximum initialization size parameter is zero. ";
			lMessage += "Could not initialize the individuals!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
		if((*mMaxInitSize)[k] > (*mMaxInitSize)[k]) {
			std::string lMessage = "LinGP::InitializationOp::initIndividual: Value of the ";
			lMessage += uint2ordinal(k);
			lMessage += " minimum initialization size parameter is bigger than ";
			lMessage += " maximum initialization size parameter. ";
			lMessage += "Could not initialize the individuals!";
			throw Beagle_RunTimeExceptionM(lMessage);
		}
	}
#endif // BEAGLE_NDEBUG

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "initialization", "Beagle::LinGP::InitializationOp",
	    std::string("Resizing the individual to ")+
	    uint2str(mMinInitSize->size())+" programs"
	);
	outIndividual.resize(mMinInitSize->size());
	LinGP::Context& lLinGPContext = castObjectT<LinGP::Context&>(ioContext);
	LinGP::InstructionSuperSet::Handle lInsSS =
	    castHandleT<LinGP::InstructionSuperSet>(lLinGPContext.getSystem().getComponent("LinGP-InstructionSuperSet"));

	const Factory& lFactory = ioContext.getSystem().getFactory();
	LinGP::Program::Alloc::Handle lProgramAlloc =
		castHandleT<LinGP::Program::Alloc>(lFactory.getConceptAllocator("Genotype"));

	for(unsigned int i=0; i<outIndividual.size(); ++i) {
		outIndividual[i] = castHandleT<LinGP::Program>(lProgramAlloc->allocate());
		LinGP::Program::Handle lActualProgram = castHandleT<LinGP::Program>(outIndividual[i]);
		unsigned int lInitProgramSize =
		    lLinGPContext.getSystem().getRandomizer().rollInteger((*mMinInitSize)[i], (*mMaxInitSize)[i]);
		lActualProgram->resize(lInitProgramSize);
		for(unsigned int j=0; j<lInitProgramSize; ++j) {
			(*lActualProgram)[j] =
			    lInsSS->getInstructionSets()[i]->selectRandomInstruction(lLinGPContext.getSystem())->giveReference(lLinGPContext);
		}
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "initialization", "Beagle::LinGP::InitializationOp",
		    std::string("Program ")+uint2str(i+1)+
		    std::string(" has ")+uint2str(lInitProgramSize)+" initialized instructions"
		);
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "initialization", "Beagle::LinGP::InitializationOp",
		    uint2ordinal(i+1)+std::string(" program is initialized as")
		);
		Beagle_LogObjectDebugM(
		    ioContext.getSystem().getLogger(),
		    "initialization",
		    "Beagle::LinGP::InitializationOp",
		    *lActualProgram
		);
	}
}

