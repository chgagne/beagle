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
 *  \file   beagle/HPC/SendFitnessToEvolverOp.hpp
 *  \brief  Definition of the class HPC::SendFitnessToEvolverOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.3 $
 *  $Date: 2007/08/13 01:04:52 $
 */
 
#ifndef Beagle_HPC_SendFitnessToEvolverOp_hpp
#define Beagle_HPC_SendFitnessToEvolverOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle{

namespace HPC{

	/*!
	 *  \class SendFitnessToEvolverOp beagle/HPC/SendFitnessToEvolverOp.hpp "beagle/HPC/SendFitnessToEvolverOp.hpp"
	 *  \brief Send a group of individuals fitness to Evolver parent node.
	 *	This operators serialize the fitness of the individuals then send those fitness \
	 *	to the Evolver from which the individuals came. Since the individuals came from
	 *	the Evolver, it knows how many fitness will come from every Evaluator, so this
	 *	operator doesn't have to send how many individuals will be sent.
	 */
	class SendFitnessToEvolverOp : public Beagle::Operator {

	public:

		//! MaxFctEvalOp allocator type.
		typedef Beagle::AllocatorT<SendFitnessToEvolverOp,Beagle::Operator::Alloc> Alloc;
		//!< MaxFctEvalOp handle type.
		typedef Beagle::PointerT<SendFitnessToEvolverOp,Beagle::Operator::Handle> Handle;
		//!< MaxFctEvalOp bag type.
		typedef Beagle::ContainerT<SendFitnessToEvolverOp,Beagle::Operator::Bag> Bag;

		explicit SendFitnessToEvolverOp(std::string inName="HPC-SendFitnessToEvolverOp");
		virtual ~SendFitnessToEvolverOp() { }
		virtual void operate(Deme& ioDeme, Context& ioContext);
		virtual void init(System& ioSystem);
	protected:
		HPC::MPICommunication::Handle mComm;
	};
}
}

#endif //Beagle_HPC_SendFitnessToEvolverOp_hpp

