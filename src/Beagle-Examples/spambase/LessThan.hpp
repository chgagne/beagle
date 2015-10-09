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
 *  \file   LessThan.hpp
 *  \brief  Definition of LessThan primitive used in the spambase problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:49 $
 */

#ifndef LessThan_hpp
#define LessThan_hpp

#include "Beagle/GP.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


/*!
 *  \class LessThan LessThan.hpp "LessThan.hpp"
 *  \brief LessThan primitive that compare the order of two Double and return a Bool.
 *  \ingroup Spambase
 */
class LessThan : public Beagle::GP::Primitive
{

public:

	//! LessThan allocator type.
	typedef Beagle::AllocatorT<LessThan,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< LessThan handle type.
	typedef Beagle::PointerT<LessThan,Beagle::GP::Primitive::Handle>
	Handle;
	//!< LessThan bag type.
	typedef Beagle::ContainerT<LessThan,Beagle::GP::Primitive::Bag>
	Bag;

	explicit LessThan();
	virtual ~LessThan()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // LessThan_hpp
