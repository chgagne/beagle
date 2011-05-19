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
 *  \file   Beagle/EC/MutationOp.cpp
 *  \brief  Source code of class MutationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.27 $
 *  $Date: 2007/08/31 13:34:01 $
 */

#include "Beagle/EC.hpp"

#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif //BEAGLE_HAVE_OPENMP

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a plain new mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inName Name of the mutation operator.
 */
MutationOp::MutationOp(std::string inMutationPbName, std::string inName) :
	BreederOp(inName),
	mMutationPbName(inMutationPbName)
{}


/*!
 *  \brief Apply the mutation operation on a breeding pool, returning a mutated individual.
 *  \param inBreedingPool Breeding pool to use for the mutation operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the mutation operation.
 *  \return Mutated individual.
 */
Individual::Handle MutationOp::breed(Individual::Bag& inBreedingPool,
                                     BreederNode::Handle inChild,
                                     Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	Individual::Handle lIndiv =
	    inChild->getBreederOp()->breed(inBreedingPool, inChild->getFirstChild(), ioContext);

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Mutating the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+" individual"
	);

	if(lIndiv != NULL) {
		bool lMutated = mutate(*lIndiv, ioContext);
		if(lMutated) {
			if(lIndiv->getFitness() != NULL) lIndiv->getFitness()->setInvalid();
			History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
			if(lHistory != NULL) {
				std::vector<HistoryID> lParent;
				HistoryID::Handle lHID = castHandleT<HistoryID>(lIndiv->getMember("HistoryID"));
				if(lHID != NULL) lParent.push_back(*lHID);
				lHistory->incrementHistoryVar(*lIndiv);
				lHistory->trace(ioContext, lParent, lIndiv, getName(), "mutation");
			}
		}
	}

	return lIndiv;
	Beagle_StackTraceEndM();
}


/*!
 *  \return Return selection probability of breeder operator.
 */
double MutationOp::getBreedingProba(BreederNode::Handle)
{
	Beagle_StackTraceBeginM();

	return mMutationProba->getWrappedValue();

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void MutationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	BreederOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Individual mutation probability",
		    "Double",
		    "0.1",
		    "Mutation probability for a single individual."
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.1f), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the mutation operation on the deme.
 *  \param ioDeme Current deme of individuals to mutate.
 *  \param ioContext Context of the evolution.
 */
void MutationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Mutating individuals of the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    std::string("Mutating individuals with probability ")+
	    dbl2str(mMutationProba->getWrappedValue())
	);

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	const int lSize = ioDeme.size();

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
	static OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioContext.getSystem().getComponent("OpenMP"));
	const Factory& lFactory = ioContext.getSystem().getFactory();
	const std::string& lContextName = lFactory.getConceptTypeName("Context");
	Context::Alloc::Handle lContextAlloc = castHandleT<Context::Alloc>(lFactory.getAllocator(lContextName));
	Context::Bag lContexts(lOpenMP->getMaxNumThreads());
	for(unsigned int i = 0; i <  lOpenMP->getMaxNumThreads(); ++i)
		lContexts[i] = castHandleT<Context>(lContextAlloc->clone(ioContext));
#ifdef BEAGLE_USE_OMP_NR
#pragma omp parallel for schedule(dynamic)
#elif defined(BEAGLE_USE_OMP_R)
	const int lChunkSize = std::max((int)(lSize / lOpenMP->getMaxNumThreads()), 1);
#pragma omp parallel for schedule(static, lChunkSize)
#endif
#endif
	for(int i=0; i<lSize; ++i) {
		if(ioContext.getSystem().getRandomizer().rollUniform() <= mMutationProba->getWrappedValue()) {
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
			lContexts[lOpenMP->getThreadNum()]->setIndividualIndex(i);
			lContexts[lOpenMP->getThreadNum()]->setIndividualHandle(ioDeme[i]);
#else
			ioContext.setIndividualIndex(i);
			ioContext.setIndividualHandle(ioDeme[i]);
#endif

			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    std::string("Mutating the ")+uint2ordinal(i+1)+" individual"
			);

#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
			if(mutate(*ioDeme[i], *lContexts[lOpenMP->getThreadNum()])) {
#else
			if(mutate(*ioDeme[i], ioContext)) {
#endif
				if(ioDeme[i]->getFitness() != NULL) {
					ioDeme[i]->getFitness()->setInvalid();
				}
				if(lHistory != NULL) {
#pragma omp critical (Beagle_History)
					{
						std::vector<HistoryID> lParent;
						HistoryID::Handle lHID = castHandleT<HistoryID>(ioDeme[i]->getMember("HistoryID"));
						if(lHID != NULL) lParent.push_back(*lHID);
						lHistory->incrementHistoryVar(*ioDeme[i]);
#if defined(BEAGLE_USE_OMP_NR) || defined(BEAGLE_USE_OMP_R)
						lHistory->trace(*lContexts[lOpenMP->getThreadNum()], lParent, ioDeme[i], getName(), "mutation");
#else
						lHistory->trace(ioContext, lParent, ioDeme[i], getName(), "mutation");
#endif
					}
				}
			}
		}
	}

	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a mutation operator from XML tree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param ioSystem Evolutionary system.
 */
void MutationOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string mMutationPbReadName = inIter->getAttribute("mutationpb");
	if(mMutationPbReadName.empty() == false) mMutationPbName = mMutationPbReadName;

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write mutation operator content into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent
 */
void MutationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.insertAttribute("mutationpb", mMutationPbName);

	Beagle_StackTraceEndM();
}

