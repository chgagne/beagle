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
 *  \file   Beagle/EC/InitializationOp.cpp
 *  \brief  Source code of class InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.39 $
 *  $Date: 2007/08/31 13:34:01 $
 */

#include "Beagle/EC.hpp"

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a plain new initialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the initialization operator.
 */
InitializationOp::InitializationOp(std::string inReproProbaName, std::string inName) :
	Beagle::BreederOp(inName),
	mReproProbaName(inReproProbaName)
{ }


/*!
 *  \brief Apply the breeding operation on a breeding pool, returning a bred individual.
 *  \param inBreedingPool
 *  \param inChild
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Bred individual.
 */
Individual::Handle InitializationOp::breed(Individual::Bag& inBreedingPool,
        BreederNode::Handle inChild,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();
	const Factory& lFactory = ioContext.getSystem().getFactory();
	Individual::Alloc::Handle lIndivAlloc =
	    castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
	Individual::Handle lNewIndiv = castHandleT<Individual>(lIndivAlloc->allocate());
	initIndividual(*lNewIndiv, ioContext);
	if(lNewIndiv->getFitness() != NULL) lNewIndiv->getFitness()->setInvalid();
	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
	if(lHistory != NULL) {
		lHistory->incrementHistoryVar(*lNewIndiv);
		lHistory->trace(ioContext, std::vector<HistoryID>(), lNewIndiv, getName(), "initialization");
	}
	ioContext.setIndividualHandle(lNewIndiv);
	return lNewIndiv;
	Beagle_StackTraceEndM();
}


/*!
 *  \return Return selection probability of breeder operator.
 */
double InitializationOp::getBreedingProba(BreederNode::Handle)
{
	Beagle_StackTraceBeginM();
	return mReproductionProba->getWrappedValue();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the paramaters of this operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void InitializationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	BreederOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Probability that an individual is reproducted as is, without modification. ";
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
	}
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
		lOSS << "Name of file to use for seeding the evolution ";
		lOSS << "with crafted individual. An empty string means no seeding.";
		Register::Description lDescription(
		    "Seeds individual filename",
		    "String",
		    "",
		    lOSS.str()
		);
		mSeedsFile = castHandleT<String>(
		                 ioSystem.getRegister().insertEntry("ec.init.seedsfile", new String(""), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the initialization operation on the deme.
 *  \param ioDeme Current deme of individuals to initialize.
 *  \param ioContext Context of the evolution.
 */
void InitializationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

#ifndef BEAGLE_NDEBUG
	if(ioContext.getVivariumHandle()!=NULL) {
		Beagle_AssertM(mPopSize->size() == ioContext.getVivarium().size());
	}
#endif // BEAGLE_NDEBUG

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Initializing the ")+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	if(!ioDeme.empty()) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    std::string("Warning!  Applying '")+getName()+"' will overwrite the "+
		    uint2str(ioDeme.size())+" individual(s) currently in the deme with newly initialized "+
		    "individuals.  If this is not what you desire consider using OversizeOp instead."
		);
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Resizing the deme from ")+
	    uint2str(ioDeme.size())+" to "+
	    uint2str((*mPopSize)[ioContext.getDemeIndex()])+" individuals"
	);

	const Factory& lFactory = ioContext.getSystem().getFactory();
	Individual::Alloc::Handle lIndividualAlloc =
	    castHandleT<Individual::Alloc>(lFactory.getConceptAllocator("Individual"));
	ioDeme.clear();
	const unsigned int lDemeSize = (*mPopSize)[ioContext.getDemeIndex()];
	for(unsigned int i=0; i<lDemeSize; ++i) {
		ioDeme.push_back(lIndividualAlloc->allocate());
	}

	unsigned int lSeededIndividuals = 0;
	if(mSeedsFile->getWrappedValue().empty() == false) {
		Beagle_LogInfoM(
		    ioContext.getSystem().getLogger(),
		    std::string("Reading seeds file '")+mSeedsFile->getWrappedValue()+
		    std::string("' to initialize the ")+uint2ordinal(ioContext.getDemeIndex()+1)+
		    std::string(" deme")
		);
		lSeededIndividuals = readSeeds(mSeedsFile->getWrappedValue(), ioDeme, ioContext);
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    uint2str(lSeededIndividuals)+std::string(" individuals read to seed the deme")
		);
	}

	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();
	for(unsigned int i=lSeededIndividuals; i<ioDeme.size(); ++i) {
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    std::string("Initializing the ")+uint2ordinal(i+1)+" individual"
		);
		ioContext.setIndividualHandle(ioDeme[i]);
		ioContext.setIndividualIndex(i);
		initIndividual(*ioDeme[i], ioContext);
		if(ioDeme[i]->getFitness() != NULL) {
			ioDeme[i]->getFitness()->setInvalid();
		}
		if(lHistory != NULL) {
			lHistory->incrementHistoryVar(*ioDeme[i]);
			lHistory->trace(ioContext, std::vector<HistoryID>(), ioDeme[i], getName(), "initialization");
		}
	}
	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read individuals from a given file to seed the evolution.
 *  \param inFileName File name to read seeds individual from.
 *  \param ioDeme Deme to initialize with seeds.
 *  \param ioContext Evolutionary context.
 *  \return Number of seeds read.
 *
 *  Seeds file format is quite simple: XML file with "Beagle" root tag, and then a
 *  "Seeds" tag with in it the individuals representation to read. Here is a
 *  seeds file example with one individual in it.
 *  \verbatim
    <?xml version="1.0" encoding="ISO-8859-1"?>
    <Beagle>
      <Seeds>
        <Individual>
          <Genotype type="bitstring">11111</Genotype>
        </Individual>
      </Seeds>
    </Beagle>
    \endverbatim
 *  If there is less individuals in the seed file than the population size, the
 *  remaining individuals are initialized as usual. If there is more individuals
 *  than needed, the last individuals of the seeds file are ignored. If more than one
 *  deme is used in the evolution, each deme will be seeded with the same seeds file.
 *
 */
