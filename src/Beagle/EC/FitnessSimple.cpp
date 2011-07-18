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
 *  \file   Beagle/FitnessSimple.cpp
 *  \brief  Source code of class FitnessSimple.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Default construct a simple fitness object.
 */
FitnessSimple::FitnessSimple() :
	Beagle::Fitness(false),
	mValue(-DBL_MAX)
{ }


/*!
 *  \brief Construct a valid simple fitness object.
 *  \param inValue Value of the fitness.
 */
FitnessSimple::FitnessSimple(double inValue) :
	Beagle::Fitness(true),
	mValue(isFinite(inValue) ? inValue : -DBL_MAX)
{ }


/*!
 *  \brief Copy fitness into current.
 *  \param inOriginal Fitness to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void FitnessSimple::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const FitnessSimple& lOriginal = castObjectT<const FitnessSimple&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get distance between two simple fitness measures.
 *  \param  inRightFitness Right fitness to compare.
 *  \return Absolute difference value between the simple measures.
 */
double FitnessSimple::getDistance(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	const FitnessSimple& lRightFitness = castObjectT<const FitnessSimple&>(inRightFitness);
	if((isValid()==false) || (lRightFitness.isValid()==false)) {
		std::ostringstream lOSS;
		lOSS << "Can't return the distance between two simple fitness values as one or both ";
		lOSS << "are invalid!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	return std::fabs(mValue - lRightFitness.mValue);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get number of objectives of simple fitness measures.
 *  \return 1, a simple fitness is made of a single objective.
 */
unsigned int FitnessSimple::getNumberOfObjectives() const
{
	Beagle_StackTraceBeginM();
	return 1;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get the objective value of the simple fitness measure.
 *  \param  inObjective Objective index to get value (should be 0).
 *  \return Simple fitness objective value.
 */
double FitnessSimple::getObjective(unsigned int inObjIndex) const
{
	Beagle_StackTraceBeginM();
	if(inObjIndex != 0) {
		std::ostringstream lOSS;
		lOSS << "The objective index value given (" << inObjIndex << ") is invalid, ";
		lOSS << "a simple fitness is made of one objective, so the index should be equal to 0!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	if(isValid() == false) {
		std::ostringstream lOSS;
		lOSS << "Can't return the value of the objective as the single fitness is invalid!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	return mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the fitness.
 *  \return Exact type of the fitness.
 */
const std::string& FitnessSimple::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("FitnessSimple");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test order of two simple fitness measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessSimple::isDominated(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	const FitnessSimple& lRightFitness = castObjectT<const FitnessSimple&>(inRightFitness);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	return mValue < lRightFitness.mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test equality of two simple fitness measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the fitness are equal, false if not.
 */
bool FitnessSimple::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FitnessSimple& lRightFitness = castObjectT<const FitnessSimple&>(inRightObj);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return true;
	return mValue == lRightFitness.mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test order of two simple fitness measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessSimple::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FitnessSimple& lRightFitness = castObjectT<const FitnessSimple&>(inRightObj);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	return mValue < lRightFitness.mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a simple fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness value.
 */
void FitnessSimple::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Fitness"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");

	const std::string& lValid = inIter->getAttribute("valid");
	if(lValid.empty() || (lValid == "yes")) {

		// Check type of fitness read
		const std::string& lType = inIter->getAttribute("type");
		if((lType.empty() == false) && (lType != getType())) {
			std::ostringstream lOSS;
			lOSS << "type given '" << lType << "' mismatch type of the fitness '";
			lOSS << getType() << "'!";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}

		// Read value
		PACC::XML::ConstIterator lChild = inIter->getFirstChild();
		if(!lChild) throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
		if(lChild->getType() != PACC::XML::eString)
			throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
		mValue = str2dbl(lChild->getValue());
		setValid();

	} else if(lValid == "no") {
		setInvalid();
	} else {
		throw Beagle_IOExceptionNodeM(*inIter, "bad fitness validity value!");
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the fitness value and validity.
 *  \param inValue New fitness value.
 */
void FitnessSimple::setValue(double inValue)
{
	Beagle_StackTraceBeginM();
	if(isFinite(inValue)) mValue = inValue;
	else mValue = -DBL_MAX;
	setValid();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a simple fitness into an XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness value.
 *  \param inIndent Whether output should be indented.
 */
void FitnessSimple::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertStringContent(dbl2str(mValue));
	Beagle_StackTraceEndM();
}
