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
 *  \file   beagle/HPC/SendDemeToSupervisorOp.hpp
 *  \brief  Definition of the class HPC::SendDemeToSupervisorOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.3 $
 *  $Date: 2007/08/13 01:04:52 $
 */
 
#ifndef Beagle_HPC_SendDemeToSupervisorOp_hpp
#define Beagle_HPC_SendDemeToSupervisorOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle{

namespace HPC{

	/*!
	 *  \class SendDemeToSupervisorOp beagle/HPC/SendDemeToSupervisorOp.hpp "beagle/HPC/SendDemeToSupervisorOp.hpp"
	 *  \brief Send deme content to supervisor parent node.
	 *  \warning If a member of the deme is modified during a generation, and the supervisor has \
	 *  to know about it, this operator can't be used since it only sends the population.
	 *  This operator is used by the Evolver to send the generation resulting deme
	 *  to the Supervisor. It sends two messages. The first message contains the
	 *  number of individuals that were evaluated. This is used to keep the number
	 *  of processed statistic up-to-date. The second message contains the serialised
	 *  Deme's population. Only the population is sent, because it is the only part
	 *  of the deme that can change during the generation.
	 */
	class SendDemeToSupervisorOp : public Beagle::Operator {

	public:

		//! MaxFctEvalOp allocator type.
		typedef Beagle::AllocatorT<SendDemeToSupervisorOp,Beagle::Operator::Alloc> Alloc;
		//!< MaxFctEvalOp handle type.
		typedef Beagle::PointerT<SendDemeToSupervisorOp,Beagle::Operator::Handle> Handle;
		//!< MaxFctEvalOp bag type.
		typedef Beagle::ContainerT<SendDemeToSupervisorOp,Beagle::Operator::Bag> Bag;

		explicit SendDemeToSupervisorOp(std::string inName="HPC-SendDemeToSupervisorOp");
		virtual ~SendDemeToSupervisorOp() { }

		virtual void operate(Deme& ioDeme, Context& ioContext);
		virtual void init(System& ioSystem);
	protected:
		HPC::MPICommunication::Handle mComm;
	};
}
}
#endif 

