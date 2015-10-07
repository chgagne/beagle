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
 *  \file   Move.cpp
 *  \brief  Declaration of class Move.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#ifndef Move_hpp
#define Move_hpp

#include "Beagle/GP.hpp"
#include <string>


/*!
 *  \class Move Move.hpp "Move.hpp"
 *  \brief Move the ant to the case ahead.
 *  \ingroup Ant
 */
class Move : public Beagle::GP::Primitive
{

public:

	//! Move allocator type.
	typedef Beagle::AllocatorT<Move,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< Move handle type.
	typedef Beagle::PointerT<Move,Beagle::GP::Primitive::Handle>
	Handle;
	//!< Move bag type.
	typedef Beagle::ContainerT<Move,Beagle::GP::Primitive::Bag>
	Bag;

	explicit Move();
	virtual ~Move()
	{ }

	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // Move_hpp
