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
