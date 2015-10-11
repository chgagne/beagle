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
 *  \file   beagle/GP/PrimitiveSuperSet.hpp
 *  \brief  Definition of the type GP::PrimitiveSuperSet.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 18:09:11 $
 */


#ifndef Beagle_GP_PrimitiveSuperSet_hpp
#define Beagle_GP_PrimitiveSuperSet_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/GP/PrimitiveSet.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class PrimitiveSuperSet beagle/GP/PrimitiveSuperSet.hpp "beagle/GP/PrimitiveSuperSet.hpp"
 *  \brief The GP primitive super set class.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup GPSys
 */
class PrimitiveSuperSet : public Component
{

public:

	//! GP::PrimitiveSuperSet allocator type.
	typedef AllocatorT<PrimitiveSuperSet,Component::Alloc> Alloc;
	//! GP::PrimitiveSuperSet handle type.
	typedef PointerT<PrimitiveSuperSet,Component::Handle> Handle;
	//! GP::PrimitiveSuperSet bag type.
	typedef ContainerT<PrimitiveSuperSet,Component::Bag> Bag;

	explicit PrimitiveSuperSet();

	/*!
	 *  \brief Return primitive set at index inIndex.
	 *  \param inIndex Index of primitive set to get access to.
	 *  \return Reference to primitive set accessed.
	 */
	inline const PrimitiveSet::Handle& operator[](unsigned int inIndex) const
	{
		Beagle_StackTraceBeginM();
		return mPrimitSets[inIndex];
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return primitive set at index inIndex.
	 *  \param inIndex Index of primitive set to get access to.
	 *  \return Reference to primitive set accessed.
	 */
	inline PrimitiveSet::Handle& operator[](unsigned int inIndex)
	{
		Beagle_StackTraceBeginM();
		return mPrimitSets[inIndex];
		Beagle_StackTraceEndM();
	}

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void insert(PrimitiveSet::Handle inPrimitiveSet, bool inReplace=false);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Add new primitive to the super set.
	 *  \param inPrimitive Primitive to add to the super set.
	 *  \param inReplace Set to true if you wish this primitive to replace a primitive with the same name.
	 *
	 *  This primitive is **NOT** necessary used in the primitive sets, it is only available here to
	 *  dynamically configure primitive set, for example when reading a the primitive sets from a
	 *  configuration file. If a primitive of the same name is already in the super set, the added
	 *  primitive will not be inserted in the super set primitives map unless inReplace is set.
	 */
	inline void addPrimitive(GP::Primitive::Handle inPrimitive, bool inReplace=false)
	{
		Beagle_StackTraceBeginM();
		if (inReplace) {
			mPrimitMap[inPrimitive->getName()] = inPrimitive;
		} else {
			GP::PrimitiveMap::const_iterator lMapIter = mPrimitMap.find(inPrimitive->getName());
			if(lMapIter == mPrimitMap.end()) mPrimitMap[inPrimitive->getName()] = inPrimitive;
		}
		Beagle_StackTraceEndM();
	}


	/*!
	 *  \brief Get the primitive of the name given.
	 *  \param inName Name of the primitive to get.
	 *  \return Handle to the named primitive, a NULL handle if there is none with the given name.
	 */
	inline GP::Primitive::Handle getPrimitiveByName(std::string inName) const
	{
		Beagle_StackTraceBeginM();
		GP::PrimitiveMap::const_iterator lMapIter = mPrimitMap.find(inName);
		if(lMapIter == mPrimitMap.end()) return GP::Primitive::Handle(NULL);
		GP::Primitive::Handle lPrimitive = castHandleT<GP::Primitive>(lMapIter->second);
		return lPrimitive;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Reference to super set's primitive map.
	 */
	inline const GP::PrimitiveMap& getPrimitiveMap() const
	{
		Beagle_StackTraceBeginM();
		return mPrimitMap;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Reference to super set's primitive map.
	 */
	inline GP::PrimitiveMap& getPrimitiveMap()
	{
		Beagle_StackTraceBeginM();
		return mPrimitMap;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Return reference to primitive set bag of the super set.
	 */
	inline const PrimitiveSet::Bag& getPrimitSetBag() const
	{
		Beagle_StackTraceBeginM();
		return mPrimitSets;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Return reference to primitive set bag of the super set.
	 */
	inline PrimitiveSet::Bag& getPrimitSetBag()
	{
		Beagle_StackTraceBeginM();
		return mPrimitSets;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return number of primitive sets of the super set.
	 */
	inline unsigned int size() const
	{
		Beagle_StackTraceBeginM();
		return mPrimitSets.size();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return true if the are no primitive sets in the super set.
	 */
	inline bool empty() const
	{
		Beagle_StackTraceBeginM();
		return mPrimitSets.empty();
		Beagle_StackTraceEndM();
	}

protected:

	void addBasicPrimitives();

	GP::PrimitiveSet::Bag mPrimitSets;  //!< Bag of primitive sets.
	GP::PrimitiveMap      mPrimitMap;   //!< Map of available primitives.

};

}
}

#endif // Beagle_GP_PrimitiveSuperSet_hpp
