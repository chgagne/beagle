/*
 *  Boolean 11-multiplexer (multiplexer)
 *
 *  Copyright (C) 2006 by Christian Gagne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   IfBoolean.hpp
 *  \brief  Definition of IfBoolean primitive used in the multiplexer problem.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#ifndef IfBoolean_hpp
#define IfBoolean_hpp

#include "Beagle/GP.hpp"
#include <string>


/*!
 *  \class IfBoolean IfBoolean.hpp "IfBoolean.hpp"
 *  \brief IfBoolean primitive that test 1st argument, return 2nd if true, 3rd if not.
 *  \ingroup Multiplexer
 */
class IfBoolean : public Beagle::GP::Primitive
{

public:

	//! IfBoolean allocator type.
	typedef Beagle::AllocatorT<IfBoolean,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< IfBoolean handle type.
	typedef Beagle::PointerT<IfBoolean,Beagle::GP::Primitive::Handle>
	Handle;
	//!< IfBoolean bag type.
	typedef Beagle::ContainerT<IfBoolean,Beagle::GP::Primitive::Bag>
	Bag;

	explicit IfBoolean();
	virtual ~IfBoolean()
	{ }

	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // IfBoolean_hpp
