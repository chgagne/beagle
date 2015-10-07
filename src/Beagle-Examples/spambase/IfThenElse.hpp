/*
 *  SPAM e-mail database (spambase):
 *  Machine learning using strongly-typed GP with Open BEAGLE
 *  Copyright (C) 2001-2006 by Christian Gagne and Marc Parizeau
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
 *  \file   IfThenElse.hpp
 *  \brief  Definition of IfThenElse primitive used in the spambase problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:49 $
 */

#ifndef IfThenElse_hpp
#define IfThenElse_hpp

#include "Beagle/GP.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


/*!
 *  \class IfThenElse IfThenElse.hpp "IfThenElse.hpp"
 *  \brief IfThenElse primitive that test 1st argument, return 2nd if true, 3rd if not.
 *  \ingroup Spambase
 */
class IfThenElse : public Beagle::GP::Primitive
{

public:

	//! IfThenElse allocator type.
	typedef Beagle::AllocatorT<IfThenElse,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< IfThenElse handle type.
	typedef Beagle::PointerT<IfThenElse,Beagle::GP::Primitive::Handle>
	Handle;
	//!< IfThenElse bag type.
	typedef Beagle::ContainerT<IfThenElse,Beagle::GP::Primitive::Bag>
	Bag;

	explicit IfThenElse();
	virtual ~IfThenElse()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // IfThenElse_hpp
