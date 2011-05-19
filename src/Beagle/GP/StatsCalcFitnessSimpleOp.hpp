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
 *  \file   beagle/GP/StatsCalcFitnessSimpleOp.hpp
 *  \brief  Definition of the class GP::StatsCalcFitnessSimpleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_GP_StatsCalcFitnessSimpleOp_hpp
#define Beagle_GP_StatsCalcFitnessSimpleOp_hpp

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
#include "beagle/StatsCalcFitnessSimpleOp.hpp"

namespace Beagle
{
namespace GP
{

/*!
 *  \class StatsCalcFitnessSimpleOp beagle/GP/StatsCalcFitnessSimpleOp.hpp
 *     "beagle/GP/StatsCalcFitnessSimpleOp.hpp"
 *  \brief Calculate statistics of a deme operator class for individual fitness is of the
 *     type FitnessSimple with individuals made of GP trees.
 *  \ingroup GPF
 */
class StatsCalcFitnessSimpleOp : public Beagle::StatsCalcFitnessSimpleOp
{

public:

	//! StatsCalcFitnessSimpleOp allocator type
	typedef AllocatorT<StatsCalcFitnessSimpleOp,Beagle::StatsCalcFitnessSimpleOp::Alloc>
	Alloc;
	//! StatsCalcFitnessSimpleOp handle type.
	typedef PointerT<StatsCalcFitnessSimpleOp,Beagle::StatsCalcFitnessSimpleOp::Handle>
	Handle;
	//! StatsCalcFitnessSimpleOp bag type.
	typedef ContainerT<StatsCalcFitnessSimpleOp,Beagle::StatsCalcFitnessSimpleOp::Bag>
	Bag;

	explicit StatsCalcFitnessSimpleOp(std::string inName="GP-StatsCalcFitnessSimpleOp");
	virtual ~StatsCalcFitnessSimpleOp()
	{ }

	virtual void calculateStatsDeme(Beagle::Stats& outStats,
	                                Beagle::Deme& ioDeme,
	                                Beagle::Context& ioContext) const;

};

}
}

#endif // Beagle_GP_StatsCalcFitnessSimpleOp_hpp
