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
 *  \file   beagle/StatsCalcFitnessMultiObjOp.hpp
 *  \brief  Definition of the class StatsCalcFitnessMultiObjOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_StatsCalcFitnessMultiObjOp_hpp
#define Beagle_StatsCalcFitnessMultiObjOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"
#include "beagle/StatsCalculateOp.hpp"

namespace Beagle
{


/*!
 *  \class StatsCalcFitnessMultiObjOp beagle/StatsCalcFitnessMultiObjOp.hpp
 *     "beagle/StatsCalcFitnessMultiObjOp.hpp"
 *  \brief Calculate statistics of a deme operator class for individual fitness is of the
 *     type FitnessMultiObj.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup FitStats
 *  \ingroup MultiObj
 */
class StatsCalcFitnessMultiObjOp : public StatsCalculateOp
{

public:

	//! StatsCalcFitnessMultiObjOp allocator type
	typedef AllocatorT<StatsCalcFitnessMultiObjOp,StatsCalculateOp::Alloc> Alloc;
	//! StatsCalcFitnessMultiObjOp handle type.
	typedef PointerT<StatsCalcFitnessMultiObjOp,StatsCalculateOp::Handle> Handle;
	//! StatsCalcFitnessMultiObjOp bag type.
	typedef ContainerT<StatsCalcFitnessMultiObjOp,StatsCalculateOp::Bag> Bag;

	explicit StatsCalcFitnessMultiObjOp(std::string inName="StatsCalcFitnessMultiObjOp");
	virtual ~StatsCalcFitnessMultiObjOp()
	{ }

	virtual void calculateStatsDeme(Stats& outStats, Deme& ioDeme, Context& ioContext) const;

};

}

#endif // Beagle_StatsCalcFitnessMultiObjOp.cpp_hpp

