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
 *  \file   LessThan.cpp
 *  \brief  Implementation of the class LessThan.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:49 $
 */

#include "Beagle/GP.hpp"
#include "LessThan.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new LessThan primitive.
 */
LessThan::LessThan() :
		Beagle::GP::Primitive(2, "LT")
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return the Double tag needed as input for the primitive LessThan.
 *  \param inN Index of the argument to get the tag.
 *  \param ioContext Evolutionary context.
 *  \return RTTO type_info tagging the data type Double.
 *  \throw AssertException If the index inN given is > to 1.
 */
const std::type_info* LessThan::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	Beagle_AssertM(inN < 2);
	return &typeid(Double);
}


/*!
 *  \brief Return the string tagging the Bool type returned but the primitive LessThan.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info tagging the Bool type returned.
 */
const std::type_info* LessThan::getReturnType(Beagle::GP::Context& ioContext) const
{

	return &typeid(Bool);
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the comparison operation between two Double, returning a Bool.
 *  \param outDatum Result of the comparison.
 *  \param ioContext Evolutionary context.
 */
void LessThan::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Bool& lResult = castObjectT<Bool&>(outDatum);
	Double lArgs[2];
	getArguments(lArgs, sizeof(Double), ioContext);
	lResult = lArgs[0].getWrappedValue() < lArgs[1].getWrappedValue();
}




