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
 *  \file   beagle/Distrib/Island/src/PackageTCPIP.cpp
 *  \brief  Source code of class Distrib::Island::PackageTCPIP.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/09/12 14:23:46 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with island-model TCP/IP distribution objects.
 */
Distrib::Island::PackageTCPIP::PackageTCPIP() :
		Beagle::Package("DBIM-PackageTCPIP")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void Distrib::Island::PackageTCPIP::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::Distrib::Island::CommunicationsTCPIP", new Distrib::Island::CommunicationsTCPIP::Alloc());
	lFactory.insertAllocator("Beagle::Distrib::Island::PackageTCPIP", new Distrib::Island::PackageTCPIP::Alloc());

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::Distrib::Island::CommunicationsTCPIP", "DBIM-CommunicationsTCPIP");
	lFactory.aliasAllocator("Beagle::Distrib::Island::PackageTCPIP", "DBIM-PackageTCPIP");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("DBIM-Communications", "DBIM-CommunicationsTCPIP");

	// Add TCP/IP communications components
	if(ioSystem.haveComponent("DBIM-CommunicationsTCPIP") == NULL) {
		ioSystem.addComponent(new Distrib::Island::CommunicationsTCPIP);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  Distrib::Island::PackageTCPIP depends on Distrib::Island::Package.
 */
Beagle::Package::Bag Distrib::Island::PackageTCPIP::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Beagle::Package::Bag lDependencies;
	lDependencies.push_back(new Distrib::Island::Package());
	return lDependencies;
	Beagle_StackTraceEndM();
}
