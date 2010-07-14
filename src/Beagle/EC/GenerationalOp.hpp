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
 *  \file   Beagle/EC/GenerationalOp.hpp
 *  \brief  Definition of the class Beagle::EC::GenerationalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:12 $
 */


#ifndef Beagle_EC_GenerationalOp_hpp
#define Beagle_EC_GenerationalOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class GenerationalOp Beagle/EC/GenerationalOp.hpp "Beagle/EC/GenerationalOp.hpp"
 *  \brief Generational replacement strategy operator class.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  A generational operator process deme with a breeding tree following a generation by generation
 *  replacement strategy.
 */
class GenerationalOp : public Beagle::ReplacementStrategyOp
{

public:

	//! GenerationalOp allocator type.
	typedef Beagle::AllocatorT<GenerationalOp,Beagle::ReplacementStrategyOp::Alloc>
	Alloc;
	//! GenerationalOp handle type.
	typedef Beagle::PointerT<GenerationalOp,Beagle::ReplacementStrategyOp::Handle>
	Handle;
	//! GenerationalOp bag type.
	typedef Beagle::ContainerT<GenerationalOp,Beagle::ReplacementStrategyOp::Bag>
	Bag;

	GenerationalOp(std::string inName="GenerationalOp");
	virtual ~GenerationalOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle  mElitismKeepSize;      //!< Number of individuals keep with elitism

};

}

}

#endif // Beagle_EC_GenerationalOp_hpp
