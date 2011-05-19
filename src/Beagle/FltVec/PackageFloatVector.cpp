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
 *  \file   beagle/GA/src/PackageFloatVector.cpp
 *  \brief  Source code of class GA::PackageFloatVector.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "beagle/GA.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with float vector GA objects.
 *  \param inInitSize Size of vectors generated at initialization.
 */
GA::PackageFloatVector::PackageFloatVector(unsigned int inInitSize) :
		Package("GA-PackageFloatVector"),
		mInitSize(inInitSize)
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void GA::PackageFloatVector::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::GA::AdaptOneFifthRuleFltVecOp", new GA::AdaptOneFifthRuleFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::AlgoESOneFifthRule", new GA::AlgoESOneFifthRule::Alloc);
	lFactory.insertAllocator("Beagle::GA::CrossoverBlendFltVecOp", new GA::CrossoverBlendFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::CrossoverOnePointFltVecOp", new GA::CrossoverOnePointFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::CrossoverSBXFltVecOp", new GA::CrossoverSBXFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::CrossoverTwoPointsFltVecOp", new GA::CrossoverTwoPointsFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::CrossoverUniformFltVecOp", new GA::CrossoverUniformFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::FloatVector", new GA::FloatVector::Alloc);
	lFactory.insertAllocator("Beagle::GA::InitFltVecOp", new GA::InitFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::InitQRFltVecOp", new GA::InitQRFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationGaussianFltVecOp", new GA::MutationGaussianFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationMoveSequenceOp", new GA::MutationMoveSequenceOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationQRGaussianFltVecOp", new GA::MutationQRGaussianFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationReverseSequenceOp", new GA::MutationReverseSequenceOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationShuffleSequenceOp", new GA::MutationShuffleSequenceOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::PackageFloatVector", new GA::PackageFloatVector::Alloc);
	lFactory.insertAllocator("Beagle::GA::RecombinationFltVecOp", new GA::RecombinationFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::RecombinationWeightedFltVecOp", new GA::RecombinationWeightedFltVecOp::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::GA::AdaptOneFifthRuleFltVecOp", "GA-AdaptOneFifthRuleFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::AlgoESOneFifthRule", "GA-AlgoESOneFifthRule");
	lFactory.aliasAllocator("Beagle::GA::CrossoverBlendFltVecOp", "GA-CrossoverBlendFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::CrossoverSBXFltVecOp", "GA-CrossoverSBXFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::CrossoverOnePointFltVecOp", "GA-CrossoverOnePointFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::CrossoverTwoPointsFltVecOp", "GA-CrossoverTwoPointsFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::CrossoverUniformFltVecOp", "GA-CrossoverUniformFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::FloatVector", "GA-FloatVector");
	lFactory.aliasAllocator("Beagle::GA::InitFltVecOp", "GA-InitFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::InitQRFltVecOp", "GA-InitQRFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::MutationGaussianFltVecOp", "GA-MutationGaussianFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::MutationMoveSequenceOp", "GA-MutationMoveSequenceOp");
	lFactory.aliasAllocator("Beagle::GA::MutationQRGaussianFltVecOp", "GA-MutationQRGaussianFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::MutationReverseSequenceOp", "GA-MutationReverseSequenceOp");
	lFactory.aliasAllocator("Beagle::GA::MutationShuffleSequenceOp", "GA-MutationShuffleSequenceOp");
	lFactory.aliasAllocator("Beagle::GA::PackageFloatVector", "GA-PackageFloatVector");
	lFactory.aliasAllocator("Beagle::GA::RecombinationFltVecOp", "GA-RecombinationFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::RecombinationWeightedFltVecOp", "GA-RecombinationWeightedFltVecOp");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("CrossoverOp", "GA-CrossoverSBXFltVecOp");
	lFactory.setConcept("Genotype", "GA-FloatVector");
	lFactory.setConcept("InitializationOp", "GA-InitFltVecOp");
	lFactory.setConcept("MutationOp", "GA-MutationGaussianFltVecOp");

	// Add components
	if(ioSystem.haveComponent("QuasiRandom") == NULL) {
		ioSystem.addComponent(new QuasiRandom);
	}

	// Set default initialization size
	ioSystem.getRegister().setEntryDefaultValue("ga.init.vectorsize",
	        new UInt(mInitSize),
	        uint2str(mInitSize));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  PackageFloatVector depends on PackageBase.
 */
Package::Bag GA::PackageFloatVector::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new PackageBase());
	return lDependencies;
	Beagle_StackTraceEndM();
}
