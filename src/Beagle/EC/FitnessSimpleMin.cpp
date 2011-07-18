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
 *  \file   Beagle/FitnessSimpleMin.cpp
 *  \brief  Source code of class FitnessSimpleMin.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a valid simple fitness (minimization) object.
 *  \param inFitness Value of the fitness.
 */
FitnessSimpleMin::FitnessSimpleMin(double inFitness)
{
	Beagle_StackTraceBeginM();
	setValue(inFitness);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Copy fitness into current.
 *  \param inOriginal Fitness to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void FitnessSimpleMin::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const FitnessSimpleMin& lOriginal = castObjectT<const FitnessSimpleMin&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the fitness.
 *  \return Exact type of the fitness.
 */
const std::string& FitnessSimpleMin::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("FitnessSimpleMin");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test order of two simple fitness measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessSimpleMin::isDominated(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	const FitnessSimpleMin& lRightFitness = castObjectT<const FitnessSimpleMin&>(inRightFitness);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	return mValue > lRightFitness.mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test order of two simple fitness (minimization) measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is of higher magnitude than the right one, false if not.
 *  \warning In a minimization problem, smallest values are better. So the actual isLess
 *    method returns true when the left FitnessSimpleMin value is higher than
 *    the right one, and false otherwize. This is the opposite of the isLess method of class
 *    FitnessSimple.
 */
bool FitnessSimpleMin::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FitnessSimpleMin& lRightFitness = castObjectT<const FitnessSimpleMin&>(inRightObj);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	return mValue > lRightFitness.mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the fitness value and validity.
 *  \param inValue New fitness value.
 */
void FitnessSimpleMin::setValue(double inValue)
{
	Beagle_StackTraceBeginM();
	if(isFinite(inValue)) mValue = inValue;
	else mValue = DBL_MAX;
	setValid();
	Beagle_StackTraceEndM();
}
