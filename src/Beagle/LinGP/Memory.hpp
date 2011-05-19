/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/Memory.hpp
 *  \brief  Definition of the type LinGP::Memory.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_Memory_hpp
#define Beagle_LinGP_Memory_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/Container.hpp"

namespace Beagle
{
namespace LinGP
{


/*!
 *  \class Memory beagle/LinGP/Memory.hpp "beagle/LinGP/Memory.hpp"
 *  \brief The linear GP memory class.
 *  \ingroup LinGPF
 *  \ingroup LinGPSys
 */
class Memory : public Container
{

public:

	//! LinGP::Memory allocator type.
	typedef AllocatorT<Memory,Container::Alloc> Alloc;
	//! LinGP::Memory handle type.
	typedef PointerT<Memory,Container::Handle> Handle;
	//! LinGP::Memory bag type.
	typedef ContainerT<Memory,Container::Bag> Bag;

	/*!
	 *  \brief Construct a shared memory for linear GP.
	 *  \param inTypeAlloc Type allocator of data in the shared memory.
	 *  \param inN Number of data in the shared memory.
	 */
	explicit Memory(unsigned int inN=0, Object::Handle inModel=NULL) :
			Container(inN, inModel),
			mIndex(0)
	{ }

	virtual ~Memory()
	{ }

	/*!
	 *  \return Memory's index value.
	 */
	inline unsigned int getIndex() const
	{
		return mIndex;
	}

	/*!
	 *  \brief Set memory's index value.
	 *  \param inIndex New index value.
	 */
	inline void setIndex(unsigned int inIndex)
	{
		mIndex = inIndex;
	}

protected:

	unsigned int mIndex;   // Index in the memory.

};

}
}

#endif // Beagle_LinGP_Memory_hpp



