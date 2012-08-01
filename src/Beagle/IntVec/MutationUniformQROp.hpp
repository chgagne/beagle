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
 *  \file   Beagle/IntVec/MutationUniformQROp.hpp
 *  \brief  Definition of the class IntVec::MutationUniformQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_MutationUniformQROp_hpp
#define Beagle_IntVec_MutationUniformQROp_hpp

#include "Beagle/IntVec.hpp"


namespace Beagle
{
namespace IntVec
{

/*!
 *  \class MutationUniformQROp Beagle/IntVec/MutationUniformQROp.hpp "Beagle/IntVec/MutationUniformQROp.hpp"
 *  \brief Integer vector uniform mutation quasi-random operator class.
 *  \ingroup IntVecF
 */
class MutationUniformQROp : public IntVec::MutationUniformOp
{

public:

	//! IntVec::MutationUniformQROp allocator type.
	typedef AllocatorT<MutationUniformQROp,IntVec::MutationUniformOp::Alloc> Alloc;
	//! IntVec::MutationUniformQROp handle type.
	typedef PointerT<MutationUniformQROp,IntVec::MutationUniformOp::Handle>	Handle;
	//! IntVec::MutationUniformQROp bag type.
	typedef ContainerT<MutationUniformQROp,IntVec::MutationUniformOp::Bag> Bag;

	explicit MutationUniformQROp(std::string inMutationPbName="intvec.mutunif.indpb",
	                             std::string inIntMutatePbName="intvec.mutunif.intpb",
	                             std::string inName="IntVec-MutationUniformQROp");
	virtual ~MutationUniformQROp()
	{ }

	virtual void init(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void registerParams(System& ioSystem);

};

}
}

#endif // Beagle_IntVec_MutationUniformQROp_hpp
