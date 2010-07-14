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
 *  \file   Beagle/EC/SelectBestOp.hpp
 *  \brief  Definition of the class SelectBestOp.
 *  \author Matthew Walker
 *  $Revision $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectBestOp_hpp
#define Beagle_EC_SelectBestOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectBestOp Beagle/EC/SelectBestOp.hpp "Beagle/EC/SelectBestOp.hpp"
 *  \brief Select best individual(s) of the population.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectBestOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectBestOp allocator type.
	typedef Beagle::AllocatorT<SelectBestOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectBestOp handle type.
	typedef Beagle::PointerT<SelectBestOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectBestOp bag type.
	typedef Beagle::ContainerT<SelectBestOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectBestOp(std::string inReproProbaName="ec.repro.prob",
	                      std::string inName="SelectBestOp");
	virtual ~SelectBestOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);
	virtual void         selectManyIndividuals(unsigned int inN,
	                                           Beagle::Individual::Bag& ioPool,
	                                           Beagle::Context& ioContext,
	                                           std::vector<unsigned int>& outSelections);

};

}

}

#endif // Beagle_EC_SelectBestOp_hpp
