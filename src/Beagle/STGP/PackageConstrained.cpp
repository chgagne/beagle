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
 *  \file   beagle/GP/src/PackageConstrained.cpp
 *  \brief  Source code of class GP::PackageConstrained.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with constrained GP operators.
 */
GP::PackageConstrained::PackageConstrained() :
		Beagle::Package("GP-PackageConstrained"),
		mPrimitiveSuperSet(new GP::PrimitiveSuperSet)
{ }


/*!
 *  \brief Construct package with constrained GP operators.
 *  \param inPrimitiveSuperSet Super set of primitives to use.
 */
GP::PackageConstrained::PackageConstrained(GP::PrimitiveSuperSet::Handle inPrimitiveSuperSet) :
		Beagle::Package("GP-PackageConstrained"),
		mPrimitiveSuperSet(inPrimitiveSuperSet)
{ }


/*!
 *  \brief Construct package with constrained GP operators.
 *  \param inPrimitiveSet Set of primitives to use.
 */
GP::PackageConstrained::PackageConstrained(GP::PrimitiveSet::Handle inPrimitiveSet) :
		Beagle::Package("GP-PackageConstrained"),
		mPrimitiveSuperSet(new GP::PrimitiveSuperSet)
{
	Beagle_StackTraceBeginM();
	mPrimitiveSuperSet->insert(inPrimitiveSet);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void GP::PackageConstrained::configure(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::GP::CrossoverConstrainedOp", new GP::CrossoverConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::InitFullConstrainedOp", new GP::InitFullConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::InitGrowConstrainedOp", new GP::InitGrowConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::InitHalfConstrainedOp", new GP::InitHalfConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationInsertConstrainedOp", new GP::MutationInsertConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationShrinkConstrainedOp", new GP::MutationShrinkConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationStandardConstrainedOp", new GP::MutationStandardConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationSwapConstrainedOp", new GP::MutationSwapConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationSwapSubtreeConstrainedOp", new GP::MutationSwapSubtreeConstrainedOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::PackageConstrained", new GP::PackageConstrained::Alloc);
	lFactory.insertAllocator("Beagle::GP::ValidateOp", new GP::ValidateOp::Alloc);

	// Set aliases
	lFactory.aliasAllocator("Beagle::GP::CrossoverConstrainedOp", "GP-CrossoverConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::InitFullConstrainedOp", "GP-InitFullConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::InitGrowConstrainedOp", "GP-InitGrowConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::InitHalfConstrainedOp", "GP-InitHalfConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::MutationInsertConstrainedOp", "GP-MutationInsertConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::MutationShrinkConstrainedOp", "GP-MutationShrinkConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::MutationStandardConstrainedOp", "GP-MutationStandardConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::MutationSwapConstrainedOp", "GP-MutationSwapConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::MutationSwapSubtreeConstrainedOp", "GP-MutationSwapSubtreeConstrainedOp");
	lFactory.aliasAllocator("Beagle::GP::PackageConstrained", "GP-PackageConstrained");
	lFactory.aliasAllocator("Beagle::GP::ValidateOp", "GP-ValidateOp");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("CrossoverOp", "GP-CrossoverConstrainedOp");
	lFactory.setConcept("InitializationOp", "GP-InitHalfConstrainedOp");
	lFactory.setConcept("MutationOp", "GP-MutationStandardConstrainedOp");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages on which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  GP::PackageConstrained depends on Beagle::PackageBase and GP::PackageBase.
 */
Beagle::Package::Bag GP::PackageConstrained::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new Beagle::PackageBase());
	lDependencies.push_back(new GP::PackageBase(mPrimitiveSuperSet));
	return lDependencies;
	Beagle_StackTraceEndM();
}
