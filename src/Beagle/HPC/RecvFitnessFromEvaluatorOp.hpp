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
 *  \file   beagle/HPC/RecvFitnessFromEvaluatorOp.hpp
 *  \brief  Definition of the class HPC::RecvFitnessFromEvaluatorOp.
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */
 
#ifndef Beagle_HPC_RecvDemeFromEvaluatorOp_hpp
#define Beagle_HPC_RecvDemeFromEvaluatorOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle {

namespace HPC {

	/*!
	 *  \class RecvFitnessFromEvaluatorOp beagle/HPC/RecvFitnessFromEvaluatorOp.hpp "beagle/HPC/RecvFitnessFromEvaluatorOp.hpp"
	 *  \brief Reception of groups of individuals fitness from evaluators childs.
	 *	This operator should be used by an Evolver node. It receives the fitnesses from the evaluators
	 *	and then using the ProcessingBuffer, it associates each fitness to its corresponding individual.
	 */
	class RecvFitnessFromEvaluatorOp : public Beagle::EvaluationOp {

	public:

		//! RecvFitnessFromEvaluatorOp allocator type.
		typedef Beagle::AllocatorT<RecvFitnessFromEvaluatorOp,Beagle::EvaluationOp::Alloc> Alloc;
		//!< RecvFitnessFromEvaluatorOp handle type.
		typedef Beagle::PointerT<RecvFitnessFromEvaluatorOp,Beagle::EvaluationOp::Handle> Handle;
		//!< RecvFitnessFromEvaluatorOp bag type.
		typedef Beagle::ContainerT<RecvFitnessFromEvaluatorOp,Beagle::EvaluationOp::Bag> Bag;

		explicit RecvFitnessFromEvaluatorOp(std::string inName="HPC-RecvFitnessFromEvaluatorOp");
		virtual ~RecvFitnessFromEvaluatorOp() { }
		
		virtual Beagle::Fitness::Handle evaluate(Individual& inIndividual, Context& ioContext);
		virtual void operate(Deme& ioDeme, Context& ioContext);
		
		virtual void init(System& ioSystem);
	protected:
		HPC::MPICommunication::Handle mComm;
	};
}
}

#endif //Beagle_HPC_RecvDemeFromEvaluatorOp_hpp

