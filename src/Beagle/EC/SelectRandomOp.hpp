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
 *  \file   Beagle/EC/SelectRandomOp.hpp
 *  \brief  Definition of the class SelectRandomOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_SelectRandomOp_hpp
#define Beagle_EC_SelectRandomOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/SelectionOp.hpp"


namespace Beagle
{
	
namespace EC
{

/*!
 *  \class SelectRandomOp Beagle/EC/SelectRandomOp.hpp "Beagle/EC/SelectRandomOp.hpp"
 *  \brief Select an individual in a population randomly operator class (uniform distribution).
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectRandomOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectRandomOp allocator type.
	typedef Beagle::AllocatorT<SelectRandomOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectRandomOp handle type.
	typedef Beagle::PointerT<SelectRandomOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectRandomOp bag type.
	typedef Beagle::ContainerT<SelectRandomOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectRandomOp(std::string inReproProbaName="ec.repro.prob",
	                        std::string inName="SelectRandomOp");
	virtual ~SelectRandomOp()
	{ }

	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);

};

}

}

#endif // Beagle_EC_SelectRandomOp_hpp
