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



