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
 *  \file   Beagle/EC/SelectTournamentOp.hpp
 *  \brief  Definition of the class SelectTournamentOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_SelectTournamentOp_hpp
#define Beagle_EC_SelectTournamentOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectTournamentOp Beagle/EC/SelectTournamentOp.hpp "Beagle/EC/SelectTournamentOp.hpp"
 *  \brief Tournament selection operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectTournamentOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectTournamentOp allocator type.
	typedef Beagle::AllocatorT<SelectTournamentOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectTournamentOp handle type.
	typedef Beagle::PointerT<SelectTournamentOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectTournamentOp bag type.
	typedef Beagle::ContainerT<SelectTournamentOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectTournamentOp(std::string inReproProbaName="ec.repro.prob",
	                            std::string inName="SelectTournamentOp");
	virtual ~SelectTournamentOp()
	{ }

	virtual void         registerParams(Beagle::System& ioSystem);
	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle mNumberParticipants;  //!< Number of participants to each tournament.

};

}

}

#endif // Beagle_EC_SelectTournamentOp_hpp
