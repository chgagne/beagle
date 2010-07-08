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
 *  \file   Beagle/Core/BreederOp.cpp
 *  \brief  Source code of class BreederOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Build breeder operator.
 *  \param inName Name of the breeder operator.
 */
BreederOp::BreederOp(std::string inName) :
		Operator(inName)
{ }


/*!
 *  \brief Write breeder operator into XML streamer.
 *  \param ioStreamer XML streamer to write breeder operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void BreederOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void BreederOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


/*!
 *  \brief Write breeder operator content into XML streamer, without opening/closing tags.
 */
void BreederOp::writeContent(PACC::XML::Streamer&, bool) const
	{ }


