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
 *  \file   beagle/GP/EphemeralDouble.hpp
 *  \brief  Definition of the templated class GP::EphemeralDouble.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_EphemeralDouble_hpp
#define Beagle_GP_EphemeralDouble_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Double.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"
#include "beagle/GP/EphemeralT.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class EphemeralDouble beagle/GP/EphemeralDouble.hpp "beagle/GP/EphemeralDouble.hpp"
 *  \brief Double ephemeral constants uniformly generated in [-1,1].
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
class EphemeralDouble : public EphemeralT<Double>
{

public:

	//! GP::EphemeralDouble allocator type.
	typedef AllocatorT<EphemeralDouble,EphemeralT<Double>::Alloc>
	Alloc;
	//! GP::EphemeralDouble handle type.
	typedef PointerT<EphemeralDouble,EphemeralT<Double>::Handle>
	Handle;
	//! GP::EphemeralDouble bag type.
	typedef ContainerT<EphemeralDouble,EphemeralT<Double>::Bag>
	Bag;

	explicit EphemeralDouble(Double::Handle inValue=NULL, std::string inName="E");
	virtual ~EphemeralDouble()
	{ }

	virtual GP::Primitive::Handle generate(std::string inName, GP::Context& ioContext);

};

}
}


#endif // Beagle_GP_EphemeralDouble_hpp

