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
 *  \file   beagle/Core/EvaluationOp.hpp
 *  \brief  Definition of the class EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_EvaluationOp_hpp
#define Beagle_Core_EvaluationOp_hpp

#include <string>
#include <iostream>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AbstractAllocT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Operator.hpp"
#include "beagle/Core/UInt.hpp"
#include "beagle/Core/System.hpp"
#include "beagle/Core/Context.hpp"
#include "beagle/Core/Logger.hpp"
#include "beagle/Core/BreederOp.hpp"
#include "beagle/Core/Double.hpp"

namespace Beagle
{

/*!
 *  \class EvaluationOp beagle/Core/EvaluationOp.hpp "beagle/Core/EvaluationOp.hpp"
 *  \brief Abstract evaluation operator class.
 *  \ingroup Core
 *  \ingroup Op
 *  \ingroup FitStats
 */
class EvaluationOp : public BreederOp
{

public:

	//! EvaluationOp allocator type.
	typedef AbstractAllocT<EvaluationOp,BreederOp::Alloc>
	Alloc;
	//! EvaluationOp handle type.
	typedef PointerT<EvaluationOp,BreederOp::Handle>
	Handle;
	//! EvaluationOp bag type.
	typedef ContainerT<EvaluationOp,BreederOp::Bag>
	Bag;

	explicit EvaluationOp(std::string inName="EvaluationOp");
	virtual ~EvaluationOp()
	{ }

	/*!
	 *  \brief Evaluate the fitness of the given individual.
	 *  \param inIndividual Current individual to evaluate.
	 *  \param ioContext Evolutionary context.
	 *  \return Handle to the fitness value of the individual.
	 */
	virtual Fitness::Handle evaluate(Individual& inIndividual, Context& ioContext) = 0;

	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual double             getBreedingProba(BreederNode::Handle inChild);
	virtual void               registerParams(System& ioSystem);
	virtual void               operate(Deme& ioDeme, Context& ioContext);
	virtual Fitness::Handle    test(Individual::Handle inIndividual, System::Handle ioSystem);

protected:

	virtual void prepareStats(Deme& ioDeme, Context& ioContext);
	virtual void updateHallOfFameWithDeme(Deme& ioDeme, Context& ioContext);
	virtual void updateHallOfFameWithIndividual(Individual& ioIndividual, Context& ioContext);
	virtual void updateStats(unsigned int inNumProcessed, Context& ioContext);

	UInt::Handle mVivaHOFSize;
	UInt::Handle mDemeHOFSize;

};

}

#endif // Beagle_Core_EvaluationOp_hpp
