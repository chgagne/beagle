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
 *  \file   EphemeralPercent.cpp
 *  \brief  Implementation of the class EphemeralPercent.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:49 $
 */

#include "Beagle/GP.hpp"
#include "EphemeralPercent.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new EphemeralPercent primitive.
 *  \param inValue Value of the ephemeral.
 */
EphemeralPercent::EphemeralPercent(Beagle::Double::Handle inValue) :
		GP::EphemeralT<Double>(inValue, "EPR")
{ }


/*!
 *  \brief  Generate a new random ephemeral Double constant in [0,100].
 *  \param  inName Name of the constant.
 *  \param  ioContext Context to use to generate the value.
 *  \return Handle to the ephemeral Double constant generated.
 */
GP::Primitive::Handle EphemeralPercent::generate(std::string inName, GP::Context& ioContext)
{
	Double::Handle lValue = new Double(ioContext.getSystem().getRandomizer().rollUniform(0.,100.));
	return new EphemeralPercent(lValue);
}





