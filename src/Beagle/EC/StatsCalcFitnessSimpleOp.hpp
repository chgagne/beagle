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
 *  \file   Beagle/EC/StatsCalcFitnessSimpleOp.hpp
 *  \brief  Definition of the class StatsCalcFitnessSimpleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_StatsCalcFitnessSimpleOp_hpp
#define Beagle_EC_StatsCalcFitnessSimpleOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/StatsCalculateOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class StatsCalcFitnessSimpleOp Beagle/EC/StatsCalcFitnessSimpleOp.hpp
 *     "Beagle/EC/StatsCalcFitnessSimpleOp.hpp"
 *  \brief Calculate statistics of a deme operator class for individual fitness is of the
 *     type FitnessSimple.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup FitStats
 */
class StatsCalcFitnessSimpleOp : public Beagle::EC::StatsCalculateOp
{

public:

	//! StatsCalcFitnessSimpleOp allocator type
	typedef Beagle::AllocatorT<StatsCalcFitnessSimpleOp,Beagle::EC::StatsCalculateOp::Alloc> Alloc;
	//! StatsCalcFitnessSimpleOp handle type.
	typedef Beagle::PointerT<StatsCalcFitnessSimpleOp,Beagle::EC::StatsCalculateOp::Handle> Handle;
	//! StatsCalcFitnessSimpleOp bag type.
	typedef Beagle::ContainerT<StatsCalcFitnessSimpleOp,Beagle::EC::StatsCalculateOp::Bag> Bag;

	explicit StatsCalcFitnessSimpleOp(std::string inName="StatsCalcFitnessSimpleOp");
	virtual ~StatsCalcFitnessSimpleOp()
	{ }

	virtual void calculateStatsDeme(Beagle::Stats& outStats, Beagle::Deme& ioDeme, Beagle::Context& ioContext) const;

};

}

}

#endif // Beagle_EC_StatsCalcFitnessSimpleOp_hpp
