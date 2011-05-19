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
 *  \file   Beagle/Core/Genotype.cpp
 *  \brief  Source code of class Genotype.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Get the name of the member concept.
 *  \return Member concept name, "Genotype".
 */
const std::string& Genotype::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("Genotype");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return size of a genotype.
 *  \return Size of genotype.
 *  \warning This method is undefined for basic genotype.
 */
unsigned int Genotype::getSize() const
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getSize", "Genotype", getType());
	return 0;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Swap the gene at inI with the gene at inJ.
 *	\param inI is the index of the first gene to swap.
 *	\param inJ is the index of the second gene to swap.
 *	\warning This method is undefined for a basic genotype, it should be implemented in order to be used in an explicit genotype.
 */
void Genotype::swap(unsigned int inI, unsigned int inJ)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("swap", "Genotype", getType());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read genotype from a Beagle XML node.
 *  \param inIter XML iterator to read the genotype from.
 *  \param ioContext Evolutionary context.
 */
void Genotype::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Genotype")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");
	}
	std::string lGenotypeType = inIter->getAttribute("type");
	if((lGenotypeType.empty() == false) && (lGenotypeType != getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lGenotypeType << "' mismatch type of the genotype '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write a genotype into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether XML output should be indented.
 */
void Genotype::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("Genotype", inIndent);
	ioStreamer.insertAttribute("type", getType());
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write content of a genotype into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether XML output should be indented.
 */
void Genotype::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{ }
