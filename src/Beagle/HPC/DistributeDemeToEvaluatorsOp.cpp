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
 *  \file   beagle/HPC/DistributeDemeToEvaluatorsOp.cpp
 *  \brief  Source code of the class HPC::DistributeDemeToEvaluatorsOp.
 *  \author Felix-Antoine Fortin
 *  $Revision: 1.4 $
 *  $Date: 2007/08/13 00:57:45 $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

/*!
 *  \brief Construct the DistributeDemeToEvaluators operator.
 *  \param inName Name of the operator.
 */
HPC::DistributeDemeToEvaluatorsOp::DistributeDemeToEvaluatorsOp(std::string inName) : Operator(inName) {}

/*!
 *  \brief Initialize the operator
 *  \param ioSystem Current system
 */
void HPC::DistributeDemeToEvaluatorsOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	mComm = castHandleT<HPC::MPICommunication>(ioSystem.getComponent("MPICommunication"));
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Distribute groups of individuals to evaluators nodes childs.
 *  \param ioDeme Current deme to execute the operator.
 *  \param ioContext Current context to execute the operator.
 *  This function distributed individuals to the evolver's evaluators.
 *
 *  It first checks if the deme has a \ref ProcessingBuffer. The buffer
 *  is used to memorize which individuals had invalid fitness, since the
 *  function only distributes individuals that were modified during the
 *  current generation. Once the buffer is filled, the individuals are
 *  distributed to evaluator.
 *  The number of individuals an evaluato will received is based on this
 *  formula :
 *  \f[ NbIndividuals =
        \left\{
	\begin{array}{lr}
	\frac{BufferSize}{NbEvaluators} + 1, &  EvaluatorRank > BufferSize \pmod{NbEvaluators} \\
	\frac{BufferSize}{NbEvaluators},  & else
	\end{array}
	\right.
    \f]
 *
 *  The function write the individuals in a streamer. The streamer opening
 *  tag is <Population> so the evaluator can directly read the string at
 *  its reception as a deme.
 *
 *  The string are sent using a non-blocking send function, so the function
 *  doesn't have to wait for the transaction to be completed before starting
 *  to build another packet of individuals. The function ends when all
 *  packets have been sent.
 */
void HPC::DistributeDemeToEvaluatorsOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	unsigned int lNbEvaluators = mComm->getNbrOfRelation("Child");

	ProcessingBuffer::Handle lBuffer = castHandleT<ProcessingBuffer>(ioDeme.getMember("ProcessingBuffer"));
	if(lBuffer==0){
		lBuffer = new ProcessingBuffer;
		ioDeme.addMember(lBuffer);
	}
	lBuffer->clear();
	for(unsigned int i = 0; i < ioDeme.size(); ++i){
		if((ioDeme[i]->getFitness() == 0) ||
		   (ioDeme[i]->getFitness()->isValid() == false)){
			lBuffer->push_back(ioDeme[i], i);
		}
	}

	unsigned int lNbIndividualInt = lBuffer->size() / lNbEvaluators;
	unsigned int lNbIndividualFrac = lBuffer->size() % lNbEvaluators;

	unsigned int lNbIndividualTotal = 0;
	unsigned int lNbIndividual = 0;

	std::vector<std::string> lStreams(lNbEvaluators);
	MPICommunication::Request::Bag lRequests(lNbEvaluators);
	for(unsigned int i = 0; i < lNbEvaluators; ++i){

		lNbIndividual = lNbIndividualInt;
		if(i < lNbIndividualFrac)
			lNbIndividual++;

		std::ostringstream lOutStream;
		PACC::XML::Streamer lStreamer(lOutStream);

		lStreamer.openTag("Population");
		for(int j = 0; j < lNbIndividual; ++j){
			(*lBuffer)[j+lNbIndividualTotal]->write(lStreamer, false);
		}
		lStreamer.closeTag();
		lStreams[i] = lOutStream.str();
		lRequests[i] = new MPICommunication::Request;
		mComm->sendNonBlocking(lStreams[i], lRequests[i], "Individuals", "Child", i);

		lNbIndividualTotal += lNbIndividual;

		Beagle_LogDetailedM(
			ioContext.getSystem().getLogger(),
			"distribute", "Beagle::HPC::DistributeDemeToEvaluatorsOp",
			std::string("Evolver send fractionnal Deme to his ")+uint2ordinal(i+1)+ " evaluator"
		);
	}
	mComm->waitAll(lRequests);
	Beagle_StackTraceEndM();
}

