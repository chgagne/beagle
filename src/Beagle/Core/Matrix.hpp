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
 *  \file   Beagle/Core/Matrix.hpp
 *  \brief  Definition of class Matrix.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/08 19:26:44 $
 */


#ifndef Beagle_Core_Matrix_hpp
#define Beagle_Core_Matrix_hpp

#include "PACC/Math.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/ContainerT.hpp"

namespace Beagle
{

class Vector;

/*!
 *  \class Matrix Beagle/Core/Matrix.hpp "Beagle/Core/Matrix.hpp"
 *  \brief Adapt the PACC::Matrix class to the Beagle Object interface.
 *  \ingroup Core
 *  \ingroup Adapt
 */
class Matrix : public Object, public PACC::Matrix
{

public:

	//! Matrix allocator type.
	typedef AllocatorT< Matrix, Object::Alloc >
	Alloc;
	//! Matrix handle type.
	typedef PointerT< Matrix, Object::Handle >
	Handle;
	//! Matrix bag type.
	typedef ContainerT< Matrix, Object::Bag >
	Bag;

	explicit Matrix(unsigned int inRows=0,
	                unsigned int inCols=0,
	                double inValue=0.0);
	Matrix(const PACC::Matrix& inMatrix);
	Matrix(const PACC::Vector& inVector);
	Matrix(const Beagle::Vector& inVector);
	virtual ~Matrix()
	{ }

	Matrix& operator=(const PACC::Matrix& inMatrix);
	Matrix& operator=(const PACC::Vector& inVector);
	Matrix& operator=(const Beagle::Vector& inVector);

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual std::string serialize(bool inIndent=false, unsigned int inIndentWidth=0) const;
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}


#endif // Beagle_Core_Matrix_hpp



