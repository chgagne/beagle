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
 *  \file   Beagle/EC/DecimateOp.hpp
 *  \brief  Definition of the class DecimateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_EC_DecimateOp_hpp
#define Beagle_EC_DecimateOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{


/*!
 *  \class DecimateOp Beagle/EC/DecimateOp.hpp "Beagle/EC/DecimateOp.hpp"
 *  \brief Population decimation operator class.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  This operator shrinks the population size (i.e. it "decimates" the
 *  population).  The decimation is done by keeping the \em n best
 *  individuals of the population, where \em n is \f$ratio \times
 *  population size\f$ and \em ratio is the decimation ratio, which is
 *  less than or equal to 1.0.
 *
 *  The name of the decimation ratio defaults to
 *  "ec.decimation.ratio".
 *
 *  A special decimation ratio exists: -1.0.  If this ratio is used
 *  then the population will be reduced to the size specified in the
 *  register variable "ec.pop.size".  If the use of this special ratio
 *  would result in an \em increase in the population size then an
 *  exception is thrown.
 *
 *  To increase the size of the population, consider
 *  Beagle::OversizeOp.
 *
 */
class DecimateOp : public Beagle::Operator
{

public:

	//! DecimateOp allocator type.
	typedef Beagle::AllocatorT<DecimateOp,Beagle::Operator::Alloc>
	Alloc;
	//! DecimateOp handle type.
	typedef Beagle::PointerT<DecimateOp,Beagle::Operator::Handle>
	Handle;
	//! DecimateOp bag type.
	typedef Beagle::ContainerT<DecimateOp,Beagle::Operator::Bag>
	Bag;

	explicit DecimateOp(std::string inDecimationRatioName="ec.decimation.ratio",
	                    std::string inName="DecimateOp");
	virtual ~DecimateOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	Beagle::UIntArray::Handle  mPopSize;              //!< Population size for the evolution.
	Beagle::Float::Handle      mDecimationRatio;      //!< Decimation ratio for the actual population.
	std::string                mDecimationRatioName;  //!< Decimation ratio parameter name.

};

}

}

#endif // Beagle_EC_DecimateOp_hpp

