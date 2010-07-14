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
 *  \file   Beagle/EC/TermMaxGenOp.cpp
 *  \brief  Source code of class TermMaxGenOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/08 19:26:47 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator based on a maximum generation value.
 *  \param inName Name of the operator.
 */
TermMaxGenOp::TermMaxGenOp(std::string inName) :
		Beagle::EC::TerminationOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxGenOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EC::TerminationOp::registerParams(ioSystem);
	
	std::ostringstream lOSS;
	lOSS << "Maximum number of generations for the evolution.";
	Register::Description lDescription(
	    "Max generation term criterion",
	    "UInt",
	    "50",
	    lOSS.str()
	);
	mMaxGeneration = castHandleT<UInt>(
	                     ioSystem.getRegister().insertEntry("ec.term.maxgen", new UInt(50), lDescription));

	Beagle_StackTraceEndM("void TermMaxGenOp::registerParams(System&)");
}


/*!
 *  \brief Check if the maximum number of generation is reached.
 *  \param ioDeme
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxGenOp::terminate(const Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMaxGeneration->getWrappedValue() == 0) {
		Beagle_LogTraceM(
	    	ioContext.getSystem().getLogger(),
	    	"termination", "Beagle::TermMaxGenOp",
	    	std::string("Maximum number of generations (") +
	    	uint2str(mMaxGeneration->getWrappedValue()) +
	    	std::string(") termination criterion disabled")
		);
		return false;
	}
	if(ioContext.getGeneration() > mMaxGeneration->getWrappedValue()) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    "termination", "Beagle::TermMaxGenOp",
		    std::string("Maximum number of generations (") +
		    uint2str(mMaxGeneration->getWrappedValue()) +
		    std::string(") termination criterion overpassed")
		);
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "termination", "Beagle::TermMaxGenOp",
		    std::string("Actual generation number is: ")+uint2str(ioContext.getGeneration())
		);
		return true;
	}
	if((ioContext.getGeneration() == mMaxGeneration->getWrappedValue()) &&
	        (ioContext.getDemeIndex() == (ioContext.getVivarium().size()-1))) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    "termination", "Beagle::TermMaxGenOp",
		    std::string("Maximum number of generations (") +
		    uint2str(mMaxGeneration->getWrappedValue()) +
		    std::string(") termination criterion reached")
		);
		return true;
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "termination", "Beagle::TermMaxGenOp",
	    std::string("Maximum number of generations (") +
	    uint2str(mMaxGeneration->getWrappedValue()) +
	    std::string(") termination criterion not reached")
	);
	return false;
	Beagle_StackTraceEndM("bool TermMaxGenOp::terminate(const Deme& ioDeme, Context& ioContext)");
}


