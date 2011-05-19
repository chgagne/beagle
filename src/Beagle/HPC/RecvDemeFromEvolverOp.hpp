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
 *  \file   beagle/HPC/RecvDemeFromEvolverOp.hpp
 *  \brief  Definition of the class HPC::RecvDemeFromEvolverOp.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.3 $
 *  $Date: 2007/08/13 00:57:42 $
 */

#ifndef Beagle_HPC_RecvDemeFromEvolverOp_hpp
#define Beagle_HPC_RecvDemeFromEvolverOp_hpp

#include "beagle/HPC.hpp"

namespace Beagle{

namespace HPC{

/*!
 *  \class RecvDemeFromEvolverOp beagle/HPC/RecvDemeFromEvolverOp.hpp  "beagle/HPC/RecvDemeFromEvolverOp.hpp"
 *  \brief Reception of demes contents from evolvers childs nodes
 *  \ingroup HPC
 */
class RecvDemeFromEvolverOp : public Beagle::EvaluationOp {

public:
	//! RecvDemeFromEvolverOp allocator type.
	typedef Beagle::AllocatorT<RecvDemeFromEvolverOp,Beagle::Operator::Alloc> Alloc;
	//!< RecvDemeFromEvolverOp handle type.
	typedef Beagle::PointerT<RecvDemeFromEvolverOp,Beagle::Operator::Handle> Handle;
	//!< RecvDemeFromEvolverOp bag type.
	typedef Beagle::ContainerT<RecvDemeFromEvolverOp,Beagle::Operator::Bag> Bag;

	explicit RecvDemeFromEvolverOp(std::string inName="HPC-RecvDemeFromEvolverOp");
	virtual ~RecvDemeFromEvolverOp() {}

	Beagle::Fitness::Handle evaluate(Individual& inIndividual, Context& ioContext);
	virtual void operate(Deme& ioDeme, Context& ioContext);

	virtual void init(System& ioSystem);
protected:
	HPC::MPICommunication::Handle mComm;
};
}
}
#endif //Beagle_HPC_RecvDemeFromEvolverOp_hpp

