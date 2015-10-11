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
 *  \file   beagle/GP/src/EphemeralDouble.cpp
 *  \brief  Source code of class GP::EphemeralDouble.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new ephemeral constant. If the value is NULL, act a an ephemeral generator.
 *  \param inValue Value of the ephemeral constant. When NULL, act a an ephemeral generator.
 *  \param inName Name of the ephemeral primitive.
 */
GP::EphemeralDouble::EphemeralDouble(Double::Handle inValue, std::string inName) :
		EphemeralT<Double>(inValue, inName)
{ }


/*!
 *  \brief  Generate a new random ephemeral Double constant between [-1,1].
 *  \param  inName Name of the constant.
 *  \param  ioContext Context to use to generate the value.
 *  \return Handle to the ephemeral Double constant generated.
 */
GP::Primitive::Handle GP::EphemeralDouble::generate(std::string inName, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Double::Handle lValue = new Double(ioContext.getSystem().getRandomizer().rollUniform(-1.,1.));
	return new EphemeralDouble(lValue, inName);
	Beagle_StackTraceEndM();
}

