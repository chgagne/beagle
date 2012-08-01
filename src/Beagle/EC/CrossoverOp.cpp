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
 *  \file   Beagle/EC/CrossoverOp.cpp
 *  \brief  Source code of class CrossoverOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.31 $
 *  $Date: 2007/08/31 13:34:01 $
 */

#include "Beagle/EC.hpp"

#include <vector>
#include <algorithm>

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a plain new crossover operator.
 *  \param inMatingPbName Mating probability parameter name used in register.
 *  \param inName Name of the crossover operator.
 */
CrossoverOp::CrossoverOp(std::string inMatingPbName, std::string inName) :
	BreederOp(inName),
	mMatingProbaName(inMatingPbName)
{ }


/*!
 *  \brief Apply the crossover operation on a breeding pool, returning a mated individual.
 *  \param inBreedingPool Breeding pool to use for the crossover operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the crossover operation.
 *  \return Mated individual.
 */
Individual::Handle CrossoverOp::breed(Individual::Bag& inBreedingPool,
                                      BreederNode::Handle inChild,
                                      Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioContext.getSystem().getFactory().getConceptAllocator("Context"));
	Context::Handle lContext2 = castHandleT<Context>(lContextAlloc->clone(ioContext));

	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	Individual::Handle lIndiv1 = inChild->getBreederOp()->breed(inBreedingPool,
	                             inChild->getFirstChild(),
	                             ioContext);

	Beagle_NonNullPointerAssertM(inChild->getNextSibling());
	Beagle_NonNullPointerAssertM(inChild->getNextSibling()->getBreederOp());
	Individual::Handle lIndiv2 =
	    inChild->getNextSibling()->getBreederOp()->breed(inBreedingPool,
	            inChild->getNextSibling()->getFirstChild(),
	            *lContext2);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Mating the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
	    std::string(" individual with the ")+uint2ordinal(lContext2->getIndividualIndex()+1)+
	    " individual"
	);

	if((lIndiv1 != NULL) && (lIndiv2 != NULL)) {
		bool lMated = mate(*lIndiv1, ioContext, *lIndiv2, *lContext2);
		if(lMated) {
			if(lIndiv1->getFitness() != NULL) lIndiv1->getFitness()->setInvalid();
			if(lIndiv2->getFitness() != NULL) lIndiv2->getFitness()->setInvalid();
			History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
			if(lHistory != NULL) {
				std::vector<HistoryID> lParents;
				HistoryID::Handle lHID1 = castHandleT<HistoryID>(lIndiv1->getMember("HistoryID"));
				if(lHID1 != NULL) lParents.push_back(*lHID1);
				HistoryID::Handle lHID2 = castHandleT<HistoryID>(lIndiv2->getMember("HistoryID"));
				if(lHID2 != NULL) lParents.push_back(*lHID2);
				lHistory->incrementHistoryVar(*lIndiv1);
				lHistory->trace(ioContext, lParents, lIndiv1, getName(), "crossover");
			}
		}
	}

	return lIndiv1;
	Beagle_StackTraceEndM();
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
double CrossoverOp::getBreedingProba(BreederNode::Handle)
{
	Beagle_StackTraceBeginM();

	return mMatingProba->getWrappedValue();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of the operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void CrossoverOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	BreederOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Individual crossover probability",
	    "Double",
	    "0.5",
	    "Single individual crossover probability for a generation."
	);
	mMatingProba = castHandleT<Double>(
	                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.5), lDescription));

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the crossover operation on the deme.
 *  \param ioDeme Current deme of individuals to mate.
 *  \param ioContext Context of the evolution.
 */
void CrossoverOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Beagle_ValidateParameterM(mMatingProba->getWrappedValue()>=0.0, mMatingProbaName, "<0");

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Mating individuals of the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Mating individuals with probability ")+
	    dbl2str(mMatingProba->getWrappedValue())
	);

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();

	std::vector<unsigned int> lMateVector;
	for(unsigned int i=0; i<ioDeme.size(); i++) {
		if(ioContext.getSystem().getRandomizer().rollUniform() <= mMatingProba->getWrappedValue()) {
			lMateVector.push_back(i);
		}
	}

	std::random_shuffle(lMateVector.begin(), lMateVector.end(),
	                    ioContext.getSystem().getRandomizer());
	if((lMateVector.size() % 2) != 0) lMateVector.pop_back();

	int j = 0;
	int lSize = lMateVector.size();

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	static OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioContext.getSystem().getComponent("OpenMP"));
	const Factory& lFactory = ioContext.getSystem().getFactory();
	const std::string& lContextName = lFactory.getConceptTypeName("Context");
	Context::Alloc::Handle lContextAlloc = castHandleT<Context::Alloc>(lFactory.getAllocator(lContextName));
	Context::Bag lContexts(lOpenMP->getMaxNumThreads());
	Context::Bag lContexts2(lOpenMP->getMaxNumThreads());
	for(unsigned int i = 0; i < lOpenMP->getMaxNumThreads(); ++i) {
		lContexts[i] = castHandleT<Context>(lContextAlloc->clone(ioContext));
		lContexts2[i] = castHandleT<Context>(lContextAlloc->clone(ioContext));
	}
