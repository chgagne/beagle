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
 *  \file   Beagle/IntVec/InitUniformQROp.hpp
 *  \brief  Definition of the class IntVec::InitUniformQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_InitUniformQROp_hpp
#define Beagle_IntVec_InitUniformQROp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{
	
namespace IntVec
{

/*!
 *  \class InitUniformQROp Beagle/IntVec/InitUniformQROp.hpp "Beagle/IntVec/InitUniformQROp.hpp"
 *  \brief Integer-valued genotype quasi-random initialization operator class.
 *  \ingroup IntVecF
 */
class InitUniformQROp : public IntVec::InitializationOp
{

public:

	//! IntVec::InitUniformQROp allocator type.
	typedef AllocatorT<InitUniformQROp,IntVec::InitializationOp::Alloc>
	Alloc;
	//! IntVec::InitUniformQROp handle type.
	typedef PointerT<InitUniformQROp,IntVec::InitializationOp::Handle>
	Handle;
	//! IntVec::InitUniformQROp bag type.
	typedef ContainerT<InitUniformQROp,IntVec::InitializationOp::Bag>
	Bag;

	explicit InitUniformQROp(unsigned int inIntVectorSize=0,
	                        std::string inReproProbaName="intvec.repro.prob",
	                        std::string inName="IntVec-InitUniformQROp");
	virtual ~InitUniformQROp()
	{ }

	virtual void init(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);
	virtual void registerParams(System& ioSystem);
	
};

}
}

#endif // Beagle_IntVec_InitUniformQROp_hpp
