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
 *  \file   beagle/GP/ModuleVectorComponent.hpp
 *  \brief  Definition of the class ModuleVectorComponent.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef ModuleVectorComponent_hpp
#define ModuleVectorComponent_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Component.hpp"
#include "beagle/RouletteT.hpp"
#include "beagle/GP/Tree.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \brief Vector of modules for evolutionary module acquisition.
 *  \ingroup GPF
 *  \ingroup EMA
 */
class ModuleVectorComponent : public Component
{

public:

	//! ModuleVectorComponent allocator type.
	typedef AllocatorT<ModuleVectorComponent,Component::Alloc>
	Alloc;
	//! ModuleVectorComponent handle type.
	typedef PointerT<ModuleVectorComponent,Component::Handle>
	Handle;
	//! ModuleVectorComponent bag type.
	typedef ContainerT<ModuleVectorComponent,Component::Bag>
	Bag;

	explicit ModuleVectorComponent();

	/*!
	 *  \brief Get the nth module.
	 *  \param inN Index of the module to get.
	 *  \return Reference to the tree handle, that is the nth module.
	 */
	inline const GP::Tree::Handle& operator[](unsigned int inN) const
	{
		Beagle_StackTraceBeginM();
		return mModules[inN];
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get the nth module.
	 *  \param inN Index of the module to get.
	 *  \return Reference to the tree handle, that is the nth module.
	 */
	inline GP::Tree::Handle& operator[](unsigned int inN)
	{
		Beagle_StackTraceBeginM();
		return mModules[inN];
		Beagle_StackTraceEndM();
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Add a new GP tree to the vector component.
	 *  \param inTree GP tree added.
	 */
	inline void add(GP::Tree::Handle inTree)
	{
		Beagle_StackTraceBeginM();
		mModules.push_back(inTree);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return True if there is no module acquired, false if not.
	 */
	inline bool empty() const
	{
		Beagle_StackTraceBeginM();
		return mModules.empty();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of module in the module vector.
	 */
	inline unsigned int size() const
	{
		Beagle_StackTraceBeginM();
		return mModules.size();
		Beagle_StackTraceEndM();
	}

protected:

	GP::Tree::Bag mModules;   //!< Vector of the modules acquired.

};

}
}

#endif // ModuleVectorComponent_hpp
