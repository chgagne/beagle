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
 *  \file   Beagle/EC/SelectRandomOp.cpp
 *  \brief  Source code of class SelectRandomOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a random selection operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the random selection operator.
 */
SelectRandomOp::SelectRandomOp(std::string inReproProbaName, std::string inName) :
		Beagle::EC::SelectionOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Select an individual in a population randomly (using an uniform distribution).
 *  \param ioPool Pool of individuals to use for selection.
 *  \param ioContext Evolutionary context.
 */
unsigned int SelectRandomOp::selectOneIndividual(Individual::Bag& ioPool, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(ioPool.size() < 2) return 0;
	return ioContext.getSystem().getRandomizer().rollInteger(0, ioPool.size()-1);
	Beagle_StackTraceEndM("unsigned int SelectRandomOp::selectIndividual(Individual::Bag& ioPool, Context& ioContext)");
}


