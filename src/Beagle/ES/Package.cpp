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
 *  \file   Beagle/ES/Package.cpp
 *  \brief  Source code of class ES::Package.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "Beagle/ES.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with ES objects.
 */
ES::Package::Package() :
		PackageAbstract("ES-Package")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void ES::Package::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::ES::AdaptOneFifthRuleOp", new ES::AdaptOneFifthRuleOp::Alloc);
	lFactory.insertAllocator("Beagle::ES::AlgoOneFifthRule", new ES::AlgoOneFifthRule::Alloc);
	lFactory.insertAllocator("Beagle::ES::Package", new ES::Package::Alloc);
	lFactory.insertAllocator("Beagle::ES::RecombinationOp", new ES::RecombinationOp::Alloc);
	lFactory.insertAllocator("Beagle::ES::RecombinationWeightedOp", new ES::RecombinationWeightedOp::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::ES::AdaptOneFifthRuleOp", "ES-AdaptOneFifthRuleOp");
	lFactory.aliasAllocator("Beagle::ES::AlgoOneFifthRule", "ES-AlgoOneFifthRule");
	lFactory.aliasAllocator("Beagle::ES::Package", "ES-Package");
	lFactory.aliasAllocator("Beagle::ES::RecombinationOp", "ES-RecombinationOp");
	lFactory.aliasAllocator("Beagle::ES::RecombinationWeightedOp", "ES-RecombinationWeightedOp");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Algorithm", "ES-AlgoOneFifthRule");
	lFactory.setConcept("Genotype", "FltVec-FloatVector");
	lFactory.setConcept("InitializationOp", "FltVec-InitGaussianOp");
	lFactory.setConcept("MutationOp", "FltVec-MutationGaussianOp");
	lFactory.setConcept("SelectionOp", "SelectRandomOp");

	// Add quasi-random component
	if(ioSystem.haveComponent("QuasiRandom") == NULL) {
		ioSystem.addComponent(new QuasiRandom);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  ES::Package depends on EC::Package and FltVec::Package.
 */
PackageAbstract::Bag ES::Package::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	PackageAbstract::Bag lDependencies;
	lDependencies.push_back(new EC::Package());
	lDependencies.push_back(new FltVec::Package());
	return lDependencies;
	Beagle_StackTraceEndM();
}
