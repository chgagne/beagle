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
 *  \file   beagle/Distrib/Master/src/AlgoCruncher.cpp
 *  \brief  Source code of class Distrib::Master::AlgoCruncher.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:33 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;


/*!
 *  \brief Construct master-slave cruncher evolutionary algorithm.
 */
Distrib::Master::AlgoCruncher::AlgoCruncher(void) :
		Algorithm("DBMS-AlgoCruncher")
{ }


/*!
 *  \brief Configure master-slave cruncher evolutionary algorithm in evolver.
 *  \param ioEvolver Evolver modified by setting the algorithm.
 *  \param ioSystem Evolutionary system.
 *
 */
void Distrib::Master::AlgoCruncher::configure(Evolver& ioEvolver, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Get reference to the factory
	const Factory& lFactory = ioSystem.getFactory();

	// Get allocators
	std::string lFetchCrunchDataOpName = "DBMS-FetchCruncherDataOp";
	FetchCruncherDataOp::Alloc::Handle lFetchCrunchDataOpAlloc =
	    castHandleT<FetchCruncherDataOp::Alloc>(lFactory.getAllocator(lFetchCrunchDataOpName));
	std::string lEvalOpName = lFactory.getConceptTypeName("EvaluationOp");
	EvaluationOp::Alloc::Handle lEvalOpAlloc =
	    castHandleT<EvaluationOp::Alloc>(lFactory.getAllocator(lEvalOpName));

	// Set boostrap operator set
	ioEvolver.getBootStrapSet().clear();
	FetchCruncherDataOp::Handle lFetchCrunchDataOpBS =
	    castHandleT<FetchCruncherDataOp>(lFetchCrunchDataOpAlloc->allocate());
	lFetchCrunchDataOpBS->setName(lFetchCrunchDataOpName);
	ioEvolver.getBootStrapSet().push_back(lFetchCrunchDataOpBS);
	EvaluationOp::Handle lEvalOpBS =
	    castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	lEvalOpBS->setName(lEvalOpName);
	ioEvolver.getBootStrapSet().push_back(lEvalOpBS);

	// Set mainloop operator set
	ioEvolver.getMainLoopSet().clear();
	FetchCruncherDataOp::Handle lFetchCrunchDataOpML =
	    castHandleT<FetchCruncherDataOp>(lFetchCrunchDataOpAlloc->allocate());
	lFetchCrunchDataOpML->setName(lFetchCrunchDataOpName);
	ioEvolver.getMainLoopSet().push_back(lFetchCrunchDataOpML);
	EvaluationOp::Handle lEvalOpML =
	    castHandleT<EvaluationOp>(lEvalOpAlloc->allocate());
	lEvalOpML->setName(lEvalOpName);
	ioEvolver.getMainLoopSet().push_back(lEvalOpML);

	Beagle_StackTraceEndM();
}
