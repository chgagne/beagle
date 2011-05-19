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
 *  \file   Beagle/Core/HistoryID.cpp
 *  \brief  Source code of class HistoryID.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/27 13:22:06 $
 */

#include "Beagle/Core.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct history ID class, to be used to tag individuals.
 *  \param inID Initial ID value.
 *  \param inVar Initial variation number value.
 */
HistoryID::HistoryID(unsigned int inID, unsigned int inVar) :
	mID(inID),
	mVar(inVar)
{ }


/*!
 *  \brief Make a copy of given HistoryID.
 *  \param inOriginal HistoryID to copy.
 *  \param ioSystem Evolutionary system.
 */
void HistoryID::copy(const Member& inOriginal, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	const HistoryID& lOrigHID = castObjectT<const HistoryID&>(inOriginal);
	operator=(lOrigHID);
	Beagle_StackTraceEndM();
}


/*!
 *  \return Name of the member, "HistoryID".
 */
const std::string& HistoryID::getName() const
{
	Beagle_StackTraceBeginM();
	const static std::string lName("HistoryID");
	return lName;
	Beagle_StackTraceEndM();
}


/*!
 *  \return Exact type of the HistoryID, "HistoryID".
 */
const std::string& HistoryID::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("HistoryID");
	return lType;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read HistoryID from XML.
 *  \param inIter Iterator to XML structure to read HistoryID from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If XML format is not valid.
 */
void HistoryID::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Be sure that root tag is HistoryID
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="HistoryID")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <HistoryID> expected!");
	}

	// Be sure that the types are corresponding
	std::string lType = inIter->getAttribute("type");
	if((lType.empty()==false) && (lType!=getType())) {
		std::ostringstream lOSS;
		lOSS << "type given '" << lType << "' mismatch type of the history ID '";
		lOSS << getType() << "'!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	// Get ID and variation number
	std::string lIDStr = inIter->getAttribute("id");
	if(lIDStr.empty()) {
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'id' should be given in HistoryID!");
	}
	mID = str2uint(lIDStr);
	std::string lVarStr = inIter->getAttribute("var");
	if(lVarStr.empty()) {
		throw Beagle_IOExceptionNodeM(*inIter, "attribute 'var' should be given in HistoryID!");
	}
	mVar = str2uint(lVarStr);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write history ID into XML.
 *  \param ioStreamer XML streamer to write history ID into.
 *  \param inIndent Whether XML output should be indented.
 */
void HistoryID::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("id", mID);
	ioStreamer.insertAttribute("var", mVar);
	Beagle_StackTraceEndM();
}
