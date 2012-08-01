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
 *  \file   Beagle/IntVec/CrossoverIndicesOp.cpp
 *  \brief  Source code of class IntVec::CrossoverIndicesOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/IntVec.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a indices integer vector crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::IntVec::CrossoverIndicesOp::CrossoverIndicesOp(std::string inMatingPbName,
                                                       std::string inName) :
		EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the indices integer vector operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::IntVec::CrossoverIndicesOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Indices int. crossover prob.",
		    "Double",
		    "0.3",
		    "Indices integer vector crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lDescription));
	}
	EC::CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mate two IntVec individuals for indices integer vector crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::IntVec::CrossoverIndicesOp::mate(Beagle::Individual& ioIndiv1,
                                              Beagle::Context&    ioContext1,
                                              Beagle::Individual& ioIndiv2,
                                              Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes==0) return false;

	Beagle_LogDebugM(
	    ioContext1.getSystem().getLogger(),
	    "Individuals mated (before indices integer vector crossover): " << ioIndiv1 << ", " << ioIndiv2
	);

	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		IntVec::IntegerVector::Handle lIntVector1 = castHandleT<IntegerVector>(ioIndiv1[i]);
		std::deque<int> lIntVecCopy1(lIntVector1->begin(), lIntVector1->end());
		std::vector<bool> lSelected1(lIntVecCopy1.size(), false);
		IntVec::IntegerVector::Handle lIntVector2 = castHandleT<IntegerVector>(ioIndiv2[i]);
		std::deque<int> lIntVecCopy2(lIntVector2->begin(), lIntVector2->end());
		std::vector<bool> lSelected2(lIntVecCopy2.size(), false);
		const unsigned int lSumSize = lIntVector1->size() + lIntVector2->size();
		unsigned int lCount1=0;
		unsigned int lCount2=0;
		while((lIntVecCopy1.empty()==false) || (lIntVecCopy2.empty()==false)) {
			// Get next selected index.
			unsigned int lSelectedIndex=0;
			if(lIntVecCopy1.empty()) {
				lSelectedIndex=lIntVecCopy2.front();
				lIntVecCopy2.pop_front();
			} else if(lIntVecCopy2.empty()) {
				lSelectedIndex=lIntVecCopy1.front();
				lIntVecCopy1.pop_front();
			} else {
				if(ioContext1.getSystem().getRandomizer().rollInteger(0,lSumSize-1) >= lIntVector1->size()) {
					lSelectedIndex=lIntVecCopy2.front();
					lIntVecCopy2.pop_front();
				} else {
					lSelectedIndex=lIntVecCopy1.front();
					lIntVecCopy1.pop_front();
				}
			}
			// Insert selected index in appropriate integer vector.
			if((lSelectedIndex>=lIntVector1->size()) || lSelected1[lSelectedIndex]) {
				Beagle_AssertM(lSelectedIndex<lIntVector2->size());
				Beagle_AssertM(lSelected2[lSelectedIndex]==false);
				Beagle_AssertM(lCount2<lIntVector2->size());
				(*lIntVector2)[lCount2++] = lSelectedIndex;
				lSelected2[lSelectedIndex] = true;
			} else if((lSelectedIndex>=lIntVector2->size()) || lSelected2[lSelectedIndex]) {
				Beagle_AssertM(lCount1<lIntVector1->size());
				(*lIntVector1)[lCount1++] = lSelectedIndex;
				lSelected1[lSelectedIndex] = true;
			} else if(ioContext1.getSystem().getRandomizer().rollUniform(0.0,1.0) >= 0.5) {
				Beagle_AssertM(lCount1<lIntVector1->size());
				(*lIntVector1)[lCount1++] = lSelectedIndex;
				lSelected1[lSelectedIndex] = true;
			} else {
				Beagle_AssertM(lCount2<lIntVector2->size());
				(*lIntVector2)[lCount2++] = lSelectedIndex;
				lSelected2[lSelectedIndex] = true;
			}
		}
	}

	Beagle_LogDebugM(
	    ioContext1.getSystem().getLogger(),
	    "Individuals mated (after indices integer vector crossover): " << ioIndiv1 << ", " << ioIndiv2
	);

	return true;
	Beagle_StackTraceEndM();
}
