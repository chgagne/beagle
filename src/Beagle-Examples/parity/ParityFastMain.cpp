/*
 *  Even-6 parity (parity):
 *  A GP example using ADFs with Open BEAGLE
 *  Copyright (C) 2001-2006 by Christian Gagne and Marc Parizeau
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
 *  \brief  Implementation of the main routine for the (fast) parity problem.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Parizeau <parizeau@gel.ulaval.ca>
 *  \author Matthew Walker
 *  $Revision: 1.8 $
 *  $Date: 2008/05/18 19:16:50 $
 */

#include "Beagle/STGP.hpp"
#include "ParityFastEvalOp.hpp"

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
 *  \brief Main routine for the even-6 parity problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup Parity
 */
int main(int argc, char *argv[])
{
	try {

		// Build primitives
		GP::PrimitiveSuperSet::Handle lSuperSet = new GP::PrimitiveSuperSet;
		lSuperSet->addPrimitive(new GP::BitwiseAnd("AND"), true);
		lSuperSet->addPrimitive(new GP::BitwiseOr("OR"), true);
		lSuperSet->addPrimitive(new GP::BitwiseNand("NAND"), true);
		lSuperSet->addPrimitive(new GP::BitwiseNor("NOR"), true);
		lSuperSet->addPrimitive(new GP::BitwiseNot("NOT"), true);
		lSuperSet->addPrimitive(new GP::BitwiseXor("XOR"), true);
		lSuperSet->addPrimitive(new GP::ADF(GP::Invoker::eGenerator, GP::Primitive::eAny, "ADFP", "ARGP"));
		lSuperSet->addPrimitive(new GP::ArgumentT<Int>(GP::Argument::eCaching, GP::Argument::eGenerator, "ARGP"));
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
		lSet->insert(new GP::BitwiseAnd("AND"));
		lSet->insert(new GP::BitwiseOr("OR"));
		lSet->insert(new GP::BitwiseNand("NAND"));
		lSet->insert(new GP::BitwiseNor("NOR"));
		for (unsigned int i=0; i<ParitySizeM; i++) {
			lSet->insert(new GP::TokenT<Int>(std::string("IN")+uint2str(i)));
		}
		lSuperSet->insert(lSet);

		// Build a system
		System::Handle lSystem = new System;

		// Set GP package
		lSystem->addPackage(new GP::PackageBase(lSuperSet));
		lSystem->addPackage(new STGP::PackageConstrained);

		// Set fitness evaluation operator allocator
		lSystem->setEvaluationOp("ParityEvalOp", new ParityFastEvalOp::Alloc);

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
