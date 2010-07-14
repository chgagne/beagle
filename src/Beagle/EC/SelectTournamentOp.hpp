/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
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
