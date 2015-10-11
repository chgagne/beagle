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
 *  \file   beagle/GA/MutationCMAFltVecOp.hpp
 *  \brief  Definition of the class GA::MutationCMAFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationCMAFltVecOp_hpp
#define Beagle_GA_MutationCMAFltVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationCMAFltVecOp beagle/GA/MutationCMAFltVecOp.hpp
 *    "beagle/GA/MutationCMAFltVecOp.hpp"
 *  \brief CMA-ES float vector mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 *
 *  This mutation operator is intended to be used in a breeder tree in conjunction of the
 *  GA::MuWCommaLambdaCMAFltVecOp replacement strategy. The parameters of the mutation
 *  are adaptively modified by this replacement strategy following the CMA-ES algorithm.
 *
 *  Reference: Hansen, N. and Ostermeier, A. (2001). Completely Derandomized Self-Adaptation
 *  in Evolution Strategies. Evolutionary Computation, 9(2), pp. 159-195.
 *  See also: http://www.bionik.tu-berlin.de/user/niko/cmaes_inmatlab.html
 */
class MutationCMAFltVecOp : public Beagle::MutationOp
{

public:

	//! GA::MutationCMAFltVecOp allocator type.
	typedef AllocatorT<MutationCMAFltVecOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GA::MutationCMAFltVecOp handle type.
	typedef PointerT<MutationCMAFltVecOp,Beagle::MutationOp::Handle>
	Handle;
	//! GA::MutationCMAFltVecOp bag type.
	typedef ContainerT<MutationCMAFltVecOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationCMAFltVecOp(std::string inMutationPbName="ga.cmaes.mutpb",
	                             std::string inName="GA-MutationCMAFltVecOp");
	virtual ~MutationCMAFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);

protected:

	DoubleArray::Handle mMaxValue;    //!< Max value of GA float vectors.
	DoubleArray::Handle mMinValue;    //!< Min value of GA float vectors.

};

}
}

#endif // Beagle_GA_MutationCMAFltVecOp_hpp
