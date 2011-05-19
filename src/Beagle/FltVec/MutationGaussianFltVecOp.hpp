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
 *  \file   beagle/GA/MutationGaussianFltVecOp.hpp
 *  \brief  Definition of the class GA::MutationGaussianFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationGaussianFltVecOp_hpp
#define Beagle_GA_MutationGaussianFltVecOp_hpp

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
 *  \class MutationGaussianFltVecOp beagle/GA/MutationGaussianFltVecOp.hpp
 *    "beagle/GA/MutationGaussianFltVecOp.hpp"
 *  \brief Real-valued GA Gaussian mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class MutationGaussianFltVecOp : public Beagle::MutationOp
{

public:

	//! GA::MutationGaussianFltVecOp allocator type.
	typedef AllocatorT<MutationGaussianFltVecOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GA::MutationGaussianFltVecOp handle type.
	typedef PointerT<MutationGaussianFltVecOp,Beagle::MutationOp::Handle>
	Handle;
	//! GA::MutationGaussianFltVecOp bag type.
	typedef ContainerT<MutationGaussianFltVecOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationGaussianFltVecOp(std::string inMutationPbName="ga.mutgauss.indpb",
	                                  std::string inMutateFloatPbName="ga.mutgauss.floatpb",
	                                  std::string inMutateGaussMuName="ga.mutgauss.mu",
	                                  std::string inMutateGaussSigmaName="ga.mutgauss.sigma",
	                                  std::string inName="GA-MutationGaussianFltVecOp");
	virtual ~MutationGaussianFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	DoubleArray::Handle mMaxValue;             //!< Max value of GA float vectors.
	DoubleArray::Handle mMinValue;             //!< Min value of GA float vectors.
	DoubleArray::Handle mIncValue;             //!< Increment of valid value of GA float vectors.
	Float::Handle       mMutateFloatPb;        //!< Single value mutation probability.
	DoubleArray::Handle mMutateGaussMu;        //!< Gaussian mutation mean.
	DoubleArray::Handle mMutateGaussSigma;     //!< Gaussian mutation standard deviation.
	std::string         mMutateFloatPbName;    //!< Single value mutation probability parameter name.
	std::string         mMutateGaussMuName;    //!< Gaussian mutation mean parameter name.
	std::string         mMutateGaussSigmaName; //!< Gaussian mutation std deviation parameter name.

};

}
}

#endif // Beagle_GA_MutationGaussianFltVecOp_hpp
