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
 *  \file   beagle/GA/MutationQRGaussianFltVecOp.hpp
 *  \brief  Definition of the class GA::MutationQRGaussianFltVecOp.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationQRGaussianFltVecOp_hpp
#define Beagle_GA_MutationQRGaussianFltVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/GA/MutationGaussianFltVecOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationQRGaussianFltVecOp beagle/GA/MutationQRGaussianFltVecOp.hpp
 *    "beagle/GA/MutationQRGaussianFltVecOp.hpp"
 *  \brief Real-valued GA Gaussian derandomized mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class MutationQRGaussianFltVecOp : public GA::MutationGaussianFltVecOp
{

public:

	//! GA::MutationQRGaussianFltVecOp allocator type.
	typedef AllocatorT<MutationQRGaussianFltVecOp,GA::MutationGaussianFltVecOp::Alloc>
	Alloc;
	//! GA::MutationQRGaussianFltVecOp handle type.
	typedef PointerT<MutationQRGaussianFltVecOp,GA::MutationGaussianFltVecOp::Handle>
	Handle;
	//! GA::MutationQRGaussianFltVecOp bag type.
	typedef ContainerT<MutationQRGaussianFltVecOp,GA::MutationGaussianFltVecOp::Bag>
	Bag;

	explicit MutationQRGaussianFltVecOp(std::string inMutationPbName="ga.mutgauss.indpb",
	                                    std::string inMutateFloatPbName="ga.mutgauss.floatpb",
	                                    std::string inMutateGaussMuName="ga.mutgauss.mu",
	                                    std::string inMutateGaussSigmaName="ga.mutgauss.sigma",
	                                    std::string inName="GA-MutationQRGaussianFltVecOp");
	virtual ~MutationQRGaussianFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);

};

}
}

#endif // Beagle_GA_MutationQRGaussianFltVecOp_hpp
