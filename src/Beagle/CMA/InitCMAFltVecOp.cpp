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
 *  \file   beagle/GA/src/InitCMAFltVecOp.cpp
 *  \brief  Source code of class GA::InitCMAFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 20:57:59 $
 */

#include "beagle/GA.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a GA float vectors uniformly distributed for CMA-ES initialization operator.
 *  \param inFloatVectorSize Size of the float vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::InitCMAFltVecOp::InitCMAFltVecOp(unsigned int inFloatVectorSize,
                                     std::string inReproProbaName,
                                     std::string inName) :
		GA::InitFltVecOp(inFloatVectorSize, inReproProbaName, inName)
{ }


/*!
 *  \brief Apply the initialization operation on the deme.
 *  \param ioDeme Current deme of individuals to initialize.
 *  \param ioContext Context of the evolution.
 */
void GA::InitCMAFltVecOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    string("Initializing the ")+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "Resizing the deme to one individual (CMA-ES initialization)"
	);

	const Factory& lFactory = ioContext.getSystem().getFactory();
	Individual::Alloc::Handle lIndividualAlloc =
		castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
	ioDeme.clear();
	ioDeme.push_back(castHandleT<Individual>(lIndividualAlloc->allocate()));

	unsigned int lSeededIndividuals = 0;
	if(mSeedsFile->getWrappedValue().empty() == false) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    string("Reading seeds file '")+mSeedsFile->getWrappedValue()+
		    string("' to initialize the ")+uint2ordinal(ioContext.getDemeIndex()+1)+
		    string(" deme")
		);
		lSeededIndividuals = readSeeds(mSeedsFile->getWrappedValue(), ioDeme, ioContext);
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    uint2str(lSeededIndividuals)+string(" individuals read to seed the deme")
		);
	}

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();
	for(unsigned int i=lSeededIndividuals; i<ioDeme.size(); ++i) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    string("Initializing the ")+uint2ordinal(i+1)+" individual"
		);
		ioContext.setIndividualHandle(ioDeme[i]);
		ioContext.setIndividualIndex(i);
		initIndividual(*ioDeme[i], ioContext);
		if(ioDeme[i]->getFitness() != NULL) {
			ioDeme[i]->getFitness()->setInvalid();
			if(lHistory != NULL) {
				lHistory->incrementHistoryVar(*ioDeme[i]);
				lHistory->trace(ioContext, std::vector<HistoryID>(), ioDeme[i], getName(), "initialization");
			}
		}
	}
	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);
	Beagle_StackTraceEndM();
}

