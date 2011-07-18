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
 *  \file   Beagle/EC/OversizeOp.cpp
 *  \brief  Source code of class OversizeOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2009/01/21 20:28:16 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <algorithm>

using namespace std;
using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Build oversize replacement strategy operator.
 *  \param inOversizeRatioName Oversize ratio parameter name used in the register.
 *  \param inName Name of the oversize operator.
 */
OversizeOp::OversizeOp(std::string inOversizeRatioName, std::string inName) :
	Beagle::ReplacementStrategyOp(inName),
	mOversizeRatioName(inOversizeRatioName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void OversizeOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	ReplacementStrategyOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Oversizing ratio, which mean how much bigger is the child population from ";
		lOSS << "the parent population.";
		Register::Description lDescription(
		    "Oversizing ratio",
		    "Float",
		    "7.0",
		    lOSS.str()
		);
		mOversizeRatio = castHandleT<Float>
		                 (ioSystem.getRegister().insertEntry(mOversizeRatioName, new Float(7.0f), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the oversize replacement strategy operation on a deme.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void OversizeOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(getRootNode());
	Beagle_ValidateParameterM
	(mOversizeRatio->getWrappedValue() >= 1.0
	 || mOversizeRatio->getWrappedValue() == -1.0,
	 mOversizeRatioName,
	 "The oversize ratio must be greater than or equal to 1.0, or equal to -1.0.");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    string("Using oversize replacement strategy to process the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogTraceM(ioContext.getSystem().getLogger(), (*this));

	RouletteT<unsigned int> lRoulette;
	buildRoulette(lRoulette, ioContext);

	// Calculate the increase in size (lambda)
	float lRatio = mOversizeRatio->getWrappedValue();
	unsigned int lLambda;
	if (lRatio == -1.0) {
		// Using special ratio of -1.0 ensures deme grows to size specified in 'ec.pop.size'
		if (!ioContext.getSystem().getRegister().isRegistered("ec.pop.size")) {
			throw Beagle_RunTimeExceptionM(getName()+" requires register variable 'ec.pop.size'");
		}
		UIntArray::Handle lPopSize = castHandleT<UIntArray>
		                             (ioContext.getSystem().getRegister().getEntry("ec.pop.size"));
		unsigned int lSpecifiedDemeSize = (*lPopSize)[ioContext.getDemeIndex()];
		unsigned int lCurrentDemeSize = ioDeme.size();
		if (lSpecifiedDemeSize < lCurrentDemeSize) {
			throw Beagle_RunTimeExceptionM
			(std::string("For the ")+uint2ordinal(ioContext.getDemeIndex()+1)+
			 " deme, the size specified in 'ec.pop.size' ("+uint2str(lSpecifiedDemeSize)+
			 ") is less than the current deme size ("+uint2str(lCurrentDemeSize)+
			 ").  "+getName()+" can only increase the size of the deme.  Consider using DecimateOp "+
			 "if you wish to decrease the size of the deme");
		}
		lLambda = lSpecifiedDemeSize - lCurrentDemeSize;
	} else {
		// Using ratio to scale the deme's population
		lLambda = (unsigned int)ceil((lRatio-1.0)*float(ioDeme.size()));
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    string("Population will be increased in size by ")+uint2str(lLambda)+" individuals"
	);

	// Create the new individuals.
	Individual::Bag lOffsprings;
	for(unsigned int i=0; i<lLambda; ++i) {
		unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
		BreederNode::Handle lSelectedBreeder=getRootNode();
		for(unsigned int j=0; j<lIndexBreeder; ++j)
			lSelectedBreeder=lSelectedBreeder->getNextSibling();
		Beagle_NonNullPointerAssertM(lSelectedBreeder);
		Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
		Individual::Handle lBredIndiv =
		    lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
		Beagle_NonNullPointerAssertM(lBredIndiv);
		lOffsprings.push_back(lBredIndiv);
	}

	// Add the new individuals into the deme.
	ioDeme.insert(ioDeme.end(), lOffsprings.begin(), lOffsprings.end());
	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    string("There are now ")+uint2str(ioDeme.size())+" individuals in the "+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a OversizeOp operator for XML tree.
 *  \param inIter XML iterator to use to read OversizeOp operator.
 *  \param ioSystem Evolutionary system.
 */
void OversizeOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	string lOversizeRatioReadName = inIter->getAttribute("ratio_name");
	if(lOversizeRatioReadName.empty() == false) mOversizeRatioName = lOversizeRatioReadName;

	ReplacementStrategyOp::readWithSystem(inIter, ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write OversizeOp operator into XML streamer.
 *  \param ioStreamer XML streamer to write OversizeOp operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void OversizeOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	ioStreamer.insertAttribute("ratio_name", mOversizeRatioName);
	for(BreederNode::Handle lNode=getRootNode(); lNode!=NULL; lNode=lNode->getNextSibling()) {
		lNode->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
