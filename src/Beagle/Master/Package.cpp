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
 *  \file   beagle/Distrib/Master/src/Package.cpp
 *  \brief  Source code of class Distrib::Master::Package.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:33 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with master-slave distribution basic objects.
 */
Distrib::Master::Package::Package() :
		Beagle::Package("DBMS-Package")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void Distrib::Master::Package::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available master-slave types to the factory
	lFactory.insertAllocator("Beagle::Distrib::Master::AlgoCruncher", new Distrib::Master::AlgoCruncher::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::AlgoSequential", new Distrib::Master::AlgoSequential::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::BootStrapBeginOp", new Distrib::Master::BootStrapBeginOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::BootStrapEndOp", new Distrib::Master::BootStrapEndOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::DistribEvalOp", new Distrib::Master::DistribEvalOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::EnvironmentManager", new Distrib::Master::EnvironmentManager::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::FetchCruncherDataOp", new Distrib::Master::FetchCruncherDataOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::MainLoopBeginOp", new Distrib::Master::MainLoopBeginOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::MainLoopEndOp", new Distrib::Master::MainLoopEndOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::MilestoneWriteOp", new Distrib::Master::MilestoneWriteOp::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Master::Package", new Distrib::Master::Package::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::Distrib::Master::AlgoCruncher", "DBMS-AlgoCruncher");
	lFactory.aliasAllocator("Beagle::Distrib::Master::AlgoSequential", "DBMS-AlgoSequential");
	lFactory.aliasAllocator("Beagle::Distrib::Master::BootStrapBeginOp", "DBMS-BootStrapBeginOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::BootStrapEndOp", "DBMS-BootStrapEndOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::DistribEvalOp", "DBMS-DistribEvalOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::EnvironmentManager", "DBMS-EnvironmentManager");
	lFactory.aliasAllocator("Beagle::Distrib::Master::FetchCruncherDataOp", "DBMS-FetchCruncherDataOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::MainLoopBeginOp", "DBMS-MainLoopBeginOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::MainLoopEndOp", "DBMS-MainLoopEndOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::MilestoneWriteOp", "DBMS-MilestoneWriteOp");
	lFactory.aliasAllocator("Beagle::Distrib::Master::Package", "DBMS-Package");

	// Set default algorithm concept
	lFactory.setConcept("Algorithm", "DBMS-AlgoSequential");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  Distrib::Master::Package depends on Beagle::PackageBase.
 */
Beagle::Package::Bag Distrib::Master::Package::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new PackageBase());
	return lDependencies;
	Beagle_StackTraceEndM();
}
