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
 *  \file   Beagle/Core/Component.hpp
 *  \brief  Definition of class Component.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_Component_hpp
#define Beagle_Core_Component_hpp

#include "PACC/XML.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/NamedObject.hpp"

namespace Beagle
{

// Forward declaration
class System;


/*!
 *  \class Component Beagle/Core/Component.hpp "Beagle/Core/Component.hpp"
 *  \brief System component base class.
 *  \ingroup Core
 *  \ingroup Sys
 */
class Component : public NamedObject
{

public:

	//! Component allocator type.
	typedef AllocatorT<Component,NamedObject::Alloc> Alloc;

	//! Component handle type.
	typedef PointerT<Component,NamedObject::Handle> Handle;

	//! Component bag type.
	typedef ContainerT<Component,NamedObject::Bag> Bag;

	explicit Component(std::string inName="Unnamed component");
	virtual ~Component()
	{ }

	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	//! Return whether the parameters of this component have been registered.
	bool hasRegisteredParams(void) const {
		return mRegistered;
	}

	//! Return whether this component has been initialized.
	bool isInitialized(void) const {
		return mInitialized;
	}

	//! Set the state of the registered flag to value \c inValue.
	void setRegisteredFlag(bool inValue=true) {
		mRegistered = inValue;
	}

	//! Set the state of the initialized flag to value \c inValue.
	void setInitializedFlag(bool inValue=true) {
		mInitialized = inValue;
	}

protected:

	bool mRegistered; //!< Flag that indicates whether component parameters have been registered.
	bool mInitialized; //!< Flag that indicates whether this component has been initialized.

};

}

#endif // Beagle_Core_Component_hpp
