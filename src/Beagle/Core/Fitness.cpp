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
 *  \file   Beagle/Core/Fitness.cpp
 *  \brief  Source code of class Fitness.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Default construct a fitness object.
 *  \param inValid If the fitness is valid or not.
 */
Fitness::Fitness(bool inValid) :
	mValid(inValid)
{ }


/*!
 *  \brief  Compute distance between two fitnesses.
 *  \param  inRightFitness Right fitness value used to compute distance.
 *  \return Distance between two fitnesses.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
double Fitness::getDistance(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getDistance", "Fitness", getType());
	return 0.;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Get the name of the member concept.
 *  \return Member concept name, "Fitness".
 */
const std::string& Fitness::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("Fitness");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get number of objectives of the fitness value.
 *  \return Number of objectives of the fitness.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
unsigned int Fitness::getNumberOfObjectives() const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getNumberOfObjectives", "Fitness", getType());
	return 0;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Get a fitness objective value.
 *  \param  inObjIndex Index of objective to get value.
 *  \return Objective value of the fitness.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
double Fitness::getObjective(unsigned int inObjIndex) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getObjective", "Fitness", getType());
	return 0.;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Test if an fitness is Pareto dominated by another.
 *  \param  inRightFitness Right fitness value used to compute domination.
 *  \return True if actual fitness is dominated by the right fitness, false if not.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
bool Fitness::isDominated(const Fitness& inRightFitness) const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("isDominated", "Fitness", getType());
	return false;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the fitness instance.
 *  \param inIter Iterator to XML node to read instance from.
 */
void Fitness::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName())) {
		std::ostringstream lOSS;
		lOSS << "expected to get tag '" << getName() << "' instead of tag '";
		lOSS << inIter->getValue() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	const std::string& lType = inIter->getAttribute("type");
	if((lType.empty() == false) && (lType != getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lType << "' mismatch type of the member '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	const std::string& lValid = inIter->getAttribute("valid");
	if(lValid == "yes") setValid();
	else setInvalid();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the fitness instance.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioContext Evolutionary context.
 */
void Fitness::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	read(inIter);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the member instance.
 *  \param ioStreamer XML streamer used to write member into.
 *  \param inIndent Whether XML output should be indented.
 */
void Fitness::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Fitness", inIndent);
	ioStreamer.insertAttribute("type", getType());
	if(isValid()) {
		ioStreamer.insertAttribute("valid", "yes");
		writeContent(ioStreamer, inIndent);
	} else {
		ioStreamer.insertAttribute("valid", "no");
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
