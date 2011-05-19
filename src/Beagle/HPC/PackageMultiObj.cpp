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
 *  \file   beagle/HPC/PackageMultiObj.cpp
 *  \brief  Source code of class HPC::PackageMultiObj.
 *  \author Christian Gagne
 *  \author Felix-Antoine Fortin
 *  $Revision: 1.4 $
 *  $Date: $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;


/*!
 *  \brief Construct HPC multi-objective package.
 */
HPC::PackageMultiObj::PackageMultiObj() :
		Package("HPC-PackageMultiObj")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 *  This package adds the HPC AlgoNSGA2 object to the factory,
 *  sets the SelectionOp concept as a random selection, and sets
 *  the algorithm concept as HPC-AlgoNSGA2.
 */
void HPC::PackageMultiObj::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::HPC::AlgoNSGA2", new HPC::AlgoNSGA2::Alloc);

	// Aliases to short name of operators
	lFactory.aliasAllocator("Beagle::HPC::AlgoNSGA2", "HPC-AlgoNSGA2");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("SelectionOp", "SelectRandomOp");
	lFactory.setConcept("Algorithm", "HPC-AlgoNSGA2");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *  HPC::PackageMultiObj depends on Beagle::PackageBase, Beagle::PackageMultiObj
 *  and HPC::PackageBase,
 */
Package::Bag HPC::PackageMultiObj::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	Package::Bag lDependencies;
	lDependencies.push_back(new Beagle::PackageBase);
	lDependencies.push_back(new Beagle::PackageMultiObj);
	lDependencies.push_back(new HPC::PackageBase);
	return lDependencies;
	Beagle_StackTraceEndM();
}
