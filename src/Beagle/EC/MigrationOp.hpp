/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
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
