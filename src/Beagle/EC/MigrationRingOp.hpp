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
 *  \file   Beagle/EC/MigrationRingOp.hpp
 *  \brief  Definition of the class MigrationRingOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_MigrationRingOp_hpp
#define Beagle_EC_MigrationRingOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class MigrationRingOp Beagle/EC/MigrationRingOp.hpp "Beagle/EC/MigrationRingOp.hpp"
 *  \brief Migrate individuals choosen randomly, following a ring topology, operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class MigrationRingOp : public Beagle::EC::MigrationMapOp
{

public:

	//! MigrationRingOp allocator type
	typedef Beagle::AllocatorT<MigrationRingOp,Beagle::EC::MigrationMapOp::Alloc>
	Alloc;
	//! MigrationRingOp handle type.
	typedef Beagle::PointerT<MigrationRingOp,Beagle::EC::MigrationMapOp::Handle>
	Handle;
	//! MigrationRingOp bag type.
	typedef Beagle::ContainerT<MigrationRingOp,Beagle::EC::MigrationMapOp::Bag>
	Bag;

	explicit MigrationRingOp(std::string inName="MigrationRingOp");
	explicit MigrationRingOp(Beagle::EC::SelectionOp::Handle inSelectionOp,
	                         Beagle::EC::SelectionOp::Handle inReplacementOp,
	                         std::string inName="MigrationRingOp");
	virtual ~MigrationRingOp()
	{ }

	virtual void init(Beagle::System& ioSystem);
	virtual void registerParams(Beagle::System& ioSystem);

protected:

	Beagle::UInt::Handle mNumberMigrants;     //!< Number of migrants

};

}

}

#endif // Beagle_EC_MigrationRingOp_hpp
