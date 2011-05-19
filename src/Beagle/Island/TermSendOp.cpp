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
 *  \file   beagle/Distrib/Island/src/TermSendOp.cpp
 *  \brief  Source code of class TermSendOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:33 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct a TermSendOp.
 *  \param inName Name of the operator.
 */
Island::TermSendOp::TermSendOp(const std::string& inCommsName,
                               const std::string& inDefaultRecipient,
                               const std::string& inName) :
		SendOp(inCommsName,inDefaultRecipient,inName)
{ }


/*!
 *  \brief Initialize the TransportSend operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void Island::TermSendOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogVerboseM(
	    ioSystem.getLogger(),
	    "transport", "Beagle::TransportSendOp",
	    "Initializing TransportSendOp"
	);
	// Add "on success" flag
	{
		std::ostringstream lOSS;
		lOSS << "Flag to indicate if termination messages should be sent when the evolution succeeds.";
		Register::Description lDescription(
		    "Flag for sending termination-on-success",
		    "Bool",
		    "1",
		    lOSS.str()
		);
		mSendOnSuccess = castHandleT<Bool>(
		                     ioSystem.getRegister().insertEntry("ec.term.sendonsuccess", new Bool(1), lDescription));
	}
	// Add "on failure" flag
	{
		std::ostringstream lOSS;
		lOSS << "Flag to indicate if termination messages should be sent when the evolution fails.";
		Register::Description lDescription(
		    "Flag for sending termination-on-failure",
		    "Bool",
		    "0",
		    lOSS.str()
		);
		mSendOnFailure = castHandleT<Bool>(
		                     ioSystem.getRegister().insertEntry("ec.term.sendonfailure", new Bool(0), lDescription));
	}
	Beagle_StackTraceEndM();
}




/*!
 *  \brief Check if termination has been requested (by checking
 *  Context::getContinueFlag()).  If termination has been requested,
 *  propagate request to specified addresses via Communications
 *  component.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void Island::TermSendOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	SendOp::operate(ioDeme, ioContext);
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "termination", "Beagle::TermSendOp",
	    "Checking if termination has been requested"
	);
	if (!ioContext.getContinueFlag()) {
		if (ioContext.isTerminationSuccessful()) {
			if (mSendOnSuccess->getWrappedValue()) {
				Beagle_LogInfoM(
				    ioContext.getSystem().getLogger(),
				    "termination", "Beagle::TermSendOp",
				    "Successful termination has been detected; propagating termination request"
				);
				// Send termination request
				sendToAll("Terminate",ioContext);
			} else {
				Beagle_LogInfoM(
				    ioContext.getSystem().getLogger(),
				    "termination", "Beagle::TermSendOp",
				    "Successful termination has been detected; termination request not propagated (see 'ec.term.sendonsuccess')"
				);
			}
		} else {
			if (mSendOnFailure->getWrappedValue()) {
				Beagle_LogInfoM(
				    ioContext.getSystem().getLogger(),
				    "termination", "Beagle::TermSendOp",
				    "Unsuccessful termination has been detected; propagating termination request"
				);
				// Send termination request
				sendToAll("Terminate",ioContext);
			} else {
				Beagle_LogInfoM(
				    ioContext.getSystem().getLogger(),
				    "termination", "Beagle::TermSendOp",
				    "Unsuccessful termination has been detected; termination will not be propagated (see 'ec.term.sendonfailure')"
				);
			}
		}
	}
	Beagle_StackTraceEndM();
}

