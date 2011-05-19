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
 *  \file   Beagle/Core/Register.cpp
 *  \brief  Source code of class Register.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.40 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "Beagle/Core.hpp"

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Default constructor of Register::Description.
 */
Register::Description::Description(void ) :
	mBrief("[No brief]"),
	mType(""),
	mDefaultValue(""),
	mDescription("[No description]")
{ }


/*!
 *  \brief Construct a Register::Description instance.
 *  \param inBrief Brief desccription of the parameter.
 *  \param inType Type of the parameter contained.
 *  \param inDefaultValue Default value of the parameter.
 *  \param inDescription Long description of the parameter.
 */
Register::Description::Description(const std::string& inBrief,
                                   const std::string& inType,
                                   const std::string& inDefaultValue,
                                   const std::string& inDescription) :
	mBrief(inBrief),
	mType(inType),
	mDefaultValue(inDefaultValue),
	mDescription(inDescription)
{ }


/*!
 *  \brief Construct register class instance.
 */
Register::Register() : Component("Register")
{ }


/*!
 *  \brief Add a new description in the register.
 *  \param inTag Tag of the entry.
 *  \param inDescription Description of the entry.
 *  \throw RunTimeException If the entry is already described.
 */
void Register::addDescription(const std::string& inTag, const Description& inDescription)
{
	Beagle_StackTraceBeginM();

	DescriptionMap::const_iterator lIterDescrip = mDescriptions.find(inTag);
	if(lIterDescrip != mDescriptions.end()) {
		std::string lMessage = "The entry '";
		lMessage += inTag;
		lMessage += "' is already described!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	mDescriptions[inTag] = inDescription;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Add a new entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inEntry Value of the entry.
 *  \param inDescription Description of the entry.
 *  \throw RunTimeException If the entry is already in the register.
 *  \deprecated Replaced by method Register::insertEntry.
 */
void Register::addEntry(const std::string& inTag, Object::Handle inEntry, const Register::Description& inDescription)
{
	Beagle_StackTraceBeginM();

	std::cerr << "WARNING: when adding entry for parameter '" << inTag;
	std::cerr << "' in Open BEAGLE register: method 'Register::addEntry' is deprecated,";
	std::cerr << "use method 'Register::insertEntry' instead!" << std::endl;
	Map::const_iterator lIterObj = mParameters.find(inTag);
	if(lIterObj != mParameters.end()) {
		std::string lMessage = "The entry '";
		lMessage += inTag;
		lMessage += "' is already in the register!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	mParameters[inTag] = inEntry;
	addDescription(inTag,inDescription);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Delete an entry of the register.
 *  \param inTag Tag of the entry.
 *  \return Smart pointer to the deleted object.
 *  \throw RunTimeException If the entry is not in the register.
 */
Object::Handle Register::deleteEntry(const std::string& inTag)
{
	Beagle_StackTraceBeginM();

	Map::iterator lIterObj = mParameters.find(inTag);
	if(lIterObj == mParameters.end()) {
		std::string lMessage = "The entry '";
		lMessage += inTag;
		lMessage += "' is not registered!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	Object::Handle lObjHandle = lIterObj->second;
	mParameters.erase(lIterObj);
	DescriptionMap::iterator lIterDescrip = mDescriptions.find(inTag);
	if(lIterDescrip == mDescriptions.end()) {
		std::string lMessage = "No description for entry '";
		lMessage += inTag;
		lMessage += "'!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	mDescriptions.erase(lIterDescrip);
	return lObjHandle;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the description of the given entry.
 *  \param inTag Tag name of the entry.
 *  \throw RunTimeException If the tag is not registered.
 */
const Register::Description& Register::getDescription(const std::string& inTag) const
{
	Beagle_StackTraceBeginM();

	DescriptionMap::const_iterator lIterDescrip = mDescriptions.find(inTag);
	if(lIterDescrip == mDescriptions.end()) {
		std::string lMessage = "No description for entry '";
		lMessage += inTag;
		lMessage += "'!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	return lIterDescrip->second;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Erase an argument from the command-line.
 *  \param inN Index of the argument to remove.
 *  \param ioArgc Number of elements on the command-line.
 *  \param ioArgv Elements of the command-line.
 *  \return Removed element of the command-line.
 */
char* Register::eraseArg(int inN, int& ioArgc, char**& ioArgv) const
{
	Beagle_StackTraceBeginM();

	Beagle_BoundCheckAssertM(inN, 1, (ioArgc-1));
	char *lTemp = ioArgv[inN];
	for(int i=inN; i<ioArgc-1; i++) ioArgv[i] = ioArgv[i+1];
	ioArgv[ioArgc-1] = NULL;
	ioArgc--;
	return lTemp;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Insert an entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inEntry Value of the entry.
 *  \param inDescription Description of the entry.
 *  \return Reference to the registered parameter (use this reference instead of the inEntry
 *          parameter).
 *  \throw RunTimeException If the entry is already in the register.
 */
Object::Handle Register::insertEntry(const std::string& inTag, Object::Handle inEntry,
                                     const Register::Description& inDescription)
{
	Beagle_StackTraceBeginM();

	Map::const_iterator lIterObj = mParameters.find(inTag);
	// Parameter not registered yet.
	if(lIterObj == mParameters.end()) {
		mDescriptions[inTag] = inDescription;
		DefaultValuesMap::const_iterator lIterDefVal = mDefaultValues.find(inTag);
		if(lIterDefVal == mDefaultValues.end()) {
			mParameters[inTag] = inEntry;
		} else {
			mParameters[inTag] = lIterDefVal->second.first;
			mDescriptions[inTag].mDefaultValue = lIterDefVal->second.second;
		}
		lIterObj = mParameters.find(inTag);
	}
	return lIterObj->second;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief To check whether a given tag-entry is registered.
 *  \param inTag Tag of the entry.
 *  \return True if an entry with the given tag is registred, false if not.
 */
bool Register::isRegistered(const std::string& inTag) const
{
	Beagle_StackTraceBeginM();

	Map::const_iterator lIterObj = mParameters.find(inTag);
	return (lIterObj != mParameters.end());

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Modify the smart pointer of an entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inNewEntry New entry that will replace to old one.
 *  \return Handle to the old entry.
 *  \throw RunTimeException If the entry is not found in the register.
 */
Object::Handle Register::modifyEntry(const std::string& inTag, Object::Handle inNewEntry)
{
	Beagle_StackTraceBeginM();

	Map::iterator lIterObj = mParameters.find(inTag);
	if(lIterObj == mParameters.end()) {
		std::string lMessage = "The entry '";
		lMessage += inTag;
		lMessage += "' is not registered!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	Object::Handle lObjHandle = lIterObj->second;
	lIterObj->second = inNewEntry;
	return lObjHandle;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Modify the description of an entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inNewDescription New description that will replace to old one.
 *  \return Old description.
 *  \throw RunTimeException If the entry is not found in the register.
 */
Register::Description Register::modifyDescription(const std::string& inTag, const Register::Description& inNewDescription)
{
	Beagle_StackTraceBeginM();
	DescriptionMap::iterator lIterDescrip = mDescriptions.find(inTag);
	if(lIterDescrip == mDescriptions.end()) {
		std::string lMessage = "The entry '";
		lMessage += inTag;
		lMessage += "' is not registered!";
		throw Beagle_RunTimeExceptionM(lMessage);
	}
	Description lDescription = lIterDescrip->second;
	lIterDescrip->second = inNewDescription;
	return lDescription;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read this register from a parse tree node.
 *  \param inNode Input parse tree node.
 *  \throw Beagle::IOException If reading from the stream problems occured.
 */
void Register::read(PACC::XML::ConstIterator inNode)
{
	Beagle_StackTraceBeginM();

	if((inNode->getType() != PACC::XML::eData) || (inNode->getValue() != "Register"))
		throw Beagle_IOExceptionNodeM(*inNode, "invalid node type or tag name!");
	for(PACC::XML::ConstIterator lChild = inNode->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType() == PACC::XML::eData) && (lChild->getValue() == "Entry")) {
			std::string lEntryKey = lChild->getAttribute("key");
			if(lEntryKey.empty())
				throw Beagle_IOExceptionNodeM(*lChild, "missing key attribute for this entry!");
			if(mParameters.find(lEntryKey) != mParameters.end()) {
				mParameters[lEntryKey]->read(lChild->getFirstChild());
			}
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read register from given XML file.
 *  \param inFileName Name of file from which register is read.
 *  \param ioSystem Evolutionary system.
 */
void Register::readFromFile(const std::string& inFileName, System& ioSystem)
{
#ifdef BEAGLE_HAVE_LIBZ
	igzstream lStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	if(lStream.good() == false) {
		throw Beagle_RunTimeExceptionM(std::string("Could not open file '")+inFileName+"'");
	}
	Beagle_LogBasicM(
	    ioSystem.getLogger(),
	    std::string("Reading file '")+inFileName+"' for system configuration"
	);
	PACC::XML::Document lDocument(lStream, inFileName);
	lStream.close();
	PACC::XML::ConstFinder lFinder(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPos = lFinder.find("/Beagle//Register");
	if(!lPos) {
		Beagle_LogBasicM(
		    ioSystem.getLogger(),
		    "WARNING: file does not contain any valid system"
		);
	} else {
		// read all system instances
		readWithSystem(lPos, ioSystem);
		if(lPos = lFinder.findNext()) {
			Beagle_LogBasicM(
			    ioSystem.getLogger(),
			    "WARNING: file contains multiple systems"
			);
			do {
				readWithSystem(lPos, ioSystem);
			} while(lPos = lFinder.findNext());
		}
	}
}


/*!
 *  \brief Read a register from a Beagle input stream.
 *  \param inIter XML iterator to read the register from.
 *  \param ioSystem Reference to evolutionary system.
 *  \throw Beagle::IOException If reading from the stream problems occured.
 */
void Register::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Register"))
		throw Beagle_IOExceptionNodeM(*inIter, "invalid node type or tag name!");
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Entry")) {
			std::string lEntryKey = lChild->getAttribute("key");
			if(lEntryKey.empty())
				throw Beagle_IOExceptionNodeM(*lChild, "no key given for actual entry!");
			if(mParameters.find(lEntryKey) == mParameters.end()) {
				Beagle_LogBasicM(
				    ioSystem.getLogger(),
				    std::string("WARNING: unknown parameter '")+lEntryKey+"'; not registered!"
				);
				continue;
			}
			mParameters[lEntryKey]->read(lChild->getFirstChild());
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Register entry '")+lEntryKey+std::string("' is now ")+
			    mParameters[lEntryKey]->serialize()
			);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the default for an entry in the register.
 *  \param inTag Tag of the entry to which the default value is set.
 *  \param inValue Default value itself.
 *  \param inValueStr String representation of the default value.
 */
void Register::setEntryDefaultValue(const std::string& inTag,
                                    Object::Handle inValue,
                                    const std::string& inValueStr)
{
	Beagle_StackTraceBeginM();

	Map::const_iterator lIterParam = mParameters.find(inTag);
	if(lIterParam != mParameters.end()) {
		// Default value already registered.
		std::ostringstream lOSS;
		lOSS << "Could not set default value for entry '" << inTag;
		lOSS << "' as the entry is already registered; if you want to change the ";
		lOSS << "default value of a given parameter, you must change it before the ";
		lOSS << "parameter is registered.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	mDefaultValues[inTag] = std::make_pair(inValue, inValueStr);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the long program usage.
 *  \param outStream Output stream.
 */
void Register::showHelp(std::ostream& outStream) const
{
	Beagle_StackTraceBeginM();

	for(DescriptionMap::const_iterator lIter = mDescriptions.begin(); lIter != mDescriptions.end(); ++lIter) {
		outStream << "  " << lIter->first;
		if(lIter->second.mType.empty() == false) {
			unsigned int lCountSize = lIter->first.size()+2;
			Beagle_AssertM(lCountSize <= 46);
			std::string lTmpStr1(46-lCountSize, ' ');
			outStream << std::string(46-lCountSize, ' ');
			outStream << "<" << lIter->second.mType << "> (def: ";
			outStream << lIter->second.mDefaultValue << ")";
		}
		outStream << std::endl;
		std::string lDescription = lIter->second.mDescription;
		wrapString(lDescription, 74);
		std::string lBlank("    ");
		lDescription = lBlank + lDescription;
		for(unsigned int i = 0; i < lDescription.size(); i++) {
			if(lDescription[i] == '\n') lDescription.insert(i+1, "    ");
		}
		outStream << lDescription << std::endl << std::endl;
	}
	outStream << std::endl << std::flush;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the short program usage.
 *  \param outStream Output stream.
 */
void Register::showUsage(std::ostream& outStream) const
{
	Beagle_StackTraceBeginM();

	for(DescriptionMap::const_iterator lIter = mDescriptions.begin(); lIter != mDescriptions.end(); ++lIter) {
		outStream << " -OB" << lIter->first;
		unsigned int lCountSize = lIter->first.size()+4;
		if(lIter->second.mType.empty() == false) {
			outStream << "=<" << lIter->second.mType << ">";
			lCountSize += lIter->second.mType.size()+3;
		}
		Beagle_AssertM(lCountSize <= 46);
		outStream << std::string(46-lCountSize, ' ');
		outStream << lIter->second.mBrief << std::endl;
	}
	outStream << std::endl << std::flush;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a register into a Beagle output stream.
 *  \param ioStreamer XML streamer to read the register to.
 *  \param inIndent Whether XML output should be indented.
 */
void Register::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag("Register", inIndent);
	std::set< std::string > lSortedKeys;
	for(Map::const_iterator lIter=mParameters.begin(); lIter != mParameters.end(); ++lIter) {
		lSortedKeys.insert(lIter->first);
	}
	for(std::set< std::string >::const_iterator lIter=lSortedKeys.begin(); lIter != lSortedKeys.end(); ++lIter) {
		Map::const_iterator lParameter=mParameters.find(*lIter);
		DescriptionMap::const_iterator lDescription=mDescriptions.find(*lIter);
		if(lDescription != mDescriptions.end()) {
			std::ostringstream lOSS;
			lOSS << lParameter->first << " [";
			lOSS << lDescription->second.mType << "]: ";
			lOSS << lDescription->second.mDescription;
			//      ioStreamer.insertComment(lOSS.str());
		}
		ioStreamer.openTag("Entry", false);
		ioStreamer.insertAttribute("key", lParameter->first);
		lParameter->second->write(ioStreamer, false);
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();

	Beagle_StackTraceEndM();
}

