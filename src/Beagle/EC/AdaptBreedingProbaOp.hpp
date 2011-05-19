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
 *  \file   Beagle/EC/AdaptBreedingProbaOp.hpp
 *  \brief  Definition of class AdaptBreedingProbaOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:11 $
 */


#ifndef Beagle_EC_AdaptBreedingProbaOp_hpp
#define Beagle_EC_AdaptBreedingProbaOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class AdaptBreedingProbaOp Beagle/EC/AdaptBreedingProbaOp.hpp "Beagle/EC/AdaptBreedingProbaOp.hpp"
 *  \brief Adaptation of breeding probabilities operator class.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  The AdaptBreedingProbaOp operator is used to adapt the breeding probabilities of underlying
 *  variation operators (generally mutation operators) during the course of evolution, by
 *  increasing the breeding probability of operators that produces offsprings with
 *  fitness better than their parents, and decreasing the breeding probability of operators that
 *  produce offsprings with a fitness worse or equal to the fitness of their parents.
 *
 *  The adaptation of breeding probabilities is done each time two variation operators have generated
 *  a number of candidates higher or equal to the adaptation period (given by parameter
 *  'ec.adaptbreed.period'). The variation operator which generate the most offsprings of improved
 *  fitness has its breeding probability increased by a multiplication with the the inverse of the
 *  adaptation factor (given by parameter 'ec.adaptbreed.factor'), that is p'_i = p_i / c, where p_i,
 *  p'_i and c are respectively the old and new breeding probabilities, and the adaptation factor.
 *  For the variation operator that generated less offsprings of improved fitness compared the the
 *  other, the breeding probabilites is decreased by multiply it with the same adaptation factor,
 *  that is p'_j = p_j * c. After an adaptation of probabilities, all the breeding probabilities
 *  are normalized to be equal to 1.0 when summed.
 */
class AdaptBreedingProbaOp : public Beagle::BreederOp
{

public:

	//! AdaptBreedingProbaOp allocator type.
	typedef Beagle::AllocatorT<AdaptBreedingProbaOp,Beagle::BreederOp::Alloc>
	Alloc;
	//! AdaptBreedingProbaOp handle type.
	typedef Beagle::PointerT<AdaptBreedingProbaOp,Beagle::BreederOp::Handle>
	Handle;
	//! AdaptBreedingProbaOp bag type.
	typedef Beagle::ContainerT<AdaptBreedingProbaOp,Beagle::BreederOp::Bag>
	Bag;

	explicit AdaptBreedingProbaOp(std::string inName="AdaptBreedingProbaOp");
	virtual ~AdaptBreedingProbaOp()
	{ }

	virtual Beagle::Individual::Handle breed(Beagle::Individual::Bag& inBreedingPool,
	        Beagle::BreederNode::Handle inChild,
	        Beagle::Context& ioContext);
	virtual double                     getBreedingProba(Beagle::BreederNode::Handle inChild);
	virtual void                       operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void                       registerParams(Beagle::System& ioSystem);

protected:

	Beagle::DoubleArray::Handle  mAdaptedProbas;     //!< Selection proba different operators.
	Beagle::UIntArray::Handle    mSuccessCount;      //!< Number of successful variations observed.
	Beagle::UIntArray::Handle    mVariationCount;    //!< Number of variations observed.
	Beagle::UInt::Handle         mAdaptationPeriod;  //!< Number of variations between adaptation.
	Beagle::Double::Handle       mAdaptationFactor;  //!< Adaptation factor used to modify breeding proba.

};

}

}

#endif // Beagle_EC_AdaptBreedingProbaOp_hpp
