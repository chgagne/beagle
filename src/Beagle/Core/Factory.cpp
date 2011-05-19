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
 *  \file   Beagle/Core/Factory.cpp
 *  \brief  Source code of class Factory.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/Core.hpp"

#include <stdexcept>

using namespace Beagle;


/*!
 *  \brief Construct object factory component.
 */
Factory::Factory() :
	Component("Factory")
{ }


/*!
 *  \brief Alias a type name to an already existing one in object factory.
 *  \param inTypeName Original name of type.
 *  \param inAlias Alias name of that type.
 *  \throw Beagle::RunTimeException If the original type name is not in the factory
 *     or if the alias is already in the factory.
 */
void Factory::aliasAllocator(const std::string& inTypeName, const std::string& inAlias)
{
	Beagle_StackTraceBeginM();
	Factory::AllocatorMap::const_iterator lIterType = mAllocatorMap.find(inTypeName);
	if(lIterType == mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The type name '" << inTypeName;
		lOSS << "' is not present in the factory's allocator map; ";
		lOSS << "could not alias it";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	Factory::AllocatorMap::const_iterator lIterAlias = mAllocatorMap.find(inAlias);
	if(lIterAlias != mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The type name '" << inAlias;
		lOSS << "' is already present in the factory's allocator map; ";
		lOSS << "could not make an alias using its name";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	mAllocatorMap[inAlias] = lIterType->second;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert new allocator in object factory.
 *  \param inTypeName Name of type inserted.
 *  \param inAllocator Allocator associated to type.
 *  \throw Beagle::RunTimeException If the type name is already in the factory.
 */
void Factory::insertAllocator(const std::string& inTypeName, Allocator::Handle inAllocator)
{
	Beagle_StackTraceBeginM();
	Factory::AllocatorMap::const_iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
	if(lIterAllocMap != mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The type name '" << inTypeName;
		lOSS << "' is already present in the factory's allocator map; ";
		lOSS << "could not add it.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	mAllocatorMap[inTypeName] = inAllocator;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a factory, obtain the different concept-type associations.
 *  \param inIter Iterator to the XML node to read the Factory from.
 *  \throw Beagle::IOException If the format is invalid.
 */
void Factory::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Factory")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Factory> expected!");
	}
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Concept")) {
			std::string lConceptName = lChild->getAttribute("name");
			if(lConceptName.empty()) {
				throw Beagle_IOExceptionNodeM(*lChild, "no name attribute given for current Concept tag!");
			}
			std::string lTypeName = lChild->getAttribute("type");
			if(lTypeName.empty()) {
				throw Beagle_IOExceptionNodeM(*lChild, "no type attribute given for current Concept tag!");
			}
			Factory::AllocatorMap::const_iterator lIterAllocMap = mAllocatorMap.find(lTypeName);
			if(lIterAllocMap == mAllocatorMap.end()) {
				std::ostringstream lOSS;
				lOSS << "Type named '" << lTypeName;
				lOSS << "' does not exists in the internal allocator map of the Factory; ";
				lOSS << "could not associated to concept '" << lConceptName << "'.";
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			setConcept(lConceptName, lTypeName);
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a Factory, obtain selection of default types for the different families.
 *  \param inIter Iterator to the XML node to read the Factory from.
 *  \param ioSystem System to use to read the type selector.
 *  \throw Beagle::IOException If the format is invalid.
 */
void Factory::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Factory")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Factory> expected!");
	}
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Concept")) {
			std::string lConceptName = lChild->getAttribute("name");
			if(lConceptName.empty()) {
				throw Beagle_IOExceptionNodeM(*lChild, "no name attribute given for current Concept tag!");
			}
			std::string lTypeName = lChild->getAttribute("type");
			if(lTypeName.empty()) {
				throw Beagle_IOExceptionNodeM(*lChild, "no type attribute given for current Concept tag!");
			}
			Factory::AllocatorMap::const_iterator lIterAllocMap = mAllocatorMap.find(lTypeName);
			if(lIterAllocMap == mAllocatorMap.end()) {
				std::ostringstream lOSS;
				lOSS << "Type named '" << lTypeName;
				lOSS << "' does not exists in the internal allocator map of the Factory; ";
				lOSS << "could not associated to concept '" << lConceptName << "'.";
				throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
			}
			setConcept(lConceptName, lTypeName);
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Type associated to concept '")+lConceptName+
			    std::string("' is now '")+lTypeName+std::string("'")
			);
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Remove allocator from the factory.
 *  \param inTypeName Type name of the allocator to remove.
 *  \return Allocator associated to the removed type name, NULL handle if unknown type.
 */
Allocator::Handle Factory::removeAllocator(const std::string& inTypeName)
{
	Beagle_StackTraceBeginM();
	Factory::AllocatorMap::iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
	if(lIterAllocMap == mAllocatorMap.end()) return NULL;
	Allocator::Handle lAlloc = lIterAllocMap->second;
	mAllocatorMap.erase(lIterAllocMap);
	return lAlloc;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Remove concept from the factory.
 *  \param inConcept Name of concept to remove.
 *  \return Name of type associated to removed concept, empty string if concept was absent.
 */
std::string Factory::removeConcept(const std::string& inConcept)
{
	Beagle_StackTraceBeginM();
	Factory::ConceptMap::iterator lIterConceptMap = mConceptMap.find(inConcept);
	if(lIterConceptMap == mConceptMap.end()) return std::string();
	std::string lTypeName = lIterConceptMap->second;
	mConceptMap.erase(lIterConceptMap);
	return lTypeName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Associate a type name to a concept.
 *  \param inConcept Concept name that get a type associated.
 *  \param inTypeName Name of type associated to the concept.
 *  \throw Beagle::RunTimeException If given type name is not present in factory's allocator map.
 */
void Factory::setConcept(const std::string& inConcept, const std::string& inTypeName)
{
	Beagle_StackTraceBeginM();
	Factory::AllocatorMap::iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
	if(lIterAllocMap == mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "Type name '" << inTypeName << "' is not present in factory's allocator map; ";
		lOSS << "it thus can't be use as type name associated to concept '" << inConcept;
		lOSS << "'.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	mConceptMap[inConcept] = inTypeName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write concept map of the factory into a XML streamer.
 *  \param ioStreamer XML streamer to write factory concept map into.
 *  \param inIndent Whether XML output should be indented.
 */
void Factory::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Factory", inIndent);
	std::map< std::string, std::string > lSortedMap;
	for(Factory::ConceptMap::const_iterator lIter=mConceptMap.begin();
	        lIter!=mConceptMap.end(); ++lIter) {
		lSortedMap[lIter->first] = lIter->second;
	}
	for(std::map< std::string, std::string >::const_iterator lIterSM=lSortedMap.begin();
	        lIterSM!=lSortedMap.end(); ++lIterSM) {
		ioStreamer.openTag("Concept", false);
		ioStreamer.insertAttribute("name", lIterSM->first);
		ioStreamer.insertAttribute("type", lIterSM->second);
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
