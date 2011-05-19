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
 *  \file   beagle/HPC/RecvDemeFromEvolverOp.cpp
 *  \brief  Source code of the class HPC::RecvDemeFromEvolverOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.4 $
 *  $Date: 2007/08/13 00:57:45 $
 */

#include "beagle/HPC.hpp"


using namespace Beagle;

/*!
 *  \brief Construct the RecvDemeFromEvolverOp evaluation operator.
 *  \param inName Name of the operator.
 */
HPC::RecvDemeFromEvolverOp::RecvDemeFromEvolverOp(std::string inName) : EvaluationOp(inName) {}

/*!
 *  \brief
 */
Fitness::Handle HPC::RecvDemeFromEvolverOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Initialize the operator
 *  \param ioSystem Current system
 */
void HPC::RecvDemeFromEvolverOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mComm = castHandleT<HPC::MPICommunication>(ioSystem.getComponent("MPICommunication"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Receive a deme content from an evolver node.
 *  \param ioDeme Deme to update by the receive deme.
 *  \param ioContext Current context of the evolution.
 */
void HPC::RecvDemeFromEvolverOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	std::string lNbrProcessed;
	std::string lDemeString;

	mComm->receive(lNbrProcessed, "NbrProcessed", "Child", ioContext.getDemeIndex());
	mComm->receive(lDemeString, "Deme", "Child", ioContext.getDemeIndex());

	Beagle_LogDetailedM(
		ioContext.getSystem().getLogger(),
		"receive", "Beagle::HPC::RecvDemeFromEvolverOp",
		std::string("Supervisor receive deme from his ")+
		uint2ordinal(ioContext.getDemeIndex()+1)+std::string(" evolver")
	);

	prepareStats(ioDeme,ioContext);
	std::istringstream lInStream(lDemeString);
	PACC::XML::Document lDocument(lInStream);

	ioDeme.readPopulation(lDocument.getFirstRoot(), ioContext);

	updateStats(str2uint(lNbrProcessed), ioContext);
	updateHallOfFameWithDeme(ioDeme,ioContext);

	Beagle_StackTraceEndM();
}

