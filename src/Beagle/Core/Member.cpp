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
 *  \file   Beagle/Core/Member.cpp
 *  \brief  Source code of class Member.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Reading a member with method read() is undefined. Use readWithContext() instead.
 */
void Member::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read", "Member", getType());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the member instance.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioContext Evolutionary context.
 */
void Member::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName())) {
		std::ostringstream lOSS;
		lOSS << "expected to get tag '" << getName() << "' instead of tag '";
		lOSS << inIter->getValue() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lType = inIter->getAttribute("type");
	if((lType.empty() == false) && (lType != getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lType << "' mismatch type of the member '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the member instance.
 *  \param ioStreamer XML streamer used to write member into.
 *  \param inIndent Whether XML output should be indented.
 */
void Member::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	ioStreamer.insertAttribute("type", getType());
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write content of the member instance.
 *  \param ioStreamer XML streamer used to write member into.
 *  \param inIndent Whether XML output should be indented.
 */
void Member::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{ }
