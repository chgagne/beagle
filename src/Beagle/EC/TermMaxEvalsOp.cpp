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
 *  \file   Beagle/EC/TermMaxEvalsOp.cpp
 *  \brief  Source code of class TermMaxEvalsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator based on a maximum number of evalutions.
 *  \param inName Name of the operator.
 */
TermMaxEvalsOp::TermMaxEvalsOp(std::string inName) :
		Beagle::EC::TerminationOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxEvalsOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	std::ostringstream lOSS;
	lOSS << "Maximum number of fitness evaluations for the evolution. ";
	lOSS << "A zero value means that there is no evaluation limits.";
	Register::Description lDescription(
	    "Max evaluations term criterion",
	    "UInt",
	    "5000",
	    lOSS.str()
	);
	mMaxEvaluations = castHandleT<UInt>(
	                      ioSystem.getRegister().insertEntry("ec.term.maxevals", new UInt(5000), lDescription));

	Beagle_StackTraceEndM("void TermMaxEvalsOp::registerParams(System&)");
}


/*!
 *  \brief Check if the maximum number of fitness evaluations is reached.
 *  \param ioDeme
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxEvalsOp::terminate(const Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMaxEvaluations->getWrappedValue() == 0) return false;
	if(ioContext.getTotalProcessedVivarium() >= mMaxEvaluations->getWrappedValue()) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    "termination", "Beagle::TermMaxEvalsOp",
		    std::string("Maximum number of fitness evaluations (") +
		    uint2str(mMaxEvaluations->getWrappedValue()) +
		    std::string(") termination criterion reached")
		);
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "termination", "Beagle::TermMaxEvalsOp",
		    std::string("Actual number of fitness evaluations is: ")+
		    uint2str(ioContext.getTotalProcessedVivarium())
		);
		return true;
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "termination", "Beagle::TermMaxEvalsOp",
	    std::string("Maximum number of fitness evaluations (") +
	    uint2str(mMaxEvaluations->getWrappedValue()) +
	    std::string(") termination criterion not reached ")
	);
	return false;
	Beagle_StackTraceEndM("bool TermMaxEvalsOp::terminate(const Deme& ioDeme, Context& ioContext)");
}


