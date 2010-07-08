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
*  \file   beagle/Core/HistoryTrace.hpp
*  \brief  Definition of the class HistoryTrace.
*  \author Alexandre Devert
*  $Revision: 1.12 $
*  $Date: 2007/08/17 20:58:00 $
*/

#ifndef Beagle_Core_HistoryTrace_hpp
#define Beagle_Core_HistoryTrace_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Component.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/HistoryEntry.hpp"


namespace Beagle
{

/*!
 *  \class HistoryTrace beagle/Core/HistoryTrace.hpp "beagle/Core/HistoryTrace.hpp"
 *  \brief Evolution history storage class.
 *
 *  This class is used internally by the History system component.
 *  Its purpose is to store history entries related to an individual
 *  with a specific identification number.
 *
 *  \ingroup ECF
 *  \ingroup Sys
 *  \ingroup History
 */
class HistoryTrace : public HistoryEntry::Bag
{

public:

	//! HistoryTrace allocator type.
	typedef AllocatorT<HistoryTrace,HistoryEntry::Bag::Alloc> Alloc;
	//! HistoryTrace handle type.
	typedef PointerT<HistoryTrace,HistoryEntry::Bag::Handle> Handle;
	//! HistoryTrace bag type.
	typedef ContainerT<HistoryTrace,HistoryEntry::Bag::Bag> Bag;

	explicit HistoryTrace(unsigned int inN=0, HistoryEntry::Handle inModel=NULL);
	virtual ~HistoryTrace()
	{ }

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return the deme id of this trace.
	 */
	inline unsigned int getDemeID() const
	{
		Beagle_StackTraceBeginM();
		return mDemeID;
		Beagle_StackTraceEndM("unsigned int HistoryTrace::getDemeID() const");
	}

	/*!
	 *  \brief Set the deme id of this trace.
	 *  \param inDemeID The new id.
	 */
	inline void setDemeID(unsigned int inDemeID)
	{
		Beagle_StackTraceBeginM();
		mDemeID = inDemeID;
		Beagle_StackTraceEndM("void HistoryTrace::setDemeID(unsigned int inDemeID)");
	}

	/*!
	 *  \brief Return the individual id of this trace.
	 */
	inline unsigned int getIndividualID() const
	{
		Beagle_StackTraceBeginM();
		return mIndividualID;
		Beagle_StackTraceEndM("unsigned int HistoryTrace::getIndividualID() const");
	}

	/*!
	 *  \brief Set the individual id of this trace.
	 *  \param inIndividualID The new id.
	 */
	inline void setIndividualID(unsigned int inIndividualID)
	{
		Beagle_StackTraceBeginM();
		mIndividualID = inIndividualID;
		Beagle_StackTraceEndM("void HistoryTrace::setIndividualID(unsigned int inIndividualID)");
	}

protected:

	unsigned int mDemeID;        //!< Deme index of the trace.
	unsigned int mIndividualID;  //!< Individual index of the trace.

};

}

#endif // Beagle_Core_HistoryTrace_hpp
