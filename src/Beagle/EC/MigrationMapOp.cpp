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
 *  \file   Beagle/EC/MigrationMapOp.cpp
 *  \brief  Source code of class MigrationMapOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.19 $
 *  $Date: 2007/08/17 18:09:14 $
 */


#include "Beagle/EC.hpp"

using namespace std;
using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a MigrationMapOp.
 *  \param inName Name of operator.
 */
MigrationMapOp::MigrationMapOp(std::string inName) :
	Beagle::EC::MigrationOp(inName),
	mSelectionOp(new Beagle::EC::SelectRandomOp),
	mReplacementOp(NULL)
{ }


/*!
 *  \brief Construct a MigrationMapOp.
 *  \param inSelectionOp Selection operator used to choose emigrants.
 *  \param inReplacementOp Selection operator to choose individuals to
 *    be replaced by immigrants (if NULL, individuals selected for migration will be replaced).
 *  \param inName Name of the operator,
 */
MigrationMapOp::MigrationMapOp(SelectionOp::Handle inSelectionOp,
                               SelectionOp::Handle inReplacementOp,
                               std::string inName) :
	Beagle::EC::MigrationOp(inName),
	mSelectionOp(inSelectionOp),
	mReplacementOp(inReplacementOp)
{ }


/*!
 *  \brief Log the links of the migration map.
 *  \param inMigrationMap Migration map logged
 *  \param ioSystem Evolutionary system (used to obtain logger)
 */
void MigrationMapOp::logMap(const std::vector< std::vector<unsigned int> >& inMigrationMap,
                            System& ioSystem) const
{
	Beagle_StackTraceBeginM();

	Beagle_LogDetailedM(
	    ioSystem.getLogger(),
	    string("'")+getName()+"' will use '"+mSelectionOp->getName()+"' for selection"
	);
	if (mReplacementOp!=NULL) {
		// Have replacement operator
		Beagle_LogDetailedM(
		    ioSystem.getLogger(),
		    string("'")+getName()+"' will use '"+mReplacementOp->getName()+"' for replacement"
		);
	} else {
		// Do not have a replacement operator
		Beagle_LogTraceM(
		    ioSystem.getLogger(),
		    string("'")+getName()+"' does not have a replacement operator installed"
		);
	}

	std::ostringstream lOSS;
	for(unsigned int i=0; i<inMigrationMap.size(); ++i) {
		std::ostringstream lOSS;
		lOSS << "Emigration connections for deme " << i << " are: ";
		const unsigned int lNbNeighborsI = inMigrationMap[i].size();
		for(unsigned int j=0; j<lNbNeighborsI; ++j) {
			lOSS << inMigrationMap[i][j];
			if(j != (lNbNeighborsI-1)) lOSS << ",";
		}
		Beagle_LogTraceM(
		    ioSystem.getLogger(),
		    lOSS.str()
		);
	}

	Beagle_StackTraceEndM();
}



/*!
 *  \brief Migrate individuals of a deme.
 *  \param ioDeme Deme containing individuals to migrate.
 *  \param ioContext Evolution context.
 */
