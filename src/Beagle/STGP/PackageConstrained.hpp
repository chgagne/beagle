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
 *  \file   beagle/GP/PackageConstrained.hpp
 *  \brief  Definition of the class GP::PackageConstrained.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/10 20:32:21 $
 */

#ifndef Beagle_GP_PackageConstrained_hpp
#define Beagle_GP_PackageConstrained_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/System.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/Package.hpp"


namespace Beagle
{

namespace GP
{

/*!
 *  \class GP::PackageConstrained beagle/GP/PackageConstrained.hpp "beagle/GP/PackageConstrained.hpp"
 *  \brief Package for adding objects related to constrained GP.
 *  \ingroup GPF
 */
class PackageConstrained : public Beagle::Package
{

public:

	//! GP::PackageConstrained allocator type.
	typedef AllocatorT< PackageConstrained, Beagle::Package::Alloc > Alloc;
	//! GP::PackageConstrained handle type.
	typedef PointerT< PackageConstrained, Beagle::Package::Handle > Handle;
	//! GP::PackageConstrained bag type.
	typedef ContainerT< PackageConstrained, Beagle::Package::Bag > Bag;

	PackageConstrained(void);
	explicit PackageConstrained(GP::PrimitiveSuperSet::Handle inPrimitiveSuperSet);
	explicit PackageConstrained(GP::PrimitiveSet::Handle inPrimitiveSet);

	virtual void                  configure(Beagle::System& ioSystem);
	virtual Beagle::Package::Bag  listDependencies(void);

protected:

	GP::PrimitiveSuperSet::Handle mPrimitiveSuperSet; //!< Super set of primitives used.

};

}
}

#endif // Beagle_GP_PackageConstrained_hpp
