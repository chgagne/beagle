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
 *  \file   TurnRight.cpp
 *  \brief  Declaration of class TurnRight.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#ifndef TurnRight_hpp
#define TurnRight_hpp

#include "Beagle/GP.hpp"
#include <string>


/*!
 *  \class TurnRight TurnRight.hpp "TurnRight.hpp"
 *  \brief Turn the ant 90 degrees clockwize.
 *  \ingroup Ant
 */
class TurnRight : public Beagle::GP::Primitive
{

public:

	//! TurnRight allocator type.
	typedef Beagle::AllocatorT<TurnRight,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< TurnRight handle type.
	typedef Beagle::PointerT<TurnRight,Beagle::GP::Primitive::Handle>
	Handle;
	//!< TurnRight bag type.
	typedef Beagle::ContainerT<TurnRight,Beagle::GP::Primitive::Bag>
	Bag;

	explicit TurnRight();
	virtual ~TurnRight()
	{ }

	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // TurnRight_hpp
