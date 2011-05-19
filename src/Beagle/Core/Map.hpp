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
 *  \file   Beagle/Core/Map.hpp
 *  \brief  Definition of class Map.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/09/21 17:45:46 $
 */

#ifndef Beagle_Core_Map_hpp
#define Beagle_Core_Map_hpp

#include <string>
#include <map>

#include "PACC/XML.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/HashString.hpp"

#ifdef BEAGLE_HAVE_STDTR1_UNORDEREDMAP
#include <tr1/unordered_map>
#else
#ifdef BEAGLE_HAVE_STD_UNORDEREDMAP
#include <unordered_map>
#endif // BEAGLE_HAVE_STD_UNORDEREDMAP
#endif //BEAGLE_HAVE_STDTR1_UNORDEREDMAP

namespace Beagle
{

/*!
 *  \class Map Beagle/Core/Map.hpp "Beagle/Core/Map.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    Object::Handle instances.
 *  \ingroup Core
 *  \ingroup DataStruct
 */

#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP)
class Map : public Object, public std::tr1::unordered_map<std::string,Object::Handle,HashString>
#elif defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
class Map : public Object, public std::unordered_map<std::string,Object::Handle,HashString>
#else // No hash_map found
class Map : public Object, public std::map< std::string,Object::Handle,std::less<std::string> >
#endif
{

public:

	//! Map allocator type.
	typedef AllocatorT<Map,Object::Alloc> Alloc;
	//! Map handle type.
	typedef PointerT<Map,Object::Handle> Handle;
	//! Map bag type.
	typedef ContainerT<Map,Object::Bag> Bag;

	Map()
	{ }
	virtual ~Map()
	{ }

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_Map_hpp
