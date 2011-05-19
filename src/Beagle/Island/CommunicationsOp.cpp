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
 *  \file   beagle/Distrib/Island/src/CommunicationsOp.cpp
 *  \brief  Source code of class CommunicationsOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct a CommunicationsOp.
 *  \param inCommsName Name of Communications component.
 *  \param inName Name of operator.
 */
Island::CommunicationsOp::CommunicationsOp(const std::string inCommsName,
        const std::string& inName) :
		Operator(inName),
		mCommsName(inCommsName)
{}


/*!
 *  \brief Get messages from Communications component.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void Island::CommunicationsOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Get Comms component
	System& lSystem = ioContext.getSystem();
	if(mCommsName.empty()) {
		mCommsName = lSystem.getFactory().getConceptTypeName("DBIM-Communications");
	}
	Communications::Handle lComms
	= castHandleT<Communications>(lSystem.getComponent( mCommsName ));

	// Get messages
	lComms->getMessages(ioContext);

	Beagle_StackTraceEndM();
}
