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
 *  \file   Beagle/FltVec/MutationGaussianQROp.hpp
 *  \brief  Definition of the class FltVec::MutationGaussianQROp.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_FltVec_MutationGaussianQROp_hpp
#define Beagle_FltVec_MutationGaussianQROp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/FltVec/MutationGaussianOp.hpp"


namespace Beagle
{

namespace FltVec
{

/*!
 *  \class MutationGaussianQROp Beagle/FltVec/MutationGaussianQROp.hpp
 *    "Beagle/FltVec/MutationGaussianQROp.hpp"
 *  \brief Real-valued Gaussian derandomized mutation operator class.
 *  \ingroup FltVecF
 */
class MutationGaussianQROp : public FltVec::MutationGaussianOp
{

public:

	//! FltVec::MutationGaussianQROp allocator type.
	typedef AllocatorT<MutationGaussianQROp,FltVec::MutationGaussianOp::Alloc>
	Alloc;
	//! FltVec::MutationGaussianQROp handle type.
	typedef PointerT<MutationGaussianQROp,FltVec::MutationGaussianOp::Handle>
	Handle;
	//! FltVec::MutationGaussianQROp bag type.
	typedef ContainerT<MutationGaussianQROp,FltVec::MutationGaussianOp::Bag>
	Bag;

	explicit MutationGaussianQROp(std::string inMutationPbName="fltvec.mutgauss.indpb",
	                              std::string inMutateFloatPbName="fltvec.mutgauss.floatpb",
	                              std::string inMutateGaussMuName="fltvec.mutgauss.mu",
	                              std::string inMutateGaussSigmaName="fltvec.mutgauss.sigma",
	                              std::string inName="FltVec-MutationGaussianQROp");
	virtual ~MutationGaussianQROp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);

};

}
}

#endif // Beagle_FltVec_MutationGaussianQROp_hpp
