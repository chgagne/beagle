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
 *  \file   beagle/Core/Operator.cpp
 *  \brief  Source code of class Operator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "beagle/Core.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a new operator.
 *  \param inName Name of the operator.
 */
Operator::Operator(std::string inName) : NamedObject(inName), mRegistered(false), mInitialized(false)
{}


/*!
 *  \brief Initialize operator.
 *
 *  This method is called after all operators have been given the chance to
 *  register their parameters, so that they can now proceed with their proper
 *  initialization.
 */
void Operator::init(System& ioSystem)
{}


/*!
 *  \brief Reading a operator with method read is undefined. Use readWithSystem instead.
 */
void Operator::read(PACC::XML::ConstIterator)
{
	Beagle_StackTraceBeginM();

	throw Beagle_UndefinedMethodInternalExceptionM("read", "Operator", getName());

	Beagle_StackTraceEndM("void Operator::read(PACC::XML::ConstIterator)");
}


/*!
 *  \brief Read an operator from a XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem Evolutionary system.
 *  \throw IOException If a reading error occurs.
 */
void Operator::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	Beagle_StackTraceEndM("void Operator::readWithSystem(PACC::XML::ConstIterator, System&)");
}


/*!
 *  \brief Register the parameters that are specific to this operator.
 *
 *  This method is called prior to method Operator::init in order
 *  for the operator to register all of its parameters. 
 */
void Operator::registerParams(System& ioSystem)
{}


/*!
 *  \brief Write an operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void Operator::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag(getName(), inIndent);
	ioStreamer.closeTag();

	Beagle_StackTraceEndM("void Operator::write(PACC::XML::Streamer&, bool) const");
}

