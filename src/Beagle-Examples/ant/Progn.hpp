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
 *  \file   Progn.cpp
 *  \brief  Declaration of class Progn.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#ifndef Progn_hpp
#define Progn_hpp

#include "Beagle/GP.hpp"
#include <string>


/*!
 *  \class Progn Progn.hpp "Progn.hpp"
 *  \brief Execute sequentialy the arguments' subtree.
 *  \ingroup Ant
 */
class Progn : public Beagle::GP::Primitive
{

public:

	//! Progn allocator type.
	typedef Beagle::AllocatorT<Progn,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< Progn handle type.
	typedef Beagle::PointerT<Progn,Beagle::GP::Primitive::Handle>
	Handle;
	//!< Progn bag type.
	typedef Beagle::ContainerT<Progn,Beagle::GP::Primitive::Bag>
	Bag;

	explicit Progn(unsigned int inNbArguments=2);
	virtual ~Progn()
	{ }

	virtual void execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // Progn_hpp
