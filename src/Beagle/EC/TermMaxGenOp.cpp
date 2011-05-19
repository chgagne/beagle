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
 *  \file   Beagle/EC/TermMaxGenOp.cpp
 *  \brief  Source code of class TermMaxGenOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/08 19:26:47 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator based on a maximum generation value.
 *  \param inName Name of the operator.
 */
TermMaxGenOp::TermMaxGenOp(std::string inName) :
	Beagle::EC::TerminationOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxGenOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EC::TerminationOp::registerParams(ioSystem);

	std::ostringstream lOSS;
	lOSS << "Maximum number of generations for the evolution.";
	Register::Description lDescription(
	    "Max generation term criterion",
	    "UInt",
	    "50",
	    lOSS.str()
	);
	mMaxGeneration = castHandleT<UInt>(
	                     ioSystem.getRegister().insertEntry("ec.term.maxgen", new UInt(50), lDescription));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if the maximum number of generation is reached.
 *  \param ioDeme
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxGenOp::terminate(const Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMaxGeneration->getWrappedValue() == 0) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    std::string("Maximum number of generations (") +
		    uint2str(mMaxGeneration->getWrappedValue()) +
		    std::string(") termination criterion disabled")
		);
		return false;
	}
	if(ioContext.getGeneration() > mMaxGeneration->getWrappedValue()) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    std::string("Maximum number of generations (") +
		    uint2str(mMaxGeneration->getWrappedValue()) +
		    std::string(") termination criterion overpassed")
		);
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    std::string("Actual generation number is: ")+uint2str(ioContext.getGeneration())
		);
		return true;
	}
	if((ioContext.getGeneration() == mMaxGeneration->getWrappedValue()) &&
	        (ioContext.getDemeIndex() == (ioContext.getVivarium().size()-1))) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    std::string("Maximum number of generations (") +
		    uint2str(mMaxGeneration->getWrappedValue()) +
		    std::string(") termination criterion reached")
		);
		return true;
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Maximum number of generations (") +
	    uint2str(mMaxGeneration->getWrappedValue()) +
	    std::string(") termination criterion not reached")
	);
	return false;
	Beagle_StackTraceEndM();
}


