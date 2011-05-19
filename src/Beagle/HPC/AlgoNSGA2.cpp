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
 *  \file   beagle/HPC/AlgoNSGA2.cpp
 *  \brief  Source code of class AlgoNSGA2.
 *  \author Christian Gagne
 *  \author Felix-Antoine Fortin
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

typedef HPC::SendDemeToEvolverOp SD2EOp;
typedef HPC::RecvDemeFromEvolverOp RDfEOp;
typedef HPC::RecvDemeFromSupervisorOp RDfSOp;
typedef HPC::DistributeDemeToEvaluatorsOp DD2EOp;
typedef HPC::RecvFitnessFromEvaluatorOp RFfEOp;
typedef HPC::SendDemeToSupervisorOp SD2SOp;
typedef HPC::RecvIndividualsFromEvolverOp RIfEOp;
typedef HPC::SendFitnessToEvolverOp SF2EOp;

/*!
 *  \brief Construct NSGA2 evolutionary algorithm.
 */
HPC::AlgoNSGA2::AlgoNSGA2(void) :
		Algorithm("HPC-AlgoNSGA2")
{ }


/*!
 *  \brief Configure NSGA2 evolutionary algorithm in evolver using a master-slave topology.
 *  \param ioEvolver Evolver modified by setting the algorithm.
 *  \param ioSystem Evolutionary system.
 *
 */
void HPC::AlgoNSGA2::configure(Evolver& ioEvolver, System& ioSystem)
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

		RDfEOp::Alloc::Handle lRDfEOpAlloc = castHandleT<RDfEOp::Alloc>(lFactory.getAllocator("HPC-RecvDemeFromEvolverOp"));
		RDfEOp::Handle lRDfEOpBS = castHandleT<RDfEOp>(lRDfEOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Supervisor", lRDfEOpBS );

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

		SD2SOp::Alloc::Handle lSD2SOpAlloc = castHandleT<SD2SOp::Alloc>(lFactory.getAllocator("HPC-SendDemeToSupervisorOp"));
		SD2SOp::Handle lSD2SOpBS = castHandleT<SD2SOp>(lSD2SOpAlloc->allocate());
		lNodeTypeSwitch->insertOp("Evolver", lSD2SOpBS );

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
		RDfEOp::Alloc::Handle lRDfEOpAlloc = castHandleT<RDfEOp::Alloc>(lFactory.getAllocator("HPC-RecvDemeFromEvolverOp"));
		RDfEOp::Handle lRDfEOpBS = castHandleT<RDfEOp>(lRDfEOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Supervisor", lRDfEOpBS );

                StatsCalculateOp::Handle lStatsCalcOpML = castHandleT<StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
                lStatsCalcOpML->setName(lStatsCalcOpName);
                lNodeTypeSwitchML->insertOp("Supervisor", lStatsCalcOpML);

		TerminationOp::Handle lTermOpML = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
		lTermOpML->setName(lTermOpName);
		lNodeTypeSwitchML->insertOp("Supervisor", lTermOpML );

		MilestoneWriteOp::Handle lMsWriteOpML = castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
		lMsWriteOpML->setName(lMsWriteOpName);
		lNodeTypeSwitchML->insertOp("Supervisor", lMsWriteOpML );
	}
	//Set the evolver mainloop set
	{
		OversizeOp::Alloc::Handle lOversizeOpAlloc = castHandleT<OversizeOp::Alloc>(lFactory.getAllocator("OversizeOp"));
		OversizeOp::Handle lOversizeOpML = castHandleT<OversizeOp>(lOversizeOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lOversizeOpML );

		// Set crossover branch
		BreederNode::Handle lCxNode = new BreederNode;
		lOversizeOpML->setRootNode(lCxNode);
		lCxNode->setBreederOp(castHandleT<CrossoverOp>(lCxOpAlloc->allocate()));
		lCxNode->getBreederOp()->setName(lCxOpName);
		BreederNode::Handle lSelectCxNode1 = new BreederNode;
		lCxNode->setFirstChild(lSelectCxNode1);
		lSelectCxNode1->setBreederOp(castHandleT<SelectionOp>(lSelectOpAlloc->allocate()));
		lSelectCxNode1->getBreederOp()->setName(lSelectOpName);
		BreederNode::Handle lSelectCxNode2 = new BreederNode;
		lSelectCxNode1->setNextSibling(lSelectCxNode2);
		lSelectCxNode2->setBreederOp(castHandleT<SelectionOp>(lSelectOpAlloc->allocate()));
		lSelectCxNode2->getBreederOp()->setName(lSelectOpName);

		//Set the mutation branch
		BreederNode::Handle lMutNode = new BreederNode;
		lCxNode->setNextSibling(lMutNode);
		lMutNode->setBreederOp(castHandleT<MutationOp>(lMutOpAlloc->allocate()));
		lMutNode->getBreederOp()->setName(lMutOpName);
		BreederNode::Handle lSelectMutNode = new BreederNode;
		lMutNode->setFirstChild(lSelectMutNode);
		lSelectMutNode->setBreederOp(castHandleT<SelectionOp>(lSelectOpAlloc->allocate()));
		lSelectMutNode->getBreederOp()->setName(lSelectOpName);

		DD2EOp::Alloc::Handle lDD2EOpAlloc = castHandleT<DD2EOp::Alloc>(lFactory.getAllocator("HPC-DistributeDemeToEvaluatorsOp"));
		DD2EOp::Handle lDD2EOpML = castHandleT<DD2EOp>(lDD2EOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lDD2EOpML );

		RFfEOp::Alloc::Handle lRFfEOpAlloc = castHandleT<RFfEOp::Alloc>(lFactory.getAllocator("HPC-RecvFitnessFromEvaluatorOp"));
		RFfEOp::Handle lRFfEOpML = castHandleT<RFfEOp>(lRFfEOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lRFfEOpML );

		NSGA2Op::Alloc::Handle lNSGA2OpAlloc = castHandleT<NSGA2Op::Alloc>(lFactory.getAllocator("NSGA2Op"));
		NSGA2Op::Handle lNSGA2OpML = castHandleT<NSGA2Op>(lNSGA2OpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lNSGA2OpML );

		SD2SOp::Alloc::Handle lSD2SOpAlloc = castHandleT<SD2SOp::Alloc>(lFactory.getAllocator("HPC-SendDemeToSupervisorOp"));
		SD2SOp::Handle lSD2SOpML = castHandleT<SD2SOp>(lSD2SOpAlloc->allocate());
		lNodeTypeSwitchML->insertOp("Evolver", lSD2SOpML );

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
