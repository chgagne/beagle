/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/TerminationOp.cpp
 *  \brief  Source code of class TerminationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator.
 *  \param inName Name of the termination operator.
 */
TerminationOp::TerminationOp(std::string inName) :
	Beagle::Operator(inName)
{ }


/*!
 *  \brief Apply the characteristic parameters reading operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void TerminationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Checking if '")+getName()+
	    std::string("' termination criterion reached for the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	if(terminate(ioDeme,ioContext)) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    "Termination criterion reached"
		);
		ioContext.setContinueFlag(false);
	}
	Beagle_StackTraceEndM();
}

