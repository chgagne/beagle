/*
 *  Artifical ant (ant)
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
 *  \file   AntMain.cpp
 *  \brief  Implementation of the main routine for the artificial ant problem.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.5 $
 *  $Date: 2007/09/17 17:25:50 $
 */

#include "Beagle/EC.hpp"
#include "AntEvalOp.hpp"
#include "IfFoodAhead.hpp"
#include "Move.hpp"
#include "Progn.hpp"
#include "TurnLeft.hpp"
#include "TurnRight.hpp"

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
 *  \brief Main routine for the artifical ant problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup Ant
 */
int main(int argc, char *argv[])
{
	try {

		// Build primitives
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
		lSet->insert(new IfFoodAhead);
		lSet->insert(new Progn(2));
		lSet->insert(new Progn(3));
		lSet->insert(new Move);
		lSet->insert(new TurnLeft);
		lSet->insert(new TurnRight);

		// Build a system and add GP package
		System::Handle lSystem = new System;
		lSystem->addPackage(new GP::PackageBase(lSet));

		// Add simulator system component
		lSystem->addComponent(new AntSimulator);
		
		// Set fitness evaluation operator allocator
		lSystem->setEvaluationOp("AntEvalOp", new AntEvalOp::Alloc);

		// Initialize the evolver
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);

		// Create population
		Vivarium::Handle lVivarium = new Vivarium;

		// Launch evolution
		lEvolver->evolve(lVivarium, lSystem);
		
		// Get best-of-run from the hall-of-fame and log their trails.
		HallOfFame::Handle lVivaHoF = lVivarium->getHallOfFame();
		if(lVivaHoF != NULL) {
			lVivaHoF->sort();
			AntEvalOp::Handle lEvalOp = new AntEvalOp;
			lEvalOp->setSimulator(
				castHandleT<AntSimulator>(lSystem->getComponent("AntSimulator")));
			for(unsigned int i=0; i<lVivaHoF->size(); ++i) {
				lEvalOp->test((*lVivaHoF)[i].mIndividual, lSystem);
				std::ostringstream lOSS;
				lOSS << uint2ordinal(i+1) << " individual from the vivarium's hall-of-fame";
				Beagle_LogInfoM(
					lSystem->getLogger(),
					lOSS.str()
				);
				Beagle_LogM(
					lSystem->getLogger(),
					Logger::eInfo,
					(const Beagle::Object&)(*lVivaHoF)[i].mIndividual
				);
				Beagle_LogM(
					lSystem->getLogger(),
					Logger::eInfo,
					(const Beagle::Object&)lEvalOp->getSimulator()
				);
			}
		}

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

