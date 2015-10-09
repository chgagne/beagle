/*
 *  Boolean 11-multiplexer (multiplexer)
 *
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
 *  \file   IfBoolean.cpp
 *  \brief  Implementation of the class IfBoolean.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/GP.hpp"
#include "IfBoolean.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new IfBoolean primitive.
 */
IfBoolean::IfBoolean() :
		Beagle::GP::Primitive(3, "IF")
{ }


/*!
 *  \brief Depending the first argument value, return the 2nd (true) or the 3rd (false).
 *  \param outDatum Result of the comparison.
 *  \param ioContext Evolutionary context.
 */
void IfBoolean::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Bool lCondition;
	get1stArgument(lCondition, ioContext);
	if(lCondition == true) get2ndArgument(outDatum, ioContext);
	else get3rdArgument(outDatum, ioContext);
}




