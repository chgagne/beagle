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
 *  \file   beagle/ParetoFrontHOF.cpp
 *  \brief  Source code of class ParetoFrontHOF.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision:  $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a pareto front hall-of-fame.
 *  \param inN Initial number of elements in hall-of-fame.
 *  \param inModel Model of elements in the hall-of-fame.
 */
ParetoFrontHOF::ParetoFrontHOF(unsigned int inN, const HallOfFame::Entry& inModel) :
		HallOfFame(inN, inModel)
{ }


/*!
 *  \brief Get exact type of the hall-of-fame.
 *  \return Exact type of the hall-of-fame.
 */
const std::string& ParetoFrontHOF::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("ParetoFrontHOF");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Update the Pareto front hall-of-fame with the current deme's individuals.
 *  \param inSizeHOF Value of 0 mean no Pareto front hall-of-fame, otherwize ignored.
 *  \param inDeme Deme to add to the hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the Pareto front has been modified, false if not.
 */
bool ParetoFrontHOF::updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	if(inSizeHOF == 0) {
		if(mEntries.empty()) return false;
		mEntries.clear();
		return true;
	}

	unsigned int lIndividualsAdded=0;
	unsigned int lIndividualsRemoved=0;

	for(unsigned int i=0; i<inDeme.size(); ++i) {
		bool lIsDominated=false;
		Fitness::Handle lIndivFitness = inDeme[i]->getFitness();
		for(unsigned int j=0; j<mEntries.size(); ++j) {
			Fitness::Handle lEntryFitness = mEntries[j].mIndividual->getFitness();
			if(lIndivFitness->isDominated(*lEntryFitness)) {
				lIsDominated=true;
				break;
			} else if(lEntryFitness->isDominated(*lIndivFitness)) {
				Beagle_LogDebugM(ioContext.getSystem().getLogger(), *mEntries[j].mIndividual);
				mEntries.erase(mEntries.begin()+j);
				--j;
				++lIndividualsRemoved;
			}
		}

		if(lIsDominated==false) {
			// Test if the individual is unique, and if so add it to the Pareto front.
			bool lIsIdentical = false;
			for(unsigned int k=0; k<mEntries.size(); ++k) {
				if(inDeme[i]->isIdentical(*mEntries[k].mIndividual)) {
					lIsIdentical = true;
					break;
				}
			}
			if(lIsIdentical == false) {
				const Factory& lFactory = ioContext.getSystem().getFactory();
				const std::string& lIndividualType = inDeme[i]->getType();
				Individual::Alloc::Handle lIndividualAlloc =
					castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
				if(lIndividualAlloc == NULL) {
					std::ostringstream lOSS;
					lOSS << "Type '" << lIndividualType << "' associated to individual to add to hall-of-fame ";
					lOSS << "is not valid!";
					throw Beagle_RunTimeExceptionM(lOSS.str());
				}
				Individual::Handle lIndivCopy = castHandleT<Individual>(lIndividualAlloc->allocate());
				lIndivCopy->copy(*inDeme[i], ioContext.getSystem());
				HallOfFame::Entry lEntry(lIndivCopy, ioContext.getGeneration(), ioContext.getDemeIndex());
				mEntries.push_back(lEntry);
				Beagle_LogDebugM(ioContext.getSystem().getLogger(), *inDeme[i]);
				++lIndividualsAdded;
			}
		}
	}

	if(lIndividualsAdded != 0) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "There are " << lIndividualsAdded << " individuals added to the Pareto front"
		);
	}

	if(lIndividualsRemoved != 0) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "There are " << lIndividualsRemoved << " individuals removed from the Pareto front"
		);
	}

	// Return if Pareto front has changed.
	return (lIndividualsAdded == 0) && (lIndividualsRemoved == 0);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if an individual should be added to the Pareto front hall-of-fame, and add it if
 *    necessary.
 *  \param inSizeHOF Value of 0 mean no Pareto front hall-of-fame, otherwize ignored.
 *  \param inIndividual Individual that is checked for addition to the Pareto front hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the Pareto front had been modified, false if not.
 */
bool ParetoFrontHOF::updateWithIndividual(unsigned int inSizeHOF,
        const Individual& inIndividual,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();

	if(inSizeHOF == 0) {
		if(mEntries.empty()) return false;
		mEntries.clear();
		return true;
	}

	bool lParetoFrontModified=false;
	bool lIsDominated=false;
	Fitness::Handle lIndivFitness = inIndividual.getFitness();
	for(unsigned int i=0; i<mEntries.size(); ++i) {
		Fitness::Handle lEntryFitness = mEntries[i].mIndividual->getFitness();
		if(lIndivFitness->isDominated(*lEntryFitness)) {
			lIsDominated=true;
			break;
		} else if(lEntryFitness->isDominated(*lIndivFitness)) {
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), *mEntries[i].mIndividual);
			mEntries.erase(mEntries.begin()+i);
			--i;
			lParetoFrontModified = true;
		}
	}
	if(lIsDominated==false) {
		// Test if the individual is unique, and if so add it to the Pareto front.
		bool lIsIdentical = false;
		for(unsigned int j=0; j<mEntries.size(); ++j) {
			if(inIndividual.isIdentical(*mEntries[j].mIndividual)) {
				lIsIdentical = true;
				break;
			}
		}
		if(lIsIdentical == false) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			std::string lIndividualType = inIndividual.getType();
			Individual::Alloc::Handle lIndividualAlloc =
				castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
			if(lIndividualAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "Type '" << lIndividualType << "' associated to individual to add to hall-of-fame ";
				lOSS << "is not valid!";
				throw Beagle_RunTimeExceptionM(lOSS.str());
			}
			Individual::Handle lIndivCopy = castHandleT<Individual>(lIndividualAlloc->allocate());
			lIndivCopy->copy(inIndividual, ioContext.getSystem());
			HallOfFame::Entry lEntry(lIndivCopy, ioContext.getGeneration(), ioContext.getDemeIndex());
			mEntries.push_back(lEntry);
			Beagle_LogDebugM(ioContext.getSystem().getLogger(), inIndividual);
			lParetoFrontModified = true;
		}
	}

	// Return if Pareto front has been modified.
	return lParetoFrontModified;

	Beagle_StackTraceEndM();
}
