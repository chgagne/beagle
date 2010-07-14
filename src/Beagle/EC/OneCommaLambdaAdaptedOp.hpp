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
 *  \file   Beagle/EC/OneCommaLambdaAdaptedOp.hpp
 *  \brief  Definition of the class OneCommaLambdaAdaptedOp.
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */


#ifndef Beagle_EC_OneCommaLambdaAdaptedOp_hpp
#define Beagle_EC_OneCommaLambdaAdaptedOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{


/*!
 *  \class OneCommaLambdaAdaptedOp Beagle/EC/OneCommaLambdaAdaptedOp.hpp "Beagle/EC/OneCommaLambdaAdaptedOp.hpp"
 *  \brief One comma lambda with adaptation (1,lambda_adapt) replacement strategy operator class.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  A (1,lambda_adapt) operator generates lambda children individuals from a population of one parent
 *  with an adaptation of the number of children generated following the success rate of the
 *  evolution. More specifically, the replacement strategy is a standard (1,lambda) approach where the
 *  lambda value (initially given by parameter 'ec.1cladp.lambdainit') is adapted at the end of each
 *  generation, when the number of children generated that have a fitness value higher than their
 *  parent is different from 1. If there are more than one individual with a fitness higher than the
 *  parent the population size is reduced by a factor (alpha), and conversely if there is no individual
 *  with a fitness value higher than the parent, the value of lambda is increased by the inverse of the
 *  factor (i.e. 1/alpha).
 */
class OneCommaLambdaAdaptedOp : public Beagle::ReplacementStrategyOp
{

public:

	//! OneCommaLambdaAdaptedOp allocator type.
	typedef Beagle::AllocatorT<OneCommaLambdaAdaptedOp,Beagle::ReplacementStrategyOp::Alloc>
	Alloc;
	//! OneCommaLambdaAdaptedOp handle type.
	typedef Beagle::PointerT<OneCommaLambdaAdaptedOp,Beagle::ReplacementStrategyOp::Handle>
	Handle;
	//! OneCommaLambdaAdaptedOp bag type.
	typedef Beagle::ContainerT<OneCommaLambdaAdaptedOp,Beagle::ReplacementStrategyOp::Bag>
	Bag;

	explicit OneCommaLambdaAdaptedOp(std::string inName="OneCommaLambdaAdaptedOp");
	virtual ~OneCommaLambdaAdaptedOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void init(Beagle::System& ioSystem);

protected:

	Beagle::Float::Handle mLambdaFactorAdapt;        //!< Factor used to adapt lambda value.
	Beagle::UInt::Handle  mLambda;                   //!< Adapted value of lambda.
	Beagle::UInt::Handle  mLambdaInit;               //!< Initial lambda value.
	Beagle::UInt::Handle  mMaxLambda;                //!< Maximum value of lambda.
	Beagle::UInt::Handle  mMinLambda;                //!< Minimum value of lambda.

};

}

}

#endif // Beagle_EC_OneCommaLambdaAdaptedOp_hpp
