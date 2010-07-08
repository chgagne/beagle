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
 *  \file   Beagle/Core/Genotype.hpp
 *  \brief  Definition of the type Genotype.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_Core_Genotype_hpp
#define Beagle_Core_Genotype_hpp

#include "PACC/XML.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/Member.hpp"


namespace Beagle
{

// Forward declaration
class Context;


/*!
 *  \class Genotype Beagle/Core/Genotype.hpp "Beagle/Core/Genotype.hpp"
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
