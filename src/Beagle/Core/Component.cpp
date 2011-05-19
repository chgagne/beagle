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

	Beagle_StackTraceEndM();
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

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write content of the system component.
 */
void Component::writeContent(PACC::XML::Streamer&, bool) const
{}
