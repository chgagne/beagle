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
 *  \file   beagle/Core/HallOfFame.hpp
 *  \brief  Definition of the class HallOfFame.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: 2007/09/12 14:23:47 $
 */

#ifndef Beagle_Core_HallOfFame_hpp
#define Beagle_Core_HallOfFame_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/Allocator.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/Container.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Individual.hpp"
#include "beagle/Core/Member.hpp"
#include "beagle/Core/Logger.hpp"


namespace Beagle
{

/*!
 *  \class HallOfFame beagle/Core/HallOfFame.hpp "beagle/Core/HallOfFame.hpp"
 *  \brief A hall-of-fame is a set containing the best-of-run individuals.
 *  \warning The individuals in the hall-of-fame are not used at all in the evolution process.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class HallOfFame : public Member
{

public:

	/*!
	 *  \struct Entry beagle/HallOfFame.hpp "beagle/HallOfFame.hpp"
	 *  \brief An entry of the hall-of-fame, that is a best-of-run individual.
	 *  \ingroup ECF
	 *  \ingroup Pop
	 */
	struct Entry
	{
		Individual::Handle mIndividual;    //!< Individual member of the hall-of-fame.
		unsigned int       mGeneration;    //!< Generation of introduction in the hall-of-fame.
		unsigned int       mDemeIndex;     //!< Deme index to which the member belong.

		Entry(Individual::Handle inIndividual=NULL,
		       unsigned int inGeneration=0,
		       unsigned int inDemeIndex=0);
		bool operator==(const HallOfFame::Entry& inRightEntry) const;
		bool operator<(const HallOfFame::Entry& inRightEntry) const;
		bool operator>(const HallOfFame::Entry& inRightEntry) const;
	};

	//! HallOfFame allocator type.
	typedef AllocatorT<HallOfFame,Member::Alloc> Alloc;
	//! HallOfFame handle type.
	typedef PointerT<HallOfFame,Member::Handle> Handle;
	//! HallOfFame bag type.
	typedef ContainerT<HallOfFame,Member::Bag> Bag;

	explicit HallOfFame(unsigned int inN=0);
	explicit HallOfFame(unsigned int inN, const HallOfFame::Entry& inModel);
	virtual ~HallOfFame()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	void                        log(Logger::LogLevel inLogLevel, Context& ioContext) const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	void                        sort();
	virtual bool                updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext);
	virtual bool                updateWithIndividual(unsigned int inSizeHOF, const Individual& inIndividual, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief  Access to a constant reference to the Nth member of the hall-of-fame.
	 *  \param  inN Indice of the member to get.
	 *  \return Constant reference to the member at the indice N.
	 */
	inline const HallOfFame::Entry& operator[](unsigned int inN) const
	{
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,mEntries.size()-1);
		return mEntries[inN];
		Beagle_StackTraceEndM("const HallOfFame::Entry& HallOfFame::operator[](unsigned int) const");
	}

	/*!
	 *  \brief  Access to a reference to the Nth member of the hall-of-fame.
	 *  \param  inN Indice of the member to get.
	 *  \return Reference to the member at the indice N.
	 */
	inline HallOfFame::Entry& operator[](unsigned int inN)
	{
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,mEntries.size()-1);
		return mEntries[inN];
		Beagle_StackTraceEndM("HallOfFame::Entry& HallOfFame::operator[](unsigned int)");
	}

	/*!
	 *  \brief  Clear individuals from hall of fame
	 */
	inline void clear()
	{
		Beagle_StackTraceBeginM();
		mEntries.clear();
		Beagle_StackTraceEndM("void clear()");
	}

	/*!
	 *  \return Number of members in the hall-of-fame.
	 */
	inline unsigned int size() const
	{
		Beagle_StackTraceBeginM();
		return mEntries.size();
		Beagle_StackTraceEndM("unsigned int HallOfFame::size() const");
	}

protected:

	std::vector< HallOfFame::Entry > mEntries;   //!< Vector of members of the hall-of-fame.

};

}


#endif // Beagle_Core_HallOfFame_hpp

