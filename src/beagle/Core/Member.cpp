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
 *  \file   beagle/Core/Member.cpp
 *  \brief  Source code of class Member.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Reading a member with method read() is undefined. Use readWithContext() instead.
 */
void Member::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("read", "Member", getType());
	Beagle_StackTraceEndM("void Member::read(PACC::XML::ConstIterator)");
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
	Beagle_StackTraceEndM("void Member::readWithContext(PACC::XML::ConstIterator,Context&)");
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
	Beagle_StackTraceEndM("void Member::write(PACC::XML::Streamer&, bool) const");
}


/*!
 *  \brief Write content of the member instance.
 *  \param ioStreamer XML streamer used to write member into.
 *  \param inIndent Whether XML output should be indented.
 */
void Member::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{ }
