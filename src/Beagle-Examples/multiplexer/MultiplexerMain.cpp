/*
 *  Boolean 11-multiplexer (multiplexer)
 *  Copyright (C) 2006 by Christian Gagne
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
 *  \file   ParityMain.cpp
 *  \brief  Implementation of the main routine for the Boolean 6-multiplexer problem.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:20 $
 */

#include "Beagle/GP.hpp"
#include "MultiplexerEvalOp.hpp"
#include "IfBoolean.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <string>


using namespace std;
using namespace Beagle;

/*!
 *  \brief Main routine for the Boolean 6-multiplexer problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup Multiplexer
 */
int main(int argc, char *argv[])
{
	try {
		// Build primitives.
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
		lSet->insert(new GP::And);
		lSet->insert(new GP::Or);
		lSet->insert(new GP::Not);
		lSet->insert(new IfBoolean);
		lSet->insert(new GP::TokenT<Bool>("A0"));
		lSet->insert(new GP::TokenT<Bool>("A1"));
		lSet->insert(new GP::TokenT<Bool>("A2"));
		lSet->insert(new GP::TokenT<Bool>("D0"));
		lSet->insert(new GP::TokenT<Bool>("D1"));
		lSet->insert(new GP::TokenT<Bool>("D2"));
		lSet->insert(new GP::TokenT<Bool>("D3"));
		lSet->insert(new GP::TokenT<Bool>("D4"));
		lSet->insert(new GP::TokenT<Bool>("D5"));
		lSet->insert(new GP::TokenT<Bool>("D6"));
		lSet->insert(new GP::TokenT<Bool>("D7"));
		
		// Build system
		System::Handle lSystem = new System;

		// Add GP package
		lSystem->addPackage(new GP::PackageBase(lSet));
		
		// Set fitness evaluation operator allocator
		lSystem->setEvaluationOp("MultiplexerEvalOp", new MultiplexerEvalOp::Alloc);
		
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

