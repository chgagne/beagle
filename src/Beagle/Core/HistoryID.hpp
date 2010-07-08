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
 *  \file   beagle/Core/HistoryID.hpp
 *  \brief  Definition of the class HistoryID.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/31 06:03:26 $
 */

#ifndef Beagle_Core_HistoryID_hpp
#define Beagle_Core_HistoryID_hpp

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


namespace Beagle
{

/*!
 *  \class HistoryID beagle/Core/HistoryID.hpp "beagle/Core/HistoryID.hpp"
 *  \brief A history ID is used as member of an individual for historical tagging.
 *  \ingroup Core
 *  \ingroup History
 */
class HistoryID : public Member
{

public:

	//! Dummy history ID
	enum ID {eDummyID=UINT_MAX};

	//! HistoryID allocator type.
	typedef AllocatorT<HistoryID,Member::Alloc> Alloc;
	//! HistoryID handle type.
	typedef PointerT<HistoryID,Member::Handle> Handle;
	//! HistoryID bag type.
	typedef ContainerT<HistoryID,Member::Bag> Bag;

	explicit HistoryID(unsigned int inID=eDummyID, unsigned int inVar=0);
	virtual ~HistoryID()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return the ID of this individual.
	 */
	inline unsigned int getID() const
	{
		Beagle_StackTraceBeginM();
		return mID;
		Beagle_StackTraceEndM("unsigned int HistoryID::getID() const");
	}

	/*!
	 *  \brief Set the ID of this individual.
	 *  \param inID The new ID.
	 */
	inline void setID(unsigned int inID)
	{
		Beagle_StackTraceBeginM();
		mID = inID;
		mVar = 0;
		Beagle_StackTraceEndM("void HistoryID::setID(unsigned int inID)");
	}

	/*!
	 *  \brief Return the history variation number of this individual.
	 */
	inline unsigned int getVar() const
	{
		Beagle_StackTraceBeginM();
		return mVar;
		Beagle_StackTraceEndM("inline unsigned int HistoryID::getVar() const");
	}

	/*!
	 *  \brief Increment the history variation number of this individual.
	 */
	inline void incrementVar()
	{
		Beagle_StackTraceBeginM();
		++mVar;
		Beagle_StackTraceEndM("inline void HistoryID::incrementVar()");
	}


protected:

	unsigned int  mID;     //!< ID for the history logging mechanism.
	unsigned int  mVar;    //!< Variation number for history logging mechanism.

};

}


#endif // Beagle_Core_HistoryID_hpp
