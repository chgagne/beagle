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
 *  \file   Beagle/EC/TermMaxEvalsOp.cpp
 *  \brief  Source code of class TermMaxEvalsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a termination check operator based on a maximum number of evalutions.
 *  \param inName Name of the operator.
 */
TermMaxEvalsOp::TermMaxEvalsOp(std::string inName) :
	Beagle::EC::TerminationOp(inName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxEvalsOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	std::ostringstream lOSS;
	lOSS << "Maximum number of fitness evaluations for the evolution. ";
	lOSS << "A zero value means that there is no evaluation limits.";
	Register::Description lDescription(
	    "Max evaluations term criterion",
	    "UInt",
	    "5000",
	    lOSS.str()
	);
	mMaxEvaluations = castHandleT<UInt>(
	                      ioSystem.getRegister().insertEntry("ec.term.maxevals", new UInt(5000), lDescription));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Check if the maximum number of fitness evaluations is reached.
 *  \param ioDeme
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxEvalsOp::terminate(const Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(mMaxEvaluations->getWrappedValue() == 0) return false;
	if(ioContext.getTotalProcessedVivarium() >= mMaxEvaluations->getWrappedValue()) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    std::string("Maximum number of fitness evaluations (") +
		    uint2str(mMaxEvaluations->getWrappedValue()) +
		    std::string(") termination criterion reached")
		);
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    std::string("Actual number of fitness evaluations is: ")+
		    uint2str(ioContext.getTotalProcessedVivarium())
		);
		return true;
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Maximum number of fitness evaluations (") +
	    uint2str(mMaxEvaluations->getWrappedValue()) +
	    std::string(") termination criterion not reached ")
	);
	return false;
	Beagle_StackTraceEndM();
}


