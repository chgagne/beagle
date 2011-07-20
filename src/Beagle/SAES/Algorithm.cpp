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
 *  \file   Beagle/SAES/Algorithm.cpp
 *  \brief  Source code of class SAES::Algorithm.
 *  \author Christian Gagne
 *  $Revision: 1.3 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/SAES.hpp"

using namespace Beagle;


/*!
 *  \brief Construct anisotropic (\mu,\lambda) self-adaptive evolution strategy algorithm.
 */
SAES::Algorithm::Algorithm(void) :
		Beagle::Algorithm("SAES-Algorithm")
{ }


/*!
 *  \brief Configure evolver with anisotropic (\mu,\lambda) self-adaptive evolution strategy algorithm.
 *  \param ioEvolver Evolver modified by setting the algorithm.
 *  \param ioSystem Evolutionary system.
 */
void SAES::Algorithm::configure(Evolver& ioEvolver, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Get reference to the factory
	const Factory& lFactory = ioSystem.getFactory();

	// Get name and allocator of used operators
	std::string lEvalOpName = lFactory.getConceptTypeName("EvaluationOp");
	EvaluationOp::Alloc::Handle lEvalOpAlloc =
	    castHandleT<EvaluationOp::Alloc>(lFactory.getAllocator(lEvalOpName));
	std::string lSelectOpName = "SelectRandomOp";
	EC::SelectionOp::Alloc::Handle lSelectOpAlloc =
	    castHandleT<EC::SelectionOp::Alloc>(lFactory.getAllocator(lSelectOpName));
	std::string lInitOpName = lFactory.getConceptTypeName("InitializationOp");
	EC::InitializationOp::Alloc::Handle lInitOpAlloc =
	    castHandleT<EC::InitializationOp::Alloc>(lFactory.getAllocator(lInitOpName));
	std::string lMutOpName = "SAES-MutationOp";
	EC::MutationOp::Alloc::Handle lMutOpAlloc =
	    castHandleT<EC::MutationOp::Alloc>(lFactory.getAllocator(lMutOpName));
	std::string lMigOpName = lFactory.getConceptTypeName("MigrationOp");
	EC::MigrationOp::Alloc::Handle lMigOpAlloc =
	    castHandleT<EC::MigrationOp::Alloc>(lFactory.getAllocator(lMigOpName));
	std::string lStatsCalcOpName = lFactory.getConceptTypeName("StatsCalculateOp");
	EC::StatsCalculateOp::Alloc::Handle lStatsCalcOpAlloc =
	    castHandleT<EC::StatsCalculateOp::Alloc>(lFactory.getAllocator(lStatsCalcOpName));
	std::string lTermOpName = lFactory.getConceptTypeName("TerminationOp");
	EC::TerminationOp::Alloc::Handle lTermOpAlloc =
	    castHandleT<EC::TerminationOp::Alloc>(lFactory.getAllocator(lTermOpName));
	std::string lMsWriteOpName = "MilestoneWriteOp";
	EC::MilestoneWriteOp::Alloc::Handle lMsWriteOpAlloc =
	    castHandleT<EC::MilestoneWriteOp::Alloc>(lFactory.getAllocator(lMsWriteOpName));
	std::string lMCLOpName = "EC-MuCommaLambdaOp";
	EC::MuCommaLambdaOp::Alloc::Handle lMCLOpAlloc =
	    castHandleT<EC::MuCommaLambdaOp::Alloc>(lFactory.getAllocator(lMCLOpName));

	// Clear bootstrap and mainloop sets
	ioEvolver.getBootStrapSet().clear();
	ioEvolver.getMainLoopSet().clear();

	// Set the boostrap operator set
	EC::InitializationOp::Handle lInitOpBS = castHandleT<EC::InitializationOp>(lInitOpAlloc->allocate());
	lInitOpBS->setName(lInitOpName);
	ioEvolver.getBootStrapSet().push_back(lInitOpBS);
	EvaluationOp::Handle lEvalOpBS = castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	lEvalOpBS->setName(lEvalOpName);
	ioEvolver.getBootStrapSet().push_back(lEvalOpBS);
	EC::StatsCalculateOp::Handle lStatsCalcOpBS = castHandleT<EC::StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
	lStatsCalcOpBS->setName(lStatsCalcOpName);
	ioEvolver.getBootStrapSet().push_back(lStatsCalcOpBS);
	EC::TerminationOp::Handle lTermOpBS = castHandleT<EC::TerminationOp>(lTermOpAlloc->allocate());
	lTermOpBS->setName(lTermOpName);
	ioEvolver.getBootStrapSet().push_back(lTermOpBS);
	EC::MilestoneWriteOp::Handle lMsWriteOpBS = castHandleT<EC::MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
	lMsWriteOpBS->setName(lMsWriteOpName);
	ioEvolver.getBootStrapSet().push_back(lMsWriteOpBS);

	// Set the mainloop operator set
	EC::MuCommaLambdaOp::Handle lMCLOp = castHandleT<EC::MuCommaLambdaOp>(lMCLOpAlloc->allocate());
	lMCLOp->setName(lMCLOpName);

	// Set breeder tree
	BreederNode::Handle lEvalNode = new BreederNode;
	lMCLOp->setRootNode(lEvalNode);
	lEvalNode->setBreederOp(castHandleT<EvaluationOp>(lEvalOpAlloc->allocate()));
	lEvalNode->getBreederOp()->setName(lEvalOpName);
	BreederNode::Handle lMutNode = new BreederNode;
	lEvalNode->setFirstChild(lMutNode);
	lMutNode->setBreederOp(castHandleT<EC::MutationOp>(lMutOpAlloc->allocate()));
	lMutNode->getBreederOp()->setName(lMutOpName);
	BreederNode::Handle lSelectMutNode = new BreederNode;
	lMutNode->setFirstChild(lSelectMutNode);
	lSelectMutNode->setBreederOp(castHandleT<EC::SelectionOp>(lSelectOpAlloc->allocate()));
	lSelectMutNode->getBreederOp()->setName(lSelectOpName);

	// Set remaining operators of mainloop
	EC::MigrationOp::Handle lMigOpML = castHandleT<EC::MigrationOp>(lMigOpAlloc->allocate());
	lMigOpML->setName(lMigOpName);
	ioEvolver.getMainLoopSet().push_back(lMigOpML);
	EC::StatsCalculateOp::Handle lStatsCalcOpML =
	    castHandleT<EC::StatsCalculateOp>(lStatsCalcOpAlloc->allocate());
	lStatsCalcOpML->setName(lStatsCalcOpName);
	ioEvolver.getMainLoopSet().push_back(lStatsCalcOpML);
	EC::TerminationOp::Handle lTermOpML = castHandleT<EC::TerminationOp>(lTermOpAlloc->allocate());
	lTermOpML->setName(lTermOpName);
	ioEvolver.getMainLoopSet().push_back(lTermOpML);
	EC::MilestoneWriteOp::Handle lMsWriteOpML =
	    castHandleT<EC::MilestoneWriteOp>(lMsWriteOpAlloc->allocate());
	lMsWriteOpML->setName(lMsWriteOpName);
	ioEvolver.getMainLoopSet().push_back(lMsWriteOpML);

	Beagle_StackTraceEndM();
}
