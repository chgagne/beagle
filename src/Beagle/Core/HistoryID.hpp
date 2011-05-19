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
 *  \file   Beagle/Core/HistoryID.hpp
 *  \brief  Definition of the class HistoryID.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/31 06:03:26 $
 */

#ifndef Beagle_Core_HistoryID_hpp
#define Beagle_Core_HistoryID_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/Member.hpp"


namespace Beagle
{

/*!
 *  \class HistoryID Beagle/Core/HistoryID.hpp "Beagle/Core/HistoryID.hpp"
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
	inline unsigned int getID() const {
		Beagle_StackTraceBeginM();
		return mID;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the ID of this individual.
	 *  \param inID The new ID.
	 */
	inline void setID(unsigned int inID) {
		Beagle_StackTraceBeginM();
		mID = inID;
		mVar = 0;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the history variation number of this individual.
	 */
	inline unsigned int getVar() const {
		Beagle_StackTraceBeginM();
		return mVar;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Increment the history variation number of this individual.
	 */
	inline void incrementVar() {
		Beagle_StackTraceBeginM();
		++mVar;
		Beagle_StackTraceEndM();
	}


protected:

	unsigned int  mID;     //!< ID for the history logging mechanism.
	unsigned int  mVar;    //!< Variation number for history logging mechanism.

};

}


#endif // Beagle_Core_HistoryID_hpp
