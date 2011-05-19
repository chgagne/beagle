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
 *  \file   Beagle/EC/SelectBestOp.cpp
 *  \brief  Source code of class SelectBestOp.
 *  \author Matthew Walker
 *  $Revision: 1.8 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


namespace
{

class TempPoolPredicate
{
public:
	TempPoolPredicate()
	{ }
	~TempPoolPredicate()
	{ }

	/*!
	 *  \brief Test the ordering of two pointed Objects.
	 *  \param inLeftPtr Pointer to the left object to compare.
	 *  \param inRightPtr Pointer to the right object to compare.
	 */
	inline bool operator()(const std::pair<unsigned int, Pointer>& inLeft,
	                       const std::pair<unsigned int, Pointer>& inRight) const {
		Beagle_StackTraceBeginM();
		if( (!inLeft.second) || (!inRight.second) ) return false;
		return inLeft.second->isLess(*inRight.second);
		Beagle_StackTraceEndM();
	}

};

}


/*!
 *  \brief Construct a selection best individuals operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the selection operator.
 */
SelectBestOp::SelectBestOp(std::string inReproProbaName, std::string inName) :
	Beagle::EC::SelectionOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Select inN best individuals.
 *  \param inN Number of individuals to select.
 *  \param ioPool Pool from which the individuals are choosen.
 *  \param ioContext Evolutionary context.
 *  \param outSelections Vector of unsigned ints that say how often an individual was selected.
 *
 *  The output of this method is via outSelection.  It produces a
 *  vector the same size as ioPool, where each index says how many
 *  times an individual was selected.  If this is not the desired
 *  output, consider using the method convertToList().
 */
void SelectBestOp::selectManyIndividuals(unsigned int inN,
        Individual::Bag& ioPool,
        Context& ioContext,
        std::vector<unsigned int>& outSelections)
{
	Beagle_StackTraceBeginM();

	// Check that we're not selecting all the individuals
	if((inN%ioPool.size()) == 0) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    std::string("Warning!  Selecting the best ")+uint2str(inN)+" individuals from a pool size of "+
		    uint2str(ioPool.size())+" (during SelectBestOp) means that every individual will be selected "+
		    uint2str(inN/ioPool.size())+" times, thus applying no selective pressure."
		);
	}

	// Copy the pool, pairing indices and handles to individuals
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "Copying pool's individuals and pairing them to their indices"
	);
	typedef std::vector< std::pair< unsigned int, Individual::Handle > > lTempPoolType;
	lTempPoolType lTempPool;
	lTempPool.resize(ioPool.size());
	unsigned int lIndex = 0;
	for(lTempPoolType::iterator lItr=lTempPool.begin(); lItr!=lTempPool.end(); ++lItr) {
		lItr->first  = lIndex;
		lItr->second = ioPool[lIndex++];
	}
	Beagle_AssertM(ioPool.size() == lTempPool.size());

	// Sort population
	if(inN > ioPool.size()) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    std::string("Warning sorting entire copy of pool because number to select (")+uint2str(inN)+
		    ") is greater than size of pool ("+uint2str(ioPool.size())+")"
		);
		std::sort(lTempPool.begin(),lTempPool.end(), TempPoolPredicate());
	} else {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    std::string("Partial sorting top ")+uint2str(inN)+" individuals in copy of pool"
		);
		std::partial_sort(lTempPool.begin(), lTempPool.begin()+inN, lTempPool.end(), TempPoolPredicate());
	}

	// Write to the selection table
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "Writing selection table"
	);
	outSelections.clear();
	outSelections.resize(lTempPool.size());
	for (unsigned int i=0; i<inN; i++) {
		unsigned int lSelection = lTempPool[i%lTempPool.size()].first;
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    std::string("Selecting ")+uint2ordinal(i+1)+" best ("+uint2ordinal(lSelection+1)+" individual)"
		);
		++(outSelections[lSelection]);
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Select best individual of a pool of individuals.
 *  \param ioPool Pool of individuals to use for selection.
 *  \param ioContext Evolutionary context.
 */
unsigned int SelectBestOp::selectOneIndividual(Individual::Bag& ioPool, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	unsigned int lBestIndex = 0;
	for(unsigned int i=1; i<ioPool.size(); ++i) {
		if(ioPool[lBestIndex]->isLess(*ioPool[i])) {
			lBestIndex = i;
		}
	}
	return lBestIndex;
	Beagle_StackTraceEndM();
}
