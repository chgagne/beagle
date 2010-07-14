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
 *  \file   Beagle/EC/SteadyStateOp.hpp
 *  \brief  Definition of the class SteadyStateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_SteadyStateOp_hpp
#define Beagle_EC_SteadyStateOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SteadyStateOp Beagle/EC/SteadyStateOp.hpp "Beagle/EC/SteadyStateOp.hpp"
 *  \brief Steady state replacement strategy operator.
 *  \ingroup EC
 *  \ingroup Op
 */
class SteadyStateOp : public Beagle::ReplacementStrategyOp
{

public:

	//! SteadyStateOp allocator type
	typedef Beagle::AllocatorT<SteadyStateOp,Beagle::ReplacementStrategyOp::Alloc> Alloc;
	//! SteadyStateOp handle type.
	typedef Beagle::PointerT<SteadyStateOp,Beagle::ReplacementStrategyOp::Handle> Handle;
	//! SteadyStateOp bag type.
	typedef Beagle::ContainerT<SteadyStateOp,Beagle::ReplacementStrategyOp::Bag> Bag;

	explicit SteadyStateOp(std::string inName="SteadyStateOp");
	virtual ~SteadyStateOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle  mElitismKeepSize;      //!< Number of individuals keep with elitism

};

}

}

#endif // Beagle_EC_SteadyStateOp_hpp
