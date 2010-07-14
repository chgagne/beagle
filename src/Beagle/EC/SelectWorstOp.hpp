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
 *  \file   Beagle/EC/SelectWorstOp.hpp
 *  \brief  Definition of the class SelectWorstOp.
 *  \author Christian Gagne
 *  $Revision $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectWorstOp_hpp
#define Beagle_EC_SelectWorstOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectWorstOp Beagle/EC/SelectWorstOp.hpp "Beagle/EC/SelectWorstOp.hpp"
 *  \brief Select worst individual(s) of the population.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectWorstOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectWorstOp allocator type.
	typedef Beagle::AllocatorT<SelectWorstOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectWorstOp handle type.
	typedef Beagle::PointerT<SelectWorstOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectWorstOp bag type.
	typedef Beagle::ContainerT<SelectWorstOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectWorstOp(std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="SelectWorstOp");
	virtual ~SelectWorstOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);
	virtual void         selectManyIndividuals(unsigned int inN,
	                                           Beagle::Individual::Bag& ioPool,
	                                           Beagle::Context& ioContext,
	                                           std::vector<unsigned int>& outSelections);

};

}

}

#endif // Beagle_EC_SelectWorstOp_hpp
