/*
 *  Multiobjective 0/1 Knapsack (knapsack):
 *  Multiobjective GA example
 *
 *  Copyright (C) 2003-2006 by Christian Gagne and Marc Parizeau
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
 *  \file   KnapsackEvalOp.hpp
 *  \brief  Definition of the type KnapsackEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/13 13:21:04 $
 */

/*!
 *  \defgroup Knapsack Multiobjective 0/1 Knapsack Example
 *  \brief Multiobjective 0/1 Knapsack (knapsack): Multiobjective GA example
 *
 *  \par Objective
 *  Find a combination of objects to put in a knapsack with a minimum weight but a maximum value.
 *
 *  \par Representation
 *  Bit strings made of 24 bits, where the ith bit designate whether the ith object is put in the
 *  bag or not. The value and weight of each objects is randomly generated at each runs.
 *
 *  \par Fitness
 *  First objective is the maximization of the knapsack's objects value. The second objective
 *  is the maximization of (maximum knapsack weight - objects weight), that is the minimization of
 *  the weight.
 *
 */

#ifndef KnapsackEvalOp_hpp
#define KnapsackEvalOp_hpp

#include "Beagle/EC.hpp"
#include "Beagle/HPC.hpp"

#include <vector>


/*!
 *  \class KnapsackEvalOp KnapsackEvalOp.hpp "KnapsackEvalOp.hpp"
 *  \brief The individual evaluation class operator for the Knapsack problem.
 *  \ingroup Knapsack
 */
class KnapsackEvalOp : public Beagle::HPC::EvaluationOp
{

public:

	//! KnapsackEvalOp allocator type.
	typedef Beagle::AllocatorT<KnapsackEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< KnapsackEvalOp handle type.
	typedef Beagle::PointerT<KnapsackEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< KnapsackEvalOp bag type.
	typedef Beagle::ContainerT<KnapsackEvalOp,Beagle::EvaluationOp::Bag>
	Bag;

	explicit KnapsackEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
	        Beagle::Context& ioContext);
	virtual void registerParams(Beagle::System& ioSystem);
	virtual void init(Beagle::System& ioSystem);

protected:
	Beagle::Vector::Handle mObjectValues;    //!< Knapsack objects value.
	Beagle::Vector::Handle mObjectWeights;   //!< Knapsack objects weight.
	Beagle::UInt::Handle   mIntVectorSize;   //!< Individual integer vectors size.
	float                  mSumWeight;       //!< Sum of objects weight that can be in knapsack.

};

#endif // KnapsackEvalOp_hpp
