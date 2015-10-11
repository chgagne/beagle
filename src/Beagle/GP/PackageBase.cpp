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
 *  \file   beagle/GP/src/PackageBase.cpp
 *  \brief  Source code of class GP::PackageBase.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with basic GP objects.
 */
GP::PackageBase::PackageBase() :
		Beagle::Package("GP-PackageBase"),
		mPrimitiveSuperSet(new GP::PrimitiveSuperSet)
{ }


/*!
 *  \brief Construct package with basic GP objects.
 *  \param inPrimitiveSuperSet Super set of primitives to use.
 */
GP::PackageBase::PackageBase(GP::PrimitiveSuperSet::Handle inPrimitiveSuperSet) :
		Beagle::Package("GP-PackageBase"),
		mPrimitiveSuperSet(inPrimitiveSuperSet)
{ }


/*!
 *  \brief Construct package with basic GP objects.
 *  \param inPrimitiveSet Set of primitives to use.
 */
GP::PackageBase::PackageBase(GP::PrimitiveSet::Handle inPrimitiveSet) :
		Beagle::Package("GP-PackageBase"),
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
void GP::PackageBase::configure(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::GP::ADF", new GP::ADF::Alloc);
	lFactory.insertAllocator("Beagle::GP::And", new GP::And::Alloc);
	lFactory.insertAllocator("Beagle::GP::Argument", new GP::Argument::Alloc);
	lFactory.insertAllocator("Beagle::GP::BitwiseAnd", new GP::BitwiseAnd::Alloc);
	lFactory.insertAllocator("Beagle::GP::BitwiseNand", new GP::BitwiseNand::Alloc);
	lFactory.insertAllocator("Beagle::GP::BitwiseNor", new GP::BitwiseNor::Alloc);
	lFactory.insertAllocator("Beagle::GP::BitwiseNot", new GP::BitwiseNot::Alloc);
	lFactory.insertAllocator("Beagle::GP::BitwiseOr", new GP::BitwiseOr::Alloc);
	lFactory.insertAllocator("Beagle::GP::BitwiseXor", new GP::BitwiseXor::Alloc);
	lFactory.insertAllocator("Beagle::GP::Context", new GP::Context::Alloc);
	lFactory.insertAllocator("Beagle::GP::Cos", new GP::Cos::Alloc);
	lFactory.insertAllocator("Beagle::GP::CrossoverOp", new GP::CrossoverOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::EphemeralDouble", new GP::EphemeralDouble::Alloc);
	lFactory.insertAllocator("Beagle::GP::Exp", new GP::Exp::Alloc);
	lFactory.insertAllocator("Beagle::GP::FitnessKoza", new GP::FitnessKoza::Alloc);
	lFactory.insertAllocator("Beagle::GP::Individual", new GP::Individual::Alloc);
	lFactory.insertAllocator("Beagle::GP::InitFullOp", new GP::InitFullOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::InitGrowOp", new GP::InitGrowOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::InitHalfOp", new GP::InitHalfOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::Log", new GP::Log::Alloc);
	lFactory.insertAllocator("Beagle::GP::ModuleCompressOp", new GP::ModuleCompressOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::Module", new GP::Module::Alloc);
	lFactory.insertAllocator("Beagle::GP::ModuleExpandOp", new GP::ModuleExpandOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::ModuleVectorComponent", new GP::ModuleVectorComponent::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationInsertOp", new GP::MutationInsertOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationShrinkOp", new GP::MutationShrinkOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationStandardOp", new GP::MutationStandardOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationSwapOp", new GP::MutationSwapOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::MutationSwapSubtreeOp", new GP::MutationSwapSubtreeOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::Nand", new GP::Nand::Alloc);
	lFactory.insertAllocator("Beagle::GP::Nor", new GP::Nor::Alloc);
	lFactory.insertAllocator("Beagle::GP::Not", new GP::Not::Alloc);
	lFactory.insertAllocator("Beagle::GP::Or", new GP::Or::Alloc);
	lFactory.insertAllocator("Beagle::GP::PackageBase", new GP::PackageBase::Alloc);
	lFactory.insertAllocator("Beagle::GP::PrimitiveSet", new GP::PrimitiveSet::Alloc);
	lFactory.insertAllocator("Beagle::GP::PrimitiveSuperSet", new GP::PrimitiveSuperSet::Alloc);
	lFactory.insertAllocator("Beagle::GP::PrimitiveUsageStatsOp", new GP::PrimitiveUsageStatsOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::Sin", new GP::Sin::Alloc);
	lFactory.insertAllocator("Beagle::GP::StatsCalcFitnessKozaOp", new GP::StatsCalcFitnessKozaOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::StatsCalcFitnessSimpleOp", new GP::StatsCalcFitnessSimpleOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::TermMaxHitsOp", new GP::TermMaxHitsOp::Alloc);
	lFactory.insertAllocator("Beagle::GP::Tree", new GP::Tree::Alloc);
	lFactory.insertAllocator("Beagle::GP::Xor", new GP::Xor::Alloc);

	// Set aliases
	lFactory.aliasAllocator("Beagle::GP::ADF", "GP-ADF");
	lFactory.aliasAllocator("Beagle::GP::And", "GP-And");
	lFactory.aliasAllocator("Beagle::GP::Argument", "GP-Argument");
	lFactory.aliasAllocator("Beagle::GP::BitwiseAnd", "GP-BitwiseAnd");
	lFactory.aliasAllocator("Beagle::GP::BitwiseNand", "GP-BitwiseNand");
	lFactory.aliasAllocator("Beagle::GP::BitwiseNor", "GP-BitwiseNor");
	lFactory.aliasAllocator("Beagle::GP::BitwiseNot", "GP-BitwiseNot");
	lFactory.aliasAllocator("Beagle::GP::BitwiseOr", "GP-BitwiseOr");
	lFactory.aliasAllocator("Beagle::GP::BitwiseXor", "GP-BitwiseXor");
	lFactory.aliasAllocator("Beagle::GP::Context", "GP-Context");
	lFactory.aliasAllocator("Beagle::GP::Cos", "GP-Cos");
	lFactory.aliasAllocator("Beagle::GP::CrossoverOp", "GP-CrossoverOp");
	lFactory.aliasAllocator("Beagle::GP::EphemeralDouble", "GP-EphemeralDouble");
	lFactory.aliasAllocator("Beagle::GP::Exp", "GP-Exp");
	lFactory.aliasAllocator("Beagle::GP::FitnessKoza", "GP-FitnessKoza");
	lFactory.aliasAllocator("Beagle::GP::Individual", "GP-Individual");
	lFactory.aliasAllocator("Beagle::GP::InitFullOp", "GP-InitFullOp");
	lFactory.aliasAllocator("Beagle::GP::InitGrowOp", "GP-InitGrowOp");
	lFactory.aliasAllocator("Beagle::GP::InitHalfOp", "GP-InitHalfOp");
	lFactory.aliasAllocator("Beagle::GP::Log", "GP-Log");
	lFactory.aliasAllocator("Beagle::GP::ModuleCompressOp", "GP-ModuleCompressOp");
	lFactory.aliasAllocator("Beagle::GP::Module", "GP-Module");
	lFactory.aliasAllocator("Beagle::GP::ModuleExpandOp", "GP-ModuleExpandOp");
	lFactory.aliasAllocator("Beagle::GP::ModuleVectorComponent", "GP-ModuleVectorComponent");
	lFactory.aliasAllocator("Beagle::GP::MutationInsertOp", "GP-MutationInsertOp");
	lFactory.aliasAllocator("Beagle::GP::MutationShrinkOp", "GP-MutationShrinkOp");
	lFactory.aliasAllocator("Beagle::GP::MutationStandardOp", "GP-MutationStandardOp");
	lFactory.aliasAllocator("Beagle::GP::MutationSwapOp", "GP-MutationSwapOp");
	lFactory.aliasAllocator("Beagle::GP::MutationSwapSubtreeOp", "GP-MutationSwapSubtreeOp");
	lFactory.aliasAllocator("Beagle::GP::Nand", "GP-Nand");
	lFactory.aliasAllocator("Beagle::GP::Nor", "GP-Nor");
	lFactory.aliasAllocator("Beagle::GP::Not", "GP-Not");
	lFactory.aliasAllocator("Beagle::GP::Or", "GP-Or");
	lFactory.aliasAllocator("Beagle::GP::PackageBase", "GP-PackageBase");
	lFactory.aliasAllocator("Beagle::GP::PrimitiveSet", "GP-PrimitiveSet");
	lFactory.aliasAllocator("Beagle::GP::PrimitiveSuperSet", "GP-PrimitiveSuperSet");
	lFactory.aliasAllocator("Beagle::GP::PrimitiveUsageStatsOp", "GP-PrimitiveUsageStatsOp");
	lFactory.aliasAllocator("Beagle::GP::Sin", "GP-Sin");
	lFactory.aliasAllocator("Beagle::GP::StatsCalcFitnessKozaOp", "GP-StatsCalcFitnessKozaOp");
	lFactory.aliasAllocator("Beagle::GP::StatsCalcFitnessSimpleOp", "GP-StatsCalcFitnessSimpleOp");
	lFactory.aliasAllocator("Beagle::GP::TermMaxHitsOp", "GP-TermMaxHitsOp");
	lFactory.aliasAllocator("Beagle::GP::Tree", "GP-Tree");
	lFactory.aliasAllocator("Beagle::GP::Xor", "GP-Xor");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Context", "GP-Context");
	lFactory.setConcept("CrossoverOp", "GP-CrossoverOp");
	lFactory.setConcept("Genotype", "GP-Tree");
	lFactory.setConcept("Individual", "GP-Individual");
	lFactory.setConcept("InitializationOp", "GP-InitHalfOp");
	lFactory.setConcept("MutationOp", "GP-MutationStandardOp");
	lFactory.setConcept("StatsCalculateOp", "GP-StatsCalcFitnessSimpleOp");
	lFactory.setConcept("GP-PrimitiveSet", "GP-PrimitiveSet");
	lFactory.setConcept("GP-PrimitiveSuperSet", "GP-PrimitiveSuperSet");

	// Add GP components
	ioSystem.addComponent(mPrimitiveSuperSet);
	ioSystem.addComponent(new GP::ModuleVectorComponent);

	// Set some default parameters
	ioSystem.getRegister().setEntryDefaultValue("ec.pop.size",
	        new UIntArray(1,500),
	        "500");
	ioSystem.getRegister().setEntryDefaultValue("ec.sel.tournsize",
	        new UInt(7),
	        "7");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages on which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  GP::PackageBase depends on Beagle::PackageBase.
 */
Beagle::Package::Bag GP::PackageBase::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new Beagle::PackageBase());
	return lDependencies;
	Beagle_StackTraceEndM();
}
