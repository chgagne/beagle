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
 *  \file   beagle/HPC/AlgoSequential.hpp
 *  \brief  Definition of the class AlgoSequential.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_HPC_AlgoSequential_hpp
#define Beagle_HPC_AlgoSequential_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Algorithm.hpp"


namespace Beagle
{
namespace HPC
{
/*!
 *  \class AlgoSequential beagle/HPC/AlgoSequential.hpp "beagle/HPC/AlgoSequential.hpp"
 *  \brief Basic algorithm class, list operations in sequences (implicit generational).
 *  \ingroup ECF
 */
class AlgoSequential : public Algorithm
{

public:

	//! AlgoSequential allocator type.
	typedef AllocatorT< AlgoSequential, Algorithm::Alloc > Alloc;
	//! AlgoSequential handle type.
	typedef PointerT< AlgoSequential, Algorithm::Handle > Handle;
	//! AlgoSequential bag type.
	typedef ContainerT< AlgoSequential, Algorithm::Bag > Bag;

	AlgoSequential(void);
	virtual ~AlgoSequential()
	{ }

	virtual void configure(Evolver& ioEvolver, System& ioSystem);

};
}
}

#endif // Beagle_HPC_AlgoSequential_hpp
