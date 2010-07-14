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
 *  \file   Beagle/EC/MigrationOp.hpp
 *  \brief  Definition of the class MigrationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.15 $
 *  $Date: 2007/08/13 13:04:34 $
 */

#ifndef Beagle_EC_MigrationOp_hpp
#define Beagle_EC_MigrationOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class MigrationOp Beagle/EC/MigrationOp.hpp "Beagle/EC/MigrationOp.hpp"
 *  \brief Individuals migration operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class MigrationOp : public Beagle::Operator
{

public:

	//! MigrationOp allocator type
	typedef Beagle::AbstractAllocT<MigrationOp,Beagle::Operator::Alloc>
	Alloc;
	//! MigrationOp handle type.
	typedef Beagle::PointerT<MigrationOp,Beagle::Operator::Handle>
	Handle;
	//! MigrationOp bag type.
	typedef Beagle::ContainerT<MigrationOp,Beagle::Operator::Bag>
	Bag;

	explicit MigrationOp(std::string inName="MigrationOp");
	virtual ~MigrationOp()
	{ }

	/*!
	 *  \brief Migrate individuals of a deme.
	 *  \param ioDeme Deme containing individuals to migrate.
	 *  \param ioContext Evolution context.
	 */
	virtual void migrate(Beagle::Deme& ioDeme, Beagle::Context& ioContext) =0;

	virtual void init(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void registerParams(Beagle::System& ioSystem);

protected:

	Beagle::UInt::Handle      mMigrationInterval;  //!< Random ring migration interval (in generation).
	Beagle::UIntArray::Handle mPopSize;            //!< Population size for the evolution.

};

}

}

#endif // Beagle_EC_MigrationOp_hpp
