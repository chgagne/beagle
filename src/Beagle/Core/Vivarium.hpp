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
 *  \file   Beagle/Core/Vivarium.hpp
 *  \brief  Definition of the class Vivarium.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/17 18:09:13 $
 */

/*!
 *  \defgroup Pop Population
 *  \ingroup Core
 *  \brief Components of the population, part of the Core library.
 *
 *  The population is structured into four layers: a vivarium, the demes, the individuals and the
 *  genotypes. The first layer, the vivarium, comprises the whole population of a specific evolution,
 *  that is, an aggregate of one or more demes. A deme is a population that evolves
 *  in an independent environment. Generally, at each generation, there are some individuals that
 *  migrate between the demes that compose a vivarium. A deme is implemented in class Beagle::Deme.
 *  The class provides and implements an interface of standard methods necessary to evolve its own
 *  population. The class Beagle::Vivarium describes a bag of demes, which is itself an bag of
 *  individuals. Each deme also has a hall-of-fame where the best-of-run individuals are conserved,
 *  and its statistics.
 *
 *  The next underlying layer, the individual, is defined in class Beagle::Individual. It is an bag
 *  of genotypes, the parts that compose a genome. The genotypes are tightly related to the
 *  representation of the individuals for a given EC algorithm. The genotype interface is declared in
 *  the abstract class Beagle::Genotype.
 *
 */


#ifndef Beagle_Core_Vivarium_hpp
#define Beagle_Core_Vivarium_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Deme.hpp"
#include "Beagle/Core/Member.hpp"
#include "Beagle/Core/MemberMap.hpp"
#include "Beagle/Core/HallOfFame.hpp"


namespace Beagle
{

/*!
 *  \class Vivarium Beagle/Core/Vivarium.hpp "Beagle/Core/Vivarium.hpp"
 *  \brief A vivarium is a bag of demes and a map of members.
 *  \ingroup Core
 *  \ingroup Pop
 */
class Vivarium : public Deme::Bag
{

public:

	//! Vivarium allocator type.
	typedef AllocatorT<Vivarium,Deme::Bag::Alloc> Alloc;
	//! Vivarium handle type.
	typedef PointerT<Vivarium,Deme::Bag::Handle> Handle;
	//! Vivarium bag type.
	typedef ContainerT<Vivarium,Deme::Bag::Bag> Bag;

	explicit Vivarium(unsigned int inN=0, Deme::Handle inModel=NULL);
	virtual ~Vivarium()
	{ }

	virtual void                addMember(Member::Handle inMember);
	virtual void                copy(const Vivarium& inOriginal, System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	virtual void                readMembers(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual Member::Handle      removeMember(const std::string& inName);
	virtual void                write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeMembers(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Get an handle to the hall-of-fame.
	 *  \return Handle to the hall-of-fame, a NULL handle if no hall-of-fame is present.
	 */
	inline const HallOfFame::Handle getHallOfFame() const {
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find("HallOfFame");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<HallOfFame>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get an handle to the hall-of-fame.
	 *  \return Handle to the hall-of-fame, a NULL handle if no hall-of-fame is present.
	 */
	inline HallOfFame::Handle getHallOfFame() {
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find("HallOfFame");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<HallOfFame>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline const Member::Handle getMember(const std::string& inName) const {
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline Member::Handle getMember(const std::string& inName) {
		Beagle_StackTraceBeginM();
		MemberMap::iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get an handle to the statistics.
	 *  \return Handle to the statistics, a NULL handle if no statistics is present.
	 */
	inline const Stats::Handle getStats() const {
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find("Stats");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Stats>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get an handle to the statistics.
	 *  \return Handle to the statistics, a NULL handle if no statistics is present.
	 */
	inline Stats::Handle getStats() {
		Beagle_StackTraceBeginM();
		MemberMap::iterator lIterMap = mMemberMap.find("Stats");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Stats>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

protected:

	MemberMap mMemberMap;     //!< Members of the vivarium.

public:

	virtual void read(PACC::XML::ConstIterator inIter);    // Undefined!

};

}

#endif // Beagle_Core_Vivarium_hpp

