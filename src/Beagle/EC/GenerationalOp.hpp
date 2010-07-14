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
 *  \file   Beagle/EC/GenerationalOp.hpp
 *  \brief  Definition of the class Beagle::EC::GenerationalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:12 $
 */


#ifndef Beagle_EC_GenerationalOp_hpp
#define Beagle_EC_GenerationalOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class GenerationalOp Beagle/EC/GenerationalOp.hpp "Beagle/EC/GenerationalOp.hpp"
 *  \brief Generational replacement strategy operator class.
 *  \ingroup EC
 *  \ingroup Op
 *
 *  A generational operator process deme with a breeding tree following a generation by generation
 *  replacement strategy.
 */
class GenerationalOp : public Beagle::ReplacementStrategyOp
{

public:

	//! GenerationalOp allocator type.
	typedef Beagle::AllocatorT<GenerationalOp,Beagle::ReplacementStrategyOp::Alloc>
	Alloc;
	//! GenerationalOp handle type.
	typedef Beagle::PointerT<GenerationalOp,Beagle::ReplacementStrategyOp::Handle>
	Handle;
	//! GenerationalOp bag type.
	typedef Beagle::ContainerT<GenerationalOp,Beagle::ReplacementStrategyOp::Bag>
	Bag;

	GenerationalOp(std::string inName="GenerationalOp");
	virtual ~GenerationalOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle  mElitismKeepSize;      //!< Number of individuals keep with elitism

};

}

}

#endif // Beagle_EC_GenerationalOp_hpp
