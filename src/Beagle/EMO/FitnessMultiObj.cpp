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
 *  \file   beagle/src/FitnessMultiObj.cpp
 *  \brief  Source code of class FitnessMultiObj.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a multiobjective fitness object.
 *  \param inSize Size of the multiobjective fitness value.
 *  \param inValue Default initial value of the multiobjective fitness measures.
 */
FitnessMultiObj::FitnessMultiObj(unsigned int inSize, double inValue) :
		Fitness(inSize != 0),
		std::vector<double>(inSize, inValue)
{ }


/*!
 *  \brief Construct a valid multiobjective fitness object.
 *  \param inFitness Value of the fitness.
 */
FitnessMultiObj::FitnessMultiObj(const std::vector<double>& inFitness) :
		Fitness(true),
		std::vector<double>(inFitness)
{ }


/*!
 *  \brief Copy fitness into current.
 *  \param inOriginal Fitness to copy.
 *  \param ioSystem Evolutionary system to use for making the copy.
 */
void FitnessMultiObj::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObj& lOriginal = castObjectT<const FitnessMultiObj&>(inOriginal);
	(*this) = lOriginal;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Compute distance between two multi-objective fitnesses.
 *  \param inRightFitness Fitness used to compute distance.
 *  \return Euclidean distance between the fitnesses using all the objectives.
 */
double FitnessMultiObj::getDistance(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObj& lRightFitness = castObjectT<const FitnessMultiObj&>(inRightFitness);
	if((isValid()==false) || (lRightFitness.isValid()==false)) {
		throw Beagle_RunTimeExceptionM("Can't compute distance between invalid fitnesses!");
	}
	if(size() != lRightFitness.size()) {
		std::ostringstream lOSS;
		lOSS << "Can't compute the distance between two multi-objective fitnesses ";
		lOSS << "that have a different number of objectives (" << size() << " vs ";
		lOSS << lRightFitness.size() << ")!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	double lSumSquare = 0.;
	for(unsigned int i=0; i<size(); ++i) {
		const double lDiff = (*this)[i] - lRightFitness[i];
		lSumSquare += lDiff * lDiff;
	}
	return std::sqrt(lSumSquare);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get number of objectives of the fitness measure.
 *  \return Number of objectives.
 */
unsigned int FitnessMultiObj::getNumberOfObjectives() const
{
	Beagle_StackTraceBeginM();
	if(isValid() == false) {
		throw Beagle_RunTimeExceptionM("Can't return number of objective of an invalid fitness!");
	}
	return size();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get the objective value of the fitness measure.
 *  \param  inObjIndex Objective index to get value.
 *  \return Fitness objective value.
 */
double FitnessMultiObj::getObjective(unsigned int inObjIndex) const
{
	Beagle_StackTraceBeginM();
	if(isValid() == false) {
		throw Beagle_RunTimeExceptionM("Can't return an objective value of an invalid fitness!");
	}
	if(inObjIndex >= size()) {
		std::ostringstream lOSS;
		lOSS << "Objective index given (" << inObjIndex;
		lOSS << ") exceed the number of objectives of the fitness (";
		lOSS << size() << ")!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	return (*this)[inObjIndex];
	Beagle_StackTraceEndM("double FitnessMultiObj::getObjective(unsigned int) const")
}


/*!
 *  \brief Get exact type of the fitness.
 *  \return Exact type of the fitness.
 */
const std::string& FitnessMultiObj::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("FitnessMultiObj");
	return lType;
	Beagle_StackTraceEndM("const std::string& FitnessMultiObj::getType() const")
}


/*!
 *  \brief Test whether the actual multiobjective fitness is Pareto-dominated.
 *  \param inRightFitness Second multiobjective fitness to test domination on.
 *  \return True if the actual multiobjective measure is dominated, false if not.
 */
bool FitnessMultiObj::isDominated(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObj& lRightFitness = castObjectT<const FitnessMultiObj&>(inRightFitness);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	Beagle_AssertM(size() == lRightFitness.size());
	bool lNotEqual = false;
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] > lRightFitness[i]) return false;
		else if((*this)[i] < lRightFitness[i]) lNotEqual = true;
	}
	return lNotEqual;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test equality of two multiobjective fitness measures.
 *  \param inRightObj Right measure to compare.
 *  \return True if the fitness are equal, false if not.
 */
bool FitnessMultiObj::isEqual(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObj& lRightFitness = castObjectT<const FitnessMultiObj&>(inRightObj);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return true;
	Beagle_AssertM(size() == lRightFitness.size());
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] != lRightFitness[i]) return false;
	}
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test order of two multiobjective fitness measures, from first to last objective.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessMultiObj::isLess(const Object& inRightObj) const
{
	Beagle_StackTraceBeginM();
	const FitnessMultiObj& lRightFitness = castObjectT<const FitnessMultiObj&>(inRightObj);
	if(isValid() != lRightFitness.isValid()) return false;
	if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
	Beagle_AssertM(size() == lRightFitness.size());
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] < lRightFitness[i]) return true;
		else if((*this)[i] > lRightFitness[i]) return false;
	}
	return false;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a multiobjective fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness value.
 */
void FitnessMultiObj::read(PACC::XML::ConstIterator inIter)
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

		// Read objective values
		clear();
		for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
			if((lChild->getType() != PACC::XML::eData) || (lChild->getValue() != "Obj")) continue;
			PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
			if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "needed a double value in the <Obj> tag!");
			if(lChild2->getType() != PACC::XML::eString)
				throw Beagle_IOExceptionNodeM(*lChild2, "needed a double value in the <Obj> tag!");
			push_back(str2dbl(lChild2->getValue()));
		}
		setValid();
		
	} else if(lValid == "no") {
		clear();
		setInvalid();
	}
	else {
		throw Beagle_IOExceptionNodeM(*inIter, "bad fitness validity value!");
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a multiobjective fitness into an XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness value.
 *  \param inIndent Whether XML output should be indented.
 */
void FitnessMultiObj::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("size", uint2str(size()));
	for(unsigned int i=0; i<size(); ++i) {
		ioStreamer.openTag("Obj", false);
		ioStreamer.insertStringContent(dbl2str((*this)[i]));
		ioStreamer.closeTag();
	}
	Beagle_StackTraceEndM();
}




