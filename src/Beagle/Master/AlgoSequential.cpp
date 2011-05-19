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
 *  \file   beagle/Distrib/Master/src/AlgoSequential.cpp
 *  \brief  Source code of class Distrib::Master::AlgoSequential.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/10/02 15:26:46 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;


/*!
 *  \brief Construct master-slave cruncher evolutionary algorithm.
 */
Distrib::Master::AlgoSequential::AlgoSequential(void) :
		Algorithm("DBMS-AlgoSequential")
{ }


/*!
 *  \brief Configure master-slave cruncher evolutionary algorithm in evolver.
 *  \param ioEvolver Evolver modified by setting the algorithm.
 *  \param ioSystem Evolutionary system.
 *
 */
void Distrib::Master::AlgoSequential::configure(Evolver& ioEvolver, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Get reference to the factory
	const Factory& lFactory = ioSystem.getFactory();

	// Get name and allocator of used operators
	std::string lEvalOpName = "DBMS-DistribEvalOp";
	Distrib::Master::DistribEvalOp::Alloc::Handle lEvalOpAlloc =
	    castHandleT<Distrib::Master::DistribEvalOp::Alloc>(lFactory.getAllocator(lEvalOpName));
	std::string lSelectOpName = lFactory.getConceptTypeName("SelectionOp");
	SelectionOp::Alloc::Handle lSelectOpAlloc =
	    castHandleT<SelectionOp::Alloc>(lFactory.getAllocator(lSelectOpName));
	std::string lInitOpName = lFactory.getConceptTypeName("InitializationOp");
	InitializationOp::Alloc::Handle lInitOpAlloc =
	    castHandleT<InitializationOp::Alloc>(lFactory.getAllocator(lInitOpName));
	std::string lCxOpName = lFactory.getConceptTypeName("CrossoverOp");
	CrossoverOp::Alloc::Handle lCxOpAlloc =
	    castHandleT<CrossoverOp::Alloc>(lFactory.getAllocator(lCxOpName));
	std::string lMutOpName = lFactory.getConceptTypeName("MutationOp");
	MutationOp::Alloc::Handle lMutOpAlloc =
	    castHandleT<MutationOp::Alloc>(lFactory.getAllocator(lMutOpName));
	std::string lMigOpName = lFactory.getConceptTypeName("MigrationOp");
	MigrationOp::Alloc::Handle lMigOpAlloc =
	    castHandleT<MigrationOp::Alloc>(lFactory.getAllocator(lMigOpName));
	std::string lStatsCalcOpName = lFactory.getConceptTypeName("StatsCalculateOp");
	StatsCalculateOp::Alloc::Handle lStatsCalcOpAlloc =
	    castHandleT<StatsCalculateOp::Alloc>(lFactory.getAllocator(lStatsCalcOpName));
	std::string lTermOpName = lFactory.getConceptTypeName("TerminationOp");
	TerminationOp::Alloc::Handle lTermOpAlloc =
	    castHandleT<TerminationOp::Alloc>(lFactory.getAllocator(lTermOpName));
	std::string lITEOpName = "IfThenElseOp";
	IfThenElseOp::Alloc::Handle lITEOpAlloc =
	    castHandleT<IfThenElseOp::Alloc>(lFactory.getAllocator(lITEOpName));
	std::string lMsWriteOpName = "DBMS-MilestoneWriteOp";
	Distrib::Master::MilestoneWriteOp::Alloc::Handle lMsWriteOpAlloc =
	    castHandleT<MilestoneWriteOp::Alloc>(lFactory.getAllocator(lMsWriteOpName));
	std::string lBSBeginOpName = "DBMS-BootStrapBeginOp";
	BootStrapBeginOp::Alloc::Handle lBSBeginOpAlloc =
	    castHandleT<BootStrapBeginOp::Alloc>(lFactory.getAllocator(lBSBeginOpName));
	std::string lBSEndOpName = "DBMS-BootStrapEndOp";
	BootStrapEndOp::Alloc::Handle lBSEndOpAlloc =
	    castHandleT<BootStrapEndOp::Alloc>(lFactory.getAllocator(lBSEndOpName));
	std::string lMLBeginOpName = "DBMS-MainLoopBeginOp";
	MainLoopBeginOp::Alloc::Handle lMLBeginOpAlloc =
	    castHandleT<MainLoopBeginOp::Alloc>(lFactory.getAllocator(lMLBeginOpName));
	std::string lMLEndOpName = "DBMS-MainLoopEndOp";
	MainLoopEndOp::Alloc::Handle lMLEndOpAlloc =
	    castHandleT<MainLoopEndOp::Alloc>(lFactory.getAllocator(lMLEndOpName));

	// Clear bootstrap and mainloop sets
	ioEvolver.getBootStrapSet().clear();
	ioEvolver.getMainLoopSet().clear();

	// Set the boostrap operator set
	BootStrapBeginOp::Handle lBSBeginOpBS = castHandleT<BootStrapBeginOp>(lBSBeginOpAlloc->allocate());
	lBSBeginOpBS->setName(lBSBeginOpName);
	ioEvolver.getBootStrapSet().push_back(lBSBeginOpBS);
	IfThenElseOp::Handle lITEOp = castHandleT<IfThenElseOp>(lITEOpAlloc->allocate());
	lITEOp->setName(lITEOpName);
	ioEvolver.getBootStrapSet().push_back(lITEOp);
	lITEOp->setConditionTag("db.restart");
	lITEOp->setConditionValue("1");
	InitializationOp::Handle lInitOpBS = castHandleT<InitializationOp>(lInitOpAlloc->allocate());
	lInitOpBS->setName(lInitOpName);
	lITEOp->insertNegativeOp(lInitOpBS);
	EvaluationOp::Handle lEvalOpBS = castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	lEvalOpBS->setName(lEvalOpName);
	lITEOp->insertNegativeOp(lEvalOpBS);
	MigrationOp::Handle lMigOpBS = castHandleT<MigrationOp>(lMigOpAlloc->allocate());
	lMigOpBS->setName(lMigOpName);
	lITEOp->insertNegativeOp(lMigOpBS);
	StatsCalculateOp::Handle lStatsCalcOpBS =
	    castHandleT<StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
	lStatsCalcOpBS->setName(lStatsCalcOpName);
	lITEOp->insertNegativeOp(lStatsCalcOpBS);
	TerminationOp::Handle lTermOpBS = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
	lTermOpBS->setName(lTermOpName);
	lITEOp->insertNegativeOp(lTermOpBS);
	MilestoneWriteOp::Handle lMsWriteOpBS = castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
	lMsWriteOpBS->setName(lMsWriteOpName);
	lITEOp->insertNegativeOp(lMsWriteOpBS);
	BootStrapEndOp::Handle lBSEndOpBS = castHandleT<BootStrapEndOp>(lBSEndOpAlloc->allocate());
	lBSEndOpBS->setName(lBSEndOpName);
	ioEvolver.getBootStrapSet().push_back(lBSEndOpBS);

	// Set the mainloop operator set
	MainLoopBeginOp::Handle lMLBeginOpML = castHandleT<MainLoopBeginOp>(lMLBeginOpAlloc->allocate());
	lMLBeginOpML->setName(lMLBeginOpName);
	ioEvolver.getMainLoopSet().push_back(lMLBeginOpML);
	SelectionOp::Handle lSelectOpML = castHandleT<SelectionOp>(lSelectOpAlloc->allocate());
	lSelectOpML->setName(lSelectOpName);
	ioEvolver.getMainLoopSet().push_back(lSelectOpML);
	CrossoverOp::Handle lCxOpML = castHandleT<CrossoverOp>(lCxOpAlloc->allocate());
	lCxOpML->setName(lCxOpName);
	ioEvolver.getMainLoopSet().push_back(lCxOpML);
	MutationOp::Handle lMutOpML = castHandleT<MutationOp>(lMutOpAlloc->allocate());
	lMutOpML->setName(lMutOpName);
	ioEvolver.getMainLoopSet().push_back(lMutOpML);
	EvaluationOp::Handle lEvalOpML = castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	lEvalOpML->setName(lEvalOpName);
	ioEvolver.getMainLoopSet().push_back(lEvalOpML);
	MigrationOp::Handle lMigOpML = castHandleT<MigrationOp>(lMigOpAlloc->allocate());
	lMigOpML->setName(lMigOpName);
	ioEvolver.getMainLoopSet().push_back(lMigOpML);
	StatsCalculateOp::Handle lStatsCalcOpML =
	    castHandleT<StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
	lStatsCalcOpML->setName(lStatsCalcOpName);
	ioEvolver.getMainLoopSet().push_back(lStatsCalcOpML);
	TerminationOp::Handle lTermOpML = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
	lTermOpML->setName(lTermOpName);
	ioEvolver.getMainLoopSet().push_back(lTermOpML);
	MilestoneWriteOp::Handle lMsWriteOpML = castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
	lMsWriteOpML->setName(lMsWriteOpName);
	ioEvolver.getMainLoopSet().push_back(lMsWriteOpML);
	MainLoopEndOp::Handle lMLEndOpML = castHandleT<MainLoopEndOp>(lMLEndOpAlloc->allocate());
	lMLEndOpML->setName(lMLEndOpName);
	ioEvolver.getMainLoopSet().push_back(lMLEndOpML);

	Beagle_StackTraceEndM();
}
