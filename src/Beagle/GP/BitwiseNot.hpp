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
 *  \file   beagle/GP/BitwiseNot.hpp
 *  \brief  Definition of the templated class GP::BitwiseNot.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_BitwiseNot_hpp
#define Beagle_GP_BitwiseNot_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

/*!
 *  \class BitwiseNot beagle/GP/BitwiseNot.hpp "beagle/GP/BitwiseNot.hpp"
 *  \brief GP bitwise NOT primitive class.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup BitwisePrimit
 */
class BitwiseNot : public Primitive
{

public:

	//! GP::BitwiseNot allocator type.
	typedef AllocatorT<BitwiseNot,Primitive::Alloc>
	Alloc;
	//! GP::BitwiseNot handle type.
	typedef PointerT<BitwiseNot,Primitive::Handle>
	Handle;
	//! GP::BitwiseNot bag type.
	typedef ContainerT<BitwiseNot,Primitive::Bag>
	Bag;

	explicit BitwiseNot(std::string inName="BITWISE-NOT");
	virtual ~BitwiseNot()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(GP::Datum& outResult, GP::Context& ioContext);

};

}
}


#endif // Beagle_GP_BitwiseNot_hpp
