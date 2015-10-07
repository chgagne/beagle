/*
 *  Artifical ant (ant)
 *  Copyright (C) 2006 by Christian Gagne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   IfFoodAhead.cpp
 *  \brief  Implementation of class IfFoodAhead.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/EC.hpp"
#include "IfFoodAhead.hpp"
#include "AntSimulator.hpp"

using namespace Beagle;


/*!
 *  \brief Construct if-food-ahead primitive.
 */
IfFoodAhead::IfFoodAhead() :
		GP::Primitive(2,"IFA")
{ }


/*!
 *  \brief Execute if-food-ahead primitive.
 *  \param outDatum GP datum, in the actual case a reference to the ant simulator.
 *  \param ioContext Evolutionary context.
 */
void IfFoodAhead::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	const AntSimulator& lSimulator = castObjectT<AntSimulator&>(outDatum);
	if(lSimulator.getMovesDone() >= lSimulator.getMaxMoves()) return;
	if(lSimulator.senseFood()) get1stArgument(outDatum, ioContext);
	else get2ndArgument(outDatum, ioContext);
}
