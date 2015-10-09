/*
 *  Symbolic regression (symbreg):
 *  A simple GP example with Open BEAGLE
 *
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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
 *  \file   SymbRegMain.cpp
 *  \brief  Implementation of the main routine for the symbolic regression problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2008/05/18 19:16:50 $
 */

#include "Beagle/STGP.hpp"
#include "SymbRegEvalOp.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Main routine for the function symbolic regression problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup SymbReg
 */
int main(int argc, char *argv[])
{
	try {

		// Build primitives
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
		lSet->insert(new GP::Add);
		lSet->insert(new GP::Subtract);
		lSet->insert(new GP::Multiply);
		lSet->insert(new GP::Divide);
		lSet->insert(new GP::Sin);
		lSet->insert(new GP::Cos);
		lSet->insert(new GP::Exp);
		lSet->insert(new GP::Log);
		lSet->insert(new GP::TokenT<Double>("X"));
		lSet->insert(new GP::EphemeralDouble);
		// lSet->insert(new GP::TokenT<Double>("Pi", Double(M_PI)));

		// Build a system with GP package
		System::Handle lSystem = new System;
		lSystem->addPackage(new GP::PackageBase(lSet));
		lSystem->addPackage(new STGP::PackageConstrained);

		// Add data set for regression component
		lSystem->addComponent(new DataSetRegression);

		// Add evaluation operator allocator
		lSystem->setEvaluationOp("SymbRegEvalOp", new SymbRegEvalOp::Alloc);

		// Initialize the evolver
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);

		// Create population
		Vivarium::Handle lVivarium = new Vivarium;

		// Launch evolution
		lEvolver->evolve(lVivarium, lSystem);

	} catch(Exception& inException) {
		inException.terminate();
	} catch(exception& inException) {
		cerr << "Standard exception caught:" << endl;
		cerr << inException.what() << endl << flush;
		return 1;
	} catch(...) {
		cerr << "Unknown exception caught!" << endl << flush;
		return 1;
	}
	return 0;
}

