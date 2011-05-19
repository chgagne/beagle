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
 *  \file   Beagle/EC/SelectionOp.cpp
 *  \brief  Source code of class SelectionOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.33 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

#include <vector>

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif // BEAGLE_HAVE_OPENMP

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a new selection operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
SelectionOp::SelectionOp(std::string inReproProbaName, std::string inName) :
	Beagle::BreederOp(inName),
	mReproProbaName(inReproProbaName)
{ }


/*!
 *  \brief Apply the breeding operation on a breeding pool, returning a bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Bred individual.
 */
Individual::Handle SelectionOp::breed(Individual::Bag& inBreedingPool,
                                      BreederNode::Handle inChild,
                                      Context& ioContext)
{
	Beagle_StackTraceBeginM();
	unsigned int lSelectedIndiv = selectOneIndividual(inBreedingPool, ioContext);
	Beagle_AssertM(lSelectedIndiv < inBreedingPool.size());
	const Factory& lFactory = ioContext.getSystem().getFactory();
	Individual::Handle lOriginalIndiv = inBreedingPool[lSelectedIndiv];
	std::string lIndividualType = lOriginalIndiv->getType();
	Individual::Alloc::Handle lIndividualAlloc =
	    castHandleT<Individual::Alloc>(lFactory.getAllocator(lIndividualType));
	Individual::Handle lClonedIndiv = castHandleT<Individual>(lIndividualAlloc->allocate());
	Beagle_NonNullPointerAssertM(lClonedIndiv);
	lClonedIndiv->copy(*lOriginalIndiv, ioContext.getSystem());
	ioContext.setIndividualIndex(lSelectedIndiv);
	ioContext.setIndividualHandle(lClonedIndiv);

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
	if(lHistory != NULL) {
		HistoryID::Handle lHID = castHandleT<HistoryID>(lOriginalIndiv->getMember("HistoryID"));
		std::vector<HistoryID> lParent;
		if(lHID != NULL) lParent.push_back(*lHID);
		lHistory->allocateNewID(*lClonedIndiv);
		lHistory->trace(ioContext, lParent, lClonedIndiv, getName(), "selection");
	}

	return lClonedIndiv;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Converts from a selection table (as produced by selectNIndividuals()) to a selection list.
 *  \param inN Number of selected individuals.
 *  \param ioSelections The selection table to be converted to a selection list.
 */
void SelectionOp::convertToList(unsigned int inN, std::vector<unsigned int>& ioSelections)
{
	Beagle_StackTraceBeginM();
	unsigned int lLength = ioSelections.size();
	ioSelections.reserve(lLength+inN);
	for (unsigned int i=0; i<lLength; i++) {
		if(ioSelections[i]>0) {
			ioSelections.insert(ioSelections.end(),ioSelections[i],i);
		}
	}
	ioSelections.erase(ioSelections.begin(), ioSelections.begin()+lLength);
	if (ioSelections.size() != inN) {
		throw Beagle_RunTimeExceptionM
		(std::string("After converting from a selection table to a selection list, the final")+
		 " number of individuals in the list ("+uint2str(ioSelections.size())+") did not"+
		 " match the expected number ("+uint2str(inN)+").");
	}

	Beagle_StackTraceEndM();
}



/*!
 *  \return Return selection probability of breeder operator.
 */
double SelectionOp::getBreedingProba(BreederNode::Handle)
{
	Beagle_StackTraceBeginM();
	return mReproductionProba->getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void SelectionOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::BreederOp::registerParams(ioSystem);
	std::ostringstream lOSS;
	lOSS << "Probability than an individual is reproducted as is, without modification. ";
	lOSS << "This parameter is useful only in selection and initialization operators that ";
	lOSS << "are composing a breeder tree.";
	Register::Description lDescription(
	    "Reproduction probability",
	    "Float",
	    "0.1",
	    lOSS.str()
	);
	mReproductionProba = castHandleT<Double>(
	                         ioSystem.getRegister().insertEntry(mReproProbaName, new Double(0.1f), lDescription));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the selection operation on all the deme.
 *  \param ioDeme Deme to which the selection operation is applied.
 *  \param ioContext Evolutionary context.
 */
void SelectionOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Applying selection on the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);

	if(ioContext.getGeneration() == 0) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    std::string("Note that the reproduction probability parameter named '") + mReproProbaName +
		    std::string("' is ignored when no breeder is set in the evolver, as it seems ") +
		    " actually the case."
		);
	}

	// Select individuals
	std::vector<unsigned int> lIndices(ioDeme.size(), 0);
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	static OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioContext.getSystem().getComponent("OpenMP"));
	int lNumThreads = lOpenMP->getMaxNumThreads();
	int lSize = ioDeme.size();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	const std::string& lContextName = lFactory.getConceptTypeName("Context");
	Context::Alloc::Handle lContextAlloc = castHandleT<Context::Alloc>(lFactory.getAllocator(lContextName));
	Context::Bag lContexts(lNumThreads);
	for(int i = 0; i < lNumThreads; ++i)
		lContexts[i] = castHandleT<Context>(lContextAlloc->clone(ioContext));

	unsigned int lNbIndividualInt = ioDeme.size() / lNumThreads;
	unsigned int lNbIndividualFrac = ioDeme.size() % lNumThreads;
	//	lIndices.resize(ioDeme.size(), 0);

