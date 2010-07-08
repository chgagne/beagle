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
*  \file   beagle/Core/HistoryEntry.hpp
*  \brief  Definition of the class HistoryEntry.
*  \author Alexandre Devert
*  $Revision: 1.18 $
*  $Date: 2007/08/17 20:58:00 $
*/

#ifndef Beagle_Core_HistoryEntry_hpp
#define Beagle_Core_HistoryEntry_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Component.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/HistoryID.hpp"


namespace Beagle
{

/*!
 *  \class HistoryEntry beagle/Core/HistoryEntry.hpp "beagle/Core/HistoryEntry.hpp"
 *  \brief Evolution history storage class.
 *
 *  This class is used internally by the History system component.
 *  Its purpose is to store entries in history traces.
 *
 *  \ingroup Core
 *  \ingroup Sys
 *  \ingroup History
 */
class HistoryEntry : public Object
{

public:

	//! HistoryEntry allocator type.
	typedef AllocatorT<HistoryEntry,Object::Alloc> Alloc;
	//! HistoryEntry handle type.
	typedef PointerT<HistoryEntry,Object::Handle> Handle;
	//! HistoryEntry bag type.
	typedef ContainerT<HistoryEntry,Object::Bag> Bag;

	explicit HistoryEntry();

	HistoryEntry(const std::vector<HistoryID>& inParents,
	             const std::string& inOpName,
	             const std::string& inAction);
	virtual ~HistoryEntry()
	{ }

	void                             addAction(const std::string& inAction);
	void                             addOpName(const std::string& inOpName);
	const std::vector<HistoryID>&    getParents() const;
	const std::vector<std::string>&  getActions() const;
	const std::vector<std::string>&  getOpNames() const;
	unsigned int                     getGeneration() const;
	unsigned int                     getIndividualVar() const;
	Individual::Handle               getIndividual();
	virtual void                     read(PACC::XML::ConstIterator inIter);
	void                             setGeneration(unsigned int inGeneration);
	void                             setIndividualVar(unsigned int inIndividualVar);
	void                             setIndividual(Individual::Handle inIndividual);
	virtual void                     write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	std::vector<HistoryID>    mParents;        //!< Parents of the individual.
	std::vector<std::string>  mActions;        //!< Actions associated with the history entry.
	std::vector<std::string>  mOpNames;        //!< Operator names associated with the history entry.
	unsigned int              mGeneration;     //!< Generation at which the action occurred.
	unsigned int              mIndividualVar;  //!< History variation of the individual.
	Individual::Handle        mIndividual;     //!< Handle to the individual.

};

}


/*!
 *  \brief Add the action to the set of actions in the entry.
 */
inline void Beagle::HistoryEntry::addAction(const std::string& inAction)
{
	Beagle_StackTraceBeginM();
	mActions.push_back(inAction);
	Beagle_StackTraceEndM("void HistoryEntry::addAction(const std::string& inAction)");
}


/*!
 *  \brief Add the opname to the set of opnames in the entry.
 */
inline void Beagle::HistoryEntry::addOpName(const std::string& inOpName)
{
	Beagle_StackTraceBeginM();
	mOpNames.push_back(inOpName);
	Beagle_StackTraceEndM("void HistoryEntry::addOpName(const std::string& inOpName)");
}


/*!
 *  \return The actions associated with this history entry.
 */
inline const std::vector<std::string>& Beagle::HistoryEntry::getActions() const
{
	Beagle_StackTraceBeginM();
	return mActions;
	Beagle_StackTraceEndM("const std::vector<std::string>& HistoryEntry::getActions() const");
}


/*!
 *  \return The entry's generation.
 */
inline unsigned int Beagle::HistoryEntry::getGeneration() const
{
	Beagle_StackTraceBeginM();
	return mGeneration;
	Beagle_StackTraceEndM("unsigned int HistoryEntry::getGeneration() const");
}


/*!
 *  \return The handle to the individual in the entry.
 */
inline Beagle::Individual::Handle Beagle::HistoryEntry::getIndividual()
{
	Beagle_StackTraceBeginM();
	return mIndividual;
	Beagle_StackTraceEndM("Individual::Handle HistoryEntry::getIndividual()");
}


/*!
 *  \return The history variation of the individual.
 */
inline unsigned int Beagle::HistoryEntry::getIndividualVar() const
{
	Beagle_StackTraceBeginM();
	return mIndividualVar;
	Beagle_StackTraceEndM("unsigned int HistoryEntry::getIndividualVar() const");
}


/*!
 *  \return The operator names associated with this history entry.
 */
inline const std::vector<std::string>& Beagle::HistoryEntry::getOpNames() const
{
	Beagle_StackTraceBeginM();
	return mOpNames;
	Beagle_StackTraceEndM("const std::vector<std::string>& HistoryEntry::getOpNames() const");
}


/*!
 *  \return The names (history id and variation) the parents of the individual.
 */
inline const std::vector<Beagle::HistoryID>& Beagle::HistoryEntry::getParents() const
{
	Beagle_StackTraceBeginM();
	return mParents;
	Beagle_StackTraceEndM("const std::vector<Beagle::HistoryID>& HistoryEntry::getParents() const");
}


/*!
 *  \brief Set the generation for this entry.
 */
inline void Beagle::HistoryEntry::setGeneration(unsigned int inGeneration)
{
	Beagle_StackTraceBeginM();
	mGeneration = inGeneration;
	Beagle_StackTraceEndM("void HistoryEntry::setGeneration(unsigned int inGeneration)");
}


/*!
 *  \brief Set the handle to the individual in the entry.
 */
inline void Beagle::HistoryEntry::setIndividual(Individual::Handle inIndividual)
{
	Beagle_StackTraceBeginM();
	mIndividual = inIndividual;
	Beagle_StackTraceEndM("void HistoryEntry::setIndividual(Individual::Handle inIndividual)");
}


/*!
 *  \brief Set the history variation of the individual.
 */
inline void Beagle::HistoryEntry::setIndividualVar(unsigned int inIndividualVar)
{
	Beagle_StackTraceBeginM();
	mIndividualVar = inIndividualVar;
	Beagle_StackTraceEndM("void HistoryEntry::setIndividualVar(unsigned int inIndividualVar)");
}


#endif // Beagle_Core_HistoryEntry_hpp
