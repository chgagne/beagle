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
 *  \file   TSPDistanceMap.hpp
 *  \brief  Definition of the type TSPDistanceMap.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.1 $
 *  $Date: 2007/08/09 21:43:11 $
 */


#ifndef TSPDistanceMap_hpp
#define TSPDistanceMap_hpp

#include "Beagle/EC.hpp"
#include <vector>


/*!
 *  \class TSPDistanceMap TSPDistanceMap.hpp "TSPDistanceMap.hpp"
 *  \brief The map containing distances between towns for the TSP problem.
 *  \ingroup TSP
 */
class TSPDistanceMap : public Beagle::Component
{

public:

	//! TSPDistanceMap allocator type.
	typedef Beagle::AllocatorT<TSPDistanceMap,Beagle::Component::Alloc>
	Alloc;
	//!< TSPDistanceMap handle type.
	typedef Beagle::PointerT<TSPDistanceMap,Beagle::Component::Handle>
	Handle;
	//!< TSPDistanceMap bag type.
	typedef Beagle::ContainerT<TSPDistanceMap,Beagle::Component::Bag>
	Bag;

	explicit TSPDistanceMap(unsigned int inNumberOfTowns=25);

	virtual void init(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void registerParams(Beagle::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Get distance between two towns in the map.
	 *  \param inIndexFirstTown Index of first town in the map.
	 *  \param inIndexSecondTown Index of second town in the map.
	 *  \return Distance between the two towns.
	 */
	inline double getDistance(unsigned int inIndexFirstTown, unsigned int inIndexSecondTown) const
	{
		Beagle_StackTraceBeginM();
		Beagle_AssertM(mDistancesMatrix.getCols() != 0);
		Beagle_AssertM(mDistancesMatrix.getRows() != 0);
		Beagle_AssertM(inIndexFirstTown < mNumberOfTowns->getWrappedValue());
		Beagle_AssertM(inIndexSecondTown < mNumberOfTowns->getWrappedValue());
		return mDistancesMatrix(inIndexFirstTown, inIndexSecondTown);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of towns in the map.
	 */
	inline unsigned int getNumberOfTowns() const
	{
		Beagle_StackTraceBeginM();
		return mNumberOfTowns->getWrappedValue();
		Beagle_StackTraceEndM();
	}

protected:

	PACC::Matrix         mDistancesMatrix;  //!< Distances between nodes of the graph.
	Beagle::UInt::Handle mNumberOfTowns;    //!< Number of towns of the problem.

};

#endif // TSPDistanceMap_hpp
