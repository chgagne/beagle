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
 *  \file   beagle/HPC/SendFitnessToEvolverOp.cpp
 *  \brief  Source code of the class HPC::SendFitnessToEvolverOp.
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
 *  \brief Construct the SendFitnessToEvolverOp operator.
 *  \param inName Name of the operator.
 */
HPC::SendFitnessToEvolverOp::SendFitnessToEvolverOp(std::string inName) : Operator(inName) {}


/*!
 *  \brief Initialize the operator
 *  \param ioSystem Current system
 */
void HPC::SendFitnessToEvolverOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mComm = castHandleT<HPC::MPICommunication>(ioSystem.getComponent("MPICommunication"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Send fitness of an individuals group to evolver node.
 *  \param ioDeme Current deme to execute the operator.
 *  \param ioContext Current context to execute the operator.
 */
void HPC::SendFitnessToEvolverOp::operate(Deme& ioDeme, Context& ioContext)
{	
	std::ostringstream lOutStream;
	PACC::XML::Streamer lStreamer(lOutStream);
	lStreamer.openTag("Bag",false);
	for(unsigned int i = 0; i < ioDeme.size(); ++i){
		ioDeme[i]->getFitness()->write(lStreamer,false);
	}
	lStreamer.closeTag();
	mComm->send(lOutStream.str(), "Fitness", "Parent");
	Beagle_LogDetailedM(
		ioContext.getSystem().getLogger(),
		"send", "Beagle::HPC::SendFitnessToEvolverOp",
		std::string("Evaluator send fitness to his evolver")
	);
}

