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
 *  \file   Beagle/Core/Vector.hpp
 *  \brief  Definition of class Vector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:13 $
 */


#ifndef Beagle_Core_Vector_hpp
#define Beagle_Core_Vector_hpp

#include "PACC/Math.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/ContainerT.hpp"

namespace Beagle
{


/*!
 *  \class Vector Beagle/Core/Vector.hpp "Beagle/Core/Vector.hpp"
 *  \brief Adapt the PACC::Vector class to the Beagle Object interface.
 *  \ingroup Core
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



