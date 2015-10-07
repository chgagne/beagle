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
 *  \file   TurnLeft.cpp
 *  \brief  Declaration of class TurnLeft.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#ifndef TurnLeft_hpp
#define TurnLeft_hpp

#include "Beagle/GP.hpp"
#include <string>


/*!
 *  \class TurnLeft TurnLeft.hpp "TurnLeft.hpp"
 *  \brief Turn the ant 90 degrees counter-clockwize.
 *  \ingroup Ant
 */
class TurnLeft : public Beagle::GP::Primitive
{

public:

	//! TurnLeft allocator type.
	typedef Beagle::AllocatorT<TurnLeft,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< TurnLeft handle type.
	typedef Beagle::PointerT<TurnLeft,Beagle::GP::Primitive::Handle>
	Handle;
	//!< TurnLeft bag type.
	typedef Beagle::ContainerT<TurnLeft,Beagle::GP::Primitive::Bag>
	Bag;

	explicit TurnLeft();
	virtual ~TurnLeft()
	{ }

	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // TurnLeft_hpp
