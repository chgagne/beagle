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
 *  \file   beagle/Coev/src/Package.cpp
 *  \brief  Source code of class Coev::Package.
 *  \author Christian Gagne
 *  $Revision: 1.3 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#include "beagle/Coev.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with island-model distribution basic objects.
 */
Coev::Package::Package() :
		Beagle::Package("Coev-Package")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void Coev::Package::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::Coev::Package", new Coev::Package::Alloc);
	lFactory.insertAllocator("Beagle::Coev::TermBroadcastOp", new Coev::TermBroadcastOp::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::Coev::Package", "Coev-Package");
	lFactory.aliasAllocator("Beagle::Coev::TermBroadcastOp", "Coev-TermBroadcastOp");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  Coev::Package depends on Beagle::PackageBase.
 */
Beagle::Package::Bag Coev::Package::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new PackageBase());
	return lDependencies;
	Beagle_StackTraceEndM();
}
