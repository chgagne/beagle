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
 *  \file   Beagle/Core/Factory.hpp
 *  \brief  Definition of the class Factory.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/09/21 17:45:46 $
 */

#ifndef Beagle_Core_Factory_hpp
#define Beagle_Core_Factory_hpp

#include <map>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/System.hpp"
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
 *  \class Factory Beagle/Factory.hpp "Beagle/Factory.hpp"
 *  \brief Object factory allowing dynamic types allocation.
 *  \ingroup Core
 *  \ingroup Sys
 *
 *  A factory is used to generate and copy data types dynamically. It contains a database of
 *  object allocators associated to a name. String-based lookup are thus possible to obtain
 *  the allocators, to allocate, clone or copy given dynamically-binded data type. For example,
 *  asking for the type given in a string "Beagle::FitnessSimple" would provide a instanciation
 *  of the allocator for that type exact type, Beagle::FitnessSimple.
 *
 *  The factory also contains a database of types to use associated to a given concept. This
 *  concept-typename structure can be modified in the code or from the configuration file.
 *  It is designed to select concrete type allocators for instanciation of genotypes, individuals,
 *  fitnesses, demes, vivariums and other similar BEAGLE objects. Combined with the package concept
 *  (see class Beagle::Package), it can simplifies greatly the configuration of a BEAGLE
 *  application, and allows a complete dynamic configuration from a XML configuration file (or such).
 *  For example, says that it is specified in the code or the configuration file that the type
 *  Beagle::FitnessMultiObj is associated to the concept "Fitness", then all the fitness objects
 *  obtained through the factory will be of the type Beagle::FitnessMultiObj.
 *
 */
class Factory : public Component
{

public:

	//! Factory allocator type.
	typedef AllocatorT< Factory, Component::Alloc >
	Alloc;
	//! Factory handle type.
	typedef PointerT< Factory, Component::Handle >
	Handle;
	//! Factory bag type.
	typedef ContainerT< Factory, Component::Bag >
	Bag;

	Factory();
	virtual ~Factory()
	{ }

	void              aliasAllocator(const std::string& inTypeName, const std::string& inAlias);
	void              insertAllocator(const std::string& inTypeName, Allocator::Handle inAllocator);
	virtual void      read(PACC::XML::ConstIterator inIter);
	virtual void      readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	Allocator::Handle removeAllocator(const std::string& inTypeName);
	std::string       removeConcept(const std::string& inConcept);
	void              setConcept(const std::string& inConcept, const std::string& inTypeName);
	virtual void      write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Obtain allocator associated to the given type.
	 *  \param inTypeName Name of the type to obtain the allocator.
	 *  \return Allocator handle to the type allocator, NULL pointer if the type is unknown.
	 */
	inline Allocator::Handle getAllocator(const std::string& inTypeName) const {
		Beagle_StackTraceBeginM();
		Factory::AllocatorMap::const_iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
		if(lIterAllocMap == mAllocatorMap.end()) return NULL;
		return lIterAllocMap->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Obtain allocator of the type associated to the given concept.
	 *  \param inConcept Concept to obtain the associated type allocator from.
	 *  \return Allocator of associated type, NULL pointer if the concept is unknown.
	 *  \throw Beagle::RunTimeException If the associated type is not found in the allocator map.
	 */
	inline Allocator::Handle getConceptAllocator(const std::string& inConcept) const {
		Beagle_StackTraceBeginM();
		Factory::ConceptMap::const_iterator lIterConceptMap = mConceptMap.find(inConcept);
		if(lIterConceptMap == mConceptMap.end()) return NULL;
		Factory::AllocatorMap::const_iterator lIterAllocMap =
		    mAllocatorMap.find(lIterConceptMap->second);
		if(lIterAllocMap == mAllocatorMap.end()) {
			std::ostringstream lOSS;
			lOSS << "Type named '" << lIterConceptMap->second << "', associated to concept '";
			lOSS << inConcept << "', does not exists in the factory's allocator map; ";
			lOSS << "could not retrieve allocator of the type.";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		return lIterAllocMap->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Obtain type name associated to the given concept.
	 *  \param inConcept Concept to obtain the type name from.
	 *  \return Associated type name, empty string if the concept is unknown.
	 */
	inline std::string getConceptTypeName(const std::string& inConcept) const {
		Beagle_StackTraceBeginM();
		Factory::ConceptMap::const_iterator lIterConceptMap = mConceptMap.find(inConcept);
		if(lIterConceptMap == mConceptMap.end()) return std::string();
		return lIterConceptMap->second;
		Beagle_StackTraceEndM();
	}


protected:

#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP)
	typedef std::tr1::unordered_map<std::string,Allocator::Handle,HashString> AllocatorMap;
	typedef std::tr1::unordered_map<std::string,std::string,HashString> ConceptMap;
#elif defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	typedef std::unordered_map<std::string,Allocator::Handle,HashString> AllocatorMap;
	typedef std::unordered_map<std::string,std::string,HashString> ConceptMap;
#else // No hash_map found
	typedef std::map<std::string,Allocator::Handle> AllocatorMap;
	typedef std::map<std::string,std::string> ConceptMap;
#endif
	AllocatorMap mAllocatorMap;  //!< Map of stored allocators.
	ConceptMap   mConceptMap;    //!< Type-concept associations.

};

}

#endif // Beagle_Core_Factory_hpp
