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
 *  \file   Beagle/EC/MigrationGridOp.hpp
 *  \brief  Definition of the class Beagle::EC::MigrationGridOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_MigrationGridOp_hpp
#define Beagle_EC_MigrationGridOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/MigrationMapOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class MigrationGridOp Beagle/EC/MigrationGridOp.hpp "Beagle/EC/MigrationGridOp.hpp"
 *  \brief Migrate individuals following a grid topology.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  This migration operator exchange individuals following a grid topology.
 */
class MigrationGridOp : public Beagle::EC::MigrationMapOp
{

public:

	//! MigrationGridOp allocator type
	typedef Beagle::AllocatorT<MigrationGridOp,Beagle::EC::MigrationMapOp::Alloc>
	Alloc;
	//! MigrationGridOp handle type.
	typedef Beagle::PointerT<MigrationGridOp,Beagle::EC::MigrationMapOp::Handle>
	Handle;
	//! MigrationGridOp bag type.
	typedef Beagle::ContainerT<MigrationGridOp,Beagle::EC::MigrationMapOp::Bag>
	Bag;

	explicit MigrationGridOp(std::string inName="MigrationGridOp");
	explicit MigrationGridOp(SelectionOp::Handle inSelectionOp,
	                         SelectionOp::Handle inReplacementOp,
	                         std::string inName="MigrationGridOp");
	virtual ~MigrationGridOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void init(Beagle::System& ioSystem);

protected:

	Beagle::UInt::Handle mNumberMigrants; //!< Number of migrants.
	Beagle::UInt::Handle mGridWidth;      //!< Width of the grid (height is determined automatically).
	Beagle::Bool::Handle mToroidal;       //!< If grid will be toroidal (wraps left-right, top-bottom).

};

}

}

#endif // Beagle_EC_MigrationGridOp_hpp
