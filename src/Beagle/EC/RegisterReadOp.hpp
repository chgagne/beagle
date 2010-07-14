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
 *  \file   Beagle/EC/RegisterReadOp.hpp
 *  \brief  Definition of the class RegisterReadOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_RegisterReadOp_hpp
#define Beagle_EC_RegisterReadOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class RegisterReadOp Beagle/EC/RegisterReadOp.hpp "Beagle/EC/RegisterReadOp.hpp"
 *  \brief Read evolution parameters operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup Sys
 */
class RegisterReadOp : public Beagle::Operator
{

public:

	//! RegisterReadOp allocator type
	typedef Beagle::AllocatorT<RegisterReadOp,Beagle::Operator::Alloc> Alloc;
	//! RegisterReadOp handle type.
	typedef Beagle::PointerT<RegisterReadOp,Beagle::Operator::Handle> Handle;
	//! RegisterReadOp bag type.
	typedef Beagle::ContainerT<RegisterReadOp,Beagle::Operator::Bag> Bag;
	
	explicit RegisterReadOp(std::string inName="RegisterReadOp");
	virtual ~RegisterReadOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

private:

	Beagle::UInt::Handle   mReadingInterval;  //!< Parameters reading interval (in generation).
	Beagle::String::Handle mFileName;         //!< Configuration file name.

};

}

}

#endif // Beagle_EC_RegisterReadOp_hpp
