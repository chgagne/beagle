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
 *  \file   Beagle/Core/Component.cpp
 *  \brief  Source code of class Component.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.20 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Component class constructor.
 *  \param inName Name of the component.
 */
Component::Component(std::string inName) : NamedObject(inName), mRegistered(false), mInitialized(false)
{}


/*!
 *  \brief Initialize this component.
 */
void Component::init(System&)
{}


/*!
 *  \brief Read the system component.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioSystem Evolutionary system.
 */
void Component::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if(inIter->getType() != PACC::XML::eData)
		throw Beagle_IOExceptionNodeM(*inIter, "tag expected!");

	if(inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	Beagle_StackTraceEndM("void Component::readWithSystem(PACC::XML::ConstIterator, System&)");
}


/*!
 *  \brief Register the parameters of this component.
 */
void Component::registerParams(System&)
{}


/*!
 *  \brief Write the system component.
 *  \param ioStreamer XML streamer used to write component into.
 *  \param inIndent Whether XML output should be indented.
 *
 *  This method opens an XML tag and calls methode Component::writeContent.
 */
void Component::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag(getName(), inIndent);
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();

	Beagle_StackTraceEndM("void Component::write(PACC::XML::Streamer&, bool) const");
}


/*!
 *  \brief Write content of the system component.
 */
void Component::writeContent(PACC::XML::Streamer&, bool) const
{}
