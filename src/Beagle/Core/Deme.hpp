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
 *  \file   Beagle/Core/Deme.hpp
 *  \brief  Definition of the class Deme.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_Deme_hpp
#define Beagle_Core_Deme_hpp

#include "PACC/XML.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Genotype.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/Member.hpp"
#include "Beagle/Core/MemberMap.hpp"
#include "Beagle/Core/HallOfFame.hpp"


namespace Beagle
{

/*!
 *  \class Deme Beagle/Core/Deme.hpp "Beagle/Core/Deme.hpp"
 *  \brief A deme is a bag of individuals and a map of members.
 *  \ingroup Core
 *  \ingroup Pop
 */
class Deme : public Individual::Bag
{

public:

	//! Deme allocator type.
	typedef AllocatorT<Deme,Individual::Bag::Alloc> Alloc;
	//! Deme handle type.
	typedef PointerT<Deme,Individual::Bag::Handle> Handle;
	//! Deme mixed bag type.
	typedef ContainerT<Deme,Individual::Bag::Bag> Bag;

	explicit Deme(unsigned int inN=0, Individual::Handle inModel=NULL);
	virtual ~Deme()
	{ }

	virtual void                addMember(Member::Handle inMember);
	virtual void                copy(const Deme& inOriginal, System& ioSystem);
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

	MemberMap mMemberMap;     //!< Members of the deme.

public:

	virtual void read(PACC::XML::ConstIterator inIter);   // Undefined!

};

}

#endif // Beagle_Core_Deme_hpp
