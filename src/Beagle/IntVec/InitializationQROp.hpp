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
 *  \file   Beagle/IntVec/InitializationQROp.hpp
 *  \brief  Definition of the class IntVec::InitializationQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_InitializationQROp_hpp
#define Beagle_IntVec_InitializationQROp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{
	
namespace IntVec
{

/*!
 *  \class InitializationQROp Beagle/IntVec/InitializationQROp.hpp "Beagle/IntVec/InitializationQROp.hpp"
 *  \brief Integer-valued genotype quasi-random initialization operator class.
 *  \ingroup IntVecF
 */
class InitializationQROp : public IntVec::InitializationOp
{

public:

	//! IntVec::InitializationQROp allocator type.
	typedef AllocatorT<InitializationQROp,IntVec::InitializationOp::Alloc>
	Alloc;
	//! IntVec::InitializationQROp handle type.
	typedef PointerT<InitializationQROp,IntVec::InitializationOp::Handle>
	Handle;
	//! IntVec::InitializationQROp bag type.
	typedef ContainerT<InitializationQROp,IntVec::InitializationOp::Bag>
	Bag;

	explicit InitializationQROp(unsigned int inIntVectorSize=0,
	                        std::string inReproProbaName="ec.repro.prob",
	                        std::string inName="IntVec-InitializationQROp");
	virtual ~InitializationQROp()
	{ }

	virtual void init(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);
	virtual void registerParams(System& ioSystem);
	
};

}
}

#endif // Beagle_IntVec_InitializationQROp_hpp
