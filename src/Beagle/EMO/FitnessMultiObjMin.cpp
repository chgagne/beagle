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
 *  \file   beagle/FitnessMultiObjMin.cpp
 *  \brief  Source code of class FitnessMultiObjMin.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a minimization multiobjective fitness object.
 *  \param inSize Size of the multiobjective fitness value.
 *  \param inValue Default initial value of the multiobjective fitness measures.
 */
FitnessMultiObjMin::FitnessMultiObjMin(unsigned int inSize, double inValue) :
		FitnessMultiObj(inSize, inValue)
{ }


/*!
 *  \brief Construct a valid minimization multiobjective fitness object.
 *  \param inFitness Value of the fitness.
 */
FitnessMultiObjMin::FitnessMultiObjMin(const std::vector<double>& inFitness) :
		FitnessMultiObj(inFitness)
{ }


/*!
 *  \brief Copy fitness into current.
 *  \param inOriginal Fitness to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void FitnessMultiObjMin::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObjMin& lOriginal = castObjectT<const FitnessMultiObjMin&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get exact type of the fitness.
 *  \return Exact type of the fitness.
 */
const std::string& FitnessMultiObjMin::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("FitnessMultiObjMin");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test whether the actual minimization multiobjective fitness is Pareto-dominated.
 *  \param inRightFitness Second multiobjective fitness to test domination on.
 *  \return True if the actual multiobjective measure is dominated, false if not.
 */
bool FitnessMultiObjMin::isDominated(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObjMin& lRightFitness = castObjectT<const FitnessMultiObjMin&>(inRightFitness);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	Beagle_AssertM(size() == lRightFitness.size());
	bool lNotEqual = false;
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] < lRightFitness[i]) return false;
		else if((*this)[i] > lRightFitness[i]) lNotEqual = true;
	}
	return lNotEqual;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test order of two minimization multiobjective fitness measures.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessMultiObjMin::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObjMin& lRightFitness = castObjectT<const FitnessMultiObjMin&>(inRightObj);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	Beagle_AssertM(size() == lRightFitness.size());
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] > lRightFitness[i]) return true;
		else if((*this)[i] < lRightFitness[i]) return false;
	}
	return false;
	Beagle_StackTraceEndM();
}
