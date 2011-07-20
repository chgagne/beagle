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
 *  \file   Beagle/SAES/MutationOp.cpp
 *  \brief  Source code of class SAES::MutationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.24 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "Beagle/SAES.hpp"

#include <cfloat>
#include <cmath>
#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a evolution strategy mutation operator.
 *  \param inMutationPbName SA-ES mutation probability parameter name used in register.
 *  \param inMinStrategyName Minimum strategy parameter parameter name.
 *  \param inName Name of the operator.
 */
SAES::MutationOp::MutationOp(std::string inMutationPbName,
                             std::string inMinStrategyName,
                             std::string inName) :
		EC::MutationOp(inMutationPbName, inName),
		mMinStrategyName(inMinStrategyName)
{ }


/*!
 *  \brief Register the parameters of the SA-ES mutation operator.
 *  \param ioSystem System of the evolution.
 */
void SAES::MutationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual ES mutation prob.",
		    "Double",
		    "1.0",
		    "SA-ES mutation probability for each individual."
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(1.0f), lDescription));
	}
	EC::MutationOp::registerParams(ioSystem);
	{
		mMinStrategy = new DoubleArray(1,0.01);
		Register::Description lDescription(
		    "Minimum SA-ES strategy parameter",
		    "DoubleArray",
		    "0.01",
		    "Minimum mutation strategy value allowed."
		);
		mMinStrategy = castHandleT<DoubleArray>(
		                   ioSystem.getRegister().insertEntry(mMinStrategyName, new DoubleArray(1,0.01), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "SA-ES vector maximum values. This can be a scalar, which limit the values on all dimensions, ";
		lOSS << "or a vector which limit the values individually. If the maximum value vector size is ";
		lOSS << "smaller than the vector size, the limit used for the last values of the vector ";
		lOSS << "is equal to the last value of the maximum value vector.";
		Register::Description lDescription(
		    "Maximum SA-ES vector values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("saes.value.max", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "SA-ES vector minimum values. This can be a scalar, which limit the values on all dimensions, ";
		lOSS << "or a vector which limit the values individually. If the minimum value vector size is ";
		lOSS << "smaller than the vector size, the limit used for the last values of the vector ";
		lOSS << "is equal to the last value of the minimum value vector.";
		Register::Description lDescription(
		    "Minimum SA-ES vector values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("saes.value.min", new DoubleArray(1,-DBL_MAX), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mutate an ES individual.
 *  \param ioIndividual ES individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True as individual are always mutated.
 */
bool SAES::MutationOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "Applying evolution strategy mutation to an individual"
	);

	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		SAES::PairVector::Handle lVector = castHandleT<SAES::PairVector>(ioIndividual[i]);
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "Mutating the " << uint2ordinal(i+1) << " ES vector"
		);
		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);

		const double lT = 1.0 / std::sqrt(2.0 * std::sqrt(double(lVector->size())));
		const double lTPrime = 1.0 / std::sqrt(2.0 * lVector->size());
		const double lN = ioContext.getSystem().getRandomizer().rollGaussian(0.0, 1.0);
		for(unsigned int j=0; j<lVector->size(); j++) {
			const double lMaxVal      = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
			const double lMinVal      = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
			const double lMinStrategy = j<mMinStrategy->size() ? (*mMinStrategy)[j] : mMinStrategy->back();
			const double lNi = ioContext.getSystem().getRandomizer().rollGaussian(0.0, 1.0);

			(*lVector)[j].mStrategy *= std::exp((lTPrime * lN) + (lT * lNi));
			if((*lVector)[j].mStrategy < lMinStrategy) (*lVector)[j].mStrategy = lMinStrategy;
			(*lVector)[j].mValue += (*lVector)[j].mStrategy * lNi;
			if((*lVector)[j].mValue > lMaxVal) (*lVector)[j].mValue = lMaxVal;
			if((*lVector)[j].mValue < lMinVal) (*lVector)[j].mValue = lMinVal;

			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    "SA-ES mutating by adding " << ((*lVector)[j].mStrategy * lNi) <<
			    " to the value and multiplying the strategy by " <<
			    std::exp((lTPrime * lN) + (lT * lNi)) <<
			    " to mutate the pair " << j << " of the vector"
			);
		}

		Beagle_LogDebugM(ioContext.getSystem().getLogger(), *lVector);
	}
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read SA-ES mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param ioSystem Evolutionary system.
 */
void SAES::MutationOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lMutationPbReadName = inIter->getAttribute("mutationpb");
	if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
	std::string lMinStrategyReadName = inIter->getAttribute("minstrategy");
	if(lMinStrategyReadName.empty() == false) mMinStrategyName = lMinStrategyReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write SA-ES mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void SAES::MutationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	EC::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("minstrategy", mMinStrategyName);
	Beagle_StackTraceEndM();
}