#if defined(BEAGLE_USE_OMP_NR)
#pragma omp parallel for shared(lSize, lMateVector, lHistory, lContexts, lContexts2) private(j) schedule(dynamic)
#elif defined(BEAGLE_USE_OMP_R)
	const int lChunkSize = std::max((int)(lSize / lOpenMP->getMaxNumThreads()), 1);
#pragma omp parallel for shared(lSize, lMateVector, lHistory, lContexts, lContexts2) private(j) schedule(static, lChunkSize)
#endif
#else
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(ioContext.getSystem().getFactory().getConceptAllocator("Context"));
	Context::Handle lContext2 = castHandleT<Context>(lContextAlloc->clone(ioContext));
#endif
	for(j=0; j<lSize; j+=2) {
		unsigned int lFirstMate  = lMateVector[j];
		unsigned int lSecondMate = lMateVector[j+1];

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
		lContexts[lOpenMP->getThreadNum()]->setIndividualIndex(lFirstMate);
		lContexts[lOpenMP->getThreadNum()]->setIndividualHandle(ioDeme[lFirstMate]);
		lContexts2[lOpenMP->getThreadNum()]->setIndividualIndex(lSecondMate);
		lContexts2[lOpenMP->getThreadNum()]->setIndividualHandle(ioDeme[lSecondMate]);
#else
		ioContext.setIndividualIndex(lFirstMate);
		ioContext.setIndividualHandle(ioDeme[lFirstMate]);
		lContext2->setIndividualIndex(lSecondMate);
		lContext2->setIndividualHandle(ioDeme[lSecondMate]);
#endif

		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    std::string("Mating the ")+uint2ordinal(lFirstMate+1)+
		    std::string(" individual with the ")+uint2ordinal(lSecondMate+1)+" individual"
		);

		std::vector<HistoryID> lParents;
		if(lHistory != NULL) {
#pragma omp critical (Beagle_History)
			{
				HistoryID::Handle lHID1 = castHandleT<HistoryID>(ioDeme[lFirstMate]->getMember("HistoryID"));
				if(lHID1 != NULL) lParents.push_back(*lHID1);
				HistoryID::Handle lHID2 = castHandleT<HistoryID>(ioDeme[lSecondMate]->getMember("HistoryID"));
				if(lHID2 != NULL) lParents.push_back(*lHID2);
			}
		}

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
		bool lMated = mate(*ioDeme[lFirstMate], *(lContexts[lOpenMP->getThreadNum()]), *ioDeme[lSecondMate], *(lContexts2[lOpenMP->getThreadNum()]));
#else
		bool lMated = mate(*ioDeme[lFirstMate], ioContext, *ioDeme[lSecondMate], *lContext2);
#endif
		if(lMated) {
			if(ioDeme[lFirstMate]->getFitness() != NULL) {
				ioDeme[lFirstMate]->getFitness()->setInvalid();
			}
			if(ioDeme[lSecondMate]->getFitness() != NULL) {
				ioDeme[lSecondMate]->getFitness()->setInvalid();
			}
			if(lHistory != NULL) {
#pragma omp critical (Beagle_History)
				{
					lHistory->incrementHistoryVar(*ioDeme[lFirstMate]);
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
					lHistory->trace(*lContexts[lOpenMP->getThreadNum()], lParents, ioDeme[lFirstMate], getName(), "crossover");
#else
					lHistory->trace(ioContext, lParents, ioDeme[lFirstMate], getName(), "crossover");
#endif
					lHistory->incrementHistoryVar(*ioDeme[lSecondMate]);
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
					lHistory->trace(*lContexts[lOpenMP->getThreadNum()], lParents, ioDeme[lSecondMate], getName(), "crossover");
#else
					lHistory->trace(ioContext, lParents, ioDeme[lSecondMate], getName(), "crossover");
#endif
				}
			}
		}
	}

	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a crossover operator from XML tree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param ioSystem Evolutionary system.
 */
void CrossoverOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lMatingProbaReadName = inIter->getAttribute("matingpb");
	if(lMatingProbaReadName.empty()==false) mMatingProbaName = lMatingProbaReadName;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write crossover operator content into XML streamer.
 *  \param ioStreamer XML streamer to write crossover operator into.
 *  \param inIndent Whether output should be indented.
 */
void CrossoverOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.insertAttribute("matingpb", mMatingProbaName);

	Beagle_StackTraceEndM();
}