#pragma omp parallel shared(lIndices, lContexts, lNumThreads)
	{
		std::vector<unsigned int> lIndicesVector(lSize, 0);
		unsigned int lNbIndividual = lNbIndividualInt;
		if(lOpenMP->getThreadNum() < lNbIndividualFrac)
			++lNbIndividual;
		selectManyIndividuals(lNbIndividual, ioDeme, *lContexts[lOpenMP->getThreadNum()], lIndicesVector);

#pragma omp critical (Beagle_SelectionOp_Reduce_Indices)
		{
			for(int i = 0; i < lSize; ++i)
				lIndices[i] += lIndicesVector[i];
		}
	}

#else
	selectManyIndividuals(ioDeme.size(),ioDeme,ioContext,lIndices);
#endif

	// Output selection counts to log
	for(unsigned int j=0; j<ioDeme.size(); ++j) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    uint2ordinal(j+1)+" individual selected "+
		    uint2str(lIndices[j])+" times"
		);
	}

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	// Copy the indices array before it is modified in the active-copy section below
	// because it used also for passive copy.  Passive copy is done after active copy
	// because history tracking changes the id of individuals during passive copy.
	std::vector<unsigned int> lIndicesCopy(lIndices);

	unsigned int lNextEmpty  = 0;
	unsigned int lNextFilled = 0;
	while((lNextFilled < ioDeme.size()) && (lIndices[lNextFilled] <= 1)) lNextFilled++;
	while(lNextFilled < ioDeme.size()) {
		while(lIndices[lNextFilled] > 1) {
			while(lIndices[lNextEmpty] != 0) lNextEmpty++;
			// This is an 'active copy' : selected individuals replace unselected individuals
			ioDeme[lNextEmpty]->copy(*ioDeme[lNextFilled], ioContext.getSystem());
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    uint2ordinal(lNextFilled+1)+" individual has replaced the "+
			    uint2ordinal(lNextEmpty+1)+" individual"
			);

			if(lHistory != NULL) {
				HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[lNextEmpty]->getMember("HistoryID"));
				std::vector<HistoryID> lParent;
				if(lHID != NULL) lParent.push_back(*lHID);
				lHistory->allocateNewID(*ioDeme[lNextEmpty]);
				lHistory->trace(ioContext, lParent, ioDeme[lNextEmpty], getName(), "selection");
			}
			lIndices[lNextFilled]--;
			lIndices[lNextEmpty]++;
		}
		while((lNextFilled < ioDeme.size()) && (lIndices[lNextFilled] <= 1)) lNextFilled++;
	}

	// Log the individuals that will be passively copied
	for(unsigned int j=0; j<ioDeme.size(); ++j) {
		if (lIndicesCopy[j] != 0) {
			Beagle_LogDebugM(
			    ioContext.getSystem().getLogger(),
			    uint2ordinal(j+1)+" individual will be passively copied into the new generation"
			);
			if (lHistory != NULL) {
				HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[j]->getMember("HistoryID"));
				std::vector<HistoryID> lParent;
				if(lHID != NULL) lParent.push_back(*lHID);
				lHistory->allocateNewID(*ioDeme[j]);
				lHistory->trace(ioContext, lParent, ioDeme[j], getName(), "selection");
			}
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a selection operator from a XML subtree.
 *  \param inIter XML iterator to use to read selection operator.
 *  \param ioSystem Evolutionary system.
 */
void SelectionOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string mReproProbaReadName = inIter->getAttribute("repropb");
	if(mReproProbaReadName.empty() == false) mReproProbaName = mReproProbaReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Select many individuals
 *  \param inN Number of individuals to select.
 *  \param ioPool Pool from which the individuals are choosen.
 *  \param ioContext Evolutionary context.
 *  \param outSelections Vector of unsigned ints that say how often an individual was selected.
 *
 *  The output of this method is via outSelection.  It produces a
 *  vector the same size as ioPool, where each index says how many
 *  times an individual was selected.  If this is not the desired
 *  output, consider using the method convertToList().
 */
void SelectionOp::selectManyIndividuals(unsigned int inN,
                                        Individual::Bag& ioPool,
                                        Context& ioContext,
                                        std::vector<unsigned int>& outSelections)
{
	Beagle_StackTraceBeginM();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Selecting ")+uint2str(inN)+" individuals from a pool of "+uint2str(ioPool.size())+
	    " individuals"
	);

	// Initialize the vector
	outSelections.resize( ioPool.size() );
	for (unsigned int i=0; i<outSelections.size(); i++) {
		outSelections[i]=0;
	}

	// Fill the vector with the number of times an indi was selected
	for (unsigned int i=0; i<inN; i++) {
		outSelections[ selectOneIndividual(ioPool,ioContext) ]++;
	}

	Beagle_StackTraceEndM();
}



/*!
 *  \brief Write selection operator into XML streamer.
 *  \param ioStreamer XML streamer to write selection operator into.
 *  \param inIndent Whether output should be indented.
 */
void SelectionOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("repropb", mReproProbaName);
	Beagle_StackTraceEndM();
}
