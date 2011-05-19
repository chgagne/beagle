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
 *  \file   beagle/HPC/RecvDemeFromSupervisorOp.hpp
 *  \brief  Definition of the class HPC::RecvDemeFromSupervisorOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.4 $
 *  $Date: 2007/08/13 01:04:52 $
 */

#ifndef Beagle_HPC_RecvDemeFromSupervisorOp_hpp
#define Beagle_HPC_RecvDemeFromSupervisorOp_hpp

#include "beagle/HPC.hpp"


namespace Beagle {

namespace HPC {

/*!
 *  \class RecvDemeFromSupervisorOp beagle/HPC/RecvDemeFromSupervisorOp.hpp "beagle/HPC/RecvDemeFromSupervisorOp.hpp"
 *  \brief Reception of a deme content from supervisor parent node
 */
class RecvDemeFromSupervisorOp : public Beagle::Operator {

public:

	//! MaxFctEvalOp allocator type.
	typedef Beagle::AllocatorT<RecvDemeFromSupervisorOp,Beagle::Operator::Alloc> Alloc;
	//!< MaxFctEvalOp handle type.
	typedef Beagle::PointerT<RecvDemeFromSupervisorOp,Beagle::Operator::Handle> Handle;
	//!< MaxFctEvalOp bag type.
	typedef Beagle::ContainerT<RecvDemeFromSupervisorOp,Beagle::Operator::Bag> Bag;

	explicit RecvDemeFromSupervisorOp(std::string inName="HPC-RecvDemeFromSupervisorOp");
	virtual ~RecvDemeFromSupervisorOp() { }

	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void init(System& ioSystem);
protected:
	HPC::MPICommunication::Handle mComm;

};
}
}
#endif //Beagle_HPC_RecvDemeFromSupervisorOp_hpp

