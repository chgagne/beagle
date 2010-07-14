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
 *  \file   Beagle/EC/SelectFirstOp.hpp
 *  \brief  Definition of the class SelectFirstOp.
 *  \author Christian Gagne
 *  $Revision $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectFirstOp_hpp
#define Beagle_EC_SelectFirstOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectFirstOp Beagle/EC/SelectFirstOp.hpp "Beagle/EC/SelectFirstOp.hpp"
 *  \brief Select first individual(s) in a population using given order.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectFirstOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectFirstOp allocator type.
	typedef Beagle::AllocatorT<SelectFirstOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectFirstOp handle type.
	typedef Beagle::PointerT<SelectFirstOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectFirstOp bag type.
	typedef Beagle::ContainerT<SelectFirstOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectFirstOp(std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="SelectFirstOp");
	virtual ~SelectFirstOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);
	virtual void         selectManyIndividuals(unsigned int inN,
	                                           Beagle::Individual::Bag& ioPool,
	                                           Beagle::Context& ioContext,
	                                           std::vector<unsigned int>& outSelections);
};

}

}

#endif // Beagle_EC_SelectFirstOp_hpp
