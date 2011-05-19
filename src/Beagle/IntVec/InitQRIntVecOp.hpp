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
 *  \file   beagle/GA/InitQRIntVecOp.hpp
 *  \brief  Definition of the class GA::InitQRIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitQRIntVecOp_hpp
#define Beagle_GA_InitQRIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Int.hpp"
#include "beagle/UInt.hpp"
#include "beagle/GA/InitIntVecOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class InitQRIntVecOp beagle/GA/InitQRIntVecOp.hpp "beagle/GA/InitQRIntVecOp.hpp"
 *  \brief Integer-valued GA genotype quasi-random initialization operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class InitQRIntVecOp : public GA::InitIntVecOp
{

public:

	//! GA::InitQRIntVecOp allocator type.
	typedef AllocatorT<InitQRIntVecOp,GA::InitIntVecOp::Alloc>
	Alloc;
	//! GA::InitQRIntVecOp handle type.
	typedef PointerT<InitQRIntVecOp,GA::InitIntVecOp::Handle>
	Handle;
	//! GA::InitQRIntVecOp bag type.
	typedef ContainerT<InitQRIntVecOp,GA::InitIntVecOp::Bag>
	Bag;

	explicit InitQRIntVecOp(unsigned int inIntVectorSize=0,
	                        std::string inReproProbaName="ec.repro.prob",
	                        std::string inName="GA-InitQRIntVecOp");
	virtual ~InitQRIntVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

};

}
}

#endif // Beagle_GA_InitQRIntVecOp_hpp
