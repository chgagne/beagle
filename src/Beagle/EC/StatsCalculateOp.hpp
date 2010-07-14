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
 *  \file   Beagle/EC/StatsCalculateOp.hpp
 *  \brief  Definition of the class StatsCalculateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_StatsCalculateOp_hpp
#define Beagle_EC_StatsCalculateOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{
	
namespace EC
{


/*!
 *  \class StatsCalculateOp Beagle/EC/StatsCalculateOp.hpp "Beagle/EC/StatsCalculateOp.hpp"
 *  \brief Calculate statistics of a deme, for a generation, operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup FitStats
 */
class StatsCalculateOp : public Beagle::Operator
{

public:

	//! StatsCalculateOp allocator type
	typedef Beagle::AbstractAllocT<StatsCalculateOp,Beagle::Operator::Alloc> Alloc;
	//! StatsCalculateOp handle type.
	typedef Beagle::PointerT<StatsCalculateOp,Beagle::Operator::Handle> Handle;
	//! StatsCalculateOp bag type.
	typedef Beagle::ContainerT<StatsCalculateOp,Beagle::Operator::Bag> Bag;

	explicit StatsCalculateOp(std::string inName="StatsCalculateOp");
	virtual ~StatsCalculateOp()
	{ }

	/*!
	 *  \brief  Calculate statistics of a given deme.
	 *  \param  outStats Computed stats of the deme.
	 *  \param  ioDeme Deme to evaluate the statistics.
	 *  \param  ioContext Context of the evolution.
	 *  \return Handle to the generational statistics of the given deme.
	 */
	virtual void calculateStatsDeme(Beagle::Stats& outStats,
	                                Beagle::Deme& ioDeme,
	                                Beagle::Context& ioContext) const =0;

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void calculateStatsVivarium(Beagle::Stats& outStats,
	                                    Beagle::Vivarium& ioVivarium,
	                                    Beagle::Context& ioContext) const;

protected:

	Beagle::UIntArray::Handle  mPopSize;              //!< Population size for the evolution.
	unsigned int               mNbDemesCalculated;    //!< Number of demes for which stats is computed.
	unsigned int               mGenerationCalculated; //!< Actual generation for which stats is computed.

};

}

}

#endif // Beagle_EC_StatsCalculateOp_hpp
