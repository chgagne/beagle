/*
 *  Traveling Salesman Problem (tsp):
 *  Indices integer vector GA example
 *
 *  Copyright (C) 2005-2006 by Christian Gagne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   TSPMain.cpp
 *  \brief  Implementation of the main routine for the TSP problem.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:20 $
 */

#include "Beagle/EC.hpp"
#include "Beagle/IntVec.hpp"
#include "TSPEvalOp.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;

/*!
 *  \brief Main routine for the TSP problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup TSP
 */
int main(int argc, char** argv)
{
	try {
		// Build the system
		System::Handle lSystem = new System;
		// Install the GA integer vector package
		const unsigned int lNumberOfTowns = 25;
		lSystem->addPackage(new IntVec::Package(lNumberOfTowns-1));
		// Add distance map component
		lSystem->addComponent(new TSPDistanceMap(lNumberOfTowns));
		// Add evaluation operator allocator
		lSystem->setEvaluationOp("TSPEvalOp", new TSPEvalOp::Alloc);
		// Set initialization, crossover and mutation operators to use
		lSystem->getFactory().setConcept("InitializationOp", "GA-InitIndicesIntVecOp");
		lSystem->getFactory().setConcept("CrossoverOp", "GA-CrossoverIndicesIntVecOp");
		lSystem->getFactory().setConcept("MutationOp", "GA-MutationShuffleIntVecOp");
		// Set the use of a minimization fitness
		lSystem->getFactory().setConcept("Fitness", "FitnessSimpleMin");
		// Initialize the evolver
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);
		// Create population
		Vivarium::Handle lVivarium = new Vivarium;
		// Launch evolution
		lEvolver->evolve(lVivarium, lSystem);
	} catch(Exception& inException) {
		inException.terminate(cerr);
	} catch(std::exception& inException) {
		cerr << "Standard exception caught:" << endl << flush;
		cerr << inException.what() << endl << flush;
		return 1;
	}
	return 0;
}
