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
 *  \file   Beagle/FltVec/FloatVector.hpp
 *  \brief  Definition of the type FltVec::FloatVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#ifndef Beagle_GA_FloatVector_hpp
#define Beagle_GA_FloatVector_hpp

#include <vector>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{

namespace FltVec
{

/*!
 *  \class FloatVector Beagle/FltVec/FloatVector.hpp "Beagle/FltVec/FloatVector.hpp"
 *  \brief Real-valued genotype class.
 *  \ingroup FltVecF
 */
class FloatVector : public Genotype, public std::vector<double>
{

public:

	//! FltVec::FloatVector allocator type.
	typedef AllocatorT<FloatVector,Genotype::Alloc> Alloc;
	//! FltVec::FloatVector handle type.
	typedef PointerT<FloatVector,Genotype::Handle> Handle;
	//! FltVec::FloatVector bag type.
	typedef ContainerT<FloatVector,Genotype::Bag> Bag;

	explicit FloatVector(unsigned int inSize=0, float inModel=0.f);

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

#endif // Beagle_FltVec_FloatVector_hpp

