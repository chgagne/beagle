/*
 *  Artifical ant (ant)
 *  Copyright (C) 2006 by Christian Gagne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   AntEvalOp.hpp
 *  \brief  Definition of the type AntEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/09 21:43:12 $
 */

/*!
 *  \defgroup Ant Artificial Ant Example
 *  \brief Artificial ant (ant).
 *
 *  \par Objective
 *  Find a program that produces to control an artifical ant so that it can
 *  eat all the food located in a given environement.
 *
 *  \par Terminal set
 *  - L: Turn ant 90 degrees counter-clockwize
 *  - R: Turn ant 90 degrees clockwize
 *  - M: Move ant one case front
 *
 *  \par Function set
 *  - IFA: If there is food sensed on front case execute second argument, else
 *         execute third argument
 *  - P2: PROGN2 lisp function, execute first argument then second argument.
 *  - P3: PROGN3 lisp function, execute first, second, and then third arguments.
 *
 *  \par Fitness
 *  Number of pieces of food picked up before the ant times out with 400
 *  operations.
 *
 *  \par Stopping criteria
 *  When the evolution reaches the maximum number of generations or an ant
 *  picked up all the 89 pieces of food in one evaluation.
 *
 *  \par Reference
 *  John R. Koza, "Genetic Programming I: On the Programming of Computers by Means
 *  of Natural Selection", MIT Press, 1992, pages 147-161.
 *
 */


#ifndef AntEvalOp_hpp
#define AntEvalOp_hpp

#include "Beagle/GP.hpp"
#include "AntSimulator.hpp"

#include <string>
#include <vector>


/*!
 *  \class AntEvalOp AntEvalOp.hpp "AntEvalOp.hpp"
 *  \brief The individual evaluation class operator for the artificial ant problem.
 *  \ingroup Ant
 */
class AntEvalOp : public Beagle::GP::EvaluationOp
{

public:

	//! AntEvalOp allocator type.
	typedef Beagle::AllocatorT<AntEvalOp,Beagle::GP::EvaluationOp::Alloc>
	Alloc;
	//!< AntEvalOp handle type.
	typedef Beagle::PointerT<AntEvalOp,Beagle::GP::EvaluationOp::Handle>
	Handle;
	//!< AntEvalOp bag type.
	typedef Beagle::ContainerT<AntEvalOp,Beagle::GP::EvaluationOp::Bag>
	Bag;

	explicit AntEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
	        Beagle::GP::Context& ioContext);
	virtual void init(Beagle::System& ioSystem);
	virtual void registerParams(Beagle::System& ioSystem);

	/*!
	 *  \return Constant reference to ant simulator.
	 */
	inline AntSimulator& getSimulator()
	{
		return *mSimulator;
	}

	/*!
	 *  \brief Set simulator to use in the fitness evaluation operator.
	 *  \param inSimulator Simulator to set.
	 */
	inline void setSimulator(AntSimulator::Handle inSimulator)
	{
		mSimulator = inSimulator;
	}

protected:

	AntSimulator::Handle   mSimulator;      //!< Ant simulator used to evaluate the fitnesses.
	Beagle::String::Handle mTrailFileName;  //!< File from which the simulator is initialized.


};

#endif // AntEvalOp_hpp
