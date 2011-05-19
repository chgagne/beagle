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
 *  \file   beagle/FitnessMultiObj.hpp
 *  \brief  Definition of the class FitnessMultiObj.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:12 $
 */

/*!
 *  \defgroup MultiObj Multiobjective Evolutionary Algorithms
 *  \ingroup ECF
 *  \brief Multiobjective fitness and related operators, part of the generic EC framework.
 *
 *  The building blocks of the Open BEAGLE's multiobjective EA (MOEA) facilities is found
 *  in the general Fitness class, where several methods are defined to measure some properties
 *  useful for multiobjective optimization. The most important is probably method
 *  Fitness::isDominated, which measure whether an fitness value dominates (following the
 *  Pareto criterion) another fitness. Other pertinent methods includes Fitness::getDistance
 *  and Fitness::getObjective, which are used for some crowding evaluation by the different
 *  operators related to MOEA.
 *
 *  A general multiobjective fitness measure is defined as class FitnessMultiObj. This fitness
 *  measure is usable for most MOEA applications. It is made of a STL vector of
 *  floats, where each value of the vector is associated to one optimization objective.
 *  Class FitnessMultiObj appropriately redefines the method isDominated. By default, the
 *  domination is evaluated on the basis that all objectives must be maximized. The minimization
 *  equivalent is defined as class FitnessMultiObjMin. For problems with mixed
 *  maximization/minization objectives, a specific fitness classes inheriting from
 *  FitnessMultiObj must be defined by the user, with the method isDominated properly overdefined.
 *  For more exotic applications, it is also possible to define a custom fitness measure derived
 *  from the base Fitness class, compatible with Pareto-based selection, by properly redefining
 *  the getDistance, getObjective, getNumberOfObjectives, and isDominated methods. A MOEA statistics
 *  computation operator compatible with base class Fitness is defined as class
 *  StatsCalcFitnessMultiObjOp.
 *
 *  MOEA method NSGA2 is defined by operator NSGA2Op. This operator can only be used as a
 *  replacement strategy in a breeder tree. In addition to the domination evaluating
 *  method defined in isDominated, the operator uses methods getObjective and
 *  getNumberOfObjectives to to compute the crowding distances.
 *
 *  In the context of MOEA, the hall-of-fame is not meaningful, as there is not strict relation
 *  order between the solutions found out of the Pareto ranking. The hall-of-fame sizes
 *  must then be set to 0. For a specific problem where the use of hall-of-fames make some sense,
 *  the method Fitness::isLess must be overdefined accordingly. In class FitnessMultiObj, relation
 *  ordering is based on the lexicographic ranking starting with the first objective value.
 *
 */

#ifndef Beagle_FitnessMultiObj_hpp
#define Beagle_FitnessMultiObj_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Fitness.hpp"

namespace Beagle
{

/*!
 *  \class FitnessMultiObj beagle/FitnessMultiObj.hpp "beagle/FitnessMultiObj.hpp"
 *  \brief Multiobjective fitness measure class.
 *  \ingroup ECF
 *  \ingroup FitStats
 *  \ingroup MultiObj
 */
class FitnessMultiObj : public Fitness, public std::vector<double>
{

public:

	//! FitnessMultiObj allocator type.
	typedef AllocatorT<FitnessMultiObj,Fitness::Alloc> Alloc;
	//! FitnessMultiObj handle type.
	typedef PointerT<FitnessMultiObj,Fitness::Handle> Handle;
	//! FitnessMultiObj bag type.
	typedef ContainerT<FitnessMultiObj,Fitness::Bag> Bag;

	explicit FitnessMultiObj(unsigned int inSize=0, double inValue=0.f);
	explicit FitnessMultiObj(const std::vector<double>& inFitness);
	virtual ~FitnessMultiObj()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual double              getDistance(const Fitness& inRightFitness) const;
	virtual unsigned int        getNumberOfObjectives() const;
	virtual double              getObjective(unsigned int inObjIndex) const;
	virtual const std::string&  getType() const;
	virtual bool                isDominated(const Fitness& inRightFitness) const;
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual bool                isLess(const Object& inRightObj) const;
	virtual void                read(PACC::XML::ConstIterator inIter);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_FitnessMultiObj_hpp
