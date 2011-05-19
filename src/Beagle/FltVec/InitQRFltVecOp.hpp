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
 *  \file   beagle/GA/InitQRFltVecOp.hpp
 *  \brief  Definition of the class GA::InitQRFltVecOp.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitQRFltVecOp_hpp
#define Beagle_GA_InitQRFltVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/GA/InitFltVecOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class InitQRFltVecOp beagle/GA/InitQRFltVecOp.hpp "beagle/GA/InitQRFltVecOp.hpp"
 *  \brief Real-valued GA genotype derandomized initialization operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class InitQRFltVecOp : public GA::InitFltVecOp
{

public:

	//! GA::InitQRFltVecOp allocator type.
	typedef AllocatorT<InitQRFltVecOp,GA::InitFltVecOp::Alloc>
	Alloc;
	//! GA::InitQRFltVecOp handle type.
	typedef PointerT<InitQRFltVecOp,GA::InitFltVecOp::Handle>
	Handle;
	//! GA::InitQRFltVecOp bag type.
	typedef ContainerT<InitQRFltVecOp,GA::InitFltVecOp::Bag>
	Bag;

	explicit InitQRFltVecOp(unsigned int inFloatVectorSize=0,
	                        std::string inReproProbaName="ec.repro.prob",
	                        std::string inName="GA-InitQRFltVecOp");
	virtual ~InitQRFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

};

}
}

#endif // Beagle_GA_InitQRFltVecOp_hpp
