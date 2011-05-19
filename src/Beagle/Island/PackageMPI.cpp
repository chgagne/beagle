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
 *  \file   beagle/Distrib/Island/src/PackageMPI.cpp
 *  \brief  Source code of class Distrib::Island::PackageMPI.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:31 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with island-model MPI distribution objects.
 */
Distrib::Island::PackageMPI::PackageMPI() :
		Beagle::Package("DBIM-PackageMPI")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void Distrib::Island::PackageMPI::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::Distrib::Island::CommunicationsMPI", new Distrib::Island::CommunicationsMPI::Alloc);
	lFactory.insertAllocator("Beagle::Distrib::Island::PackageMPI", new Distrib::Island::PackageMPI::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::Distrib::Island::CommunicationsMPI", "DBIM-CommunicationsMPI");
	lFactory.aliasAllocator("Beagle::Distrib::Island::PackageMPI", "DBIM-PackageMPI");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("DBIM-Communications", "DBIM-CommunicationsMPI");

	// Add TCP/IP communications components
	if(ioSystem->haveComponent("DBIM-CommunicationsMPI") == false) {
		ioSystem->addComponent(new Distrib::Island::CommunicationsMPI);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  Distrib::Island::PackageMPI depends on Distrib::Island::Package.
 */
Beagle::Package::Bag Distrib::Island::PackageMPI::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Beagle::Package::Bag lDependencies;
	lDependencies.push_back(new Distrib::Island::Package());
	return lDependencies;
	Beagle_StackTraceEndM();
}