void MigrationMapOp::migrate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Count number of emigrants.
	Beagle_AssertM(mMigrationMap.size() > ioContext.getDemeIndex());
	const std::vector<unsigned int>& lMigrationConn = mMigrationMap[ioContext.getDemeIndex()];
	unsigned int lNbEmigrants = 0;
	for(unsigned int i=0; i<lMigrationConn.size(); ++i) lNbEmigrants += lMigrationConn[i];
	if(lNbEmigrants > ioDeme.size()) {
		std::ostringstream lOSS;
		lOSS << "Number of emigrants for deme " << ioContext.getDemeIndex();
		lOSS << " according to migration map (" << lNbEmigrants;
		lOSS << ") is higher than the number of individuals in actual deme (" << ioDeme.size();
		lOSS << "); could not proceed to migration.";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}

	// Get index of individuals to migrate.
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Selecting ")+uint2str(lNbEmigrants)+" individuals for migration from "+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme using '"+mSelectionOp->getName()+"'"
	);
	std::vector<unsigned int> lEmigrantIndices;
	const unsigned int lOldSelectionIndex = ioContext.getSelectionIndex();
	ioContext.setSelectionIndex(0);
	mSelectionOp->selectManyIndividuals(lNbEmigrants, ioDeme, ioContext, lEmigrantIndices);
	mSelectionOp->convertToList(lNbEmigrants, lEmigrantIndices);
	std::random_shuffle(lEmigrantIndices.begin(), lEmigrantIndices.end(),
	                    ioContext.getSystem().getRandomizer());
	std::vector<unsigned int> lReplacedIndices;
	if(mReplacementOp == NULL) lReplacedIndices = lEmigrantIndices;
	else {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    std::string("Selecting ")+uint2str(lNbEmigrants)+" individuals for replacement from "+
		    uint2ordinal(ioContext.getDemeIndex()+1)+" deme using '"+mReplacementOp->getName()+"'"
		);
		ioContext.setSelectionIndex(0);
		mReplacementOp->selectManyIndividuals(lNbEmigrants, ioDeme, ioContext, lEmigrantIndices);
		mReplacementOp->convertToList(lNbEmigrants, lEmigrantIndices);
		std::random_shuffle(lReplacedIndices.begin(), lReplacedIndices.end(),
		                    ioContext.getSystem().getRandomizer());
	}
	ioContext.setSelectionIndex(lOldSelectionIndex);

	MigrationBuffer::Handle lMigBuffer =
	    castHandleT<MigrationBuffer>(ioDeme.getMember("MigrationBuffer"));
	if(lMigBuffer == NULL) {
		const Factory& lFactory = ioContext.getSystem().getFactory();
		MigrationBuffer::Alloc::Handle lMigBufferAlloc =
		    castHandleT<MigrationBuffer::Alloc>(lFactory.getConceptAllocator("MigrationBuffer"));
		lMigBuffer = castHandleT<MigrationBuffer>(lMigBufferAlloc->allocate());
		ioDeme.addMember(lMigBuffer);
	}

	// Do effective migration.
	lMigBuffer->insertEmigrants(lEmigrantIndices, ioDeme, ioContext);
	lMigBuffer->insertReplaced(lReplacedIndices, ioDeme);
	const unsigned int lNbDeme = ioContext.getVivarium().size();
	Beagle_AssertM(lNbDeme == lMigrationConn.size());
	for(unsigned int i=0; i<lNbDeme; ++i) {
		if(lMigrationConn[i] > 0) {
			std::ostringstream lOSS;
			lOSS << "Migrating " << lMigrationConn[i] << " individuals from the ";
			lOSS << uint2ordinal(ioContext.getDemeIndex()+1) << " deme to the ";
			lOSS << uint2ordinal(i+1) << " deme";
			Beagle_LogTraceM(
			    ioContext.getSystem().getLogger(),
			    lOSS.str()
			);
			Deme& lDestDeme = *ioContext.getVivarium()[i];
			lMigBuffer->moveMigrants(lMigrationConn[i], lDestDeme, ioContext);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem Evolutionary system.
 */
void MigrationMapOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::EC::MigrationOp::init(ioSystem);
	logMap(mMigrationMap, ioSystem);
	validateMap(mMigrationMap, ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MigrationMapOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MigrationOp::registerParams(ioSystem);
	if(mSelectionOp!=NULL)
		mSelectionOp->registerParams(ioSystem);
	if(mReplacementOp!=NULL)
		mReplacementOp->registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read migration map classes from configuration file.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem Evolutionary system.
 *
 *  This method reads 'select' and 'replace' attributes so that
 *  mSelectionOp and mReplacementOp can be specified from a
 *  configuration file.
 */
void MigrationMapOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MigrationOp::readWithSystem(inIter, ioSystem);
	if (inIter->isDefined("select")) {
		std::string lSelectOpName = inIter->getAttribute("select");
		Operator::Alloc::Handle lOpAlloc =
		    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lSelectOpName));
		if(lOpAlloc == NULL) {
			std::ostringstream lOSS;
			lOSS << "Operator '" << lSelectOpName << "' not found in operator map!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		mSelectionOp = castHandleT<SelectionOp>(lOpAlloc->allocate());
		mSelectionOp->setName(lSelectOpName);
	}

	if (inIter->isDefined("replace")) {
		std::string lReplaceOpName = inIter->getAttribute("replace");
		Operator::Alloc::Handle lOpAlloc =
		    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lReplaceOpName));
		if(lOpAlloc == NULL) {
			std::ostringstream lOSS;
			lOSS << "Operator '" << lReplaceOpName << "' not found in operator map!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		mReplacementOp = castHandleT<SelectionOp>(lOpAlloc->allocate());
		mReplacementOp->setName(lReplaceOpName);
	}
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Validate the migration map.
 *  \param inMigrationMap Migration map to validate.
 *  \param ioSystem Evolutionary system.
 *  \throw Beagle::ValidationException If the migration map is invalid.
 */
void MigrationMapOp::validateMap(const std::vector< std::vector<unsigned int> >& inMigrationMap,
                                 System& ioSystem) const
{
	Beagle_StackTraceBeginM();

	// Be sure that the size of the migration map fit the population size.
	const unsigned int lNbDeme = mPopSize->size();
	if(lNbDeme != inMigrationMap.size()) {
		std::ostringstream lOSS;
		lOSS << "Size of migration map (" << inMigrationMap.size() << ") does not correspond to ";
		lOSS << "number of demes in population (" << lNbDeme << "); validation of migration map failed.";
		throw Beagle::ValidationException(lOSS.str());
	}

	// Count number of emigrants and immigrants for each deme, be sure that it is correctly balanced.
	std::vector<unsigned int> lIngoing(lNbDeme, 0);
	std::vector<unsigned int> lOutgoing(lNbDeme, 0);
	for(unsigned int i=0; i<inMigrationMap.size(); ++i) {
		const unsigned int lMigMapISize = inMigrationMap[i].size();
		if(lMigMapISize != inMigrationMap.size()) {
			std::ostringstream lOSS;
			lOSS << "Topology of migration map is not square, for example the number emigration entries ";
			lOSS << "for the " << uint2ordinal(i+1);
			lOSS << " deme (" <<  lMigMapISize << ") differ from the total number of demes in ";
			lOSS << "the population (" << inMigrationMap.size() << "); ";
			lOSS << "validation of migration map failed.";
			throw Beagle::ValidationException(lOSS.str());
		}
		for(unsigned int j=0; j<lMigMapISize; ++j) {
			lIngoing[i] += inMigrationMap[i][j];
			lOutgoing[j] += inMigrationMap[i][j];
		}
	}
	for(unsigned int i=0; i<lIngoing.size(); ++i) {
		if(lIngoing[i] != lOutgoing[i]) {
			std::ostringstream lOSS;
			lOSS << "Number of emigrants (outgoing) individual for the " << uint2ordinal(i+1);
			lOSS << " deme (" << lOutgoing[i] << ") mismatch the number of immigrants (ingoing) ";
			lOSS << "individuals in this deme (" << lOutgoing[i] << "); ";
			lOSS << "validation of migration map as it requires the number emigrants/immigrants to ";
			lOSS << "be balanced for each deme.";
			throw Beagle::ValidationException(lOSS.str());
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write breeder operator into XML streamer.
 *  \param ioStreamer XML streamer to write breeder operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void MigrationMapOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 *
 *  Writes two attributes: "select" and "replace" that declare the
 *  selection and replacement operators that were installed at the
 *  time of calling write().
 */
void MigrationMapOp::writeContent(PACC::XML::Streamer &ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if (mSelectionOp!=NULL) {
		ioStreamer.insertAttribute("select",mSelectionOp->getName());
	}
	if (mReplacementOp!=NULL) {
		ioStreamer.insertAttribute("replace",mReplacementOp->getName());
	}
	Beagle_StackTraceEndM();
}
