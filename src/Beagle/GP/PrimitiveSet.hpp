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
 *  \file   beagle/GP/PrimitiveSet.hpp
 *  \brief  Definition of the type GP::PrimitiveSet.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_PrimitiveSet_hpp
#define Beagle_GP_PrimitiveSet_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/System.hpp"
#include "beagle/RouletteT.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveMap.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

/*!
 *  \class PrimitiveSet beagle/GP/PrimitiveSet.hpp "beagle/GP/PrimitiveSet.hpp"
 *  \brief The GP primitive set class.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup GPSys
 */
class PrimitiveSet : public Primitive::Bag
{

public:

	//! GP::PrimitiveSet allocator type.
	typedef AllocatorT<PrimitiveSet,Primitive::Bag::Alloc>
	Alloc;
	//! GP::PrimitiveSet handle type.
	typedef PointerT<PrimitiveSet,Primitive::Bag::Handle>
	Handle;
	//! GP::PrimitiveSet mixed bag type.
	typedef ContainerT<PrimitiveSet,Primitive::Bag::Bag>
	Bag;

#ifdef BEAGLE_HAVE_RTTI
	explicit PrimitiveSet(const std::type_info* inRootType=NULL);
#else  // BEAGLE_HAVE_RTTI
	PrimitiveSet()
	{ }
#endif // BEAGLE_HAVE_RTTI

	virtual ~PrimitiveSet()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getRootType() const;
#endif // BEAGLE_HAVE_RTTI

	virtual void              registerParams(System& ioSystem);
	virtual void              insert(Primitive::Handle inPrimitive, double inBias=1.0);
	virtual Primitive::Handle select(unsigned int inNumberArguments, GP::Context& ioContext);

#ifdef BEAGLE_HAVE_RTTI
	virtual Primitive::Handle selectWithType(unsigned int inNumberArguments,
	        const std::type_info* inDesiredType,
	        GP::Context& ioContext);
#endif // BEAGLE_HAVE_RTTI

	virtual void              readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext);
	virtual void              write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Get the primitive of the name given.
	 *  \param inName Name of the primitive to get.
	 *  \return Handle to the named primitive, a NULL handle if there is no primitive of the given name.
	 */
	inline GP::Primitive::Handle getPrimitiveByName(std::string inName) const
	{
		Beagle_StackTraceBeginM();
		GP::PrimitiveMap::const_iterator lMapIter = mNames.find(inName);
		if(lMapIter == mNames.end()) return GP::Primitive::Handle(NULL);
		GP::Primitive::Handle lPrimitive = castHandleT<GP::Primitive>(lMapIter->second);
		return lPrimitive;
		Beagle_StackTraceEndM();
	}

#ifdef BEAGLE_HAVE_RTTI
	/*!
	 *  \brief Set the associated tree's root type.
	 *  \param inRootType Root type.
	 */
	inline void setRootType(const std::type_info* inRootType)
	{
		Beagle_StackTraceBeginM();
		mRootType = inRootType;
		Beagle_StackTraceEndM();
	}
#endif // BEAGLE_HAVE_RTTI

protected:

	typedef std::pair<bool,RouletteT<unsigned int> > RoulettePair;
	typedef std::map< unsigned int,RoulettePair,std::less<unsigned int> >
	RouletteMap;

	std::vector<double> mSelectionBiases;  //!< Selection biases.
	GP::PrimitiveMap    mNames;            //!< Look-up table (by name).
	RouletteMap         mRoulettes;        //!< Nb. args -> roulette map.

#ifdef BEAGLE_HAVE_RTTI
	const std::type_info* mRootType;       //!< Associated root type. Used only with STGP.
#endif // BEAGLE_HAVE_RTTI

};

}
}

#endif // Beagle_GP_PrimitiveSet_hpp

