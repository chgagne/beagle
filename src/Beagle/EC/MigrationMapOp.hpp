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
 *  \file   Beagle/EC/MigrationMapOp.hpp
 *  \brief  Definition of the class MigrationMapOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.14 $
 *  $Date: 2007/08/13 13:04:34 $
 */

#ifndef Beagle_EC_MigrationMapOp_hpp
#define Beagle_EC_MigrationMapOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/MigrationOp.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class MigrationMapOp Beagle/EC/MigrationMapOp.hpp "Beagle/EC/MigrationMapOp.hpp"
 *  \brief Migration operator that stores a mapping of deme to deme migration
 *  \ingroup EC
 *  \ingroup Op
 *
 *  readWithSystem() allows specification via configuration file of
 *  selection and replacement strategies.  So
 *  <tt>\<NameOfMigrationOp select="SelectTournamentOp"\\\></tt>
 *  specifies that tournament selection will be used to select a deme's emigrants.
 *
 *  If a replacement operator is specified then it will be used to
 *  select which individuals will be replaced by incomming immigrants.
 *  <tt>\<NameOfMigrationOp replace="SelectRandomOp"\\\></tt> will replace
 *  randomly selected individuals with any incomming immigrants.

 */
class MigrationMapOp : public Beagle::EC::MigrationOp
{

public:

	//! MigrationMapOp allocator type
	typedef Beagle::AllocatorT<MigrationMapOp,Beagle::EC::MigrationOp::Alloc>
	Alloc;
	//! MigrationMapOp handle type.
	typedef Beagle::PointerT<MigrationMapOp,Beagle::EC::MigrationOp::Handle>
	Handle;
	//! MigrationMapOp bag type.
	typedef Beagle::ContainerT<MigrationMapOp,Beagle::EC::MigrationOp::Bag>
	Bag;

	explicit MigrationMapOp(std::string inName="MigrationMapOp");
	explicit MigrationMapOp(Beagle::EC::SelectionOp::Handle inSelectionOp,
	                        Beagle::EC::SelectionOp::Handle inReplacementOp,
	                        std::string inName="MigrationMapOp");
	virtual ~MigrationMapOp()
	{ }

	virtual void migrate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void init(Beagle::System& ioSystem);
	virtual void registerParams(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void writeContent(PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

protected:

	void logMap(const std::vector< std::vector<unsigned int> >& inMigrationMap,
	            Beagle::System& ioSystem) const;
	void validateMap(const std::vector< std::vector<unsigned int> >& inMigrationMap,
	                 Beagle::System& ioSystem) const;

	Beagle::EC::SelectionOp::Handle mSelectionOp;   //!< To select the emigrants.
	Beagle::EC::SelectionOp::Handle mReplacementOp; //!< To select individuals replaced by immigrants.

	//! Migration matrix listing number of emigrants between each deme.
	std::vector< std::vector<unsigned int> > mMigrationMap;


};

}

}

#endif // Beagle_EC_MigrationMapOp_hpp
