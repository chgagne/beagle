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
 *  \file   Beagle/EC/TermMinFitnessOp.hpp
 *  \brief  Definition of the class TermMinFitnessOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_TermMinFitnessOp_hpp
#define Beagle_EC_TermMinFitnessOp_hpp

#include "Beagle/Core.hpp"
#include "Beagle/EC/TerminationOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class TermMinFitnessOp Beagle/EC/TermMinFitnessOp.hpp "Beagle/EC/TermMinFitnessOp.hpp"
 *  \brief Minimum fitness value termination criterion operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class TermMinFitnessOp : public Beagle::EC::TerminationOp
{

public:

	//! TermMinFitnessOp allocator type
	typedef Beagle::AllocatorT<TermMinFitnessOp,Beagle::EC::TerminationOp::Alloc> Alloc;
	//! TermMinFitnessOp handle type.
	typedef Beagle::PointerT<TermMinFitnessOp,Beagle::EC::TerminationOp::Handle> Handle;
	//! TermMinFitnessOp bag type.
	typedef Beagle::ContainerT<TermMinFitnessOp,Beagle::EC::TerminationOp::Bag> Bag;

	explicit TermMinFitnessOp(float inMinFitness=0.0, std::string inName="TermMinFitnessOp");
	virtual ~TermMinFitnessOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Beagle::Float::Handle mMinFitness;        //!< Minimum fitness of an individual for an evolution.
	float                 mMinFitnessDefault; //!< Default minimum fitness value.

};

}

}

#endif // Beagle_EC_TerminationCheckOp_hpp