unsigned int InitializationOp::readSeeds(std::string inFileName,
        Deme& ioDeme,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_LIBZ
	igzstream lIFStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lIFStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	PACC::XML::Document lParser;
	try {
		lParser.parse(lIFStream, inFileName);
	} catch(IOException& inExcept) {
		std::ostringstream lOSS;
		lOSS << "The seeds file is invalid: " << inExcept.what();
		throw Beagle_IOExceptionMessageM(lOSS.str());
	}

	unsigned int lReadIndividuals=0;
	unsigned int lOldIndivIndex = ioContext.getIndividualIndex();
	Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
	for(PACC::XML::ConstIterator lNode=lParser.getFirstRoot(); lNode; ++lNode) {
		if((lNode->getType()==PACC::XML::eData) && (lNode->getValue()=="Beagle")) {
			for(PACC::XML::ConstIterator lChild=lNode->getFirstChild(); lChild; ++lChild) {
				if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Seeds")) {
					for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
						if((lChild2->getType()==PACC::XML::eData) && (lChild2->getValue()=="Individual")) {
							if(lReadIndividuals >= ioDeme.size()) break;
							Beagle_NonNullPointerAssertM(ioDeme[lReadIndividuals]);
							ioContext.setIndividualIndex(lReadIndividuals);
							ioContext.setIndividualHandle(ioDeme[lReadIndividuals]);
							Beagle_LogVerboseM(
							    ioContext.getSystem().getLogger(),
							    std::string("Reading the ")+uint2ordinal(lReadIndividuals+1)+
							    std::string(" individual from seeds file")
							);
							ioDeme[lReadIndividuals]->readWithContext(lChild2, ioContext);
							Beagle_LogDebugM(ioContext.getSystem().getLogger(), *ioDeme[lReadIndividuals]);
							++lReadIndividuals;
						}
					}
				}
			}
		}
	}
	ioContext.setIndividualHandle(lOldIndivHandle);
	ioContext.setIndividualIndex(lOldIndivIndex);
	return lReadIndividuals;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a initialization operator from a XML subtree.
 *  \param inIter XML iterator to use to read initialization operator.
 *  \param ioSystem Evolutionary system.
 */
void InitializationOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
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
 *  \brief Write initialization operator into XML streamer.
 *  \param ioStreamer XML streamer to write initialization operator into.
 *  \param inIndent Whether output should be indented.
 */
void InitializationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("repropb", mReproProbaName);
	Beagle_StackTraceEndM();
}
