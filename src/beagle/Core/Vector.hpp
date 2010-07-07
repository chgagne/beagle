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
 *  \file   beagle/Core/Vector.hpp
 *  \brief  Definition of class Vector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:13 $
 */


#ifndef Beagle_Core_Vector_hpp
#define Beagle_Core_Vector_hpp

#include "PACC/Math.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/ContainerT.hpp"

namespace Beagle
{


/*!
 *  \class Vector beagle/Core/Vector.hpp "beagle/Core/Vector.hpp"
 *  \brief Adapt the PACC::Vector class to the Beagle Object interface.
 *  \ingroup OOF
 *  \ingroup Adapt
 */
class Vector : public Object, public PACC::Vector
{

public:

	//! Vector allocator type.
	typedef AllocatorT< Vector, Object::Alloc > Alloc;
	//! Vector handle type.
	typedef PointerT< Vector, Object::Handle > Handle;
	//! Vector bag type.
	typedef ContainerT< Vector, Object::Bag > Bag;

	explicit Vector(unsigned int inSize=0, double inValue=0.0);
	Vector(const PACC::Matrix& inMatrix);
	Vector(const PACC::Vector& inVector);
	Vector(const Beagle::Matrix& inMatrix);
	virtual ~Vector()
	{ }

	Vector& operator=(const PACC::Matrix& inMatrix);
	Vector& operator=(const PACC::Vector& inVector);
	Vector& operator=(const Beagle::Matrix& inMatrix);

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual std::string serialize(bool inIndent=false, unsigned int inIndentWidth=0) const;
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}


#endif // Beagle_Core_Vector_hpp



