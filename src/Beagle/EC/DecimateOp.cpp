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
 *  \file   Beagle/EC/DecimateOp.cpp
 *  \brief  Source code of class DecimateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.22 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#include "Beagle/EC.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a plain new decimation operator.
 *  \param inDecimationRatioName Decimation ratio parameter name.
 *  \param inName Name of the decimation operator.
 */
DecimateOp::DecimateOp(std::string inDecimationRatioName, std::string inName) :
	Operator(inName),
	mDecimationRatioName(inDecimationRatioName)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the system to use for decimation.
 */
void DecimateOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Operator::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Number of demes and size of each deme of the population. ";
		lOSS << "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lOSS << "The size of the UIntArray is the number of demes present in the ";
		lOSS << "vivarium, while each value of the vector is the size of the corresponding ";
		lOSS << "deme.";
		Register::Description lDescription(
		    "Vivarium and demes sizes",
		    "UIntArray",
		    "100",
		    lOSS.str()
		);
		mPopSize = castHandleT<UIntArray>(
		               ioSystem.getRegister().insertEntry("ec.pop.size", new UIntArray(1,100), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Decimation ratio, which mean how much smaller will be the population ";
		lOSS << "resulting from a decimation operation compared to the actual population size. ";
		lOSS << "If the ratio value is -1.0, the deme is resized to the size given in parameter ";
		lOSS << "'ec.pop.size'.";
		Register::Description lDescription(
		    "Decimation ratio",
		    "Float",
		    "-1",
		    lOSS.str()
		);
		mDecimationRatio = castHandleT<Float>(
		                       ioSystem.getRegister().insertEntry(mDecimationRatioName, new Float(-1.0f), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the decimation operation on the deme.
 *  \param ioDeme Current deme of individuals to decimate.
 *  \param ioContext Context of the evolution.
 */
void DecimateOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM((mDecimationRatio->getWrappedValue()<=1.0),
	                          mDecimationRatioName,
	                          "The decimation ratio must be less than or equal to 1.0.");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Applying decimation operation on the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	// Calculate the number of individuals to keep from the deme
	unsigned int lMu = 0;
	if(mDecimationRatio->getWrappedValue() == -1.0) {
		Beagle_AssertM(ioContext.getDemeIndex() < mPopSize->size());
		lMu = (*mPopSize)[ioContext.getDemeIndex()];
	} else {
		lMu = (unsigned int)std::ceil(mDecimationRatio->getWrappedValue()*float(ioDeme.size()));
		Beagle_AssertM(ioContext.getDemeIndex() < mPopSize->size());
		int lDiffSize = (*mPopSize)[ioContext.getDemeIndex()] - lMu;
		if((lDiffSize >= -1) && (lDiffSize <= 1)) lMu = (*mPopSize)[ioContext.getDemeIndex()];
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Keeping ")+uint2str(lMu)+" of the "+uint2str(ioDeme.size())+
	    " individuals from the "+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	// Check that the number of individuals to keep (mu) isn't greater than the deme size
	if(lMu > ioDeme.size()) {
		std::ostringstream lOSS;
		lOSS << "Warning: the actual population size (" << ioDeme.size();
		lOSS << ") is less than the desired decimation size (" << lMu;
		lOSS << "). Decimation is thus not applied.";
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    lOSS.str()
		);
		return;
	}

	// Copy the individuals to be kept into the deme
	std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
	Individual::Bag lSurvivors;
	for(unsigned int i=0; i<lMu; ++i) {
		lSurvivors.push_back(ioDeme.front());
		std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
	}
	ioDeme.clear();
	ioDeme.insert(ioDeme.begin(), lSurvivors.begin(), lSurvivors.end());
	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    std::string("There are now ")+uint2str(ioDeme.size())+" individuals in the "+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_StackTraceEndM();
}

