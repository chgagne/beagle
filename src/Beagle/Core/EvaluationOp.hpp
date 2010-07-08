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
 *  \file   Beagle/Core/EvaluationOp.hpp
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

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AbstractAllocT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Operator.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/System.hpp"
#include "Beagle/Core/Context.hpp"
#include "Beagle/Core/Logger.hpp"
#include "Beagle/Core/BreederOp.hpp"
#include "Beagle/Core/Double.hpp"

namespace Beagle
{

/*!
 *  \class EvaluationOp Beagle/Core/EvaluationOp.hpp "Beagle/Core/EvaluationOp.hpp"
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
