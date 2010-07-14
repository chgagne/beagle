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
 *  \file   Beagle/EC/SelectWorstTournOp.hpp
 *  \brief  Definition of the class SelectWorstTournOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectWorstTournOp_hpp
#define Beagle_EC_SelectWorstTournOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{
	
namespace EC
{

/*!
 *  \class SelectWorstTournOp Beagle/EC/SelectWorstTournOp.hpp "Beagle/EC/SelectWorstTournOp.hpp"
 *  \brief Selection of worst individual of a tournament operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectWorstTournOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectWorstTournOp allocator type.
	typedef Beagle::AllocatorT<SelectWorstTournOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectWorstTournOp handle type.
	typedef Beagle::PointerT<SelectWorstTournOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectWorstTournOp bag type.
	typedef Beagle::ContainerT<SelectWorstTournOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectWorstTournOp(std::string inReproProbaName="ec.repro.prob",
	                            std::string inName="SelectWorstTournOp");
	virtual ~SelectWorstTournOp()
	{ }

	virtual void         registerParams(Beagle::System& ioSystem);
	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle mNumberParticipants;  //!< Number of participants to each tournament.

};

}

}

#endif // Beagle_EC_SelectWorstTournOp_hpp
