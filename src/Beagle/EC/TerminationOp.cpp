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
 *  \file   Beagle/EC/TerminationOp.cpp
 *  \brief  Source code of class TerminationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator.
 *  \param inName Name of the termination operator.
 */
TerminationOp::TerminationOp(std::string inName) :
		Beagle::Operator(inName)
{ }


/*!
 *  \brief Apply the characteristic parameters reading operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void TerminationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "termination", "Beagle::TerminationOp",
	    std::string("Checking if '")+getName()+
	    std::string("' termination criterion reached for the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	if(terminate(ioDeme,ioContext)) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    "termination", "Beagle::TerminationOp",
		    "Termination criterion reached"
		);
		ioContext.setContinueFlag(false);
	}
	Beagle_StackTraceEndM("void TerminationOp::operate(Deme& ioDeme, Context& ioContext)");
}

