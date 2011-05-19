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
 *  \file   beagle/HPC/SendProcessedToSupervisorOp.hpp
 *  \brief  Definition of the class HPC::SendProcessedToSupervisorOp.
 *  \author Felix-Antoine Fortin
 *  $Revision: 1.3 $
 *  $Date: 2007/08/13 01:04:52 $
 */
 
#ifndef Beagle_HPC_SendProcessedToSupervisorOp_hpp
#define Beagle_HPC_SendProcessedToSupervisorOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle{

namespace HPC{

	/*!
	 *  \class SendProcessedToSupervisorOp beagle/HPC/SendProcessedToSupervisorOp.hpp "beagle/HPC/SendProcessedToSupervisorOp.hpp"
	 *  \brief Send processed individuals to supervisor parent node.
	 *  \warning This operator cannot be used if the number of individuals is variable during the evolution.
	 *  This operator uses the ProcessingBuffer member of the deme to determine which individuals
	 *  were modified during the current generation. It sends three messages. The first message
	 *  contains the number of individuals who where modified. The second message contains a list
	 *  of the individual's index. The third message is the individual's genotypes.
	 */
	class SendProcessedToSupervisorOp : public Beagle::Operator {

	public:

		//! MaxFctEvalOp allocator type.
		typedef Beagle::AllocatorT<SendProcessedToSupervisorOp,Beagle::Operator::Alloc> Alloc;
		//!< MaxFctEvalOp handle type.
		typedef Beagle::PointerT<SendProcessedToSupervisorOp,Beagle::Operator::Handle> Handle;
		//!< MaxFctEvalOp bag type.
		typedef Beagle::ContainerT<SendProcessedToSupervisorOp,Beagle::Operator::Bag> Bag;

		explicit SendProcessedToSupervisorOp(std::string inName="HPC-SendProcessedToSupervisorOp");
		virtual ~SendProcessedToSupervisorOp() { }

		virtual void operate(Deme& ioDeme, Context& ioContext);
		virtual void init(System& ioSystem);
	protected:
		HPC::MPICommunication::Handle mComm;
	};
}
}
#endif 

