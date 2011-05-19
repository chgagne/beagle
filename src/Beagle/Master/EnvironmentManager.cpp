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
 *  \file   beagle/Distrib/Master/src/EnvironmentManager.cpp
 *  \brief  Source code of class EnvironmentManager.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.6 $
 *  $Date: 2007/10/02 14:56:36 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Read a XMLNode of the environment.
 *  \param inRootNode Handle to the root node of the environment.
 *  \param ioDeme Current deme of the environment.
 *  \param ioContext Context of the evolution.
 */
void Master::EnvironmentManager::readEnvironment(PACC::XML::ConstIterator inRootNode,
        Beagle::Deme& ioDeme,
        Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();

	ioDeme.readMembers(inRootNode, ioContext);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write in a XMLStreamer the environment.
 *  \param ioStreamer The deme's environment will be added to the streamer's end.
 *  \param ioDeme Current deme of the environment.
 *  \param ioContext Context of the evolution.
 */
void Master::EnvironmentManager::writeEnvironment(PACC::XML::Streamer& ioStreamer,
        Beagle::Deme& ioDeme,
        Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();

	ioStreamer.insertAttribute("dist", "no");   //distribute the environment
	ioDeme.writeMembers(ioStreamer);

	Beagle_StackTraceEndM();
}


void Master::EnvironmentManager::setAsEnvManager() const
{
	Beagle_StackTraceBeginM();

	if(Master::EvolClient::getSingleton() != NULL) {
		Master::EvolClient::getSingleton()->setEnvManager(Handle(this));
	}
	if(Master::EvalClient::getSingleton() != NULL) {
		Master::EvalClient::getSingleton()->setEnvManager(Handle(this));
	}

	Beagle_StackTraceEndM();
}

