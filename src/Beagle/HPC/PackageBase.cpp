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
 *  \file   beagle/HPC/PackageBase.cpp
 *  \brief  Source code of class HPC::PackageBase.
 *  \author Christian Gagne
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;


/*!
 *  \brief Construct the HPC base package.
 */
HPC::PackageBase::PackageBase() :
		Package("HPC-PackageBase")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void HPC::PackageBase::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::HPC::DistributeDemeToEvaluatorsOp", new HPC::DistributeDemeToEvaluatorsOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::SendDemeToEvolverOp", new HPC::SendDemeToEvolverOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::SendDemeToSupervisorOp", new HPC::SendDemeToSupervisorOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::SendFitnessToEvolverOp", new HPC::SendFitnessToEvolverOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::SendProcessedToSupervisorOp", new HPC::SendProcessedToSupervisorOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::RecvDemeFromEvolverOp", new HPC::RecvDemeFromEvolverOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::RecvDemeFromSupervisorOp", new HPC::RecvDemeFromSupervisorOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::RecvFitnessFromEvaluatorOp", new HPC::RecvFitnessFromEvaluatorOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::RecvIndividualsFromEvolverOp", new HPC::RecvIndividualsFromEvolverOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::RecvProcessedFromEvolverOp", new HPC::RecvProcessedFromEvolverOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::SwitchTypeOp", new HPC::SwitchTypeOp::Alloc);
	lFactory.insertAllocator("Beagle::HPC::LoggerXMLD", new HPC::LoggerXMLD::Alloc);
	lFactory.insertAllocator("Beagle::HPC::Randomizer", new HPC::Randomizer::Alloc);
	lFactory.insertAllocator("Beagle::HPC::AlgoSequential", new HPC::AlgoSequential::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::HPC::DistributeDemeToEvaluatorsOp", "HPC-DistributeDemeToEvaluatorsOp");
	lFactory.aliasAllocator("Beagle::HPC::SendDemeToEvolverOp", "HPC-SendDemeToEvolverOp");
	lFactory.aliasAllocator("Beagle::HPC::SendDemeToSupervisorOp", "HPC-SendDemeToSupervisorOp");
	lFactory.aliasAllocator("Beagle::HPC::SendFitnessToEvolverOp", "HPC-SendFitnessToEvolverOp");
	lFactory.aliasAllocator("Beagle::HPC::SendProcessedToSupervisorOp", "HPC-SendProcessedToSupervisorOp");
	lFactory.aliasAllocator("Beagle::HPC::RecvDemeFromEvolverOp", "HPC-RecvDemeFromEvolverOp");
	lFactory.aliasAllocator("Beagle::HPC::RecvDemeFromSupervisorOp", "HPC-RecvDemeFromSupervisorOp");
	lFactory.aliasAllocator("Beagle::HPC::RecvFitnessFromEvaluatorOp", "HPC-RecvFitnessFromEvaluatorOp");
	lFactory.aliasAllocator("Beagle::HPC::RecvIndividualsFromEvolverOp", "HPC-RecvIndividualsFromEvolverOp");
	lFactory.aliasAllocator("Beagle::HPC::RecvProcessedFromEvolverOp","HPC-RecvProcessedFromEvolverOp");
 	lFactory.aliasAllocator("Beagle::HPC::LoggerXMLD", "HPC-LoggerXMLD");
	lFactory.aliasAllocator("Beagle::HPC::Randomizer", "HPC-Randomizer");
	lFactory.aliasAllocator("Beagle::HPC::AlgoSequential", "HPC-AlgoSequential");
	lFactory.aliasAllocator("Beagle::HPC::SwitchTypeOp", "SwitchTypeOp");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Logger", "HPC-LoggerXMLD");
	lFactory.setConcept("Randomizer", "HPC-Randomizer");
	lFactory.setConcept("Algorithm", "HPC-AlgoSequential");

	// Add components
	if(ioSystem.haveComponent("MPICommunication") == 0) {
		ioSystem.addComponent(HPC::MPICommunication::getInstance());
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  HPC::PackageBase depends on Beagle::PackageBase.
 */
Package::Bag HPC::PackageBase::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new Beagle::PackageBase);
	return lDependencies;
	Beagle_StackTraceEndM();
}
