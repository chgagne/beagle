/*
 *  SPAM e-mail database (spambase):
 *  Machine learning using strongly-typed GP with Open BEAGLE
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
 *  \file   SpambaseMain.cpp
 *  \brief  Implementation of the main routine for the spambase problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:21 $
 */

#include "Beagle/STGP.hpp"
#include "SpambaseEvalOp.hpp"
#include "LessThan.hpp"
#include "EqualTo.hpp"
#include "IfThenElse.hpp"
#include "EphemeralPercent.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <typeinfo>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Main routine for the function spambase problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup Spambase
 */
int main(int argc, char *argv[])
{
	try {

		// Build primitives
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&typeid(Bool));
		lSet->insert(new GP::And);
		lSet->insert(new GP::Or);
		lSet->insert(new GP::Not);
		lSet->insert(new GP::Add);
		lSet->insert(new GP::Subtract);
		lSet->insert(new GP::Multiply);
		lSet->insert(new GP::Divide);
		lSet->insert(new LessThan);
		lSet->insert(new EqualTo);
		lSet->insert(new IfThenElse);
		lSet->insert(new EphemeralPercent);
		lSet->insert(new GP::TokenT<Bool>("0", Bool(false)));
		lSet->insert(new GP::TokenT<Bool>("1", Bool(true)));
		lSet->insert(new GP::TokenT<Double>("IN0"));
		lSet->insert(new GP::TokenT<Double>("IN1"));
		lSet->insert(new GP::TokenT<Double>("IN2"));
		lSet->insert(new GP::TokenT<Double>("IN3"));
		lSet->insert(new GP::TokenT<Double>("IN4"));
		lSet->insert(new GP::TokenT<Double>("IN5"));
		lSet->insert(new GP::TokenT<Double>("IN6"));
		lSet->insert(new GP::TokenT<Double>("IN7"));
		lSet->insert(new GP::TokenT<Double>("IN8"));
		lSet->insert(new GP::TokenT<Double>("IN9"));
		lSet->insert(new GP::TokenT<Double>("IN10"));
		lSet->insert(new GP::TokenT<Double>("IN11"));
		lSet->insert(new GP::TokenT<Double>("IN12"));
		lSet->insert(new GP::TokenT<Double>("IN13"));
		lSet->insert(new GP::TokenT<Double>("IN14"));
		lSet->insert(new GP::TokenT<Double>("IN15"));
		lSet->insert(new GP::TokenT<Double>("IN16"));
		lSet->insert(new GP::TokenT<Double>("IN17"));
		lSet->insert(new GP::TokenT<Double>("IN18"));
		lSet->insert(new GP::TokenT<Double>("IN19"));
		lSet->insert(new GP::TokenT<Double>("IN20"));
		lSet->insert(new GP::TokenT<Double>("IN21"));
		lSet->insert(new GP::TokenT<Double>("IN22"));
		lSet->insert(new GP::TokenT<Double>("IN23"));
		lSet->insert(new GP::TokenT<Double>("IN24"));
		lSet->insert(new GP::TokenT<Double>("IN25"));
		lSet->insert(new GP::TokenT<Double>("IN26"));
		lSet->insert(new GP::TokenT<Double>("IN27"));
		lSet->insert(new GP::TokenT<Double>("IN28"));
		lSet->insert(new GP::TokenT<Double>("IN29"));
		lSet->insert(new GP::TokenT<Double>("IN30"));
		lSet->insert(new GP::TokenT<Double>("IN31"));
		lSet->insert(new GP::TokenT<Double>("IN32"));
		lSet->insert(new GP::TokenT<Double>("IN33"));
		lSet->insert(new GP::TokenT<Double>("IN34"));
		lSet->insert(new GP::TokenT<Double>("IN35"));
		lSet->insert(new GP::TokenT<Double>("IN36"));
		lSet->insert(new GP::TokenT<Double>("IN37"));
		lSet->insert(new GP::TokenT<Double>("IN38"));
		lSet->insert(new GP::TokenT<Double>("IN39"));
		lSet->insert(new GP::TokenT<Double>("IN40"));
		lSet->insert(new GP::TokenT<Double>("IN41"));
		lSet->insert(new GP::TokenT<Double>("IN42"));
		lSet->insert(new GP::TokenT<Double>("IN43"));
		lSet->insert(new GP::TokenT<Double>("IN44"));
		lSet->insert(new GP::TokenT<Double>("IN45"));
		lSet->insert(new GP::TokenT<Double>("IN46"));
		lSet->insert(new GP::TokenT<Double>("IN47"));
		lSet->insert(new GP::TokenT<Double>("IN48"));
		lSet->insert(new GP::TokenT<Double>("IN49"));
		lSet->insert(new GP::TokenT<Double>("IN50"));
		lSet->insert(new GP::TokenT<Double>("IN51"));
		lSet->insert(new GP::TokenT<Double>("IN52"));
		lSet->insert(new GP::TokenT<Double>("IN53"));
		lSet->insert(new GP::TokenT<Double>("IN54"));
		lSet->insert(new GP::TokenT<Double>("IN55"));
		lSet->insert(new GP::TokenT<Double>("IN56"));
		
		// Build a system.
		System::Handle lSystem = new System();

		// Add constrained GP package
		lSystem->addPackage(new STGP::PackageConstrained(lSet));
		
		// Read and add classification data set of spambase data
		DataSetClassification::Handle lDataSet = new DataSetClassification("DataSet");
		std::ifstream lIFS("spambase.data");
		if(!lIFS) {
			throw Beagle_RunTimeExceptionM("Could not open spambase data file 'spambase.data'!");;
		}
		lDataSet->readCSV(lIFS);
		lIFS.close();
		lSystem->addComponent(lDataSet);

		// Set fitness evaluation operator
		lSystem->setEvaluationOp("SpambaseEvalOp", new SpambaseEvalOp::Alloc);

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

