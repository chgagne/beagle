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
 *  \file   beagle/HPC/AlgoSequential.cpp
 *  \brief  Source code of class AlgoSequential.
 *  \author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  $Revision: $
 *  $Date: $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

typedef HPC::SendDemeToEvolverOp SD2EOp;
typedef HPC::RecvProcessedFromEvolverOp RPfEOp;
typedef HPC::RecvDemeFromSupervisorOp RDfSOp;
typedef HPC::DistributeDemeToEvaluatorsOp DD2EOp;
typedef HPC::RecvFitnessFromEvaluatorOp RFfEOp;
typedef HPC::SendProcessedToSupervisorOp SP2SOp;
typedef HPC::RecvIndividualsFromEvolverOp RIfEOp;
typedef HPC::SendFitnessToEvolverOp SF2EOp;

/*!
 *  \brief Construct sequential evolutionary algorithm.
 */
HPC::AlgoSequential::AlgoSequential(void) :
		Algorithm("HPC-AlgoSequential")
{ }


/*!
 *  \brief Configure sequential evolutionary algorithm in evolver using a master-slave topology.
 *  \param ioEvolver Evolver modified by setting the algorithm.
 *  \param ioSystem Evolutionary system.
 *
 */
void HPC::AlgoSequential::configure(Evolver& ioEvolver, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Get reference to the factory
	const Factory& lFactory = ioSystem.getFactory();

	// Get name and allocator of used operators
	std::string lEvalOpName = lFactory.getConceptTypeName("EvaluationOp");
	Beagle::EvaluationOp::Alloc::Handle lEvalOpAlloc = castHandleT<Beagle::EvaluationOp::Alloc>(lFactory.getAllocator(lEvalOpName));

	std::string lSelectOpName = lFactory.getConceptTypeName("SelectionOp");
	SelectionOp::Alloc::Handle lSelectOpAlloc = castHandleT<SelectionOp::Alloc>(lFactory.getAllocator(lSelectOpName));

	std::string lInitOpName = lFactory.getConceptTypeName("InitializationOp");
	InitializationOp::Alloc::Handle lInitOpAlloc = castHandleT<InitializationOp::Alloc>(lFactory.getAllocator(lInitOpName));

	std::string lCxOpName = lFactory.getConceptTypeName("CrossoverOp");
	CrossoverOp::Alloc::Handle lCxOpAlloc = castHandleT<CrossoverOp::Alloc>(lFactory.getAllocator(lCxOpName));

	std::string lMutOpName = lFactory.getConceptTypeName("MutationOp");
	MutationOp::Alloc::Handle lMutOpAlloc = castHandleT<MutationOp::Alloc>(lFactory.getAllocator(lMutOpName));

	std::string lMigOpName = lFactory.getConceptTypeName("MigrationOp");
	MigrationOp::Alloc::Handle lMigOpAlloc = castHandleT<MigrationOp::Alloc>(lFactory.getAllocator(lMigOpName));

	std::string lStatsCalcOpName = lFactory.getConceptTypeName("StatsCalculateOp");
	StatsCalculateOp::Alloc::Handle lStatsCalcOpAlloc = castHandleT<StatsCalculateOp::Alloc>(lFactory.getAllocator(lStatsCalcOpName));

	std::string lTermOpName = lFactory.getConceptTypeName("TerminationOp");
	TerminationOp::Alloc::Handle lTermOpAlloc = castHandleT<TerminationOp::Alloc>(lFactory.getAllocator(lTermOpName));

	std::string lMsWriteOpName = "MilestoneWriteOp";
	MilestoneWriteOp::Alloc::Handle lMsWriteOpAlloc = castHandleT<MilestoneWriteOp::Alloc>(lFactory.getAllocator(lMsWriteOpName));

	// Clear bootstrap and mainloop sets
	ioEvolver.getBootStrapSet().clear();
	ioEvolver.getMainLoopSet().clear();

	Operator::Bag& lBootStrapSet = ioEvolver.getBootStrapSet();
	Operator::Bag& lMainLoopSet = ioEvolver.getMainLoopSet();

	// Set the bootstrap operator set
	SwitchTypeOp::Alloc::Handle lSwitchTypeOpAlloc = castHandleT<SwitchTypeOp::Alloc>(lFactory.getAllocator("SwitchTypeOp"));
	SwitchTypeOp::Handle lNodeTypeSwitch = castHandleT<SwitchTypeOp>(lSwitchTypeOpAlloc->allocate());
	lBootStrapSet.push_back( lNodeTypeSwitch );

	//Set the supervisor bootstrap set
	{
		InitializationOp::Handle lInitOpBS = castHandleT<InitializationOp>(lInitOpAlloc->allocate());
		lInitOpBS->setName(lInitOpName);
		lNodeTypeSwitch->insertOp("Supervisor", lInitOpBS );

		SD2EOp::Alloc::Handle lDVEOpAlloc = castHandleT<SD2EOp::Alloc>(lFactory.getAllocator("HPC-SendDemeToEvolverOp"));
 		SD2EOp::Handle lSD2EOpBS = castHandleT<SD2EOp>(lDVEOpAlloc->allocate());
 		lNodeTypeSwitch->insertOp("Supervisor", lSD2EOpBS );

		RPfEOp::Alloc::Handle lRPfEOpAlloc = castHandleT<RPfEOp::Alloc>(lFactory.getAllocator("HPC-RecvProcessedFromEvolverOp"));
		RPfEOp::Handle lRPfEOpBS = castHandleT<RPfEOp>(lRPfEOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Supervisor", lRPfEOpBS );

                StatsCalculateOp::Handle lStatsCalcOpBS = castHandleT<StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
                lStatsCalcOpBS->setName(lStatsCalcOpName);
                lNodeTypeSwitch->insertOp("Supervisor", lStatsCalcOpBS);

		TerminationOp::Handle lTermOpBS = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpBS->setName(lTermOpName);
		lNodeTypeSwitch->insertOp("Supervisor", lTermOpBS );

		MilestoneWriteOp::Handle lMsWriteOpBS = castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
		lMsWriteOpBS->setName(lMsWriteOpName);
		lNodeTypeSwitch->insertOp("Supervisor", lMsWriteOpBS );
	}
	//Set the evolver bootstrap set
	{
		RDfSOp::Alloc::Handle lRDfSOpAlloc = castHandleT<RDfSOp::Alloc>(lFactory.getAllocator("HPC-RecvDemeFromSupervisorOp"));
		RDfSOp::Handle lRDfSOpBS = castHandleT<RDfSOp>(lRDfSOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evolver", lRDfSOpBS );

		DD2EOp::Alloc::Handle lDD2EOpAlloc = castHandleT<DD2EOp::Alloc>(lFactory.getAllocator("HPC-DistributeDemeToEvaluatorsOp"));
		DD2EOp::Handle lDD2EOpBS = castHandleT<DD2EOp>(lDD2EOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evolver", lDD2EOpBS );

		RFfEOp::Alloc::Handle lRFfEOpAlloc = castHandleT<RFfEOp::Alloc>(lFactory.getAllocator("HPC-RecvFitnessFromEvaluatorOp"));
		RFfEOp::Handle lRFfEOpBS = castHandleT<RFfEOp>(lRFfEOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evolver", lRFfEOpBS );

		SP2SOp::Alloc::Handle lSP2SOpAlloc = castHandleT<SP2SOp::Alloc>(lFactory.getAllocator("HPC-SendProcessedToSupervisorOp"));
		SP2SOp::Handle lSP2SOpBS = castHandleT<SP2SOp>(lSP2SOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evolver", lSP2SOpBS );

		TerminationOp::Handle lTermOpBS = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpBS->setName(lTermOpName);
		lNodeTypeSwitch->insertOp("Evolver", lTermOpBS );
	}
	//Set the evaluator bootstrap set
	{
		RIfEOp::Alloc::Handle lRIfEOpAlloc = castHandleT<RIfEOp::Alloc>(lFactory.getAllocator("HPC-RecvIndividualsFromEvolverOp"));
		RIfEOp::Handle lRIfEOpBS = castHandleT<RIfEOp>(lRIfEOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evaluator", lRIfEOpBS );

		Beagle::EvaluationOp::Handle lEvalOpBS = castHandleT<Beagle::EvaluationOp>(lEvalOpAlloc->allocate());
		lEvalOpBS->setName(lEvalOpName);
		lNodeTypeSwitch->insertOp("Evaluator", lEvalOpBS );

		SF2EOp::Alloc::Handle lSF2EOpAlloc = castHandleT<SF2EOp::Alloc>(lFactory.getAllocator("HPC-SendFitnessToEvolverOp"));
		SF2EOp::Handle lSF2EOpBS = castHandleT<SF2EOp>(lSF2EOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evaluator", lSF2EOpBS );

		TerminationOp::Handle lTermOpBS = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpBS->setName(lTermOpName);
		lNodeTypeSwitch->insertOp("Evaluator", lTermOpBS );
	}
	//Set the main loop set
	SwitchTypeOp::Handle lNodeTypeSwitchML = castHandleT<SwitchTypeOp>(lSwitchTypeOpAlloc->allocate());
	lMainLoopSet.push_back( lNodeTypeSwitchML );

	//Set the supervistor mainloop set
	{
		RPfEOp::Alloc::Handle lRPfEOpAlloc = castHandleT<RPfEOp::Alloc>(lFactory.getAllocator("HPC-RecvProcessedFromEvolverOp"));
		RPfEOp::Handle lRPfEOpBS = castHandleT<RPfEOp>(lRPfEOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Supervisor",  lRPfEOpBS );

                StatsCalculateOp::Handle lStatsCalcOpML = castHandleT<StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
                lStatsCalcOpML->setName(lStatsCalcOpName);
                lNodeTypeSwitchML->insertOp("Supervisor",  lStatsCalcOpML);

		TerminationOp::Handle lTermOpML = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpML->setName(lTermOpName);
		lNodeTypeSwitchML->insertOp("Supervisor",  lTermOpML );

		MilestoneWriteOp::Handle lMsWriteOpML = castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
		lMsWriteOpML->setName(lMsWriteOpName);
		lNodeTypeSwitchML->insertOp("Supervisor",  lMsWriteOpML );
	}
	//Set the evolver mainloop set
	{
		SelectionOp::Handle lSelectOpML = castHandleT<SelectionOp>(lSelectOpAlloc->allocate());
		lSelectOpML->setName(lSelectOpName);
		lNodeTypeSwitchML->insertOp("Evolver", lSelectOpML );

		CrossoverOp::Handle lCxOpML = castHandleT<CrossoverOp>(lCxOpAlloc->allocate());
		lCxOpML->setName(lCxOpName);
		lNodeTypeSwitchML->insertOp("Evolver", lCxOpML );

		MutationOp::Handle lMutOpML = castHandleT<MutationOp>(lMutOpAlloc->allocate());
		lMutOpML->setName(lMutOpName);
		lNodeTypeSwitchML->insertOp("Evolver", lMutOpML );

		DD2EOp::Alloc::Handle lDD2EOpAlloc = castHandleT<DD2EOp::Alloc>(lFactory.getAllocator("HPC-DistributeDemeToEvaluatorsOp"));
		DD2EOp::Handle lDD2EOpML = castHandleT<DD2EOp>(lDD2EOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lDD2EOpML );

		RFfEOp::Alloc::Handle lRFfEOpAlloc = castHandleT<RFfEOp::Alloc>(lFactory.getAllocator("HPC-RecvFitnessFromEvaluatorOp"));
		RFfEOp::Handle lRFfEOpML = castHandleT<RFfEOp>(lRFfEOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lRFfEOpML );

		SP2SOp::Alloc::Handle lSP2SOpAlloc = castHandleT<SP2SOp::Alloc>(lFactory.getAllocator("HPC-SendProcessedToSupervisorOp"));
		SP2SOp::Handle lSP2SOpML = castHandleT<SP2SOp>(lSP2SOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lSP2SOpML );

		TerminationOp::Handle lTermOpML = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpML->setName(lTermOpName);
		lNodeTypeSwitchML->insertOp("Evolver", lTermOpML );
	}
	//Set the evaluator mainloop set
	{
		RIfEOp::Alloc::Handle lRIfEOpAlloc = castHandleT<RIfEOp::Alloc>(lFactory.getAllocator("HPC-RecvIndividualsFromEvolverOp"));
		RIfEOp::Handle lRIfEOpML = castHandleT<RIfEOp>(lRIfEOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evaluator", lRIfEOpML );

		Beagle::EvaluationOp::Handle lEvalOpML = castHandleT<Beagle::EvaluationOp>(lEvalOpAlloc->allocate());
		lEvalOpML->setName(lEvalOpName);
		lNodeTypeSwitchML->insertOp("Evaluator", lEvalOpML );

		SF2EOp::Alloc::Handle lSF2EOpAlloc = castHandleT<SF2EOp::Alloc>(lFactory.getAllocator("HPC-SendFitnessToEvolverOp"));
		SF2EOp::Handle lSF2EOpML = castHandleT<SF2EOp>(lSF2EOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evaluator", lSF2EOpML );

		TerminationOp::Handle lTermOpML = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpML->setName(lTermOpName);
		lNodeTypeSwitchML->insertOp("Evaluator", lTermOpML );
	}

	Beagle_StackTraceEndM();
}
