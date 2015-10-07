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
 *  \file   TSPDistanceMap.cpp
 *  \brief  Implementation of the class TSPDistanceMap.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.1 $
 *  $Date: 2007/08/09 21:43:11 $
 */

#include "Beagle/EC.hpp"
#include "TSPDistanceMap.hpp"

#include <cmath>

using namespace Beagle;


/*!
 *  \brief Construct a distance map for the TSP problem.
 *  \param inNumberOfTowns Number of towns used in the TSP problem.
 */
TSPDistanceMap::TSPDistanceMap(unsigned int inNumberOfTowns) :
		Component("TSPDistanceMap"),
		mNumberOfTowns(new UInt(inNumberOfTowns))
{ }


/*!
 *  \brief Initialize distance map.
 *  \param ioSystem Evolutionary system to initialize distance map.
 */
void TSPDistanceMap::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// The distance map is already initialized, don't touch it.
	if((mDistancesMatrix.getRows()!=0) && (mDistancesMatrix.getCols()!=0)) return;

	// Resize the distance map to the size given by parameter 'tsp.map.size'.
	mDistancesMatrix.resize(mNumberOfTowns->getWrappedValue(),
	        mNumberOfTowns->getWrappedValue());

	// Generating randomly the TSP graph to solve.
	std::vector< std::pair<double,double> > lNodePosition(mNumberOfTowns->getWrappedValue());
	for(unsigned int i=0; i<mNumberOfTowns->getWrappedValue(); ++i) {
		lNodePosition[i].first  = ioSystem.getRandomizer().rollUniform(0.0, 10.0);
		lNodePosition[i].second = ioSystem.getRandomizer().rollUniform(0.0, 10.0);
	}
	
	// Computing distance (simply Euclidean distances) between the nodes.
	for(unsigned int i=0; i<mNumberOfTowns->getWrappedValue(); ++i) {
		for(unsigned int j=(i+1); j<mNumberOfTowns->getWrappedValue(); ++j) {
			double lDistance = ((lNodePosition[j].first-lNodePosition[i].first) *
				                (lNodePosition[j].first-lNodePosition[i].first)) +
				               ((lNodePosition[j].second-lNodePosition[i].second) *
				                (lNodePosition[j].second-lNodePosition[i].second));
			lDistance = std::sqrt(lDistance);
			lDistance += ioSystem.getRandomizer().rollGaussian(0.0, 0.5);       // Add some noise
			mDistancesMatrix(i,j) = mDistancesMatrix(j,i) = lDistance;
		}
	}

	// Set distance to self to 0.
	for(unsigned int i=0; i<mNumberOfTowns->getWrappedValue(); ++i) {
		mDistancesMatrix(i,i) = 0.;
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read distance map.
 *  \param inIter XML iterator to use to read the distance map.
 *  \param ioSystem Evolutionary system.
 */
void TSPDistanceMap::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "TSPDistanceMap"))
		throw Beagle_IOExceptionNodeM(*inIter, "invalid node type or tag name!");
	for(PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; ++lChild)
		if(lChild->getValue() == "TSPDistanceMatrix")
			mDistancesMatrix.read(lChild);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register parameters of the distance map component.
 *  \param ioSystem Evolutionary system.
 */
void TSPDistanceMap::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Component::registerParams(ioSystem);
	const unsigned int lNumberOfTowns = mNumberOfTowns->getWrappedValue();
	Register::Description lDescription(
	    "Number of towns of TSP problem",
	    "UInt",
	    uint2str(lNumberOfTowns),
	    std::string("Integer giving the number of town of the TSP problem, that is the number of ")+
	    std::string("nodes in the graph in which the optimization is done.")
	);
	mNumberOfTowns = castHandleT<UInt>(
	        ioSystem.getRegister().insertEntry("tsp.map.size", new UInt(lNumberOfTowns), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write distance map into XML streamer.
 *  \param ioStreamer XML streamer.
 *  \param inIndent Indent (or not) XML output.
 */
void TSPDistanceMap::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	mDistancesMatrix.write(ioStreamer, "TSPDistanceMatrix");
	Beagle_StackTraceEndM();
}
