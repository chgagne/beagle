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
 *  \file   beagle/HPC/RecvIndividualsFromEvolverOp.cpp
 *  \brief  Source code of the class HPC::RecvIndividualsFromEvolverOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.5 $
 *  $Date: 2007/08/13 01:04:55 $
 */

#include "beagle/HPC.hpp"


using namespace Beagle;

/*!
 *  \brief Construct the RecvIndividualsFromEvolverOp operator.
 *  \param inName Name of the operator.
 */
HPC::RecvIndividualsFromEvolverOp::RecvIndividualsFromEvolverOp(std::string inName) : Operator(inName) {}


/*!
 *  \brief Initialize the operator
 *  \param ioSystem Current system
 */
void HPC::RecvIndividualsFromEvolverOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mComm = castHandleT<HPC::MPICommunication>(ioSystem.getComponent("MPICommunication"));
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Receive a group of individuals to evaluate from the parent evolver node.
 *  \param ioDeme Current deme to evaluate the individus group.
 *  \param ioContext Current context of the evolution.
 */
void HPC::RecvIndividualsFromEvolverOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	std::string lPopulationString;

	mComm->receive(lPopulationString, "Individuals", "Parent");

	Beagle_LogDetailedM(
		ioContext.getSystem().getLogger(),
		"receive", "Beagle::HPC::RecvIndividualsFromEvolverOp",
		std::string("Evaluator receive individuals from his parent evolver")
	);

	std::istringstream lInStream(lPopulationString);
	PACC::XML::Document lDocument(lInStream);
	ioDeme.readPopulation(lDocument.getFirstRoot(),ioContext);

	Beagle_StackTraceEndM();
}


