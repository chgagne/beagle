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
 *  \file   Beagle/EC/TermMaxFitnessOp.hpp
 *  \brief  Definition of the class TermMaxFitnessOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_TermMaxFitnessOp_hpp
#define Beagle_EC_TermMaxFitnessOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/TerminationOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class TermMaxFitnessOp Beagle/EC/TermMaxFitnessOp.hpp "Beagle/EC/TermMaxFitnessOp.hpp"
 *  \brief Maximum fitness value termination criterion operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class TermMaxFitnessOp : public Beagle::EC::TerminationOp
{

public:

	//! TermMaxFitnessOp allocator type
	typedef Beagle::AllocatorT<TermMaxFitnessOp,Beagle::EC::TerminationOp::Alloc> Alloc;
	//! TermMaxFitnessOp handle type.
	typedef Beagle::PointerT<TermMaxFitnessOp,Beagle::EC::TerminationOp::Handle> Handle;
	//! TermMaxFitnessOp bag type.
	typedef Beagle::ContainerT<TermMaxFitnessOp,Beagle::EC::TerminationOp::Bag> Bag;

	explicit TermMaxFitnessOp(float inMaxFitness=1.0, std::string inName="TermMaxFitnessOp");
	virtual ~TermMaxFitnessOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Beagle::Float::Handle  mMaxFitness;        //!< Maximum fitness of an individual for an evolution.
	float                  mMaxFitnessDefault; //!< Default maximum fitness value.

};

}

}

#endif // Beagle_EC_TerminationCheckOp_hpp
