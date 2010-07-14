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
 *  \file   Beagle/EC/SelectWorstTournOp.cpp
 *  \brief  Source code of class SelectWorstTournOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2007/08/29 17:38:32 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct operator to select worst individual by tournaments.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the selection operator.
 */
SelectWorstTournOp::SelectWorstTournOp(std::string inReproProbaName, std::string inName) :
		Beagle::EC::SelectionOp(inReproProbaName, inName),
		mNumberParticipants(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void SelectWorstTournOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EC::SelectionOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Selection tournaments size",
	    "UInt",
	    "2",
	    "Number of participants for worst tournament selection."
	);
	mNumberParticipants =
		castHandleT<UInt>(ioSystem.getRegister().insertEntry("ec.sel.worsttournsize", new UInt(2), lDescription));

	Beagle_StackTraceEndM("void SelectWorstTournOp::registerParams(System&)");
}


/*!
 *  \brief Select a bad individual using the tournament selection method.
 *  \param ioPool Individual pool to use for selection.
 *  \param ioContext Context of the evolution.
 *  \return Index of the choosen individual in the pool.
 */
unsigned int SelectWorstTournOp::selectOneIndividual(Individual::Bag& ioPool, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mNumberParticipants->getWrappedValue() > 0,
	                          "ec.sel.worsttournsize", ">0");

	unsigned int lChoosenIndividual =
	    ioContext.getSystem().getRandomizer().rollInteger(0,((unsigned int)ioPool.size())-1);
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "selection", "Beagle::SelectWorstTournOp",
	    std::string("Starting by choosing the ")+
	    uint2ordinal(lChoosenIndividual+1)+" individual"
	);

	for(unsigned int j=1; j<mNumberParticipants->getWrappedValue(); j++) {
		unsigned int lTriedIndividual =
		    ioContext.getSystem().getRandomizer().rollInteger(0,((unsigned int)ioPool.size())-1);
		if(ioPool[lTriedIndividual]->isLess(*ioPool[lChoosenIndividual])) {
			lChoosenIndividual = lTriedIndividual;
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "selection", "Beagle::SelectWorstTournOp",
			    std::string("Trying the ")+uint2ordinal(lTriedIndividual+1)+
			    " individual -> choosing it"
			);
		} else {
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "selection", "Beagle::SelectWorstTournOp",
			    std::string("Trying the ")+uint2ordinal(lTriedIndividual+1)+
			    " individual -> the previously choosen one is worse"
			);
		}
	}

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "selection",
	    "Beagle::SelectWorstTournOp",
	    std::string("Selecting the ")+uint2ordinal(lChoosenIndividual+1)+" individual"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "selection",
	    "Beagle::SelectWorstTournOp",
	    *ioPool[lChoosenIndividual]
	);

	return lChoosenIndividual;
	Beagle_StackTraceEndM("unsigned int SelectWorstTournOp::selectIndividual(Individual::Bag&,Context&)");
}
