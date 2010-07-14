/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
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
