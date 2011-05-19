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
 *  \file   Beagle/Core/Register.hpp
 *  \brief  Definition of the class Register.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_Register_hpp
#define Beagle_Core_Register_hpp

#include <map>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/String.hpp"
#include "Beagle/Core/Operator.hpp"
#include "Beagle/Core/Component.hpp"


namespace Beagle
{

// Forward declaration
class Context;
class System;


/*!
 *  \class Register Beagle/Core/Register.hpp "Beagle/Core/Register.hpp"
 *  \brief Register implementation, where all the parameters of the framework are centralized.
 *  \ingroup Core
 *  \ingroup Sys
 */
class Register : public Component
{

public:

	/*!
	 *  \struct Description Beagle/Core/Register.hpp "Beagle/Core/Register.hpp"
	 *  \brief Parameter extensive description.
	 */
	struct Description {
		std::string mBrief;         //!< Brief description of the parameter.
		std::string mType;          //!< Type of the parameter.
		std::string mDefaultValue;  //!< Default value of the parameter.
		std::string mDescription;   //!< Long description of the parameter.

		Description();
		explicit Description(const std::string& inBrief, const std::string& inType, const std::string& inDefaultValue, const std::string& inDescription);
	};

	//! Register allocator type.
	typedef AllocatorT<Register,Component::Alloc> Alloc;

	//! Register handle type.
	typedef PointerT<Register,Component::Handle> Handle;

	//! Register bag type.
	typedef ContainerT<Register,Component::Bag> Bag;

	Register(void);
	virtual ~Register()
	{ }

	void               addEntry(const std::string& inTag, Object::Handle inEntry, const Description& inDescription);
	Object::Handle     deleteEntry(const std::string& inTag);
	const Description& getDescription(const std::string& inTag) const;
	Object::Handle     insertEntry(const std::string& inTag, Object::Handle inEntry, const Description& inDescription);
	bool               isRegistered(const std::string& inTag) const;
	Object::Handle     modifyEntry(const std::string& inTag, Object::Handle inNewEntry);
	Description        modifyDescription(const std::string& inTag, const Description& inNewDescription);
	virtual void       read(PACC::XML::ConstIterator inIter);
	virtual void       readFromFile(const std::string& inFileName, System& ioSystem);
	virtual void       readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	void               setEntryDefaultValue(const std::string& inTag, Object::Handle inValue, const std::string& inValueStr);
	virtual void       write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Get an entry of the register.
	 *  \param inTag Tag of the parameter.
	 *  \return Smart pointer to the entry. Return a NULL pointer if there is no entry
	 *     associated with the tag.
	 */
	inline Object::Handle operator[](const std::string& inTag) {
		Beagle_StackTraceBeginM();
		Map::iterator lIterObj = mParameters.find(inTag);
		if(lIterObj == mParameters.end()) return Object::Handle(NULL);
		else return lIterObj->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get an entry of the register.
	 *  \param inTag Tag of the parameter.
	 *  \return Constant smart pointer to the entry. Return a NULL pointer if there is no entry
	 *     associated with the tag.
	 */
	inline const Object::Handle operator[](const std::string& inTag) const {
		Beagle_StackTraceBeginM();
		Map::const_iterator lIterObj = mParameters.find(inTag);
		if(lIterObj == mParameters.end()) return Object::Handle(NULL);
		else return lIterObj->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get an entry of the register.
	 *  \param inTag Tag of the parameter.
	 *  \return Smart pointer to the entry. Return a NULL pointer if there is no entry
	 *     associated with the tag.
	 */
	inline Object::Handle getEntry(const std::string& inTag) {
		Beagle_StackTraceBeginM();
		Map::iterator lIterObj = mParameters.find(inTag);
		if(lIterObj == mParameters.end()) return Object::Handle(NULL);
		else return lIterObj->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get an entry of the register.
	 *  \param inTag Tag of the parameter.
	 *  \return Constant smart pointer to the entry. Return a NULL pointer if there is no entry
	 *     associated with the tag.
	 */
	inline const Object::Handle getEntry(const std::string& inTag) const {
		Beagle_StackTraceBeginM();
		Map::const_iterator lIterObj = mParameters.find(inTag);
		if(lIterObj == mParameters.end()) return Object::Handle(NULL);
		else return lIterObj->second;
		Beagle_StackTraceEndM();
	}

protected:

	typedef std::map< std::string, Description, std::less<std::string> > DescriptionMap;
	typedef std::vector< Operator::Handle > OperatorVector;
	typedef std::map< std::string, std::pair<Object::Handle, std::string> > DefaultValuesMap;

	void  addDescription(const std::string& inTag, const Description& inDescription);
	char* eraseArg(int inN, int& ioArgc, char**& ioArgv) const;
	void  showHelp(std::ostream& outStream=std::cerr) const;
	void  showUsage(std::ostream& outStream=std::cerr) const;

	Map              mParameters;     //!< Param. of the register.
	DefaultValuesMap mDefaultValues;  //!< Map of parameters default values.
	DescriptionMap   mDescriptions;   //!< Parameters descriptions.
	OperatorVector   mOperatorUsed;   //!< Operator used.

	friend class System;
	friend class Evolver;

};

}

#endif // Beagle_Core_Register_hpp
