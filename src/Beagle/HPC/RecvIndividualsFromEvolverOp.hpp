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
 *  \file   beagle/HPC/RecvIndividualsFromEvolverOp.hpp
 *  \brief  Definition of the class HPC::RecvIndividualsFromEvolverOp.
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */
 
#ifndef Beagle_HPC_RecvIndividualsFromEvolverOp_hpp
#define Beagle_HPC_RecvIndividualsFromEvolverOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle {

namespace HPC {

	/*!
	 *  \class RecvIndividualsFromEvolverOp beagle/HPC/RecvIndividualsFromEvolverOp.hpp "beagle/HPC/RecvIndividualsFromEvolverOp.hpp"
	 *  \brief Reception of a group of individuals to evaluate from evolver parent node.
	 *	This operators receives individuals from the Evolver node. Those individuals should
	 *	have invalid fitnesses, but it isn't the responsability of this operator to make sure
	 *	that the individuals have invalid fitnesses. The operator \c DistributeDemeToEvaluatorsOp
	 *	implements a distribution operation to Evaluator that only sends invalid fitnesses to
	 *	Evaluators.
	 */
	class RecvIndividualsFromEvolverOp : public Beagle::Operator {

	public:

		//! RecvIndividualsFromEvolverOp allocator type.
		typedef Beagle::AllocatorT<RecvIndividualsFromEvolverOp,Beagle::Operator::Alloc> Alloc;
		//!< RecvIndividualsFromEvolverOp handle type.
		typedef Beagle::PointerT<RecvIndividualsFromEvolverOp,Beagle::Operator::Handle> Handle;
		//!< RecvIndividualsFromEvolverOp bag type.
		typedef Beagle::ContainerT<RecvIndividualsFromEvolverOp,Beagle::Operator::Bag> Bag;

		explicit RecvIndividualsFromEvolverOp(std::string inName="HPC-RecvIndividualsFromEvolverOp");
		virtual ~RecvIndividualsFromEvolverOp() { }

		virtual void operate(Deme& ioDeme, Context& ioContext);
		virtual void init(System& ioSystem);
	protected:
		HPC::MPICommunication::Handle mComm;
	};
}
}
#endif //Beagle_HPC_RecvIndividualsFromEvolverOp_hpp

