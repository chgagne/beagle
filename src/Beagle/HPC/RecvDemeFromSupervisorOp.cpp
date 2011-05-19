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
 *  \file   beagle/HPC/RecvDemeFromSupervisorOp.cpp
 *  \brief  Source code of the class HPC::RecvDemeFromSupervisorOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.8 $
 *  $Date: 2007/08/13 01:04:55 $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

/*!
 *  \brief Construct the RecvDemeFromSupervisorOp operator.
 *  \param inName Name of the operator.
 */
HPC::RecvDemeFromSupervisorOp::RecvDemeFromSupervisorOp(std::string inName) : Operator(inName) {}

/*!
 *  \brief Initialize the operator
 *  \param ioSystem Current system
 */
void HPC::RecvDemeFromSupervisorOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mComm = castHandleT<HPC::MPICommunication>(ioSystem.getComponent("MPICommunication"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Receive a deme content from supervisor node.
 *  \param ioDeme Deme to update by the receive deme.
 *  \param ioContext Current context of the evolution.
 */
void HPC::RecvDemeFromSupervisorOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
		
	std::string lDemeString;	
	mComm->receive(lDemeString, "Deme", "Parent");	

	std::istringstream lInStream(lDemeString);
	PACC::XML::Document lDocument(lInStream);
	ioDeme.readWithContext(lDocument.getFirstRoot(), ioContext);
	
	Beagle_LogDetailedM(
		ioContext.getSystem().getLogger(),
		"receive", "Beagle::HPC::RecvDemeFromSupervisorOp",
		std::string("Receive ") + uint2ordinal( ioContext.getDemeIndex() ) + std::string(" deme from supervisor") //remplacer (1) par l'index du deme
	);
	
	Beagle_StackTraceEndM();  
}

