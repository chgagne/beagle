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
 *  \file   Beagle/EC/TermMaxEvalsOp.hpp
 *  \brief  Definition of the class TermMaxEvalsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_TermMaxEvalsOp_hpp
#define Beagle_EC_TermMaxEvalsOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{
	
namespace EC
{

/*!
 *  \class TermMaxEvalsOp Beagle/EC/TermMaxEvalsOp.hpp "Beagle/EC/TermMaxEvalsOp.hpp"
 *  \brief Maximum number of fitness evaluations termination criterion operator.
 *  \ingroup EC
 *  \ingroup Op
 */
class TermMaxEvalsOp : public Beagle::EC::TerminationOp
{

public:

	//! TermMaxEvalsOp allocator type
	typedef Beagle::AllocatorT<TermMaxEvalsOp,Beagle::EC::TerminationOp::Alloc> Alloc;
	//! TermMaxEvalsOp handle type.
	typedef Beagle::PointerT<TermMaxEvalsOp,Beagle::EC::TerminationOp::Handle> Handle;
	//! TermMaxEvalsOp bag type.
	typedef Beagle::ContainerT<TermMaxEvalsOp,Beagle::EC::TerminationOp::Bag> Bag;

	explicit TermMaxEvalsOp(std::string inName="TermMaxEvalsOp");
	virtual ~TermMaxEvalsOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle mMaxEvaluations;   //!< Maximum number of fitness evaluations of the evolution.

};

}

}

#endif // Beagle_EC_TerminationCheckOp_hpp
