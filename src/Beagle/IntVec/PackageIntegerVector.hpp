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
 *  \file   beagle/GA/PackageIntegerVector.hpp
 *  \brief  Definition of the class GA::PackageIntegerVector.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_PackageIntegerVector_hpp
#define Beagle_GA_PackageIntegerVector_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/System.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/Package.hpp"


namespace Beagle
{

namespace GA
{

/*!
 *  \class GA::PackageIntegerVector beagle/GA/PackageIntegerVector.hpp "beagle/GA/PackageIntegerVector.hpp"
 *  \brief Package for adding objects related to integer vector GA.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class PackageIntegerVector : public Package
{

public:

	//! GA::PackageIntegerVector allocator type.
	typedef AllocatorT< PackageIntegerVector, Package::Alloc >
	Alloc;
	//! GA::PackageIntegerVector handle type.
	typedef PointerT< PackageIntegerVector, Package::Handle >
	Handle;
	//! GA::PackageIntegerVector bag type.
	typedef ContainerT< PackageIntegerVector, Package::Bag >
	Bag;

	explicit PackageIntegerVector(unsigned int inInitSize=0);

	virtual void          configure(System& ioSystem);
	virtual Package::Bag  listDependencies(void);

protected:

	unsigned int mInitSize;  //!< Size of vector to use at initialization.

};

}
}

#endif // Beagle_GA_PackageIntegerVector_hpp
