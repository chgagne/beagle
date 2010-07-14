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
 *  \file   Beagle/EC/TermMaxGenOp.hpp
 *  \brief  Definition of the class TermMaxGenOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_TermMaxGenOp_hpp
#define Beagle_EC_TermMaxGenOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/TerminationOp.hpp"


namespace Beagle
{
	
namespace EC
{

/*!
 *  \class TermMaxGenOp Beagle/EC/TermMaxGenOp.hpp "Beagle/EC/TermMaxGenOp.hpp"
 *  \brief Maximum generation termination criterion operator.
 *  \ingroup EC
 *  \ingroup Op
 */
class TermMaxGenOp : public Beagle::EC::TerminationOp
{

public:

	//! TermMaxGenOp allocator type
	typedef Beagle::AllocatorT<TermMaxGenOp,Beagle::EC::TerminationOp::Alloc> Alloc;
	//! TermMaxGenOp handle type.
	typedef Beagle::PointerT<TermMaxGenOp,Beagle::EC::TerminationOp::Handle> Handle;
	//! TermMaxGenOp bag type.
	typedef Beagle::ContainerT<TermMaxGenOp,Beagle::EC::TerminationOp::Bag> Bag;

	explicit TermMaxGenOp(std::string inName="TermMaxGenOp");
	virtual ~TermMaxGenOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle mMaxGeneration;     //!< Maximum number of generation of the evolution.

};

}

}

#endif // Beagle_EC_TerminationCheckOp_hpp
