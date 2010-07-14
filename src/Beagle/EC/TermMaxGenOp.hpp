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
 *  \file   Beagle/EC/TermMaxGenOp.hpp
 *  \brief  Definition of the class TermMaxGenOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_TermMaxGenOp_hpp
#define Beagle_EC_TermMaxGenOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/TerminationOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class TermMaxGenOp Beagle/EC/TermMaxGenOp.hpp "Beagle/EC/TermMaxGenOp.hpp"
 *  \brief Maximum generation termination criterion operator.
 *  \ingroup EC
 *  \ingroup Op
 */
class TermMaxGenOp : public Beagle::EC::TerminationOp
{

public:

	//! TermMaxGenOp allocator type
	typedef Beagle::AllocatorT<TermMaxGenOp,Beagle::EC::TerminationOp::Alloc> Alloc;
	//! TermMaxGenOp handle type.
	typedef Beagle::PointerT<TermMaxGenOp,Beagle::EC::TerminationOp::Handle> Handle;
	//! TermMaxGenOp bag type.
	typedef Beagle::ContainerT<TermMaxGenOp,Beagle::EC::TerminationOp::Bag> Bag;

	explicit TermMaxGenOp(std::string inName="TermMaxGenOp");
	virtual ~TermMaxGenOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle mMaxGeneration;     //!< Maximum number of generation of the evolution.

};

}

}

#endif // Beagle_EC_TerminationCheckOp_hpp
