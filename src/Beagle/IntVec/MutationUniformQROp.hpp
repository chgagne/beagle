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
 *  \file   beagle/GA/MutationQRUniformIntVecOp.hpp
 *  \brief  Definition of the class GA::MutationQRUniformIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationQRUniformIntVecOp_hpp
#define Beagle_GA_MutationQRUniformIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/GA/MutationUniformIntVecOp.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationQRUniformIntVecOp beagle/GA/MutationQRUniformIntVecOp.hpp "beagle/GA/MutationQRUniformIntVecOp.hpp"
 *  \brief Integer vector GA quasi-random operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class MutationQRUniformIntVecOp : public GA::MutationUniformIntVecOp
{

public:

	//! GA::MutationQRUniformIntVecOp allocator type.
	typedef AllocatorT<MutationQRUniformIntVecOp,GA::MutationUniformIntVecOp::Alloc>
	Alloc;
	//! GA::MutationQRUniformIntVecOp handle type.
	typedef PointerT<MutationQRUniformIntVecOp,GA::MutationUniformIntVecOp::Handle>
	Handle;
	//! GA::MutationQRUniformIntVecOp bag type.
	typedef ContainerT<MutationQRUniformIntVecOp,GA::MutationUniformIntVecOp::Bag>
	Bag;

	explicit MutationQRUniformIntVecOp(std::string inMutationPbName="ga.mutunif.indpb",
	                                   std::string inIntMutatePbName="ga.mutunif.intpb",
	                                   std::string inName="GA-MutationQRUniformIntVecOp");
	virtual ~MutationQRUniformIntVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);

};

}
}

#endif // Beagle_GA_MutationQRUniformIntVecOp_hpp
