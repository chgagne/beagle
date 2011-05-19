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
 *  \file   Beagle/EC/RecombinationOp.cpp
 *  \brief  Source code of class RecombinationOp.
 *  \author Christian Gagne
 *  $Revision: 1.15 $
 *  $Date: 2007/08/31 13:34:01 $
 */

#include "Beagle/EC.hpp"

#include <vector>
#include <algorithm>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a plain new recombination operator.
 *  \param inRecombProbaName Recombination probability name.
 *  \param inNumberRecombName Number of individuals recombined parameter name used in register.
 *  \param inName Name of the recombination operator.
 */
RecombinationOp::RecombinationOp(std::string inRecombProbaName, std::string inNumberRecombName, std::string inName) :
	Beagle::BreederOp(inName),
	mRecombProbaName(inRecombProbaName),
	mNumberRecombName(inNumberRecombName)
{ }


/*!
 *  \brief Apply the recombination operation on a breeding pool, returning a recombined individual.
 *  \param inBreedingPool Breeding pool to use for the recombination operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the recombination operation.
 *  \return Recombined individual.
 */
Individual::Handle RecombinationOp::breed(Individual::Bag& inBreedingPool,
        BreederNode::Handle inChild,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Generate parents for recombination.
	Individual::Bag::Handle lParents = new Individual::Bag;
	if(inChild == NULL) {
		const unsigned int lNbGenerated =
		    (mNumberRecomb->getWrappedValue()==0) ? inBreedingPool.size() :
		    minOf<unsigned int>(inBreedingPool.size(), mNumberRecomb->getWrappedValue());
		if(lNbGenerated == inBreedingPool.size()) (*lParents) = inBreedingPool;
		else {
			std::vector<unsigned int> lIndices(inBreedingPool.size());
			for(unsigned int i=0; i<lIndices.size(); ++i) lIndices[i] = i;
			std::random_shuffle(lIndices.begin(), lIndices.end(),
			                    ioContext.getSystem().getRandomizer());
			for(unsigned int i=0; i<lNbGenerated; ++i) {
				lParents->push_back(inBreedingPool[lIndices[i]]);
			}
		}
	} else {
		Beagle_NonNullPointerAssertM(inChild->getBreederOp());
		const unsigned int lNbGenerated =
		    (mNumberRecomb->getWrappedValue()==0) ? inBreedingPool.size() :
		    minOf<unsigned int>(inBreedingPool.size(), mNumberRecomb->getWrappedValue());
		for(unsigned int i=0; i<lNbGenerated; ++i) {
			Individual::Handle lIndiv = inChild->getBreederOp()->breed(inBreedingPool,
			                            inChild->getFirstChild(),
			                            ioContext);
			lParents->push_back(lIndiv);
		}
	}

	// Log parents selected for recombination.
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Recombining ")+uint2str(lParents->size())+std::string(" individuals together")
	);

	// Do recombination operation on parent and get the resulting child.
	Individual::Handle lChildIndiv = recombine(*lParents, ioContext);
	if(lChildIndiv->getFitness() != NULL) {
		lChildIndiv->getFitness()->setInvalid();
	}

	// Log information to history, if it is used.
	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
	if(lHistory != NULL) {
		std::vector<HistoryID> lParentNames;
		for(unsigned int i=0; i<lParents->size(); ++i) {
			HistoryID::Handle lHID = castHandleT<HistoryID>(lParents->at(i)->getMember("HistoryID"));
			if(lHID != NULL) lParentNames.push_back(*lHID);
		}
		lHistory->incrementHistoryVar(*lChildIndiv);
		lHistory->trace(ioContext, lParentNames, lChildIndiv, getName(), "recombination");
	}

	return lChildIndiv;
	Beagle_StackTraceEndM();
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
double RecombinationOp::getBreedingProba(BreederNode::Handle)
{
	Beagle_StackTraceBeginM();
	return mRecombProba->getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void RecombinationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	BreederOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Number of individuals used for each recombination. If this parameter is set to 0, ";
		lOSS << "the number of individual recombined is equal to the complete population size.";
		Register::Description lDescription(
		    "Number of individuals recombined",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mNumberRecomb = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry(mNumberRecombName, new UInt(0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Individual recombination probability",
		    "Float",
		    "0.5",
		    "Single individual recombination probability for a generation."
		);
		mRecombProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mRecombProbaName, new Double(0.5f), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the recombination operation on the deme.
 *  \param ioDeme Current deme of individuals to recombine.
 *  \param ioContext Context of the evolution.
 */
void RecombinationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_ValidateParameterM(mRecombProba->getWrappedValue()>=0.0, mRecombProbaName, "<0");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Recombining individuals of the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Recombining individuals with probability ")+
	    dbl2str(mRecombProba->getWrappedValue())
	);

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
	Individual::Bag lRecombinedIndiv(ioDeme.size());
	for(unsigned int i=0; i<lRecombinedIndiv.size(); ++i) {
		if(ioContext.getSystem().getRandomizer().rollUniform() <= mRecombProba->getWrappedValue()) {
			Individual::Bag::Handle lParents = new Individual::Bag;
			const unsigned int lNbGenerated =
			    (mNumberRecomb->getWrappedValue()==0) ? ioDeme.size() :
			    minOf<unsigned int>(ioDeme.size(), mNumberRecomb->getWrappedValue());
			if(lNbGenerated == ioDeme.size()) (*lParents) = ioDeme;
			else {
				std::vector<unsigned int> lIndices(ioDeme.size());
				for(unsigned int j=0; j<lIndices.size(); ++j) lIndices[j] = j;
				std::random_shuffle(lIndices.begin(), lIndices.end(),
				                    ioContext.getSystem().getRandomizer());
				for(unsigned int j=0; j<lNbGenerated; ++j) {
					lParents->push_back(ioDeme[lIndices[j]]);
				}
			}
			lRecombinedIndiv[i] = recombine(*lParents, ioContext);
			if(lRecombinedIndiv[i]->getFitness() != NULL) {
				lRecombinedIndiv[i]->getFitness()->setInvalid();
			}
			// Log and update history.
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    std::string("The ")+uint2ordinal(i+1)+
			    std::string(" individual as been replaced by recombination")
			);
			if(lHistory != NULL) {
				std::vector<HistoryID> lParentNames;
				for(unsigned int j=0; j<lParents->size(); ++j) {
					HistoryID::Handle lHID = castHandleT<HistoryID>(lParents->at(j)->getMember("HistoryID"));
					if(lHID != NULL) lParentNames.push_back(*lHID);
				}
				lHistory->incrementHistoryVar(*lRecombinedIndiv[i]);
				lHistory->trace(ioContext, lParentNames, lRecombinedIndiv[i], getName(), "recombination");
			}
		}
	}
	for(unsigned int i=0; i<ioDeme.size(); ++i) {
		if(lRecombinedIndiv[i] != NULL) ioDeme[i] = lRecombinedIndiv[i];
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a recombination operator from XML tree.
 *  \param inIter XML iterator to use to read recombination operator.
 *  \param ioSystem Evolutionary system.
 */
void RecombinationOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lRecombProbaReadName = inIter->getAttribute("recombpb");
	if(lRecombProbaReadName.empty()==false) mRecombProbaName = lRecombProbaReadName;
	std::string lNumberRecombReadName = inIter->getAttribute("nbrindrecomb");
	if(lNumberRecombReadName.empty()==false) mNumberRecombName = lNumberRecombReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write recombination operator content into XML streamer.
 *  \param ioStreamer XML streamer to write recombination operator into.
 *  \param inIndent Whether output should be indented.
 */
void RecombinationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("recombpb", mRecombProbaName);
	ioStreamer.insertAttribute("nbrindrecomb", mNumberRecombName);
	Beagle_StackTraceEndM();
}
