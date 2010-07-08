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
 *  \file   beagle/Core/Deme.hpp
 *  \brief  Definition of the class Deme.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_Deme_hpp
#define Beagle_Core_Deme_hpp

#include "PACC/XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/Allocator.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/Container.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Genotype.hpp"
#include "beagle/Core/Individual.hpp"
#include "beagle/Core/Member.hpp"
#include "beagle/Core/MemberMap.hpp"
#include "beagle/Core/HallOfFame.hpp"


namespace Beagle
{

/*!
 *  \class Deme beagle/Core/Deme.hpp "beagle/Core/Deme.hpp"
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
	inline const HallOfFame::Handle getHallOfFame() const
	{
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find("HallOfFame");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<HallOfFame>(lIterMap->second);
		Beagle_StackTraceEndM("const HallOfFame::Handle getHallOfFame() const");
	}

	/*!
	 *  \brief Get an handle to the hall-of-fame.
	 *  \return Handle to the hall-of-fame, a NULL handle if no hall-of-fame is present.
	 */
	inline HallOfFame::Handle getHallOfFame()
	{
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find("HallOfFame");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<HallOfFame>(lIterMap->second);
		Beagle_StackTraceEndM("HallOfFame::Handle getHallOfFame()");
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline const Member::Handle getMember(const std::string& inName) const
	{
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM("const Member::Handle Deme::getMember(const std::string&) const");
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline Member::Handle getMember(const std::string& inName)
	{
		Beagle_StackTraceBeginM();
		MemberMap::iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM("Member::Handle Deme::getMember(const std::string&)");
	}

	/*!
	 *  \brief Get an handle to the statistics.
	 *  \return Handle to the statistics, a NULL handle if no statistics is present.
	 */
	inline const Stats::Handle getStats() const
	{
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find("Stats");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Stats>(lIterMap->second);
		Beagle_StackTraceEndM("const HallOfFame::Handle getHallOfFame() const");
	}

	/*!
	 *  \brief Get an handle to the statistics.
	 *  \return Handle to the statistics, a NULL handle if no statistics is present.
	 */
	inline Stats::Handle getStats()
	{
		Beagle_StackTraceBeginM();
		MemberMap::iterator lIterMap = mMemberMap.find("Stats");
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Stats>(lIterMap->second);
		Beagle_StackTraceEndM("HallOfFame::Handle getHallOfFame()");
	}

protected:

	MemberMap mMemberMap;     //!< Members of the deme.

public:

	virtual void read(PACC::XML::ConstIterator inIter);   // Undefined!

};

}

#endif // Beagle_Core_Deme_hpp
