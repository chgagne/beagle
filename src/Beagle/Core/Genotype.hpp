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
 *  \file   beagle/Core/Genotype.hpp
 *  \brief  Definition of the type Genotype.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_Core_Genotype_hpp
#define Beagle_Core_Genotype_hpp

#include "PACC/XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Allocator.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/Pointer.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/Container.hpp"
#include "beagle/Core/Member.hpp"


namespace Beagle
{

// Forward declaration
class Context;


/*!
 *  \class Genotype beagle/Core/Genotype.hpp "beagle/Core/Genotype.hpp"
 *  \brief A plain empty genotype.
 *  \ingroup Core
 *  \ingroup Pop
 */
class Genotype : public Member
{

public:

	//! Genotype allocator type.
	typedef AbstractAllocT<Genotype,Member::Alloc> Alloc;
	//! Genotype handle type.
	typedef PointerT<Genotype,Member::Handle> Handle;
	//! Genotype bag type.
	typedef ContainerT<Genotype,Member::Bag> Bag;

	//! Default constructor.
	Genotype()
	{ }
	virtual ~Genotype()
	{ }

	virtual const std::string&  getName() const;
	virtual unsigned int        getSize() const;
	virtual void				swap(unsigned int inI, unsigned int inJ);
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_Genotype_hpp
