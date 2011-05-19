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
 *  \file   beagle/LinGP/src/Package.cpp
 *  \brief  Source code of class LinGP::Package.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.2 $
 *  $Date: 2007/09/12 14:23:47 $
 */

#include "beagle/LinGP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with LinGP objects.
 */
LinGP::Package::Package() :
		Beagle::Package("LinGP-Package")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void LinGP::Package::configure(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::LinGP::Context", new LinGP::Context::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::CrossoverOnePointOp", new LinGP::CrossoverOnePointOp::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::CrossoverTwoPointsOp", new LinGP::CrossoverTwoPointsOp::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::CrossoverUniformOp", new LinGP::CrossoverUniformOp::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::InitializationOp", new LinGP::InitializationOp::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::InstructionSet", new LinGP::InstructionSet::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::InstructionSuperSet", new LinGP::InstructionSuperSet::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::Memory", new LinGP::Memory::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::MutationOp", new LinGP::MutationOp::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::Package", new LinGP::Package::Alloc);
	lFactory.insertAllocator("Beagle::LinGP::Program", new LinGP::Program::Alloc);

	// Set aliases
	lFactory.aliasAllocator("Beagle::LinGP::Context", "LinGP-Context");
	lFactory.aliasAllocator("Beagle::LinGP::CrossoverOnePointOp", "LinGP-CrossoverOnePointOp");
	lFactory.aliasAllocator("Beagle::LinGP::CrossoverTwoPointsOp", "LinGP-CrossoverTwoPointsOp");
	lFactory.aliasAllocator("Beagle::LinGP::CrossoverUniformOp", "LinGP-CrossoverUniformOp");
	lFactory.aliasAllocator("Beagle::LinGP::InitializationOp", "LinGP-InitializationOp");
	lFactory.aliasAllocator("Beagle::LinGP::InstructionSet", "LinGP-InstructionSet");
	lFactory.aliasAllocator("Beagle::LinGP::InstructionSuperSet", "LinGP-InstructionSuperSet");
	lFactory.aliasAllocator("Beagle::LinGP::Memory", "LinGP-Memory");
	lFactory.aliasAllocator("Beagle::LinGP::MutationOp", "LinGP-MutationOp");
	lFactory.aliasAllocator("Beagle::LinGP::Package", "LinGP-Package");
	lFactory.aliasAllocator("Beagle::LinGP::Program", "LinGP-Program");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Context", "LinGP-Context");
	lFactory.setConcept("CrossoverOp", "LinGP-CrossoverOnePointOp");
	lFactory.setConcept("Genotype", "LinGP-Program");
	lFactory.setConcept("InitializationOp", "LinGP-InitializationOp");
	lFactory.setConcept("MutationOp", "LinGP-MutationOp");

	// Add LinGP components
	if(ioSystem.haveComponent("LinGP-InstructionSuperSet") == NULL) {
		ioSystem.addComponent(new LinGP::InstructionSuperSet);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages on which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  LinGP::Package depends on PackageBase.
 */
Beagle::Package::Bag LinGP::Package::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new PackageBase());
	return lDependencies;
	Beagle_StackTraceEndM();
}
