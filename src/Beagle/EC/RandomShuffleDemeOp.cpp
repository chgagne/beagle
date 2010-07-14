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
 *  \file   Beagle/EC/RandomShuffleDemeOp.cpp
 *  \brief  Source code of class RandomShuffleDemeOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

#include <algorithm>
#include <fstream>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a random shuffle deme operator.
 */
RandomShuffleDemeOp::RandomShuffleDemeOp(std::string inName) :
		Operator(inName)
{ }


/*!
 *  \brief Apply the characteristic operation.
 *  \param ioDeme Deme to use to write the milestone.
 *  \param ioContext Context of the evolution.
 */
void RandomShuffleDemeOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	std::random_shuffle(ioDeme.begin(), ioDeme.end(),
	                    ioContext.getSystem().getRandomizer());
	Beagle_StackTraceEndM("void RandomShuffleDemeOp::operate(Deme& ioDeme, Context& ioContext)");
}
