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
 *  \file   beagle/GA/src/PackageCMAES.cpp
 *  \brief  Source code of class GA::PackageCMAES.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "beagle/GA.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with CMA-ES objects.
 */
GA::PackageCMAES::PackageCMAES(unsigned int inInitSize) :
		Package("GA-PackageCMAES"),
		mInitSize(inInitSize)
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void GA::PackageCMAES::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::GA::AlgoCMAES", new GA::AlgoCMAES::Alloc);
	lFactory.insertAllocator("Beagle::GA::CMAHolder", new GA::CMAHolder::Alloc);
	lFactory.insertAllocator("Beagle::GA::InitCMAFltVecOp", new GA::InitCMAFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationCMAFltVecOp", new GA::MutationCMAFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MutationQRCMAFltVecOp", new GA::MutationQRCMAFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::MuWCommaLambdaCMAFltVecOp", new GA::MuWCommaLambdaCMAFltVecOp::Alloc);
	lFactory.insertAllocator("Beagle::GA::PackageCMAES", new GA::PackageCMAES::Alloc);
	lFactory.insertAllocator("Beagle::GA::TermCMAOp", new GA::TermCMAOp::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::GA::AlgoCMAES", "GA-AlgoCMAES");
	lFactory.aliasAllocator("Beagle::GA::CMAHolder", "GA-CMAHolder");
	lFactory.aliasAllocator("Beagle::GA::InitCMAFltVecOp", "GA-InitCMAFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::MutationCMAFltVecOp", "GA-MutationCMAFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::MutationQRCMAFltVecOp", "GA-MutationQRCMAFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::MuWCommaLambdaCMAFltVecOp", "GA-MuWCommaLambdaCMAFltVecOp");
	lFactory.aliasAllocator("Beagle::GA::PackageCMAES", "GA-PackageCMAES");
	lFactory.aliasAllocator("Beagle::GA::TermCMAOp", "GA-TermCMAOp");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Algorithm", "GA-AlgoCMAES");
	lFactory.setConcept("InitializationOp", "GA-InitCMAFltVecOp");
	lFactory.setConcept("MutationOp", "GA-MutationCMAFltVecOp");
	lFactory.setConcept("TerminationOp", "GA-TermCMAOp");

	// Add components
	if(ioSystem.haveComponent("GA-CMAHolder") == NULL) {
		ioSystem.addComponent(new CMAHolder);
	}

	// Set default initialization size
	ioSystem.getRegister().setEntryDefaultValue("ga.init.vectorsize",
	        new UInt(mInitSize),
	        uint2str(mInitSize));

	// Set default pop size, as advised in CMA tutorial of Hansen
	const unsigned int lLambda = 4 + (unsigned int)std::floor(3. * std::log((double)mInitSize));
	const unsigned int lMu = (unsigned int)std::ceil((double(lLambda) - 1.) / 2.);
	ioSystem.getRegister().setEntryDefaultValue("ec.pop.size",
	        new UIntArray(1,lMu),
	        uint2str(lMu));
	ioSystem.getRegister().setEntryDefaultValue("ga.cmaes.mulambdaratio",
	        new Float(2.f),
	        "2.0");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  GA::PackageCMAES depends on PackageBase and GA::PackageFloatVector.
 */
Package::Bag GA::PackageCMAES::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new PackageBase());
	lDependencies.push_back(new GA::PackageFloatVector(mInitSize));
	return lDependencies;
	Beagle_StackTraceEndM();
}
