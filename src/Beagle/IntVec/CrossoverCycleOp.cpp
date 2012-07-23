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
 *  \file   Beagle/IntVec/CrossoverCycleOp.cpp
 *  \brief  Source code of class IntVec::CrossoverCycleOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>

#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP)	// std::tr1::unordered_map
#include <tr1/unordered_map>
typedef std::tr1::unordered_map<int, int> CCXMap;
#elif defined(BEAGLE_HAVE_UNORDERED_MAP)		// std::unordered_map
#include <unordered_map>
typedef std::unordered_map<int, int> CCXMap;
#elif defined(BEAGLE_HAVE_STD_HASHMAP)			// std::hash_map
#include <hash_map>
typedef std::hash_map<int, int> CCXMap;
#elif defined(BEAGLE_HAVE_STDEXT_HASHMAP)		// stdext::hash_map
#include <hash_map>
typedef stdext::hash_map<int, int> CCXMap;
#elif defined(BEAGLE_HAVE_GNUCXX_HASHMAP)		// __gnu_cxx::hash_map
#include <ext/hash_map>
typedef __gnu_cxx::hash_map<int, int> CCXMap;
#else											// std::map
#include <map>
typedef std::map<int, int> CCXMap;
#endif


/*!
 *  \brief Construct a IntVec cycle crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::IntVec::CrossoverCycleOp::CrossoverCycleOp(std::string inMatingPbName,
                                                   std::string inName) :
	EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the IntVec cycle crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::IntVec::CrossoverCycleOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
			"Individual cycle crossover pb.",
			"Double",
			"0.3",
			"IntVec cycle crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lDescription));
	}
	CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mate two indices integer vector IntVec individuals for cycle crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::IntVec::CrossoverCycleOp::mate(Beagle::Individual& ioIndiv1,
                                            Beagle::Context&    ioContext1,
											Beagle::Individual& ioIndiv2,
											Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (before integer vector cycle crossover): " << ioIndiv1 << ", " << ioIndiv2
	);
	
	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		IntVec::IntegerVector::Handle lIndividual1 = castHandleT<IntegerVector>(ioIndiv1[i]);
		IntVec::IntegerVector::Handle lIndividual2 = castHandleT<IntegerVector>(ioIndiv2[i]);

		unsigned int lSize = minOf<unsigned int>(lIndividual1->size(), lIndividual2->size());
		CCXMap lParent1;
		for(unsigned int j = 0; j < lSize; ++j){
			lParent1.insert(std::make_pair((*lIndividual1)[j], j));
		}
		CCXMap lChild1;
		std::vector<int> lChild2(lSize);
		
		// i = 1, X'_i = X_i
		lChild1.insert(std::make_pair((*lIndividual1)[0], 0));
		lChild2[0] = (*lIndividual2)[0];
		// Search for Y_i in X
		CCXMap::const_iterator lParent1Iter = lParent1.find((*lIndividual2)[0]);
		// Continue until Y_i is present in X'_i (a cycle)
		while(lChild1.find(lParent1Iter->first) == lChild1.end()){
			// Insert Y_i at X_i
			lChild1.insert(std::make_pair(lParent1Iter->first, lParent1Iter->second));
			// The two childs are symetric
			lChild2[lParent1Iter->second] = (*lIndividual2)[lParent1Iter->second];
			// Search for Y_i in X, set i at the index of Y_i in X
			lParent1Iter = lParent1.find((*lIndividual2)[lParent1Iter->second]);
		}
		// Complete the childs by swapping what is not contained in the cycle
		CCXMap lChild1Copy(lChild1.begin(), lChild1.end());
		for(unsigned int j = 0; j < lSize; ++j){
			if(lChild1Copy.find((*lIndividual1)[j]) == lChild1Copy.end()){
				lChild1.insert(std::make_pair((*lIndividual2)[j], j));
				lChild2[j] = (*lIndividual1)[j];
			}
		}
		// Transfer childs in lIndividuals
		for(CCXMap::const_iterator lChildIter = lChild1.begin(); lChildIter != lChild1.end() ; ++lChildIter){
			(*lIndividual1)[lChildIter->second] = lChildIter->first;
			(*lIndividual2)[lChildIter->second] = lChild2[lChildIter->second];
		}
	}
	
	Beagle_LogDebugM(
		ioContext1.getSystem().getLogger(),
		"Individuals mated (after integer vector cycle crossover): " << ioIndiv1 << ", " << ioIndiv2;
	);
	
	return true;
	Beagle_StackTraceEndM();
}
