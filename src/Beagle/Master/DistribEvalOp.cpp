/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/Distrib/Master/src/DistribEvalOp.cpp
 *  \brief  Source code of class DistribEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.10 $
 *  $Date: 2007/10/03 14:27:49 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Build distribution of evaluations operator.
 *  \param inName Name of the distribution of evaluations operator.
 *  \param inDefaultAppName default application's name.  Usually dbeagle-app.
 *  \param inDefaultVersion Client's version
 */
Master::DistribEvalOp::DistribEvalOp(std::string inName,
                                     std::string inDefaultAppName,
                                     std::string inDefaultVersion) :
		Beagle::EvaluationOp(inName),
		mDefaultAppName(inDefaultAppName),
		mDefaultVersion(inDefaultVersion)
{ }


/*!
 *  \brief Register parameters of the distribution of evaluations operator.
 *  \param ioSystem Reference to the system.
 */
void Master::DistribEvalOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EvaluationOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Application name",
		    "String",
		    std::string("'")+mDefaultAppName+std::string("'"),
		    "Name of the application, as registered in the server."
		);
		mAppName = castHandleT<Beagle::String>(
		               ioSystem.getRegister().insertEntry("db.app.name", new Beagle::String(mDefaultAppName), lDescription));
	}
	{
		Register::Description lDescription(
		    "IP server address",
		    "String",
		    "127.0.0.1",
		    "IP address of the master-slave server."
		);
		mServerIP = castHandleT<Beagle::String>(
		                ioSystem.getRegister().insertEntry("db.server.ip", new Beagle::String("127.0.0.1"), lDescription));
	}
	{
		Register::Description lDescription(
		    "IP server port",
		    "UInt",
		    "9123",
		    "IP port of the master-slave server."
		);
		mServerPort = castHandleT<UInt>(
		                  ioSystem.getRegister().insertEntry("db.server.port", new UInt(9123), lDescription));
	}
	{
		Register::Description lDescription(
		    "Communication retry",
		    "UInt",
		    "5",
		    "Number of retry allowed to the server before stopping."
		);
		mNumberRetry = castHandleT<UInt>(
		                   ioSystem.getRegister().insertEntry("db.comm.retry", new UInt(5), lDescription));
	}
	{
		Register::Description lDescription(
		    "Compression level",
		    "UInt",
		    "3",
		    "Evolver communication compression."
		);
		mCompressionLevel = castHandleT<UInt>(
		                        ioSystem.getRegister().insertEntry("db.evolver.compression", new UInt(0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Client version",
		    "String",
		    std::string("'") + mDefaultVersion + std::string("'"),
		    "Client version, to tell the server if the current version is up-to-date."
		);
		mVersion = castHandleT<Beagle::String>(
		               ioSystem.getRegister().insertEntry("db.client.version", new Beagle::String(mDefaultVersion), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of individuals kept in vivarium's hall-of-fame ";
		lOSS << "(best individuals so far). Note that a hall-of-fame contains only ";
		lOSS << "copies of the best individuals so far and is not used by the evolution ";
		lOSS << "process.";
		Register::Description lDescription(
		    "Vivarium's hall-of-fame size",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mVivaHOFSize = castHandleT<UInt>(
		                   ioSystem.getRegister().insertEntry("ec.hof.vivasize", new UInt(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of individuals kept in each deme's hall-of-fame ";
		lOSS << "(best individuals so far). Note that a hall-of-fame contains only ";
		lOSS << "copies of the best individuals so far and is not used by the evolution ";
		lOSS << "process.";
		Register::Description lDescription(
		    "Demes' hall-of-fame size",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mDemeHOFSize = castHandleT<UInt>(
		                   ioSystem.getRegister().insertEntry("ec.hof.demesize", new UInt(1), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the distribution of evaluations operation on the deme.
 *  \param ioDeme Current deme of individuals that evaluations will be distributed.
 *  \param ioContext Context of the evolution.
 */
void Master::DistribEvalOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	
	EvolClient& lClient = *EvolClient::getSingleton();
	lClient.setContext(&ioContext);

	if((lClient.getDeme() != NULL) && (lClient.getDeme()->empty() == false)) {
		Beagle_AssertM(lClient.getDeme().getPointer() == &ioDeme);

		unsigned int lToBeProcessed = 0;
		for(unsigned int i=0; i<ioDeme.size(); ++i) {
			Fitness::Handle lFitness = ioDeme[i]->getFitness();
			if((lFitness == NULL) || !lFitness->isValid()) ++lToBeProcessed;
		}

		ioContext.setProcessedDeme(lToBeProcessed);
		ioContext.setTotalProcessedDeme(ioContext.getTotalProcessedDeme() + lToBeProcessed);
		if(ioContext.getDemeIndex() == 0) {
			ioContext.setProcessedVivarium(lToBeProcessed);
		}
		else {
			ioContext.setProcessedVivarium(ioContext.getProcessedVivarium() + lToBeProcessed);
		}
		if(ioContext.getGeneration() == 0) {
			ioContext.setTotalProcessedVivarium(lToBeProcessed);
		}
		else {
			ioContext.setTotalProcessedVivarium(ioContext.getTotalProcessedVivarium() + lToBeProcessed);
		}

		Beagle::Stats::Handle lDemeStats = ioDeme.getStats();
		if(lDemeStats == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			Stats::Alloc::Handle lStatsAlloc =
				castHandleT<Stats::Alloc>(lFactory.getConceptAllocator("Stats"));
			lDemeStats = castHandleT<Stats>(lStatsAlloc->allocate());
			ioDeme.addMember(lDemeStats);
		}
		lDemeStats->modifyItem("processed", ioContext.getProcessedDeme());
		lDemeStats->modifyItem("total-processed", ioContext.getTotalProcessedDeme());
		lDemeStats->setValid();

		lClient.setGeneration(ioContext.getGeneration());
		lClient.sendDeme(DAGS::Client::ExistingGroup);
		lClient.getDeme()->clear();

		if(ioContext.getDemeIndex() != lClient.getDemeID()) {
			Beagle_AssertM(lClient.getDemeID() < ioContext.getVivarium().size());
			Deme::Handle lTmp = ioContext.getVivarium()[lClient.getDemeID()];
			ioContext.getVivarium()[lClient.getDemeID()] =
				ioContext.getVivarium()[ioContext.getDemeIndex()];
			ioContext.getVivarium()[ioContext.getDemeIndex()] = lTmp;
			ioContext.setDemeHandle(lTmp);
		}
	}

	lClient.setDeme(ioContext.getVivarium()[ioContext.getDemeIndex()]);
	lClient.setDemeIDEvol(ioContext.getDemeIndex());
	lClient.receiveDeme();
	ioContext.setGeneration(lClient.getGeneration());

	if(lClient.getDemeID() != ioContext.getDemeIndex()) {
		Beagle_AssertM(lClient.getDemeID() < ioContext.getVivarium().size());
		Deme::Handle lDemeInID = ioContext.getVivarium()[lClient.getDemeID()];
		Deme::Handle lDemeInContext = ioContext.getVivarium()[ioContext.getDemeIndex()];
		lDemeInID->clear();
		lDemeInID->insert(lDemeInID->begin(), lDemeInContext->begin(), lDemeInContext->end());
		lClient.setDeme(lDemeInID);
		lDemeInContext->clear();
		ioContext.getVivarium()[lClient.getDemeID()] = lDemeInContext;
		ioContext.getVivarium()[ioContext.getDemeIndex()] = lDemeInID;
		ioContext.setDemeHandle(lDemeInID);
	}

	Deme& lActualDeme = *ioContext.getVivarium()[ioContext.getDemeIndex()];
	ioContext.setDemeIndex(lClient.getDemeID());

	Beagle::Stats::Handle lActualDemeStats = lActualDeme.getStats();
	if(lActualDemeStats->existItem("processed") == false) {
		lActualDemeStats->addItem("processed", 0.);
	}
	if(lActualDemeStats->existItem("total-processed") == false) {
		lActualDemeStats->addItem("total-processed", 0.);
	}
	ioContext.setProcessedDeme((unsigned int)lActualDemeStats->getItem("processed"));
	ioContext.setTotalProcessedDeme((unsigned int)lActualDemeStats->getItem("total-processed"));
	lActualDemeStats->setInvalid();

	Beagle::Stats::Handle lVivaStats = ioContext.getVivarium().getStats();
	if(lVivaStats == NULL) {
		const Factory& lFactory = ioContext.getSystem().getFactory();
		Stats::Alloc::Handle lStatsAlloc =
			castHandleT<Stats::Alloc>(lFactory.getConceptAllocator("Stats"));
		lVivaStats = castHandleT<Stats>(lStatsAlloc->allocate());
		ioContext.getVivarium().addMember(lVivaStats);
	}
	lVivaStats->setInvalid();

	if(mDemeHOFSize->getWrappedValue() > 0) {
		Beagle_LogDetailedM(
			ioContext.getSystem().getLogger(),
			"evaluation", "Beagle::EvaluationOp",
			"Updating the deme's hall-of-fame"
		);
		HallOfFame::Handle lHoF = lActualDeme.getHallOfFame();
		if(lHoF == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const HallOfFame::Alloc::Handle lHoFAlloc =
				castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
			lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
			lActualDeme.addMember(lHoF);
		}
		lHoF->updateWithDeme(mDemeHOFSize->getWrappedValue(), lActualDeme, ioContext);
		lHoF->log(Logger::eVerbose, ioContext);
	}

	if(mVivaHOFSize->getWrappedValue() > 0) {
		Beagle_LogDetailedM(
			ioContext.getSystem().getLogger(),
			"evaluation", "Beagle::EvaluationOp",
			"Updating the vivarium's hall-of-fame"
		);
		HallOfFame::Handle lHoF = ioContext.getVivarium().getHallOfFame();
		if(lHoF == NULL) {
			const Factory& lFactory = ioContext.getSystem().getFactory();
			const HallOfFame::Alloc::Handle lHoFAlloc =
				castHandleT<HallOfFame::Alloc>(lFactory.getConceptAllocator("HallOfFame"));
			lHoF = castHandleT<HallOfFame>(lHoFAlloc->allocate());
			ioContext.getVivarium().addMember(lHoF);
		}
		lHoF->updateWithDeme(mVivaHOFSize->getWrappedValue(), lActualDeme, ioContext);
		lHoF->log(Logger::eVerbose, ioContext);
	}

	// Sync migration buffer of actual deme.
	MigrationBuffer::Handle lMigBuffer =
		castHandleT<MigrationBuffer>(ioDeme.getMember("MigrationBuffer"));
	if(lMigBuffer != NULL) lMigBuffer->mergeImmigrantsToDeme(ioDeme);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the distribution of evaluations operator.
 *  \param ioSystem Evolutionary system.
 */
void Master::DistribEvalOp::init(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::EvaluationOp::init(ioSystem);

	if(EvolClient::getSingleton() == NULL) {
		EvolClient::setSingleton(new EvolClient(mServerIP->getWrappedValue(),
		                                        mServerPort->getWrappedValue(),
		                                        mCompressionLevel->getWrappedValue(),
		                                        mVersion->getWrappedValue()));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 *  \throw  Beagle::InternalException This method is undefined.
 */
Fitness::Handle Master::DistribEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("evaluate","Distrib::Master::DistribEvalOp",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the evaluation operation on a breeding pool, returning a evaluated bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Evaluated bred individual.
 *  \throw  Beagle::InternalException This method is undefined.
 */
Individual::Handle Master::DistribEvalOp::breed(Individual::Bag& inBreedingPool,
                                                BreederNode::Handle inChild,
                                                Context& ioContext)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("breed","Distrib::Master::DistribEvalOp",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 *  \throw  Beagle::InternalException This method is undefined.
 */
float Master::DistribEvalOp::getBreedingProba(BreederNode::Handle inChild)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("getBreedingProba","Distrib::Master::DistribEvalOp",getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Test the fitness of a given individual.
 *  \param inIndividual Handle to the individual to test.
 *  \param ioSystem Handle to the system to use to test the individual.
 *  \throw Beagle::InternalException This method is undefined.
 */
Fitness::Handle Master::DistribEvalOp::test(Individual::Handle inIndividual, System::Handle ioSystem)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("test","Distrib::Master::DistribEvalOp",getName());
	Beagle_StackTraceEndM();
}

