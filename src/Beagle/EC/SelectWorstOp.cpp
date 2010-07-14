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
 *  \file   Beagle/EC/SelectWorstOp.cpp
 *  \brief  Source code of class SelectWorstOp.
 *  \author Matthew Walker
 *  $Revision: 1.2 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


namespace
{

class TempPoolInvPredicate
{
public:
	TempPoolInvPredicate()
	{ }
	~TempPoolInvPredicate()
	{ }

	/*!
	 *  \brief Test the inverse ordering of two pointed Objects.
	 *  \param inLeftPtr Pointer to the left object to compare.
	 *  \param inRightPtr Pointer to the right object to compare.
	 */
	inline bool operator()(const std::pair<unsigned int, Pointer>& inLeft,
	                       const std::pair<unsigned int, Pointer>& inRight) const
	{
		Beagle_StackTraceBeginM();
		if( (!inLeft.second) || (!inRight.second) ) return false;
		return inRight.second->isLess(*inLeft.second);
		Beagle_StackTraceEndM("bool TempPoolPredicate::operator()(const std::pair<unsigned int,Pointer&>,const std::pair<unsigned int, Pointer&>) const");
	}

};

}


/*!
 *  \brief Construct a selection worst individuals operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the selection operator.
 */
SelectWorstOp::SelectWorstOp(std::string inReproProbaName, std::string inName) :
		SelectionOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Select worst individual of a pool of individuals.
 *  \param ioPool Pool of individuals to use for selection.
 *  \param ioContext Evolutionary context.
 */
unsigned int SelectWorstOp::selectOneIndividual(Individual::Bag& ioPool, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	unsigned int lWorstIndex = 0;
	for(unsigned int i=1; i<ioPool.size(); ++i) {
		if(ioPool[i]->isLess(*ioPool[lWorstIndex])) {
			lWorstIndex = i;
		}
	}
	return lWorstIndex;
	Beagle_StackTraceEndM("unsigned int SelectWorstOp::selectIndividual(Individual::Bag&,Context&)");
}


/*!
 *  \brief Select the worst individuals.
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
void SelectWorstOp::selectManyIndividuals(unsigned int inN,
                                          Individual::Bag& ioPool,
                                          Context& ioContext,
                                          std::vector<unsigned int>& outSelections)
{
	Beagle_StackTraceBeginM();

	// Check that we're not selecting all the individuals
	if((inN%ioPool.size()) == 0) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "selection", "Beagle::SelectWorstOp",
		    std::string("Warning!  Selecting the worst ")+uint2str(inN)+" individuals from a pool size of "+
		    uint2str(ioPool.size())+" (during SelectWorstOp) means that every individual will be selected "+
		    uint2str(inN/ioPool.size())+" times, thus applying no selective pressure."
		);
	}

	// Copy the pool, pairing indices and handles to individuals
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "selection", "Beagle::SelectWorstOp",
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
		    "selection", "Beagle::SelectWorstOp",
		    std::string("Warning sorting entire copy of pool because number to select (")+uint2str(inN)+
		    ") is greater than size of pool ("+uint2str(ioPool.size())+")"
		);
		std::sort(lTempPool.begin(),lTempPool.end(), TempPoolInvPredicate());
	} else {
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "selection", "Beagle::SelectWorstOp",
		    std::string("Partial sorting top ")+uint2str(inN)+" individuals in copy of pool"
		);
		std::partial_sort(lTempPool.begin(), lTempPool.begin()+inN, lTempPool.end(), TempPoolInvPredicate());
	}

	// Write to the selection table
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "selection", "Beagle::SelectWorstOp",
	    "Writing selection table"
	);
	outSelections.clear();
	outSelections.resize(lTempPool.size());
	for (unsigned int i=0; i<inN; i++) {
		unsigned int lSelection = lTempPool[i%lTempPool.size()].first;
		Beagle_LogDebugM(
		    ioContext.getSystem().getLogger(),
		    "selection", "Beagle::SelectWorstOp",
		    std::string("Selecting ")+uint2ordinal(i+1)+" worst ("+uint2ordinal(lSelection+1)+" individual)"
		);
		++(outSelections[lSelection]);
	}

	Beagle_StackTraceEndM("void SelectWorstOp::selectNIndividuals(unsigned int,Individual::Bag&,Context&,std::vector<unsigned int>&)");
}
