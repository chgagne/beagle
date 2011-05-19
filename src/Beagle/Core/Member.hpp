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
 *  \file   Beagle/Core/Member.hpp
 *  \brief  Definition of class Member.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_Core_Member_hpp
#define Beagle_Core_Member_hpp

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


namespace Beagle
{

// Forward declarations
class Context;
class System;


/*!
 *  \class Member Beagle/Core/Member.hpp "Beagle/Core/Member.hpp"
 *  \brief Population member base class.
 *  \ingroup Core
 *  \ingroup Sys
 */
class Member : public Object
{

public:

	//! Member allocator type.
	typedef AbstractAllocT<Member,Object::Alloc> Alloc;
	//! Member handle type.
	typedef PointerT<Member,Object::Handle> Handle;
	//! Member bag type.
	typedef ContainerT<Member,Object::Bag> Bag;

	Member()
	{ }
	virtual ~Member()
	{ }

	/*!
	 *  \brief Copy member into current.
	 *  \param inOriginal Member to copy.
	 *  \param ioSystem Evolutionary system to use for making the copy.
	 */
	virtual void copy(const Member& inOriginal, System& ioSystem) =0;

	/*!
	 *  \brief Get the name of the member (as held in the member map).
	 *  \return Name of the member.
	 */
	virtual const std::string& getName() const =0;

	/*!
	 *  \brief Get exact type of the member.
	 *  \return Exact type of the member.
	 */
	virtual const std::string& getType() const =0;

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_Member_hpp
