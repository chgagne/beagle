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
 *  \file   beagle/GA/InitQRESVecOp.hpp
 *  \brief  Definition of the class GA::InitQRESVecOp.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitQRESVecOp_hpp
#define Beagle_GA_InitQRESVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/GA/InitESVecOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class InitQRESVecOp beagle/GA/InitQRESVecOp.hpp "beagle/GA/InitQRESVecOp.hpp"
 *  \brief ES genotype derandomized initialization operator class.
 *  \ingroup GAF
 *  \ingroup GAES
 */
class InitQRESVecOp : public GA::InitESVecOp
{

public:

	//! GA::InitQRESVecOp allocator type.
	typedef AllocatorT<InitQRESVecOp,GA::InitESVecOp::Alloc>
	Alloc;
	//! GA::InitQRESVecOp handle type.
	typedef PointerT<InitQRESVecOp,GA::InitESVecOp::Handle>
	Handle;
	//! GA::InitQRESVecOp bag type.
	typedef ContainerT<InitQRESVecOp,GA::InitESVecOp::Bag>
	Bag;

	explicit InitQRESVecOp(unsigned int inESVectorSize=0,
	                       std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="GA-InitQRESVecOp");
	virtual ~InitQRESVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

};

}
}

#endif // Beagle_GA_InitQRESVecOp_hpp
