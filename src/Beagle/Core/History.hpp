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
 *  \file   beagle/Core/History.hpp
 *  \brief  Definition of the class History.
 *  \author Alexandre Devert
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 20:58:00 $
 */

/*!
 *  \defgroup History History Storage Mechanism
 *  \ingroup Core
 *
 *  \brief The History mechanism stores the operations that occurred
 *  during evolution (e.g. crossover, mutation, evaluation).  It
 *  allows you to produce a genealogy tree for each individual in the
 *  population.
 *
 *  All that is required for the storage of History data is for the
 *  History component to be installed into the System.  To do this,
 *  create an instance of Beagle::System and then call the
 *  Beagle::System::addComponent() method passing a handle to an
 *  instance of a Beagle::History component:
 *
 *  \code 
 *  Beagle::System::Handle lSystem = new System;
 *  lSystem->addComponent(new Beagle::History());
 *  \endcode
 *
 *  The history data can then be found in the milestone files that
 *  Beagle produces (see Beagle::MilestoneWriteOp).  When a History
 *  component is installed, individuals will be allocated
 *  identification numbers.  Along with identification numbers,
 *  variation numbers are also stored; these state how many times an
 *  individual has been modified.  The identification and variation
 *  numbers together uniquely identify an individual in evolutionary
 *  history.
 *
 *  The history data produced by the above code example does not
 *  contain the actual individuals.  If the individuals are also to
 *  be written to the history data, an appropriate individual allocator
 *  should be passed as the argument to the History constructor in the
 *  above code example.
 *
 *  Even if a System has a History component installed, the output of
 *  data can be switched off with the use of the \c hs.record.activated
 *  register variable.  The use of this flag will reduce memory
 *  consumption, and history data will still appear in the log file if
 *  the log level is set to 'trace' (level 5) or higher.
 *
 *  If memory consumption becomes an issue, the Beagle::HistoryFlushOp
 *  operator will clear the memory used once the data has been written
 *  to a milestone file.  Continuity of history data between consecutive
 *  milestone files is maintained by the history identification numbers.
 *
 *  If, for example, you have created a new genetic operator and would
 *  like to add the storage of your operation to the history
 *  mechanism, then the Beagle::History::trace() method should be
 *  used.  You can find examples of its use in the
 *  Beagle::CrossoverOp::operate() and Beagle::MutationOp::operate()
 *  methods.  If you are modifying an individual with your operator
 *  then you should also increment the individual's variation number
 *  with a call to the History::incrementHistoryVar() method.
 */

#ifndef Beagle_Core_History_hpp
#define Beagle_Core_History_hpp

#include "PACC/Util.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Component.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/Bool.hpp"
#include "beagle/Core/HistoryTrace.hpp"


namespace Beagle
{

// Forward declaration
class System;

/*!
 *  \class History beagle/Core/History.hpp "beagle/Core/History.hpp"
 *  \brief System component for the storage of history data.
 *
 *  The History component is the principle interface for the user to
 *  interact with the history storage mechanism.  The vast majority of
 *  uses will be of the Beagle::History::trace() method.
 *
 *  \ingroup Core
 *  \ingroup Sys
 *  \ingroup History
 */
class History : public Component
{

public:

	//! History allocator type.
	typedef AllocatorT<History,Component::Alloc> Alloc;
	//! History handle type.
	typedef PointerT<History,Component::Handle> Handle;
	//! History bag type.
	typedef ContainerT<History,Component::Bag> Bag;

	explicit History();
	virtual ~History()
	{ }

	void                      allocateID(Beagle::Individual& ioIndividual);
	void                      allocateNewID(Beagle::Individual& ioIndividual);
	void                      flush();
	unsigned int              getFirstID() const;
	const HistoryTrace::Bag&  getTraces() const;
	void                      incrementHistoryVar(Beagle::Individual& ioIndividual);
	virtual void              init(System& ioSystem);
	virtual void              registerParams(System& ioSystem);
	virtual void              readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	void                      trace(Context& ioContext,
	                                const std::vector<HistoryID>& inParents,
	                                const Individual::Handle inIndividual,
	                                const std::string& inOpName,
	                                const std::string& inAction);
	virtual void              writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	HistoryTrace::Bag  mTraces;            //!< Record of all history traces.
	Bool::Handle       mRecordActivated;   //!< Whether record is activated or not.
	unsigned int       mTracesFirst;       //!< The first history trace id in mTraces.
	unsigned int       mTracesCount;       //!< Total number of traces in the history.

};

}


/*!
 *  \brief Assign a new Id to the individual if it has a none.
 */
inline void Beagle::History::allocateID(Beagle::Individual& ioIndividual)
{
	Beagle_StackTraceBeginM();
	if(ioIndividual.getMember("HistoryID") == NULL) {
		ioIndividual.addMember(new HistoryID(mTracesCount++, 0));
	}
	Beagle_StackTraceEndM("void History::allocateID(Beagle::Individual& ioIndividual)");
}


/*!
 *  \brief Assign a new Id to the individual.
 */
inline void Beagle::History::allocateNewID(Beagle::Individual& ioIndividual)
{
	Beagle_StackTraceBeginM();
	HistoryID::Handle lHID = castHandleT<HistoryID>(ioIndividual.getMember("HistoryID"));
	if(lHID == NULL) ioIndividual.addMember(new HistoryID(mTracesCount++, 0));
	else lHID->setID(mTracesCount++);
	Beagle_StackTraceEndM("void History::allocateNewID(Beagle::Individual& ioIndividual)");
}


/*!
 *  \brief Increment the history variation number of the individual.
 */
inline void Beagle::History::incrementHistoryVar(Beagle::Individual& ioIndividual)
{
	Beagle_StackTraceBeginM();
	HistoryID::Handle lHID = castHandleT<HistoryID>(ioIndividual.getMember("HistoryID"));
	if(lHID == NULL) ioIndividual.addMember(new HistoryID(mTracesCount++, 0));
	else lHID->incrementVar();
	Beagle_StackTraceEndM("void History::incrementHistoryVar(Beagle::Individual& ioIndividual)");
}


/*!
 *  \return The history traces.
 */
inline const Beagle::HistoryTrace::Bag& Beagle::History::getTraces() const
{
	Beagle_StackTraceBeginM();
	return mTraces;
	Beagle_StackTraceEndM("const HistoryTrace::Bag& History::getTraces() const");
}


/*!
 *  \return The first history trace ID among currently stored traces.
 */
inline unsigned int Beagle::History::getFirstID() const
{
	Beagle_StackTraceBeginM();
	return mTracesFirst;
	Beagle_StackTraceEndM("unsigned int Beagle::History::getFirstID() const");
}


#endif // Beagle_Core_History_hpp
