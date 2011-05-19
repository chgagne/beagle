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
 *  \file   beagle/Coev/src/TermBroadcastOp.cpp
 *  \brief  Source code of class TermBroadcastOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#include "beagle/Coev.hpp"

using namespace Beagle;


/*
 *  Initialize condition for synchronizing threads termination.
 */
PACC::Threading::Condition Coev::TermBroadcastOp::smCondition;

/*
 *  Initialize actual number of threads locked on condition.
 */
unsigned int Coev::TermBroadcastOp::smTrigger(0);

/*
 *  Initialize map of system pointer used to count number of thread involved in coevolution.
 */
std::set<void*> Coev::TermBroadcastOp::smSysPtrsInvolved;

/*
 *  Initialize number of threads involved in coevolution.
 */
unsigned int Coev::TermBroadcastOp::smNbrThreads(0);

/*
 *  Initialize stopping criterion.
 */
bool Coev::TermBroadcastOp::smTermReached(false);


/*!
 *  \brief Construct a co-evolution broadcast termination stop operator.
 *  \param inName Name of the operator.
 */
Coev::TermBroadcastOp::TermBroadcastOp(std::string inName) :
		TerminationOp(inName)
{ }


/*!
 *  \brief Register parameters for the maximum fitness value termination operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void Coev::TermBroadcastOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::TerminationOp::registerParams(ioSystem);
	smCondition.lock();
	if(smSysPtrsInvolved.find(&ioSystem) == smSysPtrsInvolved.end()) {
		smSysPtrsInvolved.insert(&ioSystem);
		++smNbrThreads;
	}
	smCondition.unlock();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Broadcast and receive termination criterion value along the running threads.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is broadcasted, false if not.
 */
bool Coev::TermBroadcastOp::terminate(const Deme& inDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	smCondition.lock();
	if(ioContext.getContinueFlag() == false) smTermReached = true;
	if(++smTrigger == smNbrThreads) smCondition.broadcast();
	else smCondition.wait();
	bool lTermReached = smTermReached;
	if(--smTrigger == 0) smTermReached = false;
	smCondition.unlock();

	if(lTermReached) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    "Termination criterion broadcasted/received"
		);
	} else {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "No termination criterion broadcasted"
		);
	}
	return lTermReached;
	Beagle_StackTraceEndM();
}


