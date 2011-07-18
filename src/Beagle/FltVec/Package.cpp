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
 *  \file   Beagle/FltVec/Package.cpp
 *  \brief  Source code of class FltVec::Package.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "Beagle/FltVec.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with float vector objects.
 *  \param inInitSize Size of vectors generated at initialization.
 */
FltVec::Package::Package(unsigned int inInitSize) :
		PackageAbstract("FltVec-Package"),
		mInitSize(inInitSize)
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void FltVec::Package::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::FltVec::CrossoverBlendOp", new FltVec::CrossoverBlendOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::CrossoverOnePointOp", new FltVec::CrossoverOnePointOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::CrossoverSBXOp", new FltVec::CrossoverSBXOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::CrossoverTwoPointsOp", new FltVec::CrossoverTwoPointsOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::CrossoverUniformOp", new FltVec::CrossoverUniformOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::FloatVector", new FltVec::FloatVector::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::InitGaussianOp", new FltVec::InitGaussianOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::InitGaussianQROp", new FltVec::InitGaussianQROp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::InitUniformOp", new FltVec::InitUniformOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::InitUniformQROp", new FltVec::InitUniformQROp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::MutationGaussianOp", new FltVec::MutationGaussianOp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::MutationGaussianQROp", new FltVec::MutationGaussianQROp::Alloc);
	lFactory.insertAllocator("Beagle::FltVec::Package", new FltVec::Package::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::FltVec::CrossoverBlendOp", "FltVec-CrossoverBlendOp");
	lFactory.aliasAllocator("Beagle::FltVec::CrossoverSBXOp", "FltVec-CrossoverSBXOp");
	lFactory.aliasAllocator("Beagle::FltVec::CrossoverOnePointOp", "FltVec-CrossoverOnePointOp");
	lFactory.aliasAllocator("Beagle::FltVec::CrossoverTwoPointsOp", "FltVec-CrossoverTwoPointsOp");
	lFactory.aliasAllocator("Beagle::FltVec::CrossoverUniformOp", "FltVec-CrossoverUniformOp");
	lFactory.aliasAllocator("Beagle::FltVec::FloatVector", "FltVec-FloatVector");
	lFactory.aliasAllocator("Beagle::FltVec::InitGaussianOp", "FltVec-InitGaussianOp");
	lFactory.aliasAllocator("Beagle::FltVec::InitGaussianQROp", "FltVec-InitGaussianQROp");
	lFactory.aliasAllocator("Beagle::FltVec::InitUniformOp", "FltVec-InitUniformOp");
	lFactory.aliasAllocator("Beagle::FltVec::InitUniformQROp", "FltVec-InitUniformQROp");
	lFactory.aliasAllocator("Beagle::FltVec::MutationGaussianOp", "FltVec-MutationGaussianOp");
	lFactory.aliasAllocator("Beagle::FltVec::MutationGaussianQROp", "FltVec-MutationGaussianQROp");
	lFactory.aliasAllocator("Beagle::FltVec::Package", "FltVec-Package");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("CrossoverOp", "FltVec-CrossoverSBXOp");
	lFactory.setConcept("Genotype", "FltVec-FloatVector");
	lFactory.setConcept("InitializationOp", "FltVec-InitUniformOp");
	lFactory.setConcept("MutationOp", "FltVec-MutationGaussianOp");

	// Add components
	if(ioSystem.haveComponent("QuasiRandom") == NULL) {
		ioSystem.addComponent(new QuasiRandom);
	}

	// Set default initialization size
	ioSystem.getRegister().setEntryDefaultValue("fltvec.init.vectorsize",
	        new UInt(mInitSize),
	        uint2str(mInitSize));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  FltVec::Package depends on EC::Package.
 */
PackageAbstract::Bag FltVec::Package::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	PackageAbstract::Bag lDependencies;
	lDependencies.push_back(new EC::Package());
	return lDependencies;
	Beagle_StackTraceEndM();
}
