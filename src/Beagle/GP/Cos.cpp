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
 *  \file   beagle/GP/src/Cos.cpp
 *  \brief  Source code of class GP::Cos.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:36 $
 */

#include "beagle/GP.hpp"

#include <cmath>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace Beagle;


/*!
 *  \brief Construct a new cosinus primitive.
 *  \param inName Name of the cosinus primitive.
 */
GP::Cos::Cos(std::string inName) :
		Beagle::GP::Primitive(1, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Cos.
 *  \param  inN Index of the argument to get the type tag.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 0.
 */
const std::type_info* Beagle::GP::Cos::getArgType(unsigned int inN, GP::Context&) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<1);
	return &typeid(Double);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the tag of the type of data return by primitive Cos.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
const std::type_info* Beagle::GP::Cos::getReturnType(GP::Context&) const
{
	Beagle_StackTraceBeginM();
	return &typeid(Double);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of a cosinus of number.
 *  \param outResult Result of the cosinus.
 *  \param ioContext Evolutionary context.
 */
void GP::Cos::execute(GP::Datum& outResult, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Double& lResult = castObjectT<Double&>(outResult);
	get1stArgument(lResult, ioContext);
	lResult = cos(lResult);
	Beagle_StackTraceEndM();
}

