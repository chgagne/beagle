/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/AlgoGenerational.cpp
 *  \brief  Source code of class AlgoGenerational.
 *  \author Christian Gagne
 *  $Revision: 1.3 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct generational evolutionary algorithm.
 */
AlgoGenerational::AlgoGenerational(void) :
	Algorithm("AlgoGenerational")
{ }


/*!
 *  \brief Configure generational evolutionary algorithm in evolver.
 *  \param ioEvolver Evolver modified by setting the algorithm.
 *  \param ioSystem Evolutionary system.
 *
 */
void AlgoGenerational::configure(Evolver& ioEvolver, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Get reference to the factory
	const Factory& lFactory = ioSystem.getFactory();

	// Get name and allocator of used operators
	std::string lEvalOpName = lFactory.getConceptTypeName("EvaluationOp");
	EvaluationOp::Alloc::Handle lEvalOpAlloc =
	    castHandleT<EvaluationOp::Alloc>(lFactory.getAllocator(lEvalOpName));
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
	std::string lMsWriteOpName = "MilestoneWriteOp";
	MilestoneWriteOp::Alloc::Handle lMsWriteOpAlloc =
	    castHandleT<MilestoneWriteOp::Alloc>(lFactory.getAllocator(lMsWriteOpName));
	std::string lGenOpName = "GenerationalOp";
	GenerationalOp::Alloc::Handle lGenOpAlloc =
	    castHandleT<GenerationalOp::Alloc>(lFactory.getAllocator(lGenOpName));

	// Clear bootstrap and mainloop sets
	ioEvolver.getBootStrapSet().clear();
	ioEvolver.getMainLoopSet().clear();

	// Set the boostrap operator set
	InitializationOp::Handle lInitOpBS = castHandleT<InitializationOp>(lInitOpAlloc->allocate());
	lInitOpBS->setName(lInitOpName);
	ioEvolver.getBootStrapSet().push_back(lInitOpBS);
	EvaluationOp::Handle lEvalOpBS = castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	lEvalOpBS->setName(lEvalOpName);
	ioEvolver.getBootStrapSet().push_back(lEvalOpBS);
	StatsCalculateOp::Handle lStatsCalcOpBS = castHandleT<StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
	lStatsCalcOpBS->setName(lStatsCalcOpName);
	ioEvolver.getBootStrapSet().push_back(lStatsCalcOpBS);
	TerminationOp::Handle lTermOpBS = castHandleT<TerminationOp>(lTermOpAlloc->allocate());
	lTermOpBS->setName(lTermOpName);
	ioEvolver.getBootStrapSet().push_back(lTermOpBS);
	MilestoneWriteOp::Handle lMsWriteOpBS = castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
	lMsWriteOpBS->setName(lMsWriteOpName);
	ioEvolver.getBootStrapSet().push_back(lMsWriteOpBS);

	// Set the mainloop operator set
	GenerationalOp::Handle lGenOp = castHandleT<GenerationalOp>(lGenOpAlloc->allocate());
	lGenOp->setName(lGenOpName);

	// Set crossover branch
	BreederNode::Handle lCxBranchNode = new BreederNode;
	lGenOp->setRootNode(lCxBranchNode);
	lCxBranchNode->setBreederOp(castHandleT<EvaluationOp>(lEvalOpAlloc->allocate()));
	lCxBranchNode->getBreederOp()->setName(lEvalOpName);
	BreederNode::Handle lCxNode = new BreederNode;
	lCxBranchNode->setFirstChild(lCxNode);
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

	// Set mutation branch
	BreederNode::Handle lMutBranchNode = new BreederNode;
	lCxBranchNode->setNextSibling(lMutBranchNode);
	lMutBranchNode->setBreederOp(castHandleT<EvaluationOp>(lEvalOpAlloc->allocate()));
	lMutBranchNode->getBreederOp()->setName(lEvalOpName);
	BreederNode::Handle lMutNode = new BreederNode;
	lMutBranchNode->setFirstChild(lMutNode);
	lMutNode->setBreederOp(castHandleT<MutationOp>(lMutOpAlloc->allocate()));
	lMutNode->getBreederOp()->setName(lMutOpName);
	BreederNode::Handle lSelectMutNode = new BreederNode;
	lMutNode->setFirstChild(lSelectMutNode);
	lSelectMutNode->setBreederOp(castHandleT<SelectionOp>(lSelectOpAlloc->allocate()));
	lSelectMutNode->getBreederOp()->setName(lSelectOpName);

	// Set reproduction branch
	BreederNode::Handle lReproBranchNode = new BreederNode;
	lMutBranchNode->setNextSibling(lReproBranchNode);
	lReproBranchNode->setBreederOp(castHandleT<SelectionOp>(lSelectOpAlloc->allocate()));
	lReproBranchNode->getBreederOp()->setName(lSelectOpName);

	// Set remaining operators of mainloop
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
	MilestoneWriteOp::Handle lMsWriteOpML =
	    castHandleT<MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
	lMsWriteOpML->setName(lMsWriteOpName);
	ioEvolver.getMainLoopSet().push_back(lMsWriteOpML);

	Beagle_StackTraceEndM();
}
