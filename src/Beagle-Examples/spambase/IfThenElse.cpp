/*
 *  SPAM e-mail database (spambase):
 *  Machine learning using strongly-typed GP with Open BEAGLE
 *  Copyright (C) 2001-2006 by Christian Gagne and Marc Parizeau
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
 *  \file   IfThenElse.cpp
 *  \brief  Implementation of the class IfThenElse.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:49 $
 */

#include "Beagle/GP.hpp"
#include "IfThenElse.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new IfThenElse primitive.
 */
IfThenElse::IfThenElse() :
		Beagle::GP::Primitive(3, "IF")
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return the type tag needed as input for the primitive (0=Bool, {1,2}=Double).
 *  \param inN Index of the argument to get the tag.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info tagging the data type.
 *  \throw AssertException If the index inN given is > to 2.
 */
const std::type_info* IfThenElse::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	Beagle_AssertM(inN < 3);
	if(inN == 0) return &typeid(Bool);
	return &typeid(Double);
}


/*!
 *  \brief Return the string tagging the Double type returned but the primitive IfThenElse.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info tagging the Double type returned.
 */
const std::type_info* IfThenElse::getReturnType(Beagle::GP::Context& ioContext) const
{
	return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Depending the first argument value, return the 2nd (true) or the 3rd (false).
 *  \param outDatum Result of the comparison.
 *  \param ioContext Evolutionary context.
 */
void IfThenElse::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Bool lCondition;
	get1stArgument(lCondition, ioContext);
	if(lCondition == true) get2ndArgument(outDatum, ioContext);
	else get3rdArgument(outDatum, ioContext);
}




