/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
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
