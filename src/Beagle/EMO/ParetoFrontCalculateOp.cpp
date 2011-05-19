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
 *  \file   beagle/src/ParetoFrontCalculateOp.cpp
 *  \brief  Source code of class ParetoFrontCalculateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "beagle/Beagle.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a Pareto front extractor operator.
 */
ParetoFrontCalculateOp::ParetoFrontCalculateOp(std::string inName) :
		Operator(inName),
		mMilestonePrefix(NULL),
		mWritingInterval(NULL),
		mPerDemeMilestone(NULL),
		mPopSize(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void ParetoFrontCalculateOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Operator::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Prefix used to name the evolution milestone files. ";
		lOSS << "An empty string means no milestone.";
		Register::Description lDescription(
		    "Milestone filename prefix",
		    "String",
		    "\"beagle\"",
		    lOSS.str()
		);
		mMilestonePrefix = castHandleT<String>(
		                       ioSystem.getRegister().insertEntry("ms.write.prefix", new String("beagle"), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Milestone saving interval (in number of generations). ";
		lOSS << "When zero, only the last generation milestone is saved.";
		Register::Description lDescription(
		    "Milestone saving interval",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mWritingInterval = castHandleT<UInt>(
		                       ioSystem.getRegister().insertEntry("ms.write.interval", new UInt(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "If true, this flag indicates that separate milestones ";
		lOSS << "should be written after each demes processing. Otherwise milestones are ";
		lOSS << "written after the processing of a complete populations.";
		Register::Description lDescription(
		    "Milestone per deme saving flag",
		    "Bool",
		    "0",
		    lOSS.str()
		);
		mPerDemeMilestone = castHandleT<Bool>(
		                        ioSystem.getRegister().insertEntry("ms.write.perdeme", new Bool(false), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of demes and size of each deme of the population. ";
		lOSS << "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lOSS << "The size of the UIntArray is the number of demes present in the ";
		lOSS << "vivarium, while each value of the vector is the size of the corresponding ";
		lOSS << "deme.";
		Register::Description lDescription(
		    "Vivarium and demes sizes",
		    "UIntArray",
		    "100",
		    lOSS.str()
		);
		mPopSize = castHandleT<UIntArray>(
		               ioSystem.getRegister().insertEntry("ec.pop.size", new UIntArray(1,100), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the characteristic operation.
 *  \param ioDeme
 *  \param ioContext Context of the evolution.
 */
void ParetoFrontCalculateOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMilestonePrefix->getWrappedValue().empty()) return;
	if(ioContext.getContinueFlag() == true) {
		if(mWritingInterval->getWrappedValue() == 0) return;
		if((ioContext.getGeneration() % mWritingInterval->getWrappedValue()) != 0) return;
		if((mPerDemeMilestone->getWrappedValue() == false) &&
		        (ioContext.getDemeIndex() != (mPopSize->size()-1))) return;
	}

	unsigned int lOldDemeIndex  = ioContext.getDemeIndex();
	Deme::Handle lOldDemeHandle = ioContext.getDemeHandle();

	for(unsigned int i=0; i<ioContext.getVivarium().size(); ++i) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    std::string("Computing Pareto front for the ")+uint2ordinal(i+1)+" deme"
		);
		ioContext.setDemeIndex(i);
		ioContext.setDemeHandle(ioContext.getVivarium()[i]);
		ParetoFrontHOF::Handle lParetoFront = new ParetoFrontHOF;
		lParetoFront->updateWithDeme(1, *ioContext.getVivarium()[i], ioContext);
		ioContext.getVivarium()[i]->removeMember("HallOfFame");
		ioContext.getVivarium()[i]->addMember(lParetoFront);
	}

	Beagle_LogInfoM(
	    ioContext.getSystem().getLogger(),
	    "Computing Pareto front for the whole vivarium"
	);
	ParetoFrontHOF::Handle lParetoFrontViva = new ParetoFrontHOF;
	for(unsigned int j=0; j<ioContext.getVivarium().size(); ++j) {
		ioContext.setDemeIndex(j);
		ioContext.setDemeHandle(ioContext.getVivarium()[j]);
		lParetoFrontViva->updateWithDeme(1, *ioContext.getVivarium()[j], ioContext);
	}
	ioContext.getVivarium().removeMember("HallOfFame");
	ioContext.getVivarium().addMember(lParetoFrontViva);

	ioContext.setDemeIndex(lOldDemeIndex);
	ioContext.setDemeHandle(lOldDemeHandle);
	Beagle_StackTraceEndM();
}
