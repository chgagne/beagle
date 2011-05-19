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
 *  \file   beagle/GP/ValidateOp.hpp
 *  \brief  Definition of the type GP::ValidateOp.
 *  \author Matthew Walker
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:36 $
 */

#ifndef Beagle_GP_ValidateOp_hpp
#define Beagle_GP_ValidateOp_hpp

#include "beagle/Operator.hpp"

namespace Beagle
{
namespace GP
{


/*!
 *  \class ValidateOp beagle/GP/ValidateOp.hpp "beagle/GP/ValidateOp.hpp"
 *  \brief Operator used primarily for debugging. It checks that every individual is valid.
 *  \ingroup GPF
 */
class ValidateOp : public Operator
{

public:

	//! Operator allocator type.
	typedef AllocatorT<ValidateOp,Operator::Alloc>
	Alloc;
	//! Operator handle type.
	typedef PointerT<ValidateOp,Operator::Handle>
	Handle;
	//! Operator bag type.
	typedef ContainerT<ValidateOp,Operator::Bag>
	Bag;

	explicit ValidateOp(std::string inName="GP-ValidateOp");
	virtual ~ValidateOp()
	{ }

	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
};

}
}

#endif // Beagle_ValidateOp_hpp
