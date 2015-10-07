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
 *  \file   AntEvalOp.cpp
 *  \brief  Implementation of class AntEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/09 21:43:12 $
 */

#include "Beagle/GP.hpp"
#include "AntEvalOp.hpp"
#include "AntSimulator.hpp"

#include <cmath>

using namespace Beagle;
using namespace std;


/*!
 *  \brief Construct a new artificial ant evaluation operator.
 */
AntEvalOp::AntEvalOp() :
		GP::EvaluationOp("AntEvalOp")
{ }


/*!
 * \brief Register the artificial ant parameters.
 * \param ioSystem System to use to initialize the operator.
 */
void AntEvalOp::registerParams(System& ioSystem)
{
	GP::EvaluationOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "Name of the file to read the trail configuration from, in order to evaluate performance ";
	lOSS << "of artificial ants.";
	Register::Description lDescription(
	    "Ant trail filename",
	    "String",
	    "santafe.trail",
	    lOSS.str()
	);
	mTrailFileName = castHandleT<String>(
	                     ioSystem.getRegister().insertEntry("ant.trail.filename", new String("santafe.trail"), lDescription));
}


/*!
 *  \brief Evaluate the individual fitness for the artificial ant problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure.
 */
Fitness::Handle AntEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
	mSimulator->reset();
	while(mSimulator->getMovesDone() < mSimulator->getMaxMoves()) {
		inIndividual.run(*mSimulator, ioContext);
	}
	return new EC::FitnessSimple(float(mSimulator->getPiecesEaten()));
}


/*!
 * \brief Initialize the artificial ant evaluation operator.
 * \param ioSystem System to use to initialize the operator.
 */
void AntEvalOp::init(System& ioSystem)
{
	GP::EvaluationOp::init(ioSystem);
	mSimulator = castHandleT<AntSimulator>(ioSystem.getComponent("AntSimulator"));
	if(mSimulator == NULL) {
		throw Beagle_RunTimeExceptionM("Ant simulator should be added to system as component!");
	}
	if(mSimulator->getNumberColumns() != 0) return;
	std::ifstream lIFS(mTrailFileName->getWrappedValue().c_str());
	PACC::XML::Document lDocument(lIFS, mTrailFileName->getWrappedValue());
	mSimulator->read(lDocument.getFirstDataTag());
	lIFS.close();
}
