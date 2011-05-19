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
 *  \file   beagle/HPC/SendDemeToEvolverOp.hpp
 *  \brief  Definition of the class HPC::SendDemeToEvolverOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: $
 *  $Date:  $
 */

#ifndef Beagle_HPC_SendDemeToEvolverOp_hpp
#define Beagle_HPC_SendDemeToEvolverOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle{

namespace HPC{

	/*!
	 *  \class SendDemeToEvolverOp beagle/HPC/SendDemeToEvolverOp.hpp "beagle/HPC/SendDemeToEvolverOp.hpp"
	 *  \brief Distribute deme content to evolvers childs nodes.
	 *  This operator is intended to be used during the bootstrap. The operator sends the entire
	 *  serialise deme, which the population and all the deme's members. The Evolver to which will
	 *  be sent is based on the deme index number. Each deme is associated to an Evolver node. The
	 *  algorithm that determines which Deme is associated to which Evolver is implemented in the
	 *  \c MPICommunication class.
	 */
	class SendDemeToEvolverOp : public Beagle::Operator {

	public:

		//! SendDemeToEvolverOp allocator type.
		typedef Beagle::AllocatorT<SendDemeToEvolverOp,Beagle::Operator::Alloc>
		Alloc;
		//!< SendDemeToEvolverOp handle type.
		typedef Beagle::PointerT<SendDemeToEvolverOp,Beagle::Operator::Handle>
		Handle;
		//!< SendDemeToEvolverOp bag type.
		typedef Beagle::ContainerT<SendDemeToEvolverOp,Beagle::Operator::Bag>
		Bag;

		explicit SendDemeToEvolverOp(std::string inName="HPC-SendDemeToEvolverOp");
		//! SendDemeToEvolverOp object destructor.
		virtual ~SendDemeToEvolverOp() { }

		virtual void operate(Deme& ioDeme, Context& ioContext);
		virtual void init(System& ioSystem);
	protected:
		//! Handle to the MPICommunication component.
		HPC::MPICommunication::Handle mComm;

	};
}
}
#endif //Beagle_HPC_SendDemeToEvolverOp_hpp

