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
*  \file   Beagle/Core/HistoryTrace.hpp
*  \brief  Definition of the class HistoryTrace.
*  \author Alexandre Devert
*  $Revision: 1.12 $
*  $Date: 2007/08/17 20:58:00 $
*/

#ifndef Beagle_Core_HistoryTrace_hpp
#define Beagle_Core_HistoryTrace_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/HistoryEntry.hpp"


namespace Beagle
{

/*!
 *  \class HistoryTrace Beagle/Core/HistoryTrace.hpp "Beagle/Core/HistoryTrace.hpp"
 *  \brief Evolution history storage class.
 *
 *  This class is used internally by the History system component.
 *  Its purpose is to store history entries related to an individual
 *  with a specific identification number.
 *
 *  \ingroup Core
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
	inline unsigned int getDemeID() const {
		Beagle_StackTraceBeginM();
		return mDemeID;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the deme id of this trace.
	 *  \param inDemeID The new id.
	 */
	inline void setDemeID(unsigned int inDemeID) {
		Beagle_StackTraceBeginM();
		mDemeID = inDemeID;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the individual id of this trace.
	 */
	inline unsigned int getIndividualID() const {
		Beagle_StackTraceBeginM();
		return mIndividualID;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the individual id of this trace.
	 *  \param inIndividualID The new id.
	 */
	inline void setIndividualID(unsigned int inIndividualID) {
		Beagle_StackTraceBeginM();
		mIndividualID = inIndividualID;
		Beagle_StackTraceEndM();
	}

protected:

	unsigned int mDemeID;        //!< Deme index of the trace.
	unsigned int mIndividualID;  //!< Individual index of the trace.

};

}

#endif // Beagle_Core_HistoryTrace_hpp
