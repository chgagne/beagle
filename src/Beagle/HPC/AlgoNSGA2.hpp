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
 *  \file   beagle/HPC/AlgoNSGA2.hpp
 *  \brief  Definition of the class AlgoNSGA2.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_HPC_AlgoNSGA2_hpp
#define Beagle_HPC_AlgoNSGA2_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Algorithm.hpp"


namespace Beagle
{
namespace HPC
{
/*!
 *  \class AlgoNSGA2 beagle/HPC/AlgoNSGA2.hpp "beagle/HPC/AlgoNSGA2.hpp"
 *  \brief Distributed NSGA2 algorithm class
 *  \ingroup ECF
 *  \ingroup HPC
 *  This algorithm is the distributed equivalent of the Beagle NSGA2.
 *  Instead of using the NSGA2 operator as a breeder tree node, it is
 *  used a simple operator. The population size is doubled by using the
 *  OversizeOp.
 *
 *  The algorithm is divided in two phase, each phase has three parts.
 *  The first phase is the bootstrap. Dclass  	AlgoNSGA2uring that phase, the Supervisor
 *  initializes and distributed the demes. The Evolver receives the demes,
 *  send the individuals to its evaluators. The Evaluator receive the
 *  individuals, evaluates them, than sends back their fitness to the
 *  Evolver The Evolver receive the fitness, and then send the deme back
 *  to the Supervisor. The Supervisor then calculates the statistics, and
 *  write the first milestone.
 *
 *  The second phase is the mainloop. This phase is exactly as the
 *  bootstrap phase, except the Supervisor is only waiting to receive demes
 *  from its evolvers. The Evolver doubles the deme population size and then
 *  performs crossover and mutation before sending the
 *  individuals to its evaluators. The Evaluator loop remains the same.
 *  Once the Evolver has received the fitnesses, it executes the NSGA2Op that
 *  selects which individual will be kept for the next generation, based
 *  on the crowding distance measure along the Pareto Front.
 */
class AlgoNSGA2 : public Algorithm
{

public:

	//! AlgoNSGA2 allocator type.
	typedef AllocatorT< AlgoNSGA2, Algorithm::Alloc > Alloc;
	//! AlgoNSGA2 handle type.
	typedef PointerT< AlgoNSGA2, Algorithm::Handle > Handle;
	//! AlgoNSGA2 bag type.
	typedef ContainerT< AlgoNSGA2, Algorithm::Bag > Bag;

	AlgoNSGA2(void);
	//! AlgoNSGA2 destructor.
	virtual ~AlgoNSGA2(){ }

	virtual void configure(Evolver& ioEvolver, System& ioSystem);
};
}
}

#endif // Beagle_HPC_AlgoNSGA2_hpp
