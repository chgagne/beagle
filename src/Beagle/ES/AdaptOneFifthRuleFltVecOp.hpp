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
 *  \file   beagle/GA/AdaptOneFifthRuleFltVecOp.hpp
 *  \brief  Definition of the class GA::AdaptOneFifthRuleFltVecOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_AdaptOneFifthRuleFltVecOp_hpp
#define Beagle_GA_AdaptOneFifthRuleFltVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/BreederOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class AdaptOneFifthRuleFltVecOp beagle/GA/AdaptOneFifthRuleFltVecOp.hpp
 *    "beagle/GA/AdaptOneFifthRuleFltVecOp.hpp"
 *  \brief Operator class to adapt mutation sigma according to the 1/5th-rule.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class AdaptOneFifthRuleFltVecOp : public Beagle::BreederOp
{

public:

	//! GA::AdaptOneFifthRuleFltVecOp allocator type.
	typedef AllocatorT<AdaptOneFifthRuleFltVecOp,Beagle::BreederOp::Alloc>
	Alloc;
	//! GA::AdaptOneFifthRuleFltVecOp handle type.
	typedef PointerT<AdaptOneFifthRuleFltVecOp,Beagle::BreederOp::Handle>
	Handle;
	//! GA::AdaptOneFifthRuleFltVecOp bag type.
	typedef ContainerT<AdaptOneFifthRuleFltVecOp,Beagle::BreederOp::Bag>
	Bag;

	explicit AdaptOneFifthRuleFltVecOp(std::string inSigmaName="ga.mutgauss.sigma",
	                                   std::string inName="GA-AdaptOneFifthRuleFltVecOp");
	virtual ~AdaptOneFifthRuleFltVecOp()
	{ }

	virtual double getBreedingProba(BreederNode::Handle inChild);
	virtual void  registerParams(System& ioSystem);
	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual void  operate(Deme& ioDeme, Context& ioContext);
	virtual void  readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void  writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	UInt::Handle        mAdaptationPeriod;      //!< Number of observations before adapting sigma.
	UInt::Handle        mSuccessCount;          //!< Number of successful mutations observed.
	UInt::Handle        mMutationsCount;        //!< Number of mutations observed.
	Double::Handle      mSigmaAdaptFactor;      //!< Adaptation factor to use to modify sigma.
	DoubleArray::Handle mMutateGaussSigma;      //!< Sigma values.
	std::string         mMutateGaussSigmaName;  //!< Sigma parameter name.

};

}
}

#endif // Beagle_GA_AdaptOneFifthRuleFltVecOp_hpp
