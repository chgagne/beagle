/*
 *  Traveling Salesman Problem (tsp):
 *  Indices integer vector GA example
 *
 *  Copyright (C) 2005-2006 by Christian Gagne
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
 *  \file   TSPEvalOp.hpp
 *  \brief  Definition of the type TSPEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.7 $
 *  $Date: 2007/08/09 21:43:11 $
 */

/*!
 *  \defgroup TSP Traveling Salesman Problem Example
 *  \brief Traveling Salesman Problem (tsp): Indices integer vector GA example
 *
 *  \par Objective
 *  Find a the shortest path to visit all the nodes of a randomly generated graph.
 *
 *  \par Representation
 *  Order in which the nodes of the graph are visited. The integers in the vector represent the
 *  indices of the nodes to visit.
 *
 *  \par Fitness
 *  Sum of the inter-node distance given the path represented by an integer vector.
 *
 */

#ifndef TSPEvalOp_hpp
#define TSPEvalOp_hpp

#include "Beagle/EC.hpp"
#include "TSPDistanceMap.hpp"


/*!
 *  \class TSPEvalOp TSPEvalOp.hpp "TSPEvalOp.hpp"
 *  \brief The individual evaluation class operator for the TSP problem.
 *  \ingroup TSP
 */
class TSPEvalOp : public Beagle::EvaluationOp
{

public:

	//! TSPEvalOp allocator type.
	typedef Beagle::AllocatorT<TSPEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< TSPEvalOp handle type.
	typedef Beagle::PointerT<TSPEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< TSPEvalOp bag type.
	typedef Beagle::ContainerT<TSPEvalOp,Beagle::EvaluationOp::Bag>
	Bag;

	TSPEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
	        Beagle::Context& ioContext);
	virtual void registerParams(Beagle::System& ioSystem);
	virtual void init(Beagle::System& ioSystem);

protected:

	TSPDistanceMap::Handle mDistanceMap;    //!< Distance map for TSP evaluation.
	Beagle::UInt::Handle   mIntVectorSize;  //!< Individual integer vectors size.

};

#endif // TSPEvalOp_hpp
