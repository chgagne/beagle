/*
 *  Artifical ant (ant)
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
 *  \file   IfFoodAhead.cpp
 *  \brief  Declaration of class IfFoodAhead.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#ifndef IfFoodAhead_hpp
#define IfFoodAhead_hpp

#include "Beagle/GP.hpp"
#include <string>


/*!
 *  \class IfFoodAhead IfFoodAhead.hpp "IfFoodAhead.hpp"
 *  \brief If there is food ahead the ant, execute 1st argument's subtree, else execute 2nd.
 *  \ingroup Ant
 */
class IfFoodAhead : public Beagle::GP::Primitive
{

public:

	//! IfFoodAhead allocator type.
	typedef Beagle::AllocatorT<IfFoodAhead,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< IfFoodAhead handle type.
	typedef Beagle::PointerT<IfFoodAhead,Beagle::GP::Primitive::Handle>
	Handle;
	//!< IfFoodAhead bag type.
	typedef Beagle::ContainerT<IfFoodAhead,Beagle::GP::Primitive::Bag>
	Bag;

	explicit IfFoodAhead();
	virtual ~IfFoodAhead()
	{ }

	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // IfFoodAhead_hpp
