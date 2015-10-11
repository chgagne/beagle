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
 *  \file   beagle/GP/PackageBase.hpp
 *  \brief  Definition of the class GP::PackageBase.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/09 21:43:10 $
 */

#ifndef Beagle_GP_PackageBase_hpp
#define Beagle_GP_PackageBase_hpp

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
 *  \class GP::PackageBase beagle/GP/PackageBase.hpp "beagle/GP/PackageBase.hpp"
 *  \brief Package for adding objects related to GP.
 *  \ingroup GPF
 */
class PackageBase : public Beagle::Package
{

public:

	//! GP::PackageBase allocator type.
	typedef AllocatorT< PackageBase, Beagle::Package::Alloc > Alloc;
	//! GP::PackageBase handle type.
	typedef PointerT< PackageBase, Beagle::Package::Handle > Handle;
	//! GP::PackageBase bag type.
	typedef ContainerT< PackageBase, Beagle::Package::Bag > Bag;

	PackageBase(void);
	explicit PackageBase(GP::PrimitiveSuperSet::Handle inPrimitiveSuperSet);
	explicit PackageBase(GP::PrimitiveSet::Handle inPrimitiveSet);

	virtual void                  configure(Beagle::System& ioSystem);
	virtual Beagle::Package::Bag  listDependencies(void);

protected:

	GP::PrimitiveSuperSet::Handle mPrimitiveSuperSet; //!< Super set of primitives used.

};

}
}

#endif // Beagle_GP_PackageBase_hpp
