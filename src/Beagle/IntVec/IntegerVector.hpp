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
 *  \file   beagle/GA/IntegerVector.hpp
 *  \brief  Definition of the type GA::IntegerVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 18:09:10 $
 */

/*!
 *  \defgroup GAIV Integer-valued GA
 *  \ingroup GAF
 *  \brief Integer-valued GA representation, operators and evolvers, part of the GA framework.
 */

#ifndef Beagle_GA_IntegerVector_hpp
#define Beagle_GA_IntegerVector_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Genotype.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class IntegerVector beagle/GA/IntegerVector.hpp "beagle/GA/IntegerVector.hpp"
 *  \brief Integer-valued GA genotype class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class IntegerVector : public Genotype, public std::vector<int>
{

public:

	//! IntegerVector allocator type.
	typedef AllocatorT<IntegerVector,Genotype::Alloc> Alloc;
	//! IntegerVector handle type.
	typedef PointerT<IntegerVector,Genotype::Handle> Handle;
	//! IntegerVector bag type.
	typedef ContainerT<IntegerVector,Genotype::Bag> Bag;

	explicit IntegerVector(unsigned int inSize=0, int inModel=0);

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual unsigned int        getSize() const;
	virtual const std::string&  getType() const;
	virtual void				swap(unsigned int inI, unsigned inJ);
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual bool                isLess(const Object& inRightObj) const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}
}

#endif // Beagle_GA_IntegerVector_hpp
