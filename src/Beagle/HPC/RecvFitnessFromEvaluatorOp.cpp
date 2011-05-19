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
 *  \file   beagle/HPC/RecvFitnessFromEvaluatorOp.cpp
 *  \brief  Source code of the class HPC::RecvFitnessFromEvaluatorOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.6 $
 *  $Date: 2007/08/13 00:57:45 $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

/*!
 *  \brief Construct the RecvFitnessFromEvaluatorOp evaluation operator.
 *  \param inName Name of the operator.
 */
HPC::RecvFitnessFromEvaluatorOp::RecvFitnessFromEvaluatorOp(std::string inName) : EvaluationOp(inName) {}


/*!
 *  \brief Initialize the operator
 *  \param ioSystem Current system
 */
void HPC::RecvFitnessFromEvaluatorOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mComm = castHandleT<HPC::MPICommunication>(ioSystem.getComponent("MPICommunication"));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief
 */
Fitness::Handle HPC::RecvFitnessFromEvaluatorOp::evaluate(Individual& inIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Receive the fitness of an individuals group from evaluator node.
 *  \param ioDeme Deme to update by the receive fitness.
 *  \param ioContext Current context of the evolution.
 */
void HPC::RecvFitnessFromEvaluatorOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	ProcessingBuffer::Handle lBuffer = castHandleT<ProcessingBuffer>(ioDeme.getMember("ProcessingBuffer"));
	Beagle_NonNullPointerAssertM(lBuffer);

	Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
	unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();

	prepareStats(ioDeme,ioContext);

	unsigned int lNbEvaluators = mComm->getNbrOfRelation("Child");

	unsigned int lNbIndividualInt = lBuffer->size() / lNbEvaluators;
	unsigned int lNbIndividualFrac = lBuffer->size() % lNbEvaluators;

	unsigned int lNbIndividualTotal = 0;

	for(int i = 0; i < lNbEvaluators; ++i ){
		int lNbIndividual = lNbIndividualInt;
		if(i < lNbIndividualFrac)
			++lNbIndividual;
		std::string lFitnessString;

		mComm->receive(lFitnessString, "Fitness", "Child", i);

		Beagle_LogDetailedM(
			ioContext.getSystem().getLogger(),
			"receive", "Beagle::HPC::RecvFitnessFromEvaluatorOp",
			std::string("Evolver receive fitness from his ")+uint2ordinal(i+1)+
			std::string(" evaluator") + std::string(" of ") + uint2str(lNbEvaluators)
		);

		std::istringstream lInStream(lFitnessString);
		PACC::XML::Document lDocument(lInStream);

		const Factory& lFactory = ioContext.getSystem().getFactory();
		Fitness::Alloc::Handle lFitnessAlloc =
				castHandleT<Fitness::Alloc>(lFactory.getConceptAllocator("Fitness"));
		for(PACC::XML::ConstIterator lIter = lDocument.getFirstRoot()->getFirstChild(); lIter; ++lIter){
			ioContext.setIndividualIndex(lNbIndividualTotal);
			ioContext.setIndividualHandle(ioDeme[lNbIndividualTotal]);
			if((*lBuffer)[lNbIndividualTotal]->getFitness() == 0)
				(*lBuffer)[lNbIndividualTotal]->setFitness( castHandleT<Fitness>(lFitnessAlloc->allocate()) );
			(*lBuffer)[lNbIndividualTotal]->getFitness()->read(lIter);
			++lNbIndividualTotal;
			updateStats(1,ioContext);
		}
	}
	Beagle_LogDetailedM(
		ioContext.getSystem().getLogger(),
		"receive", "Beagle::HPC::RecvFitnessFromEvaluatorOp",
		std::string("Evolver receive all its fitness.")
	);

	ioContext.setIndividualIndex(lOldIndividualIndex);
	ioContext.setIndividualHandle(lOldIndividualHandle);
	Beagle_StackTraceEndM();
}

